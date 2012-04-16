#include "BaseStation.h"
#include "Controller.h"

SDL_Event curEvent, lastEvent;
byte cur_move;              // Current movement mode variable
byte cur_turn;              // Current turning mode variable
byte cur_h_aim;             // Current horizontal aim mode variable
byte cur_v_aim;             // Current vertical aim mode variable
byte cur_fire;              // Current firing mode variable
byte cur_l_strf;            // Current left-strafing mode variable
byte cur_r_strf;            // Current right-strafing mode variable

int eventFilter (const SDL_Event * event){

    if((event->type == SDL_KEYDOWN) ||
       (event->type == SDL_KEYUP)){

        return 1;
    }
    else if((ctrlmode == GAMEPAD) &&
            ((event->type == SDL_JOYAXISMOTION) ||
             (event->type == SDL_JOYBUTTONDOWN) ||
             (event->type == SDL_JOYBUTTONUP))){

        // Filtering of joystick movements
        if(event->type == SDL_JOYAXISMOTION){

            SDL_JoyAxisEvent jevent = event->jaxis;

            switch(jevent.axis){

                case XLSTICK_X:
                    if((jevent.value < JOY_THRESH_LEFT) &&
                       (cur_turn != TRN_LEFT))
                        return 1;

                    else if((jevent.value > JOY_THRESH_RIGHT) &&
                            (cur_turn != TRN_RIGHT))
                        return 1;

                    else if((jevent.value > JOY_THRESH_LEFT) &&
                            (jevent.value < JOY_THRESH_RIGHT) &&
                            (cur_turn != TRN_NONE))
                        return 1;

                    break;

                case XLSTICK_Y:
                    if((jevent.value > JOY_THRESH_DOWN) &&
                       (cur_move != MOV_BKD))
                        return 1;

                    else if((jevent.value < JOY_THRESH_UP) &&
                            (cur_move != MOV_FWD))
                        return 1;

                    else if((jevent.value < JOY_THRESH_DOWN) &&
                            (jevent.value > JOY_THRESH_UP) &&
                            (cur_move != MOV_STOP))
                        return 1;
                
                    break;

                case XRSTICK_X:
                    if((jevent.value < JOY_THRESH_LEFT) &&
                       (cur_h_aim != AIM_H_LEFT))
                        return 1;

                    else if((jevent.value > JOY_THRESH_RIGHT) &&
                            (cur_h_aim != AIM_H_RIGHT))
                        return 1;

                    else if((jevent.value > JOY_THRESH_LEFT) &&
                            (jevent.value < JOY_THRESH_RIGHT) &&
                            (cur_h_aim != AIM_H_STRGHT))
                        return 1;

                    break;

                case XRSTICK_Y:
                    if((jevent.value > JOY_THRESH_DOWN) &&
                       (cur_v_aim != AIM_V_DWN))
                        return 1;

                    else if((jevent.value < JOY_THRESH_UP) &&
                            (cur_v_aim != AIM_V_UP))
                        return 1;

                    else if((jevent.value < JOY_THRESH_DOWN) &&
                            (jevent.value > JOY_THRESH_UP) &&
                            (cur_v_aim != AIM_V_STRGHT))
                        return 1;

                    break;

                case XRTRIG:
                    if((jevent.value < TRIG_THRESH) &&
                       (cur_fire != FIRE_OFF))
                        return 1;
                    
                    else if((jevent.value > TRIG_THRESH) &&
                            (cur_fire != FIRE_ON))
                        return 1;

                    break;

                default:
                    return 0;
            }

            return 0;
        }

        // Filtering of joystick buttons
        else if((event->type == SDL_JOYBUTTONUP) ||
                (event->type == SDL_JOYBUTTONDOWN)){

            SDL_JoyButtonEvent bevent = event->jbutton;

            switch(bevent.button){

                case XBTN_LBUMP:
                case XBTN_RBUMP:
                    return 1;

                default:
                    return 0;

            }
        }

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
                
                // Set initial modes
                cur_move = MOV_STOP;
                cur_turn = TRN_NONE;
                cur_h_aim = AIM_H_STRGHT;
                cur_v_aim = AIM_V_STRGHT;
                cur_fire = FIRE_OFF;
                cur_l_strf = STRF_L_OFF;
                cur_r_strf = STRF_R_OFF;

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

void next_event (packet * next_pkt){

    if(SDL_PollEvent(&curEvent)){

        if(printMode == VERBOSE){

            printEventInfo(&curEvent, 
                           ((ctrlmode == GAMEPAD) ?
                            (SDL_JOYAXISMOTION |
                             SDL_JOYBUTTONUP |
                             SDL_JOYBUTTONDOWN |
                             SDL_KEYDOWN |
                             SDL_KEYUP) :
                            (SDL_KEYDOWN |
                             SDL_KEYUP)));
        }

        if(curEvent.type == SDL_JOYAXISMOTION){

            SDL_JoyAxisEvent jevent = curEvent.jaxis;

            switch(jevent.axis){

                case XLSTICK_X:

                    if(jevent.value < JOY_THRESH_LEFT)
                        cur_turn = TRN_LEFT;
                    
                    else if(jevent.value > JOY_THRESH_RIGHT)
                        cur_turn = TRN_RIGHT;

                    else
                        cur_turn = TRN_NONE;

                    next_pkt->type = PKT_TURN;
                    next_pkt->value = cur_turn;
                    break;

                case XLSTICK_Y:

                    if(jevent.value > JOY_THRESH_DOWN)
                        cur_move = MOV_BKD;

                    else if(jevent.value < JOY_THRESH_UP)
                        cur_move = MOV_FWD;

                    else
                        cur_move = MOV_STOP;

                    next_pkt->type = PKT_MOVE;
                    next_pkt->value = cur_move;
                    break;
                
                case XRSTICK_X:

                    if(jevent.value < JOY_THRESH_LEFT)
                        cur_h_aim = AIM_H_LEFT;
                    
                    else if(jevent.value > JOY_THRESH_RIGHT)
                        cur_h_aim = AIM_H_RIGHT;

                    else
                        cur_h_aim = AIM_H_STRGHT;

                    next_pkt->type = PKT_AIM_H;
                    next_pkt->value = cur_h_aim;
                    break;

                case XRSTICK_Y:

                    if(jevent.value > JOY_THRESH_DOWN)
                        cur_v_aim = AIM_V_DWN;
                    
                    else if(jevent.value < JOY_THRESH_UP)
                        cur_v_aim = AIM_V_UP;

                    else
                        cur_v_aim = AIM_V_STRGHT;

                    next_pkt->type = PKT_AIM_V;
                    next_pkt->value = cur_v_aim;
                    break;

                case XRTRIG:

                    if(jevent.value < TRIG_THRESH)
                        cur_fire = FIRE_OFF;

                    else
                        cur_fire = FIRE_ON;

                    next_pkt->type = PKT_FIRE;
                    next_pkt->value = cur_fire;
                    break;
            }
        }

        else if((curEvent.type == SDL_JOYBUTTONDOWN) &&
                (curEvent.type == SDL_JOYBUTTONUP)){

            SDL_JoyButtonEvent bevent = curEvent.jbutton;

            switch(bevent.button){

                case XBTN_LBUMP:

                    if(bevent.state == SDL_PRESSED)
                        cur_l_strf = STRF_L_ON;

                    else
                        cur_l_strf = STRF_L_OFF;

                    next_pkt->type = PKT_STRF_L;
                    next_pkt->value = cur_l_strf;
                    break;

                case XBTN_RBUMP:

                    if(bevent.state == SDL_PRESSED)
                        cur_r_strf = STRF_R_ON;

                    else
                        cur_r_strf = STRF_R_OFF;

                    next_pkt->type = PKT_STRF_R;
                    next_pkt->value = cur_r_strf;
                    break;
            }
        }
                 
        else if(curEvent.type == SDL_KEYDOWN &&
           (curEvent.key.keysym.sym == SDLK_q) && 
           (curEvent.key.keysym.mod & KMOD_CTRL)){

            quit_base();
        }
    }
    else                                // If no pending events
        next_pkt->type = PKT_STDBY;
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
        sprintf(termbuf, "JOYAXISMOTION: Axis: %d, Value: %d\n",
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

void close_ctrl(){

    if(ctrlmode == GAMEPAD)
        SDL_JoystickClose(pad);

    SDL_Quit();
}
