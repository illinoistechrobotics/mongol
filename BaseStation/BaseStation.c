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
#include "GUI.h"

char* comm_dev;             /**< Communications port device name. */
CTRL_mode pres_CTRL_mode;   /**< Present control mode of program.
                              \see CTRL_mode */
COMM_mode pres_COMM_mode;   /**< Present communication mode of program.
                              \see COMM_mode */
UI_mode pres_UI_mode;       /**< Present user interface mode of program.
                              \see UI_mode */
PRINT_mode pres_PRINT_mode; /**< Present error/warning output mode of program.
                              \see PRINT_mode */

/**
 * \brief Wait function.
 *
 * \details This function prints a prompt to the terminal and waits for input
 * from the user. Used mainly for debugging.
 */
void wait_for_user()
{
	sprintf( termbuf, "Press enter to continue..." );
    print_msg();
	fgetc( stdin );

    return;
}

/**
 * \details This global function is for the use of subsystems to output error
 * and warning messages to the terminal and/or log file. Whether or not it
 * ignores messages of minor severity is determined by the value of
 * pres_PRNT_Mode, which is set by command line argument in ::parse_args()
 *
 * \todo Add routine to redirect output for GUI mode.
 */
void print_msg()
{
    if( uiMode == CMD_LINE )
    {
        puts( termbuf );
    }

    return;
}

/**
 * \details This function accepts a PKT_type variable and a pointer to a buffer
 * to store a string representation of the packet type. Useful for debugging.
 *
 * \param str Pointer to buffer to store string representation of type.
 * \param type The PKT_type to be converted.
 */
void type_to_str( char* str, PKT_type type )
{
    switch( type )
    {
        case PKT_HELLO : sprintf( str, "HELLO" );  break;
        case PKT_GDBY  : sprintf( str, "GDBY" );   break;
        case PKT_STDBY : sprintf( str, "STDBY");   break;
        case PKT_MOVE  : sprintf( str, "MOVE" );   break;
        case PKT_TURN  : sprintf( str, "TURN" );   break;
        case PKT_AIM_H : sprintf( str, "AIM_H" );  break;
        case PKT_AIM_V : sprintf( str, "AIM_V" );  break;
        case PKT_FIRE  : sprintf( str, "FIRE" );   break;
        case PKT_STRF_L: sprintf( str, "STRF_L" ); break;
        case PKT_STRF_R: sprintf( str, "STRF_R" ); break;
        case PKT_RDY   : sprintf( str, "RDY" );    break;
    }

    return;
}

/**
 * \details This function properly closes all subsystems and communication
 * channels before terminating the BaseStation program.
 */
void quit_base()
{
    sprintf( termbuf, "Exiting...\n" );
    printmsg();

    close_ctrl();

    if( commMode == ONLINE )
        close_serial();

	exit( 0 );
}

/**
 * \brief Command-line argument parser.
 *
 * \details This function takes the arguments given to the program via the
 * command line, validates them, and sets the values of pres_CTRL_mode,
 * pres_COMM_mode, pres_UI_mode, and pres_PRINT_mode appropriately. Any
 * variables that aren't modified by arguments are set to initial values for
 * normal operation of the robot.
 *
 * \param argc Number of arguments to parse.
 * \param argv Array of pointers to individual arguments.
 */
void parse_args ( int argc, char* argv[] ){

    int a;
    for( a = 1; a < argc; a ++ )
    {
        // Get character after dash
        switch( argv[ a ][ 1 ] )
        {
        // Port specifier flag (-d): defaults to /dev/ttyUSB0 if not used (for Linux)
        case 'd':
            dev = argv[++a];
            printf("Port specified: %s\n", dev);
            break;

        // Joystick mode flag (-j): If not specified, defaults to keyboard control
        case 'j':
            ctrlmode = GAMEPAD;
            printf("Joystick mode activated.\n");
            break;

        // Calibrate flag (-c): Do not attempt to connect; used for testing input devices
        case 'c':
            commMode = OFFLINE;
            printMode = VERBOSE;
            printf("Controller calibration activated.\n");
            break;

        // GUI mode flag (-g)
        case 'g':
            uiMode = GRAPH_UI;
            printf("Graphical mode activated.\n");
            break;
     
        // Verbose mode flag (-v): prints all non-critical error messages
        case 'v':  
            printMode = VERBOSE;    
            printf("Verbose mode activated.\n");    
            break;

        // Print help flag (-h)
        case 'h':
            printf("\nRobotic Control BaseStation Program\n");
            printf("Written by Allen Baker for Illinois Tech Robotics\n\n");
            printf("Usage:\n");
            printf("BaseStation [-d <port-name>] [-v] [-h]\n\n");
            printf("***** Arguements *****\n");
            printf(" -d <port-name>  : Specify port to use (default is /dev/ttyUSB0)\n");
            printf(" -j              : Activate joystick mode (default is keyboard mode)\n");
            printf(" -v              : Set verbose mode\n");
            printf(" -h              : Print help information\n\n");
            exit(0);
        }
    }
}

/**
 * \brief The main function of BaseStation for Mongol.
 *
 * \details Entry point of program. Upon starting with normal mode values set,
 * this function parses any command line arguments, initializes the controller
 * interface (\sa Controller.h), intitializes the robot communication interface
 * (\sa Serial.h), and then enters the main control loop of the program. There,
 * the program basically gathers user input data from the controller, builds a
 * serial packet, and then sends it to the robot. The program waits for
 * confirmation from the robot in the form of a PKT_RDY packet type before
 * looping again.
 */

int main (int argc, char* argv[]){

    // Set default values
    ctrlmode = GAMEPAD;
    commMode = ONLINE;
    uiMode = CMD_LINE;
    printMode = QUIET; 

    // Parse command line arguements
    parseArgs(argc, argv);

    if(uiMode == GRAPH_UI){
        initGUI(&argc, &argv);
    } 

    // Initialize SDL (VIDEO flag also initializes event handling)
    sprintf(termbuf,"Initializing Controller... ");
    printmsg();
    if(!(initCtrl())){

        sprintf(termbuf,"\nERROR: Controller Failed to initialize.\n");
        quit_base();
    }
    sprintf(termbuf,"Controller Initialized.\n");
    printmsg();

    // Initialize serial port (includes looking for HELLO packet
    // If not port name specified, default to /dev/ttyUSB0 (for Linux)
    if(commMode == ONLINE){
        sprintf(termbuf,"Connecting to robot...\n");
        printmsg();
        if((dev ? init_serial(dev) : init_serial("/dev/ttyUSB0")) < 0){
            
            commMode = OFFLINE;
            quit_base();
        }
        sprintf(termbuf,"Connected!\n\n");
        printmsg();
    }

    packet out_pkt;
    packet * in_pkt;
    char intype_buf [128];
    char outtype_buf [128];
    int print_nxt_received = 0;

    // Main loop
    for(;;){

        next_event(&out_pkt);

        if(commMode == ONLINE){

            do{
                write_serial(&out_pkt);
                usleep(10);
            } while (((in_pkt = read_serial()) == NULL) ||
                     (in_pkt->type != PKT_RDY));

            if(out_pkt.type != PKT_STDBY){

                type2str(intype_buf, out_pkt.type);
                printf("Sent: %s\n", intype_buf);
                type2str(outtype_buf, in_pkt->type);
                printf("Received: %s\n", outtype_buf);
            }
        }
    }

    return 0;
}
