#include <string.h>
#include <ax12.h>
#include <BioloidController.h>
#include "MongolMotion.h"
#include "MongolSerial.h"

// Serial communication helper functions
void reconnect (void);
void init_serial (void);
int extract_msg (packet* msg, char* inbuf);
int read_serial (packet* msg);
void write_serial (packet* msg);
void say_hello (void);
void say_ready (void);
void close_serial (void);

BioloidController servos = BioloidController(1000000);

void setup(){
  
  // Initialize serial communications (includes first HELLO packet)
  init_serial();
}

packet out_pkt;
packet in_pkt;

// Main loop of program
void loop(){
    
    say_ready();
    while(read_serial(&in_pkt) < 0)
        delay(10);      // Allow time for a complete packet to arrive
    
    switch(in_pkt.type){
    
        case PKT_GDBY:
            reconnect();
            break;
    }
}

/*** SERIAL COMMUNICATION FUNCTIONS ***/

void reconnect(){
    
    packet response;
    
    do{
        say_hello();
        delay(1000);
    } while((read_serial(&response) < 0) ||
            (response.type != PKT_HELLO));
            
    return;
}
    
void init_serial (){

    Serial.begin(38400);
    reconnect();
    return;
}

int extract_msg (packet* msg, char* inbuf){

    char* start;
    char* end;

    if ((start = strchr(inbuf, PKT_BND)) &&
        (end = strchr((start+1), PKT_BND))){
        *(end+1) = 0;
        strcpy((char*)msg, start);
        return strlen(start);
    }
    else
        return -1;
}

int read_serial (packet* msg){
    
    char inbuf [SER_BUFSIZ];
    int inbuf_len = 0;
    int inmsg_len = 0;
    byte nxt_byte;

    while(Serial.available() &&
          (inbuf_len < SER_BUFSIZ) &&
          ((nxt_byte = Serial.read()) > 0)){
    
        inbuf[inbuf_len++] = nxt_byte;
    }
    inbuf[inbuf_len] = 0;
    
    if((inmsg_len = extract_msg(msg, inbuf)) < PKT_LEN)
        return -1;
        
    return inmsg_len;
}

void write_serial (packet * msg){
    
    char outbuf [SER_BUFSIZ];

    if(msg && (msg->type)){
    
        msg->front_bnd = PKT_BND;
        msg->end_bnd = PKT_BND;

        // These packet types have no defined values, but must be filled anyways
        if((msg->type == PKT_HELLO) ||
           (msg->type == PKT_GDBY) ||
           (msg->type == PKT_STDBY) ||
           (msg->type == PKT_RDY))
            msg->value = PKT_FIL;

        msg->null_term = 0;
        
        strcpy(outbuf, (char*)msg);
        Serial.write((byte*)outbuf,PKT_LEN);
        Serial.flush();
        
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



  


