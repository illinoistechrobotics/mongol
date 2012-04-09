#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// Communication mode flags
#define OFFLINE 0x00
#define ONLINE  0x01

#define	PKT_BND	0xFF
#define	HELLO	0x01

int printAll;   // Flag to control output of serial communication errors

int commMode;   // Flage to control mode of communication to robot

int initSerial(char *port, int printMode);

char *readSerial();

int writeSerial(char *msg);

void closeSerial();

void sayHello();
