#include <string.h>
#include <ax12.h>
#include <BioloidController.h>
#include "MongolMotion.h"
#include "MongolSerial.h"

// Serial communication helper functions
void init_serial (void);
int extract_msg (packet* msg, char* inbuf);
int read_serial (packet* msg);
void write_serial (packet* msg);
void say_hello();
void say_ready();
void close_serial();

BioloidController servos = BioloidController(1000000);

void setup(){
  
  // Initialize serial communications (includes first HELLO packet)
  init_serial();
}

// Main loop of program
void loop(){

}

/*** SERIAL COMMUNICATION FUNCTIONS ***/

void init_serial (){

    Serial.begin(38400);
    
    packet response;
    
    do{
        say_hello();
        delay(1000);
    } while((read_serial(&response) < 0) ||
            (response.type != PKT_HELLO));
    
    return;
}

int extract_msg (packet* msg, char* inbuf){

    char* start;
    char* end;

    if ((start = strchr(inbuf, PKT_BND)) &&
        (end = strchr(start, PKT_BND))){
        end++;
        strncpy((char*)msg, start, (end-start));
        return (end-start);
    }
    else
        return -1;
}

int read_serial (packet* msg){
    
    char inbuf [SER_BUFSIZ];
    int inpkt_len = 0;
    byte nxt_byte;

    while(Serial.available() &&
          (inpkt_len < SER_BUFSIZ) &&
          ((nxt_byte = Serial.read()) > 0)){
    
        inbuf[inpkt_len++] = nxt_byte;
    }
    inbuf[inpkt_len] = 0;
    
    if(extract_msg(msg, inbuf) < 0)
        return -1;
    
    return 0;
}

void write_serial (packet * msg){
    
    char outbuf [SER_BUFSIZ];
    int msg_size;
    int write_count;

    if(msg && (msg->type)){

        packet* target = (packet*)strncpy(outbuf, (char*)msg, PKT_LEN);
        target->front_bnd = PKT_BND;
        target->end_bnd = PKT_BND;

        // These packet types have no defined values, but must be filled anyways
        if((target->type == PKT_HELLO) ||
           (target->type == PKT_GDBY) ||
           (target->type == PKT_STDBY) ||
           (target->type == PKT_RDY))
            target->value = PKT_FIL;

        target->null_term = 0;
        
        Serial.write((byte*)target,PKT_LEN);
        
        return;
    }
}

void say_hello(){

    packet hello;
    hello.type = PKT_HELLO;
    write_serial(&hello);
    return;
}

void say_ready(){

    packet ready;
    ready.type = PKT_RDY;
    write_serial(&ready);
    return;
}

void close_serial (){

    Serial.end();
}



  


