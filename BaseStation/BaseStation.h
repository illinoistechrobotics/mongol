/**
 * \file BaseStation.h
 *
 * \brief Main header file for BaseStation.
 *
 * \details This header file contains the communication packet structure,
 * enumerations for packet components, and enumerations for different modes of
 * operation for the program.
 */

#include <stdio.h>

typedef char byte;

/**
 * \brief Packet structure definition.
 *
 * \details Packet bounds should always be set to 0xFF. Packet type should be
 * set to a PKT_Type. Values must be specified for every type except HELLO,
 * GDBY, STDBY, and RDY.
 */
typedef struct packet_struct
{
    byte front_bnd;         /**< Fill with PKT_BND */
    PKT_type type;          /**< Type of packet */
    int value;              /**< Relevant value for type of packet */
    byte end_bnd;           /**< Fill with PKT_BND */
    byte null_term;         /**< Null terminator to ensure proper operation with string functions */
} packet;

/**
 * \brief Available packet types.
 *
 * \details These are the different types of packets available for
 * communication with the robot. PKT_HELLO, PKT_GDBY, PKT_STDBY, and PKT_RDY
 * are used to regulate communication and do not need corresponding values.
 */
enum PKT_type
{
    PKT_HELLO = 1,          /**< Hello packet: used to initialize
                              communications */
    PKT_GDBY,               /**< Goodbye packet: used to close connections */
    PKT_STDBY,              /**< Standby packet: no robot updates (most common) */
    PKT_MOVE,               /**< Move packet: update moving */
    PKT_TURN,               /**< Turn packet: update turning */
    PKT_AIM_H,              /**< Horizontal aim packet: update horizontal aim
                              adjustment */
    PKT_AIM_V,              /**< Vertical aim packet: update vertical aim adjustment */
    PKT_FIRE,               /**< Fire packet: update gun-firing status */
    PKT_STRF_L,             /**< Strafe packet: update left strafe status */
    PKT_STRF_R,             /**< Strafe packet: update right strafe status */
    PKT_RDY                 /**< Ready packet: indication that robot is ready
                              for next packet */
};

/**
 * \brief Controller mode values.
 *
 * \details This enumeration specifies possible modes for controlling the
 * robot.
 */
enum CTRL_mode
{
    CTRL_KEYBOARD,          /**< Use keyboard keys to control robot */
    CTRL_GAMEPAD            /**< Use gamepad's joystick and buttons to control robot */
};

/**
 * BaseStation-to-Robot communication mode.
 *
 * This enumeration specifies possible modes for communicating with the robot.
 */
enum COMM_mode
{
    COMM_ONLINE,            /**< Attempt to establish connection to robot */
    COMM_OFFLINE            /**< Do not attempt to establish connection to robot */
};

/**
 * User interface mode.
 *
 * This enumeration specifies modes for command-line interface or graphical
 * interface.
 */
enum UI_mode
{
    UI_CLINE,               /**< Do not initialize GUI, stay in command line. */
    UI_GRAPH                /**< Initialize GUI. */
};

/**
 * Error and warning printing mode.
 *
 * This enumeration specifies modes for quiet or verbose modes for reporting
 * minor errors and warnings during runtime.
 */
enum PRINT_mode
{
    PRNT_QUIET,             /**< Do not print messages about minor communication and input errors. */
    PRNT_VERB               /**< Print messages about minor communication and input errors. */
};

/**
 * Robot movement packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_MOVE
 * that will direct the robot to move either forwards or backwards or not at
 * all.
 */
enum MOV_val
{
    MOV_STOP = 1,           /**< Do not move the robot in any direction */
    MOV_FWD,                /**< Move the robot forward */
    MOV_BKD                 /**< Move the robot backward */
};

/**
 * Robot turning packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_TURN
 * that will direct the robot to turn to the left or the right or not at all.
 */
enum TRN_val
{
    TRN_NONE = 1,           /**< Do not turn the robot in any direction */
    TRN_LEFT,               /**< Turn the robot to the left */
    TRN_RIGHT               /**< Turn the robot to the right */
};

/**
 * Horizontal robot turrent aim adjustment packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_AIM_H
 * that will direct the robot to turn its turrent to either the left or right
 * or not at all.
 */
enum AIM_H_val
{
    AIM_H_STRGHT = 1,       /**< Do not turn the turret in any direction */
    AIM_H_LEFT,             /**< Turn the turret to the left */
    AIM_H_RIGHT             /**< Turn the turret to the right */
};

/**
 * Horizontal robot turrent aim adjustment packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_AIM_V
 * that will direct the robot to raise, lower, or keep still its turret.
 */
enum AIM_V_val
{
    AIM_V_STRGHT = 1,       /**< Do not raise or lower the turret */
    AIM_V_DWN,              /**< Lower the turret */
    AIM_V_UP                /**< Raise the turret */
};

/**
 * Robot turret firing mode values.
 *
 * This enumeration provides possible values for a packet of type PKT_FIRE
 * that will direct the robot to either fire or not fire the gun on its
 * turret.
 */
enum FIRE_val
{
    FIRE_ON = 1,            /**< Fire the gun */
    FIRE_OFF                /**< Do not fire the gun */
};

/**
 * Robot left-strafing packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_STRF_L
 * that will direct the robot to strafe to the left or not.
 */
enum STRF_L_val
{
    STRF_L_OFF = 1,
    STRF_L_ON
};

/**
 * Robot right-strafing packet values.
 *
 * This enumeration provides possible values for a packet of type PKT_STRF_L
 * that will direct the robot to strafe to the right or not.
 */
enum STRF_R_val
{
    STRF_R_OFF = 1,
    STRF_R_ON
};

/**
 * Message printing buffer
 *
 * \todo Make private so that only printmsg() can access.
 */
char termbuf[ BUFSIZ ];

/**
 * \brief Message printing/logging function.
 */
void printmsg( void );

/**
 * \brief Packet-type to string conversion function.
 */
void type_to_str( char* str, PKT_type type );

/**
 * \brief Quit BaseStation function.
 */
void quit_base( void );
