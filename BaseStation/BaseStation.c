/**
 * \file BaseStation.c
 *
 * Main program file for BaseStation for Mongol.
 *
 * This file contains the code for main(), which initializes the subsystems and
 * contains the main control loop. Also included are global functions for
 * printing/logging messages and terminating the program.
 */

#include "BaseStation.h"
#include "Controller.h"
#include "Serial.h"

char* ms_comm_device; /**< Communications port device name. */

Ctrl_Mode  m_ctrl_mode;  /**< Present control mode of program. */
Comm_Mode  m_comm_mode;  /**< Present communication mode of program. */
UI_Mode    m_ui_mode;    /**< Present user interface mode of program. */
Print_Mode m_print_mode; /**< Present error/warning output mode of program. */

/**
 * Wait function.
 *
 * This function prints a prompt to the terminal and waits for input
 * from the user. Used mainly for debugging.
 */
void
wait_for_user ()
{
    print_log ("Press enter to continue...", Log_Level_High);
    fgetc (stdin);

    return;
}

/**
 * This global function is for the use of subsystems to output error
 * and warning messages to the terminal and/or log file. Whether or not it
 * ignores messages of minor severity is determined by the value of
 * pres_PRNT_Mode, which is set by command line argument in ::parse_args()
 *
 * \todo Add routine to redirect output for GUI mode.
 */
void
print_log (char* s_log, Log_Level level)
{
    if (m_ui_mode == UI_Mode_Terminal)
    {
        if (level >= m_print_mode)
        {
            puts (s_log);
        }
    }

    return;
}

/**
 * \param str Pointer to buffer to store string representation of type.
 * \param type The PKT_type to be converted.
 *
 * This function accepts a PKT_type variable and a pointer to a buffer
 * to store a string representation of the packet type. Useful for debugging.
 */
void
type_to_str (char* pc_str, Msg_Type type)
{
    switch (type)
    {
        case Msg_Type_Hello        : sprintf (pc_str, "HELLO" );  break;
        case Msg_Type_Goodbye      : sprintf (pc_str, "GDBY" );   break;
        case Msg_Type_Standby      : sprintf (pc_str, "STDBY");   break;
        case Msg_Type_Move         : sprintf (pc_str, "MOVE" );   break;
        case Msg_Type_Turn         : sprintf (pc_str, "TURN" );   break;
        case Msg_Type_Aim_Horz     : sprintf (pc_str, "AIM_H" );  break;
        case Msg_Type_Aim_Vert     : sprintf (pc_str, "AIM_V" );  break;
        case Msg_Type_Fire         : sprintf (pc_str, "FIRE" );   break;
        case Msg_Type_Strafe_Left  : sprintf (pc_str, "STRF_L" ); break;
        case Msg_Type_Strafe_Right : sprintf (pc_str, "STRF_R" ); break;
        case Msg_Type_Ready        : sprintf (pc_str, "RDY" );    break;
    }

    return;
}

/**
 * This function properly closes all subsystems and communication
 * channels before terminating the BaseStation program.
 */
void
quit_basestation ()
{
    print_log ("Exiting...", Log_Level_High);

    close_ctrl ();

    if (m_comm_mode == Comm_Mode_Offline)
    {
        close_serial ();
    }

	exit (0);
}

/**
 * Command-line argument parser.
 * \param argc Number of arguments to parse.
 * \param argv Array of pointers to individual arguments.
 *
 * \details This function takes the arguments given to the program via the
 * command line, validates them, and sets the values of pres_CTRL_mode,
 * pres_COMM_mode, pres_UI_mode, and pres_PRINT_mode appropriately. Any
 * variables that aren't modified by arguments are set to initial values for
 * normal operation of the robot.
 */
void
parse_args (int n_argc, char* s_argv[])
{
    int i;
    for (i = 1; i < n_argc; i++)
    {
        // Get character after dash
        switch (s_argv [i][1])
        {
        // Port specifier flag (-d): defaults to /dev/ttyUSB0 if not used (for Linux)
        case 'd':
            ms_comm_device = s_argv [++i];
            printf ("Device specified: %s\n", ms_comm_device);
            break;

        // Gamepad mode flag (-j): If not specified, defaults to keyboard control
        case 'j':
            m_ctrl_mode = Ctrl_Mode_Gamepad;
            printf ("Gamepad mode activated.\n");
            break;

        // Calibrate flag (-c): Do not attempt to connect; used for testing input devices
        case 'c':
            m_comm_mode = Comm_Mode_Offline;
            m_print_mode = Print_Mode_All;
            printf ("Controller calibration activated.\n");
            break;

        // GUI mode flag (-g)
        case 'g':
            m_ui_mode = UI_Mode_Graphical;
            printf ("Graphical mode activated.\n");
            break;
     
        // Verbose mode flag (-v): prints all non-critical error messages
        case 'v':  
            m_print_mode = Print_Mode_All;
            printf ("Verbose mode activated.\n");    
            break;

        // Print help flag (-h)
        case 'h':
            printf ("\nRobotic Control BaseStation Program\n");
            printf ("Written by Allen Baker for Illinois Tech Robotics\n\n");
            printf ("Usage:\n");
            printf ("BaseStation [-d <port-name>] [-v] [-h]\n\n");
            printf ("***** Arguements *****\n");
            printf (" -d <port-name>  : Specify port to use (default is /dev/ttyUSB0)\n");
            printf (" -j              : Activate joystick mode (default is keyboard mode)\n");
            printf (" -v              : Set verbose mode\n");
            printf (" -h              : Print help information\n\n");
            exit (0);
        }
    }
}

/**
 * The main function of BaseStation for Mongol.
 *
 * Entry point of program. Upon starting with normal mode values set,
 * this function parses any command line arguments, initializes the controller
 * interface (\sa Controller.h), intitializes the robot communication interface
 * (\sa Serial.h), and then enters the main control loop of the program. There,
 * the program basically gathers user input data from the controller, builds a
 * serial packet, and then sends it to the robot. The program waits for
 * confirmation from the robot in the form of a PKT_RDY packet type before
 * looping again.
 */
int
main (int argc, char* argv[])
{
    // Set default values
    m_ctrl_mode = Ctrl_Mode_Keyboard;
    m_comm_mode = Comm_Mode_Offline;
    m_ui_mode   = UI_Mode_Terminal;

    // Parse command line arguements
    parse_args (argc, argv);

    // Initialize SDL (VIDEO flag also initializes event handling)
    print_log ("Initializing controller... ", Log_Level_Med);
    if (!(initCtrl()))
    {
        print_log ("Controller failed to intialize.\n", Log_Level_High);
        quit_basestation ();
    }
    print_log ("Controller Initialized.\n", Log_Level_Med);

    // Initialize serial port (includes looking for HELLO packet
    // If not port name specified, default to /dev/ttyUSB0 (for Linux)
    if (m_comm_mode == Comm_Mode_Online)
    {
        print_log ("Connecting to robot... ", Log_Level_Med);
        if ((ms_comm_device ? init_serial (ms_comm_device) : init_serial ("/dev/ttyUSB0")) < 0)
        {
            print_log ("Failed to connect\n", Log_Level_High);
            m_comm_mode = Comm_Mode_Offline;
            quit_basestation ();
        }
        print_log ("Robot connected.\n", Log_Level_Med);
    }

    Msg out_msg;
    Msg in_msg;
    char intype_buf [128];
    char outtype_buf [128];
    int print_nxt_received = 0;

    // Main program loop
    for(;;)
    {
        next_event (&out_msg);

        if(m_comm_mode == Comm_Mode_Online)
        {
            do
            {
                serial_write (&out_msg);
                usleep (10);
            }
            while (serial_read (&in_msg) ||
                   (in_msg.type != Msg_Type_Ready));

            if (out_msg.type != Msg_Type_Standby)
            {
                type_to_str (intype_buf, out_msg.type);
                printf ("Sent: %s\n", intype_buf);
                type_to_str (outtype_buf, in_msg.type);
                printf ("Received: %s\n", outtype_buf);
            }
        }
    }

    return 0;
}
