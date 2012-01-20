#include "Serial.h"

#define TERMBUFSIZ 100

char termBuf [TERMBUFSIZ];

void waitForUser (){

    printf("Press enter to continue...\n");
    fgetc(stdin);
}

int main (int argc, char * argv[]){

    printf("FakeRobot started.\n");

    if(argc < 2){

        printf("ERROR: No port name given.\n");
        return -1;
    }

    if(initSerial(argv[1]) < 0){

        return -1;
    }

    printf("Spamming Hello...\n");

    do{

        sayHello();
        sleep(1);

    } while (readSerial()[0] != HELLO);

    printf("Received \"Hello\"!\n");

    waitForUser();

    closeSerial();

    return 0;
}
