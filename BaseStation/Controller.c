#include "BaseStation.h"
#include "Controller.h"

SDL_Event curEvent, lastEvent;
SDL_EventFilter mouseMotionFilter;

int eventFilter (const SDL_Event * event){

    switch(event->type){

    // Events to be posted to queue
    case SDL_KEYDOWN:
        return 1;
    }

    // All other events get dropped
    return 0;
}

int initCtrl (){
    
    if(!(SDL_Init(SDL_INIT_EVERYTHING))){

        SDL_SetEventFilter(eventFilter);
        SDL_SetVideoMode(200, 200, 0, 0);
        // SDL_WM_GrabInput(SDL_GRAB_ON);
        return -1;
    }

    return 0;
}

SDL_Event * getNextEvent (){

    if(SDL_PollEvent(&curEvent)){

        printEventInfo(&curEvent, SDL_KEYDOWN);
        if(curEvent.type == SDL_KEYDOWN &&
           (curEvent.key.keysym.sym == SDLK_q) && 
           (curEvent.key.keysym.mod & KMOD_CTRL)){

            quitBase();
        }
        return &curEvent;
    }

    return NULL;
}

void printEventInfo (SDL_Event * event, int flags){
            
    char typeStr [256];
    typeStr[0] = '\0';

    switch(event->type & flags){

    case SDL_ACTIVEEVENT:
        sprintf(typeStr, "ACTIVEEVENT\n");
        break;

    case SDL_KEYDOWN:
    case SDL_KEYUP:
        sprintf(typeStr, "KEYBOARDEVENT: Key: %c Modifier: %d\n",
                event->key.keysym.sym,
                event->key.keysym.mod);
        break;

    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        sprintf(typeStr, "MOUSEEVENT\n");
        break;

    case SDL_JOYAXISMOTION:
    case SDL_JOYBALLMOTION:
    case SDL_JOYHATMOTION:
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
        sprintf(typeStr, "JOYEVENT\n");
        break;

    case SDL_VIDEORESIZE:
    case SDL_VIDEOEXPOSE:
        sprintf(typeStr, "VIDEOEVENT\n");
        break;

    case SDL_QUIT:
        sprintf(typeStr, "QUITEVENT\n");
        break;

    case SDL_USEREVENT:
        sprintf(typeStr, "USEREVENT\n");
        break;

    case SDL_SYSWMEVENT:
        sprintf(typeStr, "SYSWMEVENT\n");
        break;
    }

    if(typeStr[0])
        printf("SDL Event Type: %s", typeStr);
}

void closeCtrl(){

    SDL_Quit();
}
