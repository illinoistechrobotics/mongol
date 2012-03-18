#include "SDL/SDL.h"
#include "Serial.h"

#define TERMBUFSIZ 100

char termBuf [TERMBUFSIZ];
char * dev;
int printMode;

void waitForUser (){

	printf("Press enter to continue...");
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

int main (int argc, char* argv[]){

    // Set initial values
    printMode = 0;

    // Parse command line arguements
    parseArgs(argc, argv);

    waitForUser();

    printf("Connecting to robot... ");
    
    // Initialize serial port
    if(initSerial(dev, printMode) < 0){

        return -1;
    }

    char msg;

    while(!(msg = readSerial()[0]) && (msg != HELLO));

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
