#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define	PKT_BND	0xFF
#define	HELLO	0x01

int initSerial (char * port);

char * readSerial ();

int writeSerial (char * message);

void closeSerial ();

void sayHello();