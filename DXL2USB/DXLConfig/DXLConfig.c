//DXLConfig
//By Allen Baker

#include <stdlib.h>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>

#define P_MODEL_NUM		0	//Word
#define P_FW_VER		2	//Byte
#define P_ID			3	//Byte

#define P_CW_ANG_LIMIT		6	//Word
#define P_CCW_ANG_LIMIT		8	//Word
#define P_MAX_TORQUE		14	//Word
#define P_LED			25	//Byte

#define P_GOAL_POS		30	//Word
#define P_MOVING_SPEED		32	//Word
#define P_PRESENT_POS		36	//Word
#define P_PRESENT_V		42	//Byte
#define P_MOVING		46	//Byte

#define MAX_CW_POS		0
#define MAX_CCW_POS		1023

#define STOP_SPEED		0
#define MED_SPEED		511
#define MAX_SPEED		1023

#define DEFAULT_DEV_INDEX	0	// /dev/ttyUSB0
#define DEFAULT_BAUDNUM		1 	// 1Mbps

int id = 0;

int cwLimitVal;
int ccwLimitVal;
int movingSpeedVal;
int presPositionVal;
int presVoltageVal;

char inputBuffer [100];

//prototypes
void printInfo(void);
int readByte(int addr);
int readWord(int addr);
int writeByte(int addr, char data);
int writeWord(int addr, short data);
int isSuccess(void);
char * readIn(void);
void flushIn(void);

int main(){
	
	/*
	char idStr [] = "(1) ID";
	char cwLimitStr [] = "(2) CW Limit";
	char ccwLimitStr [] = "(3) CCW Limit";
	char movingSpeedStr [] = "(4) Moving Speed";
	char presPositionStr [] = "Current Position";
	char presVoltageStr [] = "Current Voltage";
	*/

	if(!(dxl_initialize(DEFAULT_DEV_INDEX, DEFAULT_BAUDNUM))){

		printf("Failed to initialize USB interface.\n");
		return 1;
	}
	else{

		printf("Successfully initialized USB interface.\n");
	}

	printf("Detecting Dynamixel...\n");

	int foundDxl = 0;

	while(!foundDxl){

		dxl_ping(id);

		if(isSuccess())
			foundDxl = 1;
		else
			id ++;
	}

	printf("Found Dynamixel!\n");

	printInfo();

	int doExit = 0;

	while(!doExit){

		printf("Enter 1 - 3 to change corresponding field, 9 to refresh, or 0 to exit:");
		
		switch(atoi(readIn())){

			case 0:
				doExit = 1;
				break;
			case 1:
				printf("New ID; Enter a number from 1 to 253:");
				int newId = atoi(readIn());
				writeByte(P_ID, newId);
				id = newId;
				if(readByte(P_ID) == id)
					printf("ID successfully changed to %d.\n", id);
				else
					printf("ID unsuccessfully changed.\n");
				break;
			case 2:
				printf("New CW Limit; Enter a number from 0 to 1023:");
				int cwLimit = atoi(readIn());
				writeWord(P_CW_ANG_LIMIT, cwLimit);
				if(readWord(P_CW_ANG_LIMIT) == cwLimit)
					printf("CW limit successfully changed to %.4d.\n", cwLimit);
				else
					printf("CW limit unsuccessfully changed.\n");
				break;
			case 3:
				printf("New CCW Limit; Enter a number from 0 to 1023:");
				int ccwLimit = atoi(readIn());
				writeWord(P_CCW_ANG_LIMIT, ccwLimit);
				if(readWord(P_CCW_ANG_LIMIT) == ccwLimit)
					printf("CCW limit successfully changed to %.4d.\n", ccwLimit);
				else
					printf("CCW limit unsuccessfully changed.\n");
				break;
			case 9:
				printf("Refreshing info...\n");
				printInfo();
				break;
			default:
				printf("Invalid entry.\n");
				break;
		}
	}

	printf("Exiting...\n");
	dxl_terminate();
	return 0;
}

void printInfo(){

	cwLimitVal = readWord(P_CW_ANG_LIMIT);
	ccwLimitVal = readWord(P_CCW_ANG_LIMIT);
	movingSpeedVal = readWord(P_MOVING_SPEED);
	presPositionVal = readWord(P_PRESENT_POS);
	presVoltageVal = readByte(P_PRESENT_V);

	printf("(1) ID:               %.5d\n", id);
	printf("(2) CW Limit:         %.5d\n", cwLimitVal);
	printf("(3) CCW Limit:        %.5d\n", ccwLimitVal);
	printf("    Moving Speed:     %.5d\n", movingSpeedVal);
	printf("    Current Position: %.5d\n", presPositionVal);
	printf("    Current Voltage:  %.5d\n", presVoltageVal);

	return;
}

int readByte(int addr){

	int data = dxl_read_byte(id, addr);
	int result;

	if(!(result = isSuccess()))
		return result;
	else
		return data;
}

int readWord(int addr){

	int data = dxl_read_word(id, addr);
	int result;

	if(!(result = isSuccess()))
		return result;
	else
		return data;
}

int writeByte(int addr, char data){

	dxl_write_byte(id, addr, data);

	return isSuccess();
}

int writeWord(int addr, short data){

	dxl_write_word(id, addr, data);

	return isSuccess();
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
			printf("Error: COMM_TXFAIL\n");
			return 0;
		case COMM_RXFAIL:
			printf("Error: COMM_RXFAIL\n");
			return 0;
		case COMM_TXERROR:
			printf("Error: COMM_TXERROR\n");
			return 0;
		case COMM_RXWAITING:
			printf("Error: COMM_RXWAITING\n");
			return 0;
		case COMM_RXTIMEOUT:
			printf("Error: COMM_RXTIMEOUT\n");
			return 0;
		case COMM_RXCORRUPT:
			printf("Error: COMM_RXCORRUPT\n");
			return 0;
		default:
			printf("Error: Unknown: %d\n", result);
			return 0;
	}
}
