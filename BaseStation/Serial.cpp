#include "Serial.h"

#define	SERBUFSIZ	100
#define MSGBUFSIZ	100
#define MAXERRORS	3

FILE *dev;
char serBuf [SERBUFSIZ];
char msgBuf [MSGBUFSIZ];
int errors = 0;

int extractMessage (){

    char * start;
    char * end;

    if((start = strchr(serBuf, PKT_BND)) && (end = strchr(start, PKT_BND))){

        start ++;
        *end = '\0';

        strcpy(msgBuf, start);

        return strlen(msgBuf);
    }
    else{

        return -1;
    }
}

int initSerial (char * port){

	// Attempt to open port

	printf("Attempting to open %s...\n", port);

	if((dev = fopen(port, "r+b"))){
        
        // Set stream to not block when reading

        if(fcntl(fileno(dev), F_SETFL, O_NONBLOCK) < 0){

            printf("ERROR: Failed to open port.\n");
            return -1;
        }
	}
	
	printf("%s opened successfully.\n", port);
	
	// Wait for handshake

	printf("Waiting for handshake...\n");

	if(readSerial()[0] != HELLO){

		printf("ERROR: Handshake not received.\n");
		return -1;
	}

	printf("Handshake successfully received.\n");

	return 0;
}

char * readSerial (){

    fflush(dev);

	for(errors = 0; errors < MAXERRORS; errors ++){

		if(!fgets(serBuf, SERBUFSIZ, dev)){

			printf("ERROR: Failed to read from  port.\n");
		}

        else if(extractMessage() < 0){

			printf("ERROR: Corrupt packet.\n");
        }

		else{

			break;
		}
	}

	return msgBuf;
}

int writeSerial (){

    int msgSize = strlen(msgBuf);
    int writeCount;

    msgSize = sprintf(serBuf, "%c%s%c\n", PKT_BND, msgBuf, PKT_BND);

    for(errors = 0; errors < MAXERRORS; errors++){

        if((writeCount = fputs(serBuf, dev)) < 0){

            printf("ERROR: Failed to write to port.\n");
        }

        else if(writeCount < msgSize){

            printf("ERROR: Write operation interrupted.\n");
        }

        else{

            break;
        }
    }

	return (errors == 3);
}

void closeSerial (){
    
    fclose(dev);

	return;
}

void sayHello(){

    sprintf(msgBuf, "%c", HELLO);

    writeSerial();
}