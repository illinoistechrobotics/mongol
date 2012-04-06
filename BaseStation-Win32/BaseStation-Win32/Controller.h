#include "SDL.h"

int initSDL (void);

SDL_Event * getNextEvent (void);

void printEventType (SDL_Event * event);

void closeSDL (void);