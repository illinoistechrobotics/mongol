#include <gtk/gtk.h>

// Print mode values
#define QUIET       0
#define VERBOSE     1

// UI mode values
#define CMD_MODE    0
#define GUI_MODE    1

char termBuf [BUFSIZ];
int uiMode;
int printMode;

void printMsg (void);

int initGUI (int * argc, char *** argv);
