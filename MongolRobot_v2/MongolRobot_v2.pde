#include <ax12.h>
#include <BioloidController.h>
#include "MongolMotions.h"
#include "Serial.h"

#define MSGBUFSIZ   126
#define SERBUFSIZ   128
#define MAXERR      3

#define	PKT_BND	0xFF
#define	HELLO	0x01

#define OFF    0x00
#define ON     0x10
#define RIGHT  0x20
#define LEFT   0x30

#define TUR_SPEED 50

byte outMsgBuf [MSGBUFSIZ];
byte outSerBuf [SERBUFSIZ];
int outPktLen;

byte inMsgBuf [MSGBUFSIZ];
byte inSerBuf [SERBUFSIZ];
int inPktLen;

BioloidController bioloid = BioloidController(1000000);

void setup(){
  
  // Initialize serial communications (includes first HELLO packet)
  initSerial();
}

void loop(){

  static byte prevDir = MOV_STOP;
  static byte nextDir = MOV_STOP;
  static byte prevTur = MOV_STOP;
  static byte nextTur = MOV_STOP;
  static byte curPose = 1;
  static byte strafeMode = OFF;
	
  if(readSerial() == 0)
  {
    byte pktType = inMsgBuf[0];
    byte pktInfo = inMsgBuf[1];
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
  }
  else
  {
    //print("Read error!");
  }
}

/*** SERIAL COMMUNICATION FUNCTIONS ***/

int initSerial (){

    Serial.begin(38400);
    
    do{
        outMsgBuf[0] = HELLO;
        writeSerial(1);
        readSerial();
        delay(1000);
    } while(inMsgBuf[0] != HELLO);
    
    return 0;
}

int readSerial(){

    inPktLen = 0;

    while(Serial.available()){
    
        inSerBuf[inPktLen++] = Serial.read();
    }
    
    if(inPktLen < 3)
        return -1;
    
    int inMsgLen = inPktLen - 2;  
    
    for(int i = 0; i < inMsgLen; i++){
    
        inMsgBuf[i] = inSerBuf[i+1];
    }
    
    return 0;
}

int writeSerial (int msgLen){

    outPktLen = msgLen + 2;
    outSerBuf[0] = PKT_BND;
    
    for (int i = 0; i < msgLen; i++){
    
        outSerBuf[i+1] = outMsgBuf[i];
    }
    
    int bytesSent;
    outSerBuf[outPktLen-1] = PKT_BND;
    
    Serial.write(outSerBuf, outPktLen);
    Serial.flush();
    
    return 0;
    
    // for (int j = 0; j < MAXERR; j++){
    // 
    //     if((bytesSent = Serial.write(outSerBuf, outPktLen)) == outPktLen)
    //         return 0;
    // }
    
    // Insert error indicator here?
    
    // return -1;
}

void closeSerial (){

    Serial.end();
}



  


