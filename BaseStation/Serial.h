#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define	PKT_BND	0xFF
#define VAL_NUL 0x55        // Value filler for hello, standby, goodbye, and ready packets

int init_serial(char *port);

packet * read_serial();

int write_serial(packet * msg);

void close_serial();

void say_hello();
