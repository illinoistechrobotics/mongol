#include <gtk/gtk.h>

char termBuf [BUFSIZ];
int uiMode;
int printMode;

void printMsg (void);

int initGUI (int * argc, char *** argv);
