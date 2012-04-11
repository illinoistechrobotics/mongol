#include "SDL/SDL.h"

int initCtrl (void);

SDL_Event * getNextEvent (void);

void printEventInfo (SDL_Event * event, int flags);

void closeCtrl (void);
