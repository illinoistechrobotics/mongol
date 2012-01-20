#include "Serial.h"

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

    waitForUser();

	return 0;
}