#include <ax12.h>
#include <BioloidController.h>
#include <string.h>
#include "MongolMotions.h"
#include "Serial.h"

#define OFF    0x00
#define ON     0x10
#define RIGHT  0x20
#define LEFT   0x30

#define TUR_SPEED 50

// Packet structure definition
typedef struct packet_struct{
    byte front_bnd;
    byte type;              // Type of packet
    byte value;             // Relevant value of type (null for HELLO, GDBY, and STDBY)
    byte end_bnd;
    byte null_term;         // Null terminator to ensure proper operation with string functions
} packet;

byte inbuf [SER_BUFSIZ];
byte outbuf [SER_BUFSIZ];
packet* inpacket;

BioloidController bioloid = BioloidController(1000000);

void setup(){
  
  // Initialize serial communications (includes first HELLO packet)
  init_serial();
}

void loop(){

  static byte prevDir = MOV_STOP;
  static byte nextDir = MOV_STOP;
  static byte prevTur = MOV_STOP;
  static byte nextTur = MOV_STOP;
  static byte curPose = 1;
  static byte strafeMode = OFF;
	
  if(read_serial())
  {
    byte pktType = inpacket->type;
    byte pktInfo = inpacket->value;
    int offset = 0;
    if(pktType == PKT_MOVE || pktType == PKT_TURN || pktType == PKT_STRF_L || pktType == PKT_STRF_R)
    {
       if(curPose > 5)
       {
         curPose = 1;
       }
       nextDir = pktInfo;
       if(prevDir == nextDir)
       {
         if(nextDir == MOV_FWD)
         { 
           if(strafeMode == OFF)
           {
             bioloid.loadPose(FORWARD[curPose++].pose);
             for(int i = 0; i++;i < FORWARD[1].pose[0])
             {
               bioloid.setNextPose(i, FORWARD[curPose].pose[i]);
             }
           }
         }
         if(nextDir == MOV_BKD)
         {
           if(strafeMode == OFF)
           {
             bioloid.loadPose(BACKWARD[curPose++].pose);
             for(int i = 0; i++;i < BACKWARD[1].pose[0])
             {
               bioloid.setNextPose(i, BACKWARD[curPose].pose[i]);
             }
           }
         }
         if(nextDir == MOV_STOP)
         {
           if(strafeMode == OFF)
           {
             bioloid.loadPose(START);
           }
         }
         if(nextDir == TRN_NONE)
         {
           //if(strafeMode == OFF)
           //  bioloid.loadPose(stopWalk);
         }
         if(nextDir == TRN_LEFT)
         {
           if(strafeMode == OFF)
           {
             bioloid.loadPose(TURN_LEFT[curPose++].pose);
             for(int i = 0; i++;i < TURN_LEFT[1].pose[0])
             {
               bioloid.setNextPose(i, TURN_LEFT[curPose].pose[i]);
             }
           }
         }
         if(nextDir == TRN_RIGHT)
         {
           if(strafeMode == OFF)
           {
             bioloid.loadPose(TURN_RIGHT[curPose++].pose);
             for(int i = 0; i++;i < TURN_RIGHT[1].pose[0])
             {
               bioloid.setNextPose(i, TURN_RIGHT[curPose].pose[i]);
             }
           }
         }
         if(nextDir == STRF_L_ON)
         {
           if(strafeMode == OFF)
           {
             strafeMode = LEFT;
             //bioloid.loadPose(stopWalk);
           }
         }
         if(nextDir == STRF_R_ON)
         {
           if(strafeMode == OFF)
           {
             strafeMode = RIGHT;
             //bioloid.loadPose(stopWalk);
           }
         }
         if(nextDir == STRF_L_OFF)
         {
           strafeMode = OFF;
         }
         if(nextDir == STRF_R_OFF)
         {
           strafeMode = OFF;
         }
       }
       else
       {
         curPose = 1;
       }
    }
    if(pktType == PKT_AIM_H || pktType == PKT_AIM_V || pktType == PKT_FIRE)
    {
      if(pktInfo == AIM_H_STRGHT)
      {
        //don't move
      }
      if(pktInfo == AIM_H_LEFT)
      {
        //bioloid.setNextPose(H_TUR, bioloid.getCurrentPose(H_TUR) - TUR_SPEED);
      }
      if(pktInfo == AIM_H_RIGHT)
      {
        //bioloid.setNextPose(H_TUR, bioloid.getCurrentPose(H_TUR) + TUR_SPEED);
      }
      if(pktInfo == AIM_V_STRGHT)
      {
        //don't move
      }
      if(pktInfo == AIM_V_DWN)
      {
        //bioloid.setNextPose(V_TUR, bioloid.getCurrentPose(V_TUR) - TUR_SPEED);
      }
      if(pktInfo == AIM_V_UP)
      {
        //bioloid.setNextPose(H_TUR, bioloid.getCurrentPose(V_TUR) + TUR_SPEED);
      }
      if(pktInfo == FIRE_ON)
      {
        //turret on
      }
      if(pktInfo == FIRE_OFF)
      {
        //turret off
      }
    }
    
    bioloid.interpolateSetup(75);
    bioloid.interpolateStep();
    say_ready();
  }
  else
  {
    //print("Read error!");
  }
}

/*** SERIAL COMMUNICATION FUNCTIONS ***/
int extract_msg (){

    byte* start;
    byte* end;

    if ((start = strchr(inbuf, PKT_BND)) && (end = strchr(start, PKT_BND))){
        inpacket = (packet*)start;
        return (end-start);
    }
    else{

        return -1;
    }
}

int init_serial(){

    Serial.begin(38400);
    
    do{
        say_hello();
        inpacket = readSerial();
        delay(1000);
    } while(!(inpacket) &&
             (inpacket->type == PKT_HELLO));
             
    say_ready();
    
    return 0;
}

packet*  read_serial(){

    inpkt_len = 0;
    byte nxt_byte;

    while(Serial.available() && ((nxt_byte = Serial.read()) > 0)){
    
        inbuf[inpkt_len++] = nxt_byte;
    }
    inbuf[inpkt_len] = 0;
    
    if(extract_msg() < 0)
        return NULL;
    
    return inpacket;
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

            if(msg_size == Serial.write(outbuf, sizeof(packet)){

                break;
            }
        }
        return (errors == MAXERRORS) ? (-1) : 0;
    }
    return -1;
}

void say_hello(){
    
    packet greeting;
    greeting.type = PKT_HELLO;
    write_serial(&greeting);
    return;
}

void say_ready(){
    
    packet ready;
    ready.type = PKT_HELLO;
    write_serial(&ready);
    return;
}

void closeSerial (){

    Serial.end();
}



  


