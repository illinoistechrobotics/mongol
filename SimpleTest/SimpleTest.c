//SimpleTest
//By Allen Baker

#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>

// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46

// Defualt setting
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define DEFAULT_ID		1

int main(){

	int deviceIndex = 0;

	if(!(dxl_initialize(deviceIndex, DEFAULT_BAUDNUM))){

		printf("Failed to initialize USB interface.\n");
		return 1;
	}
	else{

		printf("Successfully initialized USB interface.\n");
	}

	printf("Exiting...\n");
	dxl_terminate();
	return 0;
}
