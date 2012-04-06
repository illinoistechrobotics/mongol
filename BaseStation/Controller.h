#include "SDL/SDL.h"

#define PRINT_ALL   (-1)

int initCtrl (void);

SDL_Event * getNextEvent (void);

void printEventType (SDL_Event * event, int flags);

void closeCtrl (void);
