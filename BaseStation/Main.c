#include "Controller.h"
#include "Serial.h"
#include "GUI.h"

char * dev;

uiMode = CMD_MODE;
printMode = QUIET;
 
void waitForUser (){

	sprintf(termBuf,"Press enter to continue...");
    printMsg();
	fgetc(stdin);

    return;
}

// Function to parse command line arguements
void parseArgs (int argc, char * argv[]){

    int a;

    for(a = 1; a < argc; a ++){

        // Get character after dash
        switch(argv[a][1]){

        // Port specifier flag (-d): defaults to /dev/ttyUSB0 if not used (for Linux)
        case 'd':
            dev = argv[++a];
            printf("Port specified: %s\n", dev);
            break;

        // Joystick mode flag (-j)

        // GUI mode flag (-g)
        case 'g':
            uiMode = GUI_MODE;
            printf("Graphical mode activated\n");
            break;
     
        // Verbose mode flag (-v): prints all non-critical error messages
        case 'v':  
            printMode = VERBOSE;    
            printf("Verbose mode activated.\n");    
            break;

        // Print help flag (-h)
        case 'h':
            printf("\nRobotic Control BaseStation Program\n");
            printf("Written by Allen Baker for Illinois Tech Robotics\n\n");
            printf("*** ARGS ***\n");
            printf("-d <port name>  : Specify port to use (default is COM1)\n");
            printf("-v              : Set verbose mode\n");
            printf("-h              : Print help information\n\n");
            exit(0);
            break;
        }
    }
}

int main (int argc, char* argv[]){

    // Parse command line arguements
    parseArgs(argc, argv);

    if(uiMode == GUI_MODE){
        initGUI(&argc, &argv);
    } 

    // Initialize SDL (VIDEO flag also initializes event handling)
    sprintf(termBuf,"Initializing SDL... ");
    printMsg();
    if(!(initCtrl())){

        sprintf(termBuf,"\nERROR: Controller Failed to initialize.\n");
        printMsg();
        waitForUser();
        exit(-1);
    }
    sprintf(termBuf,"Controller Initialized.\n");
    printMsg();

    // Initialize serial port (includes looking for HELLO packet
    // If not port name specified, default to /dev/ttyUSB0 (for Linux)
    sprintf(termBuf,"Connecting to robot...\n");
    printMsg();
    if((dev ? initSerial(dev, printMode) : initSerial("/dev/ttyUSB0", printMode)) < 0){

        exit(-1);
    }
    sayHello();
    sprintf(termBuf,"Connected!\n\n");
    printMsg();

    // Test keyboard press
    for(;;){

        getNextEvent();
    }

    closeCtrl();
    closeSerial();
	exit(0);
}
