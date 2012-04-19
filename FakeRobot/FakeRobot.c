#include "Serial.h"

#define TERMBUFSIZ 100

char termBuf [TERMBUFSIZ];
char * dev;
int printMode;
packet * inpacket;
packet outpacket;

void waitForUser (){

    printf("Press enter to continue...\n");
    fgetc(stdin);
}

// Function to parse command line arguements
void parseArgs (int argc, char * argv[]){

    if(argc < 2){

        printf("ERROR: No port name given.\n");
        exit(-1);
    }
    else{

        dev = argv[1];
    }

    int i;
    for(i = 2; i < argc-1; i ++){

        switch(argv[i][1]){ // Get character after dash

            case 'v':
                printMode = 1;
                break;
        }
    }
}

void reconnect(){

    // Connection loop
    printf("Spamming \"Hello\"...\n");
    do{
        say_hello();
        sleep(1);

    } while((inpacket = read_serial()) &&
            ((inpacket->type) == PKT_HELLO));
    printf("Received \"Hello\"!\n");
    say_ready();
}

void type2str(char * buf, byte type){
    
    switch(type){
        case PKT_HELLO : sprintf(buf, "HELLO");  break;
        case PKT_GDBY  : sprintf(buf, "GDBY");   break;
        case PKT_STDBY : sprintf(buf, "STDBY");  break;
        case PKT_MOVE  : sprintf(buf, "MOVE");   break;
        case PKT_TURN  : sprintf(buf, "TURN");   break;
        case PKT_AIM_H : sprintf(buf, "AIM_H");  break;
        case PKT_AIM_V : sprintf(buf, "AIM_V");  break;
        case PKT_FIRE  : sprintf(buf, "FIRE");   break;
        case PKT_STRF_L: sprintf(buf, "STRF_L"); break;
        case PKT_STRF_R: sprintf(buf, "STRF_R"); break;
        case PKT_RDY   : sprintf(buf, "RDY");    break;
    }
}

int main (int argc, char * argv[]){

    // Set initial values
    printMode = 0;

    // Parse command line arguements
    parseArgs(argc, argv);

    printf("FakeRobot started.\n");
    // Initialize serial port
    if(init_serial(dev) < 0)
        return -1;

    char type_str [100];
    char value_str [100];

    // Main loop
    for(;;){

        inpacket = read_serial();
        sprintf(type_str, "Unknown");
        sprintf(value_str, "Unknown");
        type2str(type_str, inpacket->type);
        printf("Received: Type:%s ", type_str);

        switch(inpacket->type){

            case PKT_HELLO:
            case PKT_GDBY:
            case PKT_STDBY:
            case PKT_RDY:
                break;

            case PKT_MOVE:
                switch(inpacket->value){

                    case MOV_STOP: sprintf(value_str, "STOP"); break;
                    case MOV_FWD:  sprintf(value_str, "FWD"); break;
                    case MOV_BKD:  sprintf(value_str, "BKD"); break;
                }
                printf("Direction:%s", value_str);
                break;

            case PKT_TURN:
                switch(inpacket->value){

                    case TRN_NONE:  sprintf(value_str, "NONE"); break;
                    case TRN_LEFT:  sprintf(value_str, "LEFT"); break;
                    case TRN_RIGHT: sprintf(value_str, "RIGHT"); break;
                }
                printf("Direction:%s", value_str);
                break;

            case PKT_AIM_H:
                switch(inpacket->value){

                    case AIM_H_STRGHT: sprintf(value_str, "STRGHT"); break;
                    case AIM_H_LEFT:   sprintf(value_str, "LEFT"); break;
                    case AIM_H_RIGHT:  sprintf(value_str, "RIGHT"); break;
                }
                printf("Direction:%s", value_str);
                break;

            case PKT_AIM_V:
                switch(inpacket->value){

                    case AIM_V_STRGHT: sprintf(value_str, "STRGHT"); break;
                    case AIM_V_DWN:    sprintf(value_str, "DWN"); break;
                    case AIM_V_UP:     sprintf(value_str, "UP"); break;
                }
                printf("Direction:%s", value_str);
                break;

            case PKT_FIRE:
                switch(inpacket->value){

                    case FIRE_ON:  sprintf(value_str, "ON"); break;
                    case FIRE_OFF: sprintf(value_str, "OFF"); break;
                }
                printf("Firing:%s", value_str);
                break;

            case PKT_STRF_L:
                switch(inpacket->value){

                    case STRF_L_OFF: sprintf(value_str, "OFF"); break;
                    case STRF_L_ON:  sprintf(value_str, "ON"); break;
                }
                printf("Strafing:%s", value_str);
                break;
            
            case PKT_STRF_R:
                switch(inpacket->value){

                    case STRF_R_OFF: sprintf(value_str, "OFF"); break;
                    case STRF_R_ON:  sprintf(value_str, "ON"); break;
                }
                printf("Strafing:%s", value_str);
                break;
        }
        printf("\n");

        say_ready();
        printf("Sent: Type:RDY\n");
    }
}
