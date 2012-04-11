#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// Packet frame types
#define	PKT_BND	0xFF
#define	HELLO	0x01

int printAll;   // Flag to control output of serial communication errors

int commMode;   // Flage to control mode of communication to robot

int initSerial(char *port);

char * readSerial();

int writeSerial(char *msg);

void closeSerial();

void sayHello();
