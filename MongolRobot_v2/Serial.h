// Mongol header file for serial communication

#define SER_BUFSIZ 100
#define	PKT_BND	0xFF
#define PKT_FIL 0x55
#define PKT_LEN 4

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