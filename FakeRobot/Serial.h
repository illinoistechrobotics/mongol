#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <termios.h>

#define	PKT_BND	0xFF
#define VAL_NUL 0x55        // Value filler for hello, standby, goodbye, and ready packets

// Matter of preference
typedef char byte;

// Packet structure definition
typedef struct packet_struct{
    byte front_bnd;
    byte type;              // Type of packet
    byte value;             // Relevant value of type (null for HELLO, GDBY, and STDBY)
    byte end_bnd;
    byte null_term;         // Null terminator to ensure proper operation with string functions
} packet;

// Packet types
enum{
    PKT_HELLO = 1,          // Hello packet: used to initialize communications
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
    MOV_STOP = 1,           // Joystick vertical center
    MOV_FWD,
    MOV_BKD
};

// Turning mode values
enum{
    TRN_NONE = 1,           // Joystick horizontal center
    TRN_LEFT,
    TRN_RIGHT
};

// Horizontal aiming mode values
enum{
    AIM_H_STRGHT = 1,
    AIM_H_LEFT,
    AIM_H_RIGHT
};

// Vertical aiming mode values
enum{
    AIM_V_STRGHT = 1,
    AIM_V_DWN,
    AIM_V_UP
};

// Firing mode values
enum{
    FIRE_ON = 1,
    FIRE_OFF
};

// Left-strafing mode values
enum{
    STRF_L_OFF = 1,
    STRF_L_ON
};

// Right-strafing mode values
enum{
    STRF_R_OFF = 1,
    STRF_R_ON
};

int printAll;   // Flag to control output of serial communication errors

int init_serial(char *port);

packet * read_serial();

int write_serial(packet * msg);

void close_serial();

void say_hello();

void say_ready();
