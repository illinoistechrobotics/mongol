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
 * Packet structure definition.
 *
 * Packet bounds should always be set to 0xFF. Packet type should be
 * set to a PKT_Type. Values must be specified for every type except HELLO,
 * GDBY, STDBY, and RDY.
 */
typedef
struct Msg_Struct
{
    byte     c_front_bound; /**< Fill with PKT_BND */
    Msg_Type type;          /**< Type of packet */
    int      n_value;       /**< Relevant value for type of packet */
    byte     c_end_bound;   /**< Fill with PKT_BND */
    byte     c_null_term;   /**< Null terminator to ensure proper operation
                                 with string functions */
}
Msg;

/**
 * Available packet types.
 *
 * These are the different types of packets available for
 * communication with the robot. PKT_HELLO, PKT_GDBY, PKT_STDBY, and PKT_RDY
 * are used to regulate communication and do not need corresponding values.
 */
enum Msg_Type
{
    Msg_Type_Hello = 1,    /**< Hello packet: used to initialize
                                communications */
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
};

/**
 * Controller mode values.
 *
 * This enumeration specifies possible modes for controlling the
 * robot.
 */
enum Ctrl_Mode
{
    Ctrl_Mode_Keyboard, /**< Use keyboard keys to control robot */
    Ctrl_Mode_Gamepad   /**< Use gamepad's joystick and buttons to control robot */
};

/**
 * BaseStation-to-Robot communication mode.
 *
 * This enumeration specifies possible modes for communicating with the robot.
 */
enum Comm_Mode
{
    Comm_Mode_Online, /**< Attempt to establish connection to robot */
    Comm_Mode_Offline /**< Do not attempt to establish connection to robot */
};

/**
 * User interface mode.
 *
 * This enumeration specifies modes for command-line interface or graphical
 * interface.
 */
enum UI_Mode
{
    UI_Mode_Cmd_Line, /**< Do not initialize GUI, stay in command line. */
    UI_Mode_Graphical /**< Initialize GUI. */
};

/**
 * Error and warning printing mode.
 *
 * This enumeration specifies modes for quiet or verbose modes for reporting
 * minor errors and warnings during runtime.
 */
enum Warn_Mode
{
    Warn_Mode_Quiet, /**< Do not print messages about minor communication and
                          input errors. */
    Warn_Mode_All    /**< Print messages about minor communication and input
                          errors. */
};

/**
 * Robot movement packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_MOVE
 * that will direct the robot to move either forwards or backwards or not at
 * all.
 */
enum Move_Val
{
    Move_Val_Stop = 1, /**< Do not move the robot in any direction */
    Move_Val_Forward,  /**< Move the robot forward */
    Move_Val_Backward  /**< Move the robot backward */
};

/**
 * Robot turning packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_TURN
 * that will direct the robot to turn to the left or the right or not at all.
 */
enum Turn_Val
{
    Turn_Val_None = 1, /**< Do not turn the robot in any direction */
    Turn_Val_Left,     /**< Turn the robot to the left */
    Turn_Val_Right     /**< Turn the robot to the right */
};

/**
 * Horizontal robot turrent aim adjustment packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_AIM_H
 * that will direct the robot to turn its turrent to either the left or right
 * or not at all.
 */
enum Aim_Horz_Val
{
    Aim_Horz_Val_Straight = 1, /**< Do not turn the turret in any direction */
    Aim_Horz_Val_Left,         /**< Turn the turret to the left */
    Aim_Horz_Val_Right         /**< Turn the turret to the right */
};

/**
 * Horizontal robot turrent aim adjustment packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_AIM_V
 * that will direct the robot to raise, lower, or keep still its turret.
 */
enum Aim_Vert_Val
{
    Aim_Vert_Val_Straight = 1, /**< Do not raise or lower the turret */
    Aim_Vert_Val_Down,         /**< Lower the turret */
    Aim_Vert_Val_Up            /**< Raise the turret */
};

/**
 * Robot turret firing mode values.
 *
 * This enumeration provides possible values for a packet of type PKT_FIRE
 * that will direct the robot to either fire or not fire the gun on its
 * turret.
 */
enum Fire_Val
{
    Fire_Val_On = 1, /**< Fire the gun */
    Fire_Val_Off     /**< Do not fire the gun */
};

/**
 * Robot left-strafing packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_STRF_L
 * that will direct the robot to strafe to the left or not.
 */
enum Strafe_Left_Val
{
    Strafe_Left_Val_Off = 1,
    Strafe_Left_Val_On
};

/**
 * Robot right-strafing packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_STRF_L
 * that will direct the robot to strafe to the right or not.
 */
enum Strafe_Right_Val
{
    Strafe_Right_Val_Off = 1,
    Strafe_Right_Val_On
};

/**
 * Message printing buffer
 *
 * \todo Make private so that only printmsg() can access.
 */
char
terminal_buf [BUFSIZ];

/**
 * Message printing/logging function.
 */
void
print_to_terminal (void);

/**
 * Packet-type to string conversion function.
 */
void
type_to_str (char* str, PKT_type type);

/**
 * Quit BaseStation function.
 */
void
quit_basestation (void);
