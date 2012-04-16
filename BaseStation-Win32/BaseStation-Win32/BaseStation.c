#include "GUI.h"
#include "Controller.h"
#include "Serial.h"

#define TERMBUFSIZ  100

#define QUIET       0
#define VERBOSE     1

char termBuf [TERMBUFSIZ];
char * dev;
int printMode = QUIET;

void waitForUser (){

	printf("Press enter to continue...");
	fgetc(stdin);
}

// Function to parse command line arguements
void parseArgs (int argc, char * argv[]){

    int a;

    for(a = 1; a < argc; a ++){

        // Get character after dash
        switch(argv[a][1]){

        // Port specifier flag (-d): defaults to COM1 if not used (for Windows)
        case 'd':
            dev = argv[++a];
            printf("Port specified: %s\n", dev);
            break;

        // Joystick mode flag (-j)

        // GUI mode flag (-g)
     
        // Verbose mode flag (-v): prints all non-critical error messages
        case 'v':  
            printMode = VERBOSE;    
            printf("Verbose mode activated.\n");    
            break;

        // Print help flag (-h)
        case 'h':
            printf("Robotic Control BaseStation Program\n");
            printf("Written by Allen Baker for Illinois Tech Robotics\n\n");
            printf("*** ARGS ***\n");
            printf("-d <port name>  : Specify port to use (default is COM1)\n");
            printf("-v              : Set verbose mode\n");
            printf("-h              : Print help information\n");
            exit(0);
            break;
        }
    }
}

int main (int argc, char* argv[]){

    // Parse command line arguements
    parseArgs(argc, argv);

    // Initialize SDL
    printf("Initializing SDL... ");
    if(!(initSDL())){

        printf("ERROR: SDL Failed to initialize.\n");
        waitForUser();
        exit(-1);
    }
    printf(" SDL Initialized.\n");
    waitForUser();

    printf("Connecting to robot...\n");
    
    // Initialize serial port (includes looking for HELLO packet)
    // If no port name specified, default to COM1 (for windows)
    if((dev ? initSerial(dev, printMode) : initSerial("COM1", printMode)) < 0){

        exit(-1);
    }

    sayHello();

    printf("Connected!\n\n");
    waitForUser();

    // Test keyboard press
    for(;;){

        getNextEvent();
    }

    waitForUser();
    closeSDL();
    closeSerial();
	exit(0);
}
