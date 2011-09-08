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
#define P_MOVE_SPEED		32	//Word

#define P_PRES_POS		36	//Word
#define P_PRES_V		42	//Byte
#define P_MOVING		46	//Byte

#define MAX_CW_POS		0
#define MAX_CCW_POS		1023

#define MED_POS			511

#define STOP_SPEED		0
#define MED_SPEED		511
#define MAX_SPEED		1023

#define DEFAULT_DEV_INDEX	0	// /dev/ttyUSB0
#define DEFAULT_BAUDNUM		1 	// 1Mbps

#define MAX_DXL_COUNT		253

char refreshStr [] = "refresh";
char quitStr [] = "quit";

int dxlCount = 0;

int beSilent = 0;

char idStr [] = "(1)ID";
char cwLimStr [] = "(2)CW Limit";
char ccwLimStr [] = "(3)CCW Limit";
char presPosStr [] = "Current Position";
char presVoltStr [] = "Current Voltage";

int idVal [MAX_DXL_COUNT];
int cwLimVal [MAX_DXL_COUNT];
int ccwLimVal [MAX_DXL_COUNT];
int presPosVal [MAX_DXL_COUNT];
int presVoltVal [MAX_DXL_COUNT];

char inputBuffer [100];

//prototypes
int getIndex(int id);
void printInfo(void);
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

			idVal[dxlCount] = i;
			cwLimVal[dxlCount] = readWord(i, P_CW_ANG_LIMIT);
			ccwLimVal[dxlCount] = readWord(i, P_CCW_ANG_LIMIT);
			presPosVal[dxlCount] = readWord(i, P_PRES_POS);
			presVoltVal[dxlCount] = readByte(i, P_PRES_V);

			dxlCount ++;
		}
	}

	printf("Found %d Dynamixels.\n", dxlCount);
	printInfo();

	printf("Press Enter to continue the test...\n");
	getchar();

	beSilent = 0;

	printf("Moving servos...\n");

	short moveSpeeds [dxlCount];
	short goalSpeeds [dxlCount];
	
	int j;
	for(j = 0; j < dxlCount; moveSpeeds[j++] = MED_SPEED);
	for(j = 0; j < dxlCount; goalSpeeds[j++] = MED_POS);

	//Write medium moving speed to all servos
	writeWordAll(P_MOVE_SPEED, &moveSpeeds[0]);

	//Write middle goal position to all servos	
	writeWordAll(P_GOAL_POS, &goalSpeeds[0]);
	sleep(3);

	printf("Servos moved.\n");

	printInfo();
	printf("Exiting...\n");
	dxl_terminate();
	return 0;
}

int getIndex(int id){

	int l;
	for(l = 0; l < dxlCount; l ++){

		if(id == idVal[l])
			return l;
	}

	return -1;
}

void printInfo(){

	printf("%6s %12s %13s %17s %16s\n", &idStr[0], &cwLimStr[0], &ccwLimStr[0], &presPosStr[0], &presVoltStr[0]);

	int j;
	for(j = 0; j < dxlCount; j ++){

		printf("%6d %12d %13d %17d %16d\n", idVal[j], cwLimVal[j], ccwLimVal[j], presPosVal[j], presVoltVal[j]);
	}

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

		*(p ++) = dxl_read_byte(idVal[i], addr);

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

		*(p ++) = dxl_read_word(idVal[i], addr);

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
		
		dxl_write_byte(idVal[i], addr, *(p ++));
		
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
		
		dxl_write_word(idVal[i], addr, *(p ++));
		
		if(!isSuccess())
			result = 0;
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
