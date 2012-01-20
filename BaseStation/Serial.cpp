#include "Serial.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define	SERBUFSIZ	100
#define MSGBUFSIZ	100
#define MAXERRORS	3

int fd;
char serBuffer [SERBUFSIZ];
char msgBuffer [MSGBUFSIZ];
int readCount = 0;
int errors = 0;

int extractMessage (){

    char * start;
    char * end;

    if((start = strchr(serBuffer, PKT_BND)) && (end = strchr(start, PKT_BND))){

        start ++;
        *end = '\0';

        strcpy(msgBuffer, start);

        return strlen(msgBuffer);
    }
    else{

        return -1;
    }
}

int initSerial (char * port){

	// Attempt to open port

	printf("Attempting to open %s...\n", port);

	if((fd = open(port, O_RDWR)) < 0){

		printf("ERROR: Failed to open port.\n");
		return -1;
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

	for(errors = 0; errors < MAXERRORS; errors ++){

		if((readCount = read(fd, serBuffer, SERBUFSIZ)) < 1){

			printf("ERROR: Failed to read from  port.\n");
		}

		else if(readCount < 3){

			printf("ERROR: Corrupt packet.\n");
		}

        else if(extractMessage < 0){

			printf("ERROR: Corrupt packet.\n");
        }

		else{

			break;
		}
	}

	return msgBuffer;
}

int writeSerial (){
    
    int msgSize = strlen(msgBuffer);
    int writeCount;

    msgSize = sprintf(serBuffer, "%c%s%c\n", PKT_BND, msgBuffer, PKT_BND);

    for(errors = 0; errors < MAXERRORS; errors++){

        if((writeCount = write(fd, serBuffer, msgSize)) < 0){

            printf("ERROR: Failed to write to port.\n");
        }

        else if(writeCount < msgSize){

            printf("ERROR: Write operation interrupted.\n");
        }

        else{

            break;
        }
    }

	return 0;
}

void CloseSerial (){
    
    close(fd);

	return;
}


void sayHello(){

    sprintf(msgBuffer, "%c", HELLO);

    writeSerial();
}