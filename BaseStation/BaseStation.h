// Main header file for BaseStation

#include <stdio.h>

// Matter of preference
typedef char byte;

// Packet structure definition
typedef struct packet_struct{
    byte front_bnd;
    byte type;              // Type of packet
    byte value;             // Relevant value of type (null for HELLO, GDBY, and STDBY)
    byte end_bnd;
} packet;

// Packet types
enum{
    PKT_HELLO,              // Hello packet: used to initialize communications
    PKT_GDBY,               // Goodbye packet: used to close connections
    PKT_STDBY,              // Standby packet: no robot updates (most common)
    PKT_MOVE,               // Move packet: update moving
    PKT_TURN,               // Turn packet: update turning
    PKT_AIM_H,              // Horizontal aim packet: update horizontal aim adjustment
    PKT_AIM_V,              // Vertical aim packet: update vertical aim adjustment
    PKT_FIRE,               // Fire packet: update gun-firing status
    PKT_STRF_L,             // Strafe packet: update left strafe status
    PKT_STRF_R,             // Strafe packet: update right strafe status
    PKT_RDY                 // Ready packet: indication that robot is ready for next packet
};

// Controller mode variable and values
int ctrlmode;
enum{
    KEYBOARD,               // Use keyboard keys to control robot
    GAMEPAD                 // Use gamepad's joystick and buttons to control robot
};

// Base-Robot communication mode variable and values
int commMode;
enum{
    ONLINE,                 // Attempt to establish connection to robot
    OFFLINE                 // Do not attempt to establish connection to roobt
};

// Interface type variable and values
int uiMode;
enum{
    CMD_LINE,               // Do not initialize GUI
    GRAPH_UI                // Initialize GUI
};

// Verbose mode variable and values
int printMode;
enum{
    QUIET,                  // Do not print messages about minor communication and input errors
    VERBOSE                 // Print messages about minor communication and input errors
};

// Movement mode values
enum{
    MOV_STOP,               // Joystick vertical center
    MOV_FWD,
    MOV_BKD
};

// Turning mode values
enum{
    TRN_NONE,               // Joystick horizontal center
    TRN_LEFT,
    TRN_RIGHT
};

// Horizontal aiming mode values
enum{
    AIM_H_STRGHT,
    AIM_H_LEFT,
    AIM_H_RIGHT
};

// Vertical aiming mode values
enum{
    AIM_V_STRGHT,
    AIM_V_DWN,
    AIM_V_UP
};

// Firing mode values
enum{
    FIRE_ON,
    FIRE_OFF
};

// Left-strafing mode values
enum{
    STRF_L_OFF,
    STRF_L_ON
};

// Right-strafing mode values
enum{
    STRF_R_OFF,
    STRF_R_ON
};

// Message printing function and buffer
char termbuf [BUFSIZ];
void printmsg (void);

// Exit function
void quit_base (void);
