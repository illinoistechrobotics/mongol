#include "Serial.h"

#define	SERBUFSIZ	100
#define MSGBUFSIZ	100
#define MAXERRORS	3

FILE *dev;
char inSerBuf [SERBUFSIZ];
char inMsgBuf [MSGBUFSIZ];
char outSerBuf [SERBUFSIZ];
char outMsgBuf [MSGBUFSIZ];
int errors = 0;

int extractMessage (){

    char * start;
    char * end;

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

int initSerial (char * port, int printMode){

    printAll = printMode;

	// Attempt to open port

	printf("Attempting to open %s...\n", port);

	if ((dev = fopen(port, "r+b"))){
        
        // Set stream to not block when reading

        fcntl(fileno(dev), F_SETFL, O_NONBLOCK);
        int flags = fcntl(fileno(dev), F_GETFL);

        if (!(flags & O_NONBLOCK)){

            printf("ERROR: Failed to open port.\n");
            return -1;
        }
	}
	
	printf("%s opened successfully.\n", port);
	
	// Wait for handshake

	printf("Waiting for handshake...\n");

    while (readSerial()[0] != HELLO){sleep(1);}
    sayHello();

	printf("Handshake received!\n");

	return 0;
}

char * readSerial (){

	for (errors = 0; errors < MAXERRORS; errors ++){

		if (!fgets(inSerBuf, SERBUFSIZ, dev) && printAll){

			printf("ERROR: Failed to read from  port.\n");
		}

        else if (extractMessage() < 0 && printAll){

			printf("ERROR: Corrupt packet.\n");
        }

		else{

			break;
		}
	}

	return inMsgBuf;
}

int writeSerial(char *msg){

    int msgSize;
    int writeCount;

    if (msg)
        strcpy(outMsgBuf, msg);

    msgSize = sprintf(outSerBuf, "%c%s%c\n", PKT_BND, outMsgBuf, PKT_BND);

    for (errors = 0; errors < MAXERRORS; errors++){

        if ((writeCount = fputs(outSerBuf, dev) && printAll) < 0){

            printf("ERROR: Failed to write to port.\n");
        }

        else if (writeCount < msgSize && printAll){

            printf("ERROR: Write operation interrupted.\n");
        }

        else {

            break;
        }
    }

	return (errors == 3) ? (-1) : 0;
}

void closeSerial(){
    
    fclose(dev);

	return;
}

void sayHello(){

    sprintf(outMsgBuf, "%c", HELLO);

    writeSerial(NULL);
}
