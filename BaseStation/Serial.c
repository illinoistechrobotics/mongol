#include "BaseStation.h"
#include "Serial.h"

#define MAXERRORS	3

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
        int flags = fcntl(fileno(dev), F_GETFL);

        // Set baud rate to 34800
        struct termios devConfig;
        tcgetattr(dev_fd, &devConfig);
        cfsetspeed(&devConfig, B38400);
        int baudSet = tcsetattr(dev_fd, TCSANOW, &devConfig);

        if (!(flags & O_NONBLOCK) && !(baudSet)){

            printf("ERROR: Failed to open port.\n");
            return -1;
        }
	}
	
	printf("%s opened successfully.\n", port);
	
	// Wait for handshake

	printf("Waiting for \"Hello\"... ");
    packet * in_shake;

    while ((in_shake = read_serial()) &&
           !((in_shake->type) == PKT_HELLO)){
        sleep(1);
    }
    say_hello();

	printf("\"Hello\" received.\n");

	return 0;
}

packet * read_serial (){

	for (errors = 0; errors < MAXERRORS; errors ++){

		if (!fgets(inbuf, BUFSIZ, dev) && (printMode == VERBOSE)){

			printf("ERROR: Failed to read from  port.\n");
		}

        else if (extract_msg() < 0 && (printMode == VERBOSE)){

			printf("ERROR: Corrupt packet.\n");
        }

		else{

            return inpacket;
		}
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

            if((write_count = fputs(outbuf, dev) && (printMode == VERBOSE)) < 0){

                printf("ERROR: Failed to write to port.\n");
            }

            else if(write_count < msg_size && (printMode == VERBOSE)){

                printf("ERROR: Write operation interrupted.\n");
            }

            else {

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
