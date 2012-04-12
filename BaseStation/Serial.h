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

// Packet structure definition
typedef struct packetStruct{
    byte frontBnd;
    byte walkDir;           // Walking Direction Variable
    byte gunDir;            // Gun-Turning Direction Variable
    byte actions;           // Action Variable
    byte endBnd;
} serPacket;

int initSerial(char *port);

char * readSerial();

int writeSerial(char *msg);

void closeSerial();

void sayHello();
