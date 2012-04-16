#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// Packet frame types
#define	PKT_BND	0xFF

int init_serial(char *port);

packet * read_serial();

int write_serial(packet * msg);

void close_serial();

void say_hello();
