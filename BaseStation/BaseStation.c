#include "BaseStation.h"
#include "Controller.h"
#include "Serial.h"
#include "GUI.h"

char * dev;

void waitForUser (){

	sprintf(termbuf,"Press enter to continue...");
    printmsg();
	fgetc(stdin);

    return;
}

void printmsg (){

    if(uiMode == CMD_LINE)
        puts(termbuf);

    return;
}

void quitBase (){

    closeCtrl();

    if(commMode == ONLINE)
        closeSerial();

	exit(0);
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

        // Joystick mode flag (-j): If not specified, defaults to keyboard control
        case 'j':
            ctrlmode = GAMEPAD;
            printf("Joystick mode activated.\n");
            break;

        // Calibrate flag (-c): Do not attempt to connect; used for testing input devices
        case 'c':
            commMode = OFFLINE;
            printf("Controller calibration activated.\n");
            break;

        // GUI mode flag (-g)
        case 'g':
            uiMode = GRAPH_UI;
            printf("Graphical mode activated.\n");
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
            printf("Usage:\n");
            printf("BaseStation [-d <port-name>] [-v] [-h]\n\n");
            printf("***** Arguements *****\n");
            printf(" -d <port-name>  : Specify port to use (default is /dev/ttyUSB0)\n");
            printf(" -j              : Activate joystick mode (default is keyboard mode)\n");
            printf(" -v              : Set verbose mode\n");
            printf(" -h              : Print help information\n\n");
            exit(0);
        }
    }
}

int main (int argc, char* argv[]){

    // Set default values
    commMode = ONLINE;
    uiMode = CMD_LINE;
    printMode = QUIET; 

    // Parse command line arguements
    parseArgs(argc, argv);

    if(uiMode == GRAPH_UI){
        initGUI(&argc, &argv);
    } 

    // Initialize SDL (VIDEO flag also initializes event handling)
    sprintf(termbuf,"Initializing Controller... ");
    printmsg();
    if(!(initCtrl())){

        sprintf(termbuf,"\nERROR: Controller Failed to initialize.\n");
        quitBase();
    }
    sprintf(termbuf,"Controller Initialized.\n");
    printmsg();

    // Initialize serial port (includes looking for HELLO packet
    // If not port name specified, default to /dev/ttyUSB0 (for Linux)
    if(commMode == ONLINE){
        sprintf(termbuf,"Connecting to robot...\n");
        printmsg();
        if((dev ? init_serial(dev) : init_serial("/dev/ttyUSB0")) < 0){

            quitBase();
        }
        sayHello();
        sprintf(termbuf,"Connected!\n\n");
        printmsg();
    }

    // Test keyboard press
    for(;;){

        getNextEvent();
    }

    quitBase();
}
