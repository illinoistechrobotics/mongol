#include "BaseStation.h"
#include "Serial.h"

#define MAXERRORS	10      /**< Maximum number of errors before serial
                              functions terminate */

FILE *dev;
byte inbuf [BUFSIZ];
packet * inpacket;
byte outbuf [BUFSIZ];
int errors = 0;

int extract_msg (){

    byte * start;
    byte * end;

    if ((start = strchr(inbuf, PKT_BND)) && (end = strchr(start, PKT_BND))){
        inpacket = (packet *)start;
        return (end-start);
    }
    else{

        return -1;
    }
}

int init_serial (char * port){

	// Attempt to open port

	printf("Attempting to open %s...\n", port);

	if ((dev = fopen(port, "r+b"))){

        int dev_fd = fileno(dev);
        
        // Set stream to not block when reading
        fcntl(dev_fd, F_SETFL, O_NONBLOCK);
        int flags = fcntl(dev_fd, F_GETFL);

        // Set baud rate to 34800
        struct termios devConfig;
        tcgetattr(dev_fd, &devConfig);
        cfsetspeed(&devConfig, B38400);
        int baudSet = tcsetattr(dev_fd, TCSANOW, &devConfig);

        if (!(flags & O_NONBLOCK) || baudSet){

            printf("ERROR: Failed to open port.\n");
            return -1;
        }
        
        printf("%s opened successfully.\n", port);
        
        // Wait for handshake

        printf("Waiting for \"Hello\"... ");
        packet * in_shake;

        int i = 0;
        while ((!(in_shake = read_serial()) ||
               ((in_shake->type) != PKT_HELLO)) &&
               (i < 10)){
            i++;
            sleep(1);
        }
        if(i == 10)
            return -1;
        
        printf("\"Hello\" received.\n");

        int j = 0;
        while ((!(in_shake = read_serial()) ||
               ((in_shake->type) != PKT_RDY)) &&
               (j < 10)){
            j++;
            say_hello();
            sleep(1);
        }
        if(i == 10)
            return -1;

        printf("Mongol is Ready!\n");

        return 0;
	}
            
    printf("ERROR: Failed to open port.\n");
    return -1;
}

packet * read_serial (){

	for (errors = 0; errors < MAXERRORS; errors ++){

        fgets(inbuf, BUFSIZ, dev);

       if ((extract_msg() < 0) &&
                 (printMode == VERBOSE))
			printf("ERROR: Corrupt packet.\n");

		else
            return inpacket;

        usleep(10);
	}

    return NULL;
}

int write_serial(packet * msg){

    int msg_size;
    int write_count;

    if(msg && (msg->type)){

        msg->front_bnd = PKT_BND;
        msg->end_bnd = PKT_BND;

        // These packet types have no defined values, but must be filled anyways
        if((msg->type == PKT_HELLO) ||
           (msg->type == PKT_GDBY) ||
           (msg->type == PKT_STDBY) ||
           (msg->type == PKT_RDY))
            msg->value = VAL_NUL;

        msg->null_term = 0;

        strcpy(outbuf, (char *)msg);
        msg_size = strlen(outbuf);

        for(errors = 0; errors < MAXERRORS; errors++){

            if((fputs(outbuf, dev) == EOF) &&
               ((printMode == VERBOSE) < 0)){

                printf("ERROR: Failed to write to port.\n");
            }
            else {

                fflush(dev);
                break;
            }
        }
        return (errors == MAXERRORS) ? (-1) : 0;
    }
    return -1;
}

void close_serial(){

    packet goodbye;
    goodbye.type = PKT_GDBY;
    write_serial(&goodbye);
    fclose(dev);
	return;
}

void say_hello(){

    packet greeting;
    greeting.type = PKT_HELLO;
    write_serial(&greeting);
    return;
}
