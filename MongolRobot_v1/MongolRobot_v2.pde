#include <ax12.h>
#include <BioloidController.h>
#include "ForwardWalk.h"

#define MSGBUFSIZ   126
#define SERBUFSIZ   128
#define MAXERR      3

#define	PKT_BND	0xFF
#define	HELLO	0x01

byte outMsgBuf [MSGBUFSIZ];
byte outSerBuf [SERBUFSIZ];
int outPktLen;

byte inMsgBuf [MSGBUFSIZ];
byte inSerBuf [SERBUFSIZ];
int inPktLen;

BioloidController servos = BioloidController(1000000);

void setup(){
  
  // Initialize serial communications (includes first HELLO packet)
  initSerial();
}

void loop(){

	static int prevDir = stand;
	static int nextDir = stand;
	static int prevTur = stand;
	static int nextTur = stand;
	
	readSerial();
	int offset = 0;
	nextDir = inSerBuf[1];
	nextTur = inSerBuf[2];
	
	if(prevDir != nextDir)
	{
		if(nextDir = 0)
		{
			bioloid.loadPose(frontWalk)
		}
		.
		.
		.
	}
	
	if(nextTur > 0)
		offset = 50;
	else if(nextTur < 0)
		offset = -50;
	else offset = 0;
	

	bioloid.setNextPose(RIGHT_FRONT, bioloid.getNextPose(RIGHT_FRONT));
	.
	.
	.
	bioloid.setNextPose(V_TUR, bioloid.getCurrentPose(V_TUR) + offset);
	bioloid.setNextPose(H_TUR, bioloid.getCurrentPose(H_TUR) + offset);	


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



  


