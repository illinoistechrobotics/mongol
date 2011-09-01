//SimpleTest
//By Allen Baker

#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>

#define P_MODEL_NUM		0
#define P_FW_VER		2
#define P_ID			3

// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46

// Defualt setting
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define DEFAULT_ID		1

int id = 0;

//prototypes
int readByte(int addr);
int readWord(int addr);
int writeByte(int addr, char data);
int isSuccess(void);
void flushIn(void);

int main(){

	int deviceIndex = 0;

	if(!(dxl_initialize(deviceIndex, DEFAULT_BAUDNUM))){

		printf("Failed to initialize USB interface.\n");
		return 1;
	}
	else{

		printf("Successfully initialized USB interface.\n");
	}
	
	printf("Press ENTER to begin the test:");
	getchar();

	dxl_ping(id);
	while(!isSuccess()){

		dxl_ping(++id);
	}
	printf("Dynamixel ID: %d\n", id);

	//Get model number of Dynamixel
	int modelNum = readWord(P_MODEL_NUM);
	printf("Model Number: %d\n", modelNum);

	//Get firmware version of Dynamixel
	int fwVer = readByte(P_FW_VER);
	printf("Firmware Version: %d\n", fwVer);

	//Get ID of Dynamixel
	id = readByte(P_ID);
	printf("Dynamixel ID: %d\n", id);

	printf("Assign different ID? (y/n):");
	char a = getchar();
	if(a == 'y'){

		flushIn();
		printf("Enter number from 0 to 9:");
		char newId = getchar() - '0';

	//	if(writeByte(newId, P_ID) && ((id = newId) == readByte(P_ID)))
	//		printf("Successfully assigned ID.\n");

		if(writeByte(newId, P_ID)){

			id = (int) newId;
			if(id == readByte(P_ID))
				printf("Successfully assigned ID.\n");
			else
				printf("Error: Could not assign ID.\n");
		}

		else
			printf("Error: Could not assign ID.\n");
	}
	
	flushIn();
	printf("Press ENTER to continue with the test:");
	getchar();

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
