//SimpleTest
//By Allen Baker

#include <stdlib.h>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>

#define P_MODEL_NUM		0
#define P_FW_VER		2
#define P_ID			3

// Control table address
#define P_GOAL_POSITION		30
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46

// Defualt setting
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define DEFAULT_ID		1

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

	printf("Press ENTER to continue with the test:");
	getchar();

	/*
	printf("Moving Dynamixel to Position 0...\n");
	writeWord(P_GOAL_POSITION, 0);
	while(dxl_read_byte(id, P_MOVING));
	
	printf("Moving Dynamixel to Position 1024...\n");
	writeWord(P_GOAL_POSITION, 1024);
	while(dxl_read_byte(id, P_MOVING));
	
	printf("Moving Dynamixel to Position 0...\n");
	writeWord(P_GOAL_POSITION, 0);
	while(dxl_read_byte(id, P_MOVING));
	*/

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
	printf("Data: %d\n", data);

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
