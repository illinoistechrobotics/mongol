// Main header file for BaseStation

// Matter of preference
typedef char byte;

// Controller mode variable

int ctrlMode;

enum{
    KEYBOARD,   // Use keyboard keys to control robot
    GAMEPAD     // Use gamepad's joystick and buttons to control robot
};

// Base-Robot communication mode variable

int commMode;

enum{
    ONLINE,     // Attempt to establish connection to robot
    OFFLINE     // Do not attempt to establish connection to roobt
};

// Interface type variable

int uiMode;

enum{
    CMD_LINE,   // Do not initialize GUI
    GRAPH_UI    // Initialize GUI
};

// Verbose mode variable

int printMode;

enum{
    QUIET,      // Do not print messages about minor communication and input errors
    VERBOSE     // Print messages about minor communication and input errors
};

// Message printing function

void printMsg (void);

// Exit function

void quitBase (void);
