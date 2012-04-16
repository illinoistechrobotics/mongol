#include "SDL/SDL.h"

// Robot Turning Directions
enum{
    L_TURN =    1,
    R_TURN =    2
};

// Gun Turning Directions
enum{
    UP =        0x01,
    LEFT =      0x02,
    RIGHT =     0x04,
    DOWN =      0x08
};

// Actions
enum{
    GO_FWD =    0x01,       // Move forward in current direction
    GO_BKD =    0x02,       // Move backward in current direction
    STRAFE =    0x04,       // Prompt robot to strafe instead of turn
    TURBO =     0x08,       // Speed-up movement
    FIRE =      0x10,       // Fire turret
    SIEGE =     0x20        // Toggle siege mode
};

// Current joystick mode variables and values

// Joystick mode thresholds
#define JOY_THRESH_UP       -16384
#define JOY_THRESH_LEFT     -16384
#define JOY_THRESH_RIGHT    16383
#define JOY_THRESH_DOWN     16383
#define TRIG_THRESH         0

// XBOX gamepad button index
enum{
    XBTN_A,
    XBTN_B,
    XBTN_X,
    XBTN_Y,
    XBTN_LBUMP,
    XBTN_RBUMP
};

// XBOX joystick/button axis index
enum{
    XLSTICK_X,
    XLSTICK_Y,
    XLTRIG,
    XRSTICK_X,
    XRSTICK_Y,
    XRTRIG
};

// Gamepad index variable
int pad_index;

// Gamepad SDL struct pointer
SDL_Joystick * pad;

int initCtrl (void);

void next_event (packet * next_pkt);

void printEventInfo (SDL_Event * event, int flags);

void close_ctrl (void);
