#include "BaseStation.h"
#include "Controller.h"

SDL_Event curEvent, lastEvent;
SDL_EventFilter mouseMotionFilter;

int eventFilter (const SDL_Event * event){

    if((event->type == SDL_KEYDOWN) ||
       (event->type == SDL_KEYUP)){

        return 1;
    }
    else if((ctrlmode == GAMEPAD) &&
            ((event->type == SDL_JOYAXISMOTION) ||
             (event->type == SDL_JOYBUTTONDOWN) ||
             (event->type == SDL_JOYBUTTONUP))){

        return 1;
    }
    else
        return 0;
}

int initCtrl (){
    
    if(!(SDL_Init(SDL_INIT_EVERYTHING))){

        SDL_SetEventFilter(eventFilter);
        SDL_SetVideoMode(200, 200, 0, 0);
        // SDL_WM_GrabInput(SDL_GRAB_ON);
        
        // If gamepad mode specified, open gamepad
        if(ctrlmode == GAMEPAD){

            int num_pads;

            // If more than one joystick attached, let user choose which one
            if((num_pads = SDL_NumJoysticks()) > 1){

                sprintf(termbuf, "Select gamepad index (number from 0 to %d): ", (num_pads-1));
                printmsg();
                while((pad_index = fgetc(stdin)) == EOF);
            }

            if((pad = SDL_JoystickOpen(pad_index))){

                sprintf(termbuf, "%s opened successfully.\n",
                        SDL_JoystickName(pad_index));
                printmsg();
                SDL_JoystickEventState(SDL_ENABLE);
            }
            else{

                sprintf(termbuf, "Gamepad %s not opened successfully.\nReverting to keyboard control\n",
                        SDL_JoystickName(pad_index));
                printmsg();
                ctrlmode = KEYBOARD;
            }
        }

        return -1;
    }

    return 0;
}

SDL_Event * getNextEvent (){

    if(SDL_PollEvent(&curEvent)){

        printEventInfo(&curEvent, 
                       (GAMEPAD ? (SDL_JOYAXISMOTION |
                                   SDL_JOYBALLMOTION |
                                   SDL_JOYHATMOTION |
                                   SDL_JOYBUTTONUP |
                                   SDL_JOYBUTTONDOWN) :
                                  (SDL_KEYDOWN |
                                   SDL_KEYUP)));
                 
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
            
    termbuf[0] = '\0';

    switch(event->type & flags){

    case SDL_ACTIVEEVENT:
        sprintf(termbuf, "ACTIVEEVENT\n");
        break;

    case SDL_KEYDOWN:
    case SDL_KEYUP:
        sprintf(termbuf, "KEYBOARDEVENT: Key: %c Modifier: %d\n",
                event->key.keysym.sym,
                event->key.keysym.mod);
        break;

    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        sprintf(termbuf, "MOUSEEVENT\n");
        break;

    case SDL_JOYAXISMOTION:
        sprintf(termbuf, "JOYAXISMOTION: Axis: %d, Value: %d",
                (event->jaxis.axis),
                (event->jaxis.value));
        break;
    // case SDL_JOYBALLMOTION:
    // case SDL_JOYHATMOTION:
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
        sprintf(termbuf, "JOYBUTTON: Button: %d, State: %d\n",
                (event->jbutton.button),
                (event->jbutton.state));
        break;

    case SDL_VIDEORESIZE:
    case SDL_VIDEOEXPOSE:
        sprintf(termbuf, "VIDEOEVENT\n");
        break;

    case SDL_QUIT:
        sprintf(termbuf, "QUITEVENT\n");
        break;

    case SDL_USEREVENT:
        sprintf(termbuf, "USEREVENT\n");
        break;

    case SDL_SYSWMEVENT:
        sprintf(termbuf, "SYSWMEVENT\n");
        break;
    }

    if(termbuf[0])
        printf("SDL Event: %s", termbuf);
}

void closeCtrl(){

    if(ctrlmode == GAMEPAD)
        SDL_JoystickClose(pad);

    SDL_Quit();
}
