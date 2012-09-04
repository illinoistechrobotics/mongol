#include <SDL/SDL.h>

// Joystick mode thresholds
#define JOY_THRESH_UP       -16384
#define JOY_THRESH_LEFT     -16384
#define JOY_THRESH_RIGHT    16383
#define JOY_THRESH_DOWN     16383
#define TRIG_THRESH         0

// XBOX gamepad button index
typedef
enum XBOX_Button_Enum{
    XBTN_A,
    XBTN_B,
    XBTN_X,
    XBTN_Y,
    XBTN_LBUMP,
    XBTN_RBUMP
}
XBOX_Button;

// XBOX joystick/button axis index
typedef
enum XBOX_Joystick_Enum{
    XLSTICK_X,
    XLSTICK_Y,
    XLTRIG,
    XRSTICK_X,
    XRSTICK_Y,
    XRTRIG
}
XBOX_Joystick;

// Gamepad index variable
int pad_index;

// Gamepad SDL struct pointer
SDL_Joystick* pad;

int
ctrl_init (void);

void
ctrl_next_event (Msg* next_msg);

void
ctrl_print_info (SDL_Event * event, int flags);

void
ctrl_close (void);
