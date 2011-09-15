//DXLConfig
//By Allen Baker

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>

#define P_MODEL_NUM		0	//Word
#define P_FW_VER		2	//Byte
#define P_ID			3	//Byte

#define P_CW_ANG_LIMIT		6	//Word
#define P_CCW_ANG_LIMIT		8	//Word
#define P_LED			25	//Byte

#define P_GOAL_POS		30	//Word
#define P_MOVE_SPD		32	//Word

#define P_PRES_POS		36	//Word
#define P_PRES_V		42	//Byte
#define P_MOVE_STATUS		46	//Byte

#define MAX_CW_POS		0
#define CW_POS			411	//Safe for all servos
#define MED_POS			511
#define CCW_POS			581	//Safe for all servos
#define MAX_CCW_POS		1023

#define STOP_SPEED		0
#define SLW_SPEED		255
#define MED_SPEED		511
#define FST_SPEED		767
#define MAX_SPEED		1023

#define DEFAULT_DEV_INDEX	0	// /dev/ttyUSB0
#define DEFAULT_BAUDNUM		1 	// 1Mbps

#define MAX_DXL_COUNT		253

char refreshStr [] = "refresh";
char quitStr [] = "quit";

int dxlCount = 0;

int beSilent = 0;

int dxlId [MAX_DXL_COUNT];
int dxlCwLim [MAX_DXL_COUNT];
int dxlCcwLim [MAX_DXL_COUNT];
int dxlPresPos [MAX_DXL_COUNT];
int dxlGoalPos [MAX_DXL_COUNT];
int dxlMoveSpd [MAX_DXL_COUNT];
int dxlMoveStatus [MAX_DXL_COUNT];
int dxlPresVolt [MAX_DXL_COUNT];

char inputBuffer [100];

//prototypes
int getIndex(int id);
void printInfo(void);
void waitForMove(void);
int readByte(int id, int addr);
int readByteAll(int addr, int * start);
int readWord(int id, int addr);
int readWordAll(int addr, int * start);
int writeByte(int id, int addr, char data);
int writeByteAll(int addr, char * data);
int writeWord(int id, int addr, short data);
int writeWordAll(int addr, short * start);
int isSuccess(void);
char * readIn(void);
void flushIn(void);

int main(){
	
	if(!(dxl_initialize(DEFAULT_DEV_INDEX, DEFAULT_BAUDNUM))){

		printf("Failed to initialize USB interface.\n");
		return 1;
	}
	else{

		printf("Successfully initialized USB interface.\n");
	}

	printf("Detecting Dynamixels...\n");

	beSilent = 1;

	int i;
	for(i = 1; i <= MAX_DXL_COUNT; i ++){

		dxl_ping(i);

		if(isSuccess()){

			dxlId[dxlCount] = i;
			dxlCount ++;
		}
	}

	beSilent = 0;

	short moveSpeed [dxlCount];
	short goalPos [dxlCount];
	
	int j;
	for(j = 0; j < dxlCount; moveSpeed[j++] = SLW_SPEED);
	for(j = 0; j < dxlCount; goalPos[j++] = MED_POS);

	//Write medium moving speed to all servos
	writeWordAll(P_MOVE_SPD, &moveSpeed[0]);

	readWordAll(P_CW_ANG_LIMIT, &dxlCwLim[0]);
	readWordAll(P_CCW_ANG_LIMIT, &dxlCcwLim[0]);
	
	readWordAll(P_PRES_POS, &dxlPresPos[0]);
	readWordAll(P_GOAL_POS, &dxlGoalPos[0]);
	readWordAll(P_MOVE_SPD, &dxlMoveSpd[0]);
	readByteAll(P_MOVE_STATUS, &dxlMoveStatus[0]);
	readByteAll(P_PRES_V, &dxlPresVolt[0]);

	printf("Found %d Dynamixels.\n", dxlCount);
	printInfo();

	printf("Press Enter to continue the test...\n");
	getchar();

	printf("Moving servos...\n");

	//Write middle goal position to all servos	
	writeWordAll(P_GOAL_POS, &goalPos[0]);
	waitForMove();
	
	readWordAll(P_PRES_POS, &dxlPresPos[0]);
	readWordAll(P_GOAL_POS, &dxlGoalPos[0]);
	readWordAll(P_MOVE_SPD, &dxlMoveSpd[0]);
	readByteAll(P_MOVE_STATUS, &dxlMoveStatus[0]);
	readByteAll(P_PRES_V, &dxlPresVolt[0]);

	printf("Servos moved.\n");
	printInfo();

	printf("Press Enter to continue the test...\n");
	getchar();

	//Write medium moving speed to all servos
	writeWordAll(P_MOVE_SPD, &moveSpeed[0]);

	//Write cw goal position to all servos
	for(j = 0; j < dxlCount; goalPos[j++] = CW_POS);
	writeWordAll(P_GOAL_POS, &goalPos[0]);
	waitForMove();
	
	readWordAll(P_PRES_POS, &dxlPresPos[0]);
	readWordAll(P_GOAL_POS, &dxlGoalPos[0]);
	readWordAll(P_MOVE_SPD, &dxlMoveSpd[0]);
	readByteAll(P_MOVE_STATUS, &dxlMoveStatus[0]);
	readByteAll(P_PRES_V, &dxlPresVolt[0]);

	printf("Servos moved.\n");
	printInfo();

	printf("Press Enter to continue the test...\n");
	getchar();

	//Write medium moving speed to all servos
	writeWordAll(P_MOVE_SPD, &moveSpeed[0]);
	
	//Write ccw goal position to all servos
	for(j = 0; j < dxlCount; goalPos[j++] = CCW_POS);
	writeWordAll(P_GOAL_POS, &goalPos[0]);
	waitForMove();
	
	readWordAll(P_PRES_POS, &dxlPresPos[0]);
	readWordAll(P_GOAL_POS, &dxlGoalPos[0]);
	readWordAll(P_MOVE_SPD, &dxlMoveSpd[0]);
	readByteAll(P_MOVE_STATUS, &dxlMoveStatus[0]);
	readByteAll(P_PRES_V, &dxlPresVolt[0]);

	printf("Servos moved.\n");
	printInfo();

	printf("Press Enter to continue the test...\n");
	getchar();

	//Write medium moving speed to all servos
	writeWordAll(P_MOVE_SPD, &moveSpeed[0]);
	
	//Write medium goal position to all servos
	for(j = 0; j < dxlCount; goalPos[j++] = MED_POS);
	writeWordAll(P_GOAL_POS, &goalPos[0]);
	waitForMove();
	
	readWordAll(P_PRES_POS, &dxlPresPos[0]);
	readWordAll(P_GOAL_POS, &dxlGoalPos[0]);
	readWordAll(P_MOVE_SPD, &dxlMoveSpd[0]);
	readByteAll(P_MOVE_STATUS, &dxlMoveStatus[0]);
	readByteAll(P_PRES_V, &dxlPresVolt[0]);

	printf("Servos moved.\n");
	printInfo();

	printf("Exiting...\n");
	dxl_terminate();
	return 0;
}

int getIndex(int id){

	int l;
	for(l = 0; l < dxlCount; l ++){

		if(id == dxlId[l])
			return l;
	}

	return -1;
}

void printInfo(){

	printf("ID  CWLim  CCWLim  PresPos  GoalPos  MoveSpd  MoveStatus  PresVolt\n");

	int j;
	for(j = 0; j < dxlCount; j ++){

		printf("%2d  %5d  %6d  %7d  %7d  %7d  %10d  %7d\n", dxlId[j], dxlCwLim[j], dxlCcwLim[j], dxlPresPos[j], dxlGoalPos[j], dxlMoveSpd[j], dxlMoveStatus[j], dxlPresVolt[j]);
	}

	return;
}

void waitForMove(){

	int isStopped = 0;
	int k;

	printf("Waiting...\n");

	while(!isStopped){

		isStopped = 1;

		for(k = 0; k < dxlCount; k ++){

			if(readByte(dxlId[k], P_MOVE_STATUS)){
				isStopped = 0;
				break;
			}
		}
	}

	printf("Done waiting.\n");

	return;
}

int readByte(int id, int addr){

	int data = dxl_read_byte(id, addr);
	int result;

	if(!(result = isSuccess()))
		return result;
	else
		return data;
}

//Stores data in int array, returned int is success bit
int readByteAll(int addr, int * start){

	int * p = start;
	int result = 1;

	int i;
	for(i = 0; i < dxlCount; i ++){

		*(p ++) = dxl_read_byte(dxlId[i], addr);

		if(!isSuccess())
			result = 0;
	}

	return result;
}

int readWord(int id, int addr){

	int data = dxl_read_word(id, addr);
	int result;

	if(!(result = isSuccess()))
		return result;
	else
		return data;
}

//Stores data in int array, returned int is success bit
int readWordAll(int addr, int * start){ 
	int * p = start;
	int result = 1;

	int i;
	for(i = 0; i < dxlCount; i ++){

		*(p ++) = dxl_read_word(dxlId[i], addr);

		if(!isSuccess())
			result = 0;
	}
	
	return result;
}

int writeByte(int id, int addr, char data){

	dxl_write_byte(id, addr, data);

	return isSuccess();
}

int writeByteAll(int addr, char * start){

	char * p = start;
	int result = 1;

	int i;
	for(i = 0; i < dxlCount; i ++){
		
		dxl_write_byte(dxlId[i], addr, *(p ++));
		
		if(!isSuccess())
			result = 0;
	}

	return result;
}

int writeWord(int id, int addr, short data){

	dxl_write_word(id, addr, data);

	return isSuccess();
}

int writeWordAll(int addr, short * start){

	short * p = start;
	int result = 1;

	int i;
	for(i = 0; i < dxlCount; i ++){
		
		dxl_write_word(dxlId[i], addr, *p);
		
		if(!isSuccess())
			result = 0;
		else
			printf("Wrote %d to DXL %d at address %d.\n", *p, dxlId[i], addr);

		p ++;
	}

	return result;
}

char * readIn(){

	int i = 0;
	char a;
	while((a = getchar()) != '\n' && a != EOF && i < 99){

		inputBuffer[i++] = a;
	}

	inputBuffer[i] = '\0';

	return &inputBuffer[0];
}

int isSuccess(){

	int result = dxl_get_result();

	switch(result){

		case COMM_TXSUCCESS:
			return 1;
		case COMM_RXSUCCESS:
			return 1;
		case COMM_TXFAIL:
			if(!beSilent)
				printf("Error: COMM_TXFAIL\n");
			return 0;
		case COMM_RXFAIL:
			if(!beSilent)
				printf("Error: COMM_RXFAIL\n");
			return 0;
		case COMM_TXERROR:
			if(!beSilent)
				printf("Error: COMM_TXERROR\n");
			return 0;
		case COMM_RXWAITING:
			if(!beSilent)
				printf("Error: COMM_RXWAITING\n");
			return 0;
		case COMM_RXTIMEOUT:
			if(!beSilent)
				printf("Error: COMM_RXTIMEOUT\n");
			return 0;
		case COMM_RXCORRUPT:
			if(!beSilent)
				printf("Error: COMM_RXCORRUPT\n");
			return 0;
		default:
			if(!beSilent)
				printf("Error: Unknown: %d\n", result);
			return 0;
	}
}
