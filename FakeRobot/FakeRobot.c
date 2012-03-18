#include "Serial.h"

#define TERMBUFSIZ 100

char termBuf [TERMBUFSIZ];
char * dev;
int printMode;

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

int main (int argc, char * argv[]){

    // Set initial values
    printMode = 0;

    // Parse command line arguements
    parseArgs(argc, argv);

    printf("FakeRobot started.\n");

    // Initialize serial port
    if(initSerial(dev, printMode) < 0){

        return -1;
    }

    printf("Spamming \"Hello\"...\n");

    do{

        sayHello();
        sleep(1);

    } while (readSerial()[0] != HELLO);

    printf("Received \"Hello\"!\n");

    waitForUser();

    closeSerial();

    return 0;
}
