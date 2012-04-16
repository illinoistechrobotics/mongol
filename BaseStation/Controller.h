#include "SDL/SDL.h"

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
