//SimpleTest
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
#define CW_POS			441
#define CENTER_POS		511
#define CCW_POS			581
#define MAX_CCW_POS		1023

#define STOP_SPEED		0
#define MED_SPEED		511
#define MAX_SPEED		1023

#define DEFAULT_BAUDNUM		1 	// 1Mbps

int id = 0;

char inputBuffer [100];

//prototypes
int readByte(int addr);
int readWord(int addr);
int writeByte(int addr, char data);
int writeWord(int addr, short data);
int isSuccess(void);
char * readIn(void);
void flushIn(void);

int main(){

	int deviceIndex = 0;
	int validId = 0;

	if(!(dxl_initialize(deviceIndex, DEFAULT_BAUDNUM))){

		printf("Failed to initialize USB interface.\n");
		return 1;
	}
	else{

		printf("Successfully initialized USB interface.\n");
	}

	while(!validId){

		printf("Enter the ID of the Dynamixel you want to test:");
		id = atoi(readIn());

		printf("Pinging ID %d...\n", id);

		dxl_ping(id);
		if(!isSuccess()){

			printf("Incorrect ID. Try again (0) or autodetect (1)?");
			
			if(atoi(readIn())){	//If autodetect is chosen

				id = 0;
				dxl_ping(id);
				while(!isSuccess()){

					dxl_ping(++id);
				}

				validId = 1;
			}
		}
		else
			validId = 1;
	}

	printf("Using Dynamixel %d\n", id);
	
	printf("Assign different ID? (y/n):");
	char a = getchar();
	if(a == 'y'){

		flushIn();
		printf("Enter number from 1 to 253:");
		char newId = atoi(readIn());

		if(writeByte(P_ID, newId)){

			id = (int) newId;
			if(id == readByte(P_ID))
				printf("Successfully assigned ID.\nNew ID: %d\n", id);
			else
				printf("Error: Could not assign ID.\n");
		}

		else
			printf("Error: Could not assign ID.\n");
	}
	
	
	printf("Press ENTER to begin the test:");
	getchar();

	//Get model number of Dynamixel
	int modelNum = readWord(P_MODEL_NUM);
	printf("Model Number: %d\n", modelNum);

	//Get firmware version of Dynamixel
	int fwVer = readByte(P_FW_VER);
	printf("Firmware Version: %d\n", fwVer);

	printf("Voltage: %d\n", readByte(P_PRESENT_V));

	//Get clockwise angle limit
	printf("ClockWise Angle Limit: %d\n", readWord(P_CW_ANG_LIMIT));

	printf("Counterclockwise Angle Limit: %d\n", readWord(P_CCW_ANG_LIMIT));

	printf("Moving Speed: %d\n", readWord(P_MOVING_SPEED));

	//Get present position of Dynamixel
	int presPos = readWord(P_PRESENT_POS);
	printf("Present Position: %d\n", presPos);

	printf("Goal Position: %d\n", readWord(P_GOAL_POS));

	printf("Press ENTER to continue with the test:");
	getchar();
 
	writeWord(P_MOVING_SPEED, MED_SPEED);

	printf("Moving Dynamixel to Center Position...\n");
	if(writeWord(P_GOAL_POS, CENTER_POS)){
		//sleep(1);
		while(readByte(P_MOVING)); /*{sleep(1);}*/
		printf("Moving completed.\n");
	}
	else
		printf("Error: Could not make servo move.\n");

	printf("Moving Dynamixel in the CW direction...\n");
	if(writeWord(P_GOAL_POS, CW_POS)){
		//sleep(1);
		while(readByte(P_MOVING)); /*{sleep(1);}*/
		printf("Moving completed.\n");
	}
	else
		printf("Error: Could not make servo move.\n");
	
	printf("Moving Dynamixel in the CCW direction...\n");
	if(writeWord(P_GOAL_POS, CCW_POS)){
		//sleep(1);
		while(readByte(P_MOVING)); /*{sleep(1);}*/
		printf("Moving completed.\n");
	}
	else
		printf("Error: Could not make servo move.\n");
	
	printf("Moving Dynamixel to Center Position...\n");
	if(writeWord(P_GOAL_POS, CENTER_POS)){
		//sleep(1);
		while(readByte(P_MOVING)); /*{sleep(1);}*/
		printf("Moving completed.\n");
	}
	else
		printf("Error: Could not make servo move.\n");

	printf("Exiting...\n");
	dxl_terminate();
	return 0;
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

void flushIn(){

	char a;
	while((a = getchar()) != '\n' && a != EOF);
	
	return;
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
