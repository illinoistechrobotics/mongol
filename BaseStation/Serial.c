#include "BaseStation.h"
#include "Serial.h"

#define	SERBUFSIZ	100
#define MSGBUFSIZ	100
#define MAXERRORS	3

FILE *dev;
byte inSerBuf [SERBUFSIZ];
byte inMsgBuf [MSGBUFSIZ];
byte outSerBuf [SERBUFSIZ];
byte outMsgBuf [MSGBUFSIZ];
int errors = 0;

int extractMessage (){

    byte * start;
    byte * end;

    if ((start = strchr(inSerBuf, PKT_BND)) && (end = strchr(start, PKT_BND))){

        start ++;
        *end = '\0';

        strcpy(inMsgBuf, start);

        return strlen(inMsgBuf);
    }
    else{

        return -1;
    }
}

int initSerial (char * port){

	// Attempt to open port

	printf("Attempting to open %s...\n", port);

	if ((dev = fopen(port, "r+b"))){

        int devFD = fileno(dev);
        
        // Set stream to not block when reading
        fcntl(devFD, F_SETFL, O_NONBLOCK);
        int flags = fcntl(fileno(dev), F_GETFL);

        // Set baud rate to 34800
        struct termios devConfig;
        tcgetattr(devFD, &devConfig);
        cfsetspeed(&devConfig, B38400);
        int baudSet = tcsetattr(devFD, TCSANOW, &devConfig);

        if (!(flags & O_NONBLOCK) && !(baudSet)){

            printf("ERROR: Failed to open port.\n");
            return -1;
        }
	}
	
	printf("%s opened successfully.\n", port);
	
	// Wait for handshake

	printf("Waiting for \"Hello\"... ");

    byte * handshake;
    while ((handshake = readSerial()) &&
           (handshake[0] != HELLO)){
        sleep(1);
    }
    sayHello();

	printf("\"Hello\" received.\n");

	return 0;
}

byte * readSerial (){

	for (errors = 0; errors < MAXERRORS; errors ++){

		if (!fgets(inSerBuf, SERBUFSIZ, dev) && (printMode == VERBOSE)){

			printf("ERROR: Failed to read from  port.\n");
		}

        else if (extractMessage() < 0 && (printMode == VERBOSE)){

			printf("ERROR: Corrupt packet.\n");
        }

		else{

            return inMsgBuf;
		}
	}

    return NULL;
}

int writeSerial(byte *msg){

    int msgSize;
    int writeCount;

    if (msg)
        strcpy(outMsgBuf, msg);

    msgSize = sprintf(outSerBuf, "%c%s%c\n", PKT_BND, outMsgBuf, PKT_BND);

    for (errors = 0; errors < MAXERRORS; errors++){

        if ((writeCount = fputs(outSerBuf, dev) && (printMode == VERBOSE)) < 0){

            printf("ERROR: Failed to write to port.\n");
        }

        else if (writeCount < msgSize && (printMode == VERBOSE)){

            printf("ERROR: Write operation interrupted.\n");
        }

        else {

            break;
        }
    }

	return (errors == MAXERRORS) ? (-1) : 0;
}

void closeSerial(){
    
    fclose(dev);

	return;
}

void sayHello(){

    sprintf(outMsgBuf, "%c", HELLO);

    writeSerial(NULL);
}
