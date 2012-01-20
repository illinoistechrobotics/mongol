#define	PKT_BND	0xFF
#define	HELLO	0x01

int initSerial (char * port);

char * readSerial ();

int writeSerial (char * message);

void closeSerial ();

void sayHello();