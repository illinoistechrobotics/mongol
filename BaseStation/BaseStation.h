/**
 * \file BaseStation.h
 *
 * Main header file for BaseStation.
 *
 * This header file contains the communication packet structure,
 * enumerations for packet components, and enumerations for different modes of
 * operation for the program.
 */

#include <stdio.h>

/**
 * Available packet types.
 *
 * These are the different types of packets available for
 * communication with the robot. PKT_HELLO, PKT_GDBY, PKT_STDBY, and PKT_RDY
 * are used to regulate communication and do not need corresponding values.
 */
typedef
enum Msg_Type_Enum
{
    Msg_Type_Hello = 1,    /**< Hello packet: used to initialize communications */
    Msg_Type_Goodbye,      /**< Goodbye packet: used to close connections */
    Msg_Type_Standby,      /**< Standby packet: no robot updates (most common) */
    Msg_Type_Move,         /**< Move packet: update moving */
    Msg_Type_Turn,         /**< Turn packet: update turning */
    Msg_Type_Aim_Horz,     /**< Horizontal aim packet: update horizontal aim
                                adjustment */
    Msg_Type_Aim_Vert,     /**< Vertical aim packet: update vertical aim adjustment */
    Msg_Type_Fire,         /**< Fire packet: update gun-firing status */
    Msg_Type_Strafe_Left,  /**< Strafe packet: update left strafe status */
    Msg_Type_Strafe_Right, /**< Strafe packet: update right strafe status */
    Msg_Type_Ready         /**< Ready packet: indication that robot is ready
                                for next packet */
}
Msg_Type;

/**
 * Controller mode values.
 *
 * This enumeration specifies possible modes for controlling the
 * robot.
 */
typedef
enum Ctrl_Mode_Enum
{
    Ctrl_Mode_Keyboard, /**< Use keyboard keys to control robot */
    Ctrl_Mode_Gamepad   /**< Use gamepad's joystick and buttons to control robot */
}
Ctrl_Mode;

/**
 * BaseStation-to-Robot communication mode.
 *
 * This enumeration specifies possible modes for communicating with the robot.
 */
typedef
enum Comm_Mode_Enum
{
    Comm_Mode_Online, /**< Attempt to establish connection to robot */
    Comm_Mode_Offline /**< Do not attempt to establish connection to robot */
}
Comm_Mode;

/**
 * User interface mode.
 *
 * This enumeration specifies modes for command-line interface or graphical
 * interface.
 */
typedef
enum UI_Mode_Enum
{
    UI_Mode_Terminal, /**< Do not initialize GUI, stay in command line. */
    UI_Mode_Graphical /**< Initialize GUI. */
}
UI_Mode;

/**
 * Error and warning printing mode.
 *
 * This enumeration specifies modes for quiet or verbose modes for reporting
 * minor errors and warnings to the terminal during runtime.
 */
typedef
enum Print_Mode_Enum
{
    Print_Mode_All, /**< Print log messages about minor communication and input
                         errors. */
    Print_Mode_Med, /**< Print logs that are at least fairly important. */
    Print_Mode_High /**< Do not print messages about minor communication and
                         input errors. */
}
Print_Mode;

/**
 * Log message importance.
 *
 * This enumberation specifies the different importance levels for log messages
 * generated throughout the program. print_log(char*, Log_Level) requires one
 * of these values as an arguement to decide whether to display it on the
 * terminal or not.
 */
typedef
enum Log_Level_Enum
{
    Log_Level_Low, /**< Denotes an unimportant log message */
    Log_Level_Med, /**< Denotes a fairly important log message. */
    Log_Level_High /**< Denotes a log message of critical importance. */
}
Log_Level;

/**
 * Robot movement packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_MOVE
 * that will direct the robot to move either forwards or backwards or not at
 * all.
 */
typedef
enum Move_Val_Enum
{
    Move_Val_Stop = 1, /**< Do not move the robot in any direction */
    Move_Val_Forward,  /**< Move the robot forward */
    Move_Val_Backward  /**< Move the robot backward */
}
Move_Val;

/**
 * Robot turning packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_TURN
 * that will direct the robot to turn to the left or the right or not at all.
 */
typedef
enum Turn_Val_Enum
{
    Turn_Val_None = 1, /**< Do not turn the robot in any direction */
    Turn_Val_Left,     /**< Turn the robot to the left */
    Turn_Val_Right     /**< Turn the robot to the right */
}
Turn_Val;

/**
 * Horizontal robot turrent aim adjustment packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_AIM_H
 * that will direct the robot to turn its turrent to either the left or right
 * or not at all.
 */
typedef
enum Aim_Horz_Val_Enum
{
    Aim_Horz_Val_Straight = 1, /**< Do not turn the turret in any direction */
    Aim_Horz_Val_Left,         /**< Turn the turret to the left */
    Aim_Horz_Val_Right         /**< Turn the turret to the right */
}
Aim_Horz_Val;

/**
 * Horizontal robot turrent aim adjustment packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_AIM_V
 * that will direct the robot to raise, lower, or keep still its turret.
 */
typedef
enum Aim_Vert_Val_Enum
{
    Aim_Vert_Val_Straight = 1, /**< Do not raise or lower the turret */
    Aim_Vert_Val_Down,         /**< Lower the turret */
    Aim_Vert_Val_Up            /**< Raise the turret */
}
Aim_Vert_Val;

/**
 * Robot turret firing mode values.
 *
 * This enumeration provides possible values for a packet of type PKT_FIRE
 * that will direct the robot to either fire or not fire the gun on its
 * turret.
 */
typedef
enum Fire_Val_Enum
{
    Fire_Val_On = 1, /**< Fire the gun */
    Fire_Val_Off     /**< Do not fire the gun */
}
Fire_Val;

/**
 * Robot left-strafing packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_STRF_L
 * that will direct the robot to strafe to the left or not.
 */
typedef
enum Strafe_Left_Val_Enum
{
    Strafe_Left_Val_Off = 1,
    Strafe_Left_Val_On
}
Strafe_Left_Val;

/**
 * Robot right-strafing packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_STRF_L
 * that will direct the robot to strafe to the right or not.
 */
typedef
enum Strafe_Right_Val_Enum
{
    Strafe_Right_Val_Off = 1,
    Strafe_Right_Val_On
}
Strafe_Right_Val;

/**
 * Message structure definition.
 *
 * Message bounds should always be set to 0xFF. Packet type should be
 * set to a Msg_Type. Values must be specified for every type except Hello,
 * Goodbye, Standby, and Ready.
 */
typedef
struct Msg_Struct
{
    char     c_front_bound; /**< Fill with PKT_BND */
    Msg_Type type;          /**< Type of packet */
    int      n_value;       /**< Relevant value for type of packet */
    char     c_end_bound;   /**< Fill with PKT_BND */
    char     c_null_term;   /**< Null terminator to ensure proper operation
                                      with string functions */
}
Msg;

/**
 * Message printing/logging function.
 */
void
print_log (char* s_log, Log_Level level);

/**
 * Packet-type to string conversion function.
 */
void
type_to_str (char* s_str, Msg_Type type);

/**
 * Quit BaseStation function.
 */
void
quit_basestation (void);
