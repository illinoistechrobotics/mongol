#include "SDL/SDL.h"

#define PRINT_ALL   0xFFFF 

int initCtrl (void);

SDL_Event * getNextEvent (void);

void printEventInfo (SDL_Event * event, int flags);

void closeCtrl (void);
