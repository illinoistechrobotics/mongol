#include "Serial.h"
#include "SDL/SDL.h"

#define TERMBUFSIZ 100

char termBuf [TERMBUFSIZ];

void waitForUser (){

	printf("Press enter to continue...");
	fgetc(stdin);
}

int main (int argc, char* argv[]){

    if(argc < 2){

        printf("ERROR: No port name given.\n");
        waitForUser();
        return -1;
    }

    if(initSerial(argv[1]) < 0){

        waitForUser();
        return -1;
    }

    waitForUser();

    printf("Connecting to robot... ");

    while(readSerial()[0] != HELLO);

    sayHello();

    printf("Connected!\n");

    // Initialize SDL (VIDEO flag also initializes event handling)

    if(SDL_Init(SDL_INIT_VIDEO) < 0){

        printf("ERROR: SDL Failed to initialize.\n");
        return -1;
    }

    SDL_Event curEvent, lastEvent;

    // Test keyboard press

    while(lastEvent.key.keysym.sym != 'q' && SDL_PollEvent(&curEvent)){

        lastEvent = curEvent;

        switch(curEvent.type){

            case SDL_KEYDOWN:
                printf("Key %d was pressed.\n", curEvent.key.keysym.sym);
                break;
                
            default:
                printf("Unknown event ocurred: %d\n", curEvent.type);
                break;
        }
    }

    waitForUser();

    SDL_Quit();

	return 0;
}
