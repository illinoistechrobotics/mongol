//Alternates between standing and sitting
//Written by Allen Baker and Bryce Shurts

#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>

#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46

#define SITTING_POS		256
#define STANDING_POS		512

#define DEFAULT_BAUDNUM		1	// 1Mbps

int main(){

	int devNumber = 0;
	int baudNumber = 1;	//For 1 Mbps baud

	printf("Running StandSit. Press ESC to terminate.\n");

	if(!(dxl_initialize(devNumber, baudNumber))){

		printf("Failed to open USB Interface. :(\n");
		return 0;
	}
	else{

		printf("USB Interfaced Opened! :D\n");
	}

//	if(SITTING_POS != dxl_read_byte(BROADCAST_ID, P_PRESENT_POSITION_L)){
//
//		printf("Going to initial sitting position.\n");
//		dxl_write_word(BROADCAST_ID, P_PRESENT_POSITION_L, SITTING_POS);
//		while(1 == dxl_read_byte(BROADCAST_ID, P_MOVING));
//	}

	int loopCount = 0;	

	printf("Going into main loop...\n");
	while(1){

		printf("Loop time! Round %d\n", loopCount++);

	//	if(getchar() == 0x1b)
	//		break;

		if(STANDING_POS == dxl_read_byte(BROADCAST_ID, P_PRESENT_POSITION_L)){

			printf("Sitting down...\n");
			dxl_write_word(BROADCAST_ID, P_PRESENT_POSITION_L, SITTING_POS);
		}

		else if(SITTING_POS == dxl_read_byte(BROADCAST_ID, P_PRESENT_POSITION_L)){

			printf("Standing up...\n");
			dxl_write_word(BROADCAST_ID, P_PRESENT_POSITION_L, STANDING_POS);
		}

		else{

			int CommStatus = dxl_get_result();

			switch(CommStatus)
			{
			case COMM_TXFAIL:
				printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
				break;

			case COMM_TXERROR:
				printf("COMM_TXERROR: Incorrect instruction packet!\n");
				break;

			case COMM_RXFAIL:
				printf("COMM_RXFAIL: Failed get status packet from device!\n");
				break;

			case COMM_RXWAITING:
				printf("COMM_RXWAITING: Now recieving status packet!\n");
				break;

			case COMM_RXTIMEOUT:
				printf("COMM_RXTIMEOUT: There is no status packet!\n");
				break;

			case COMM_RXCORRUPT:
				printf("COMM_RXCORRUPT: Incorrect status packet!\n");
				break;

			default:
				printf("This is unknown error code!\n");
				break;
			}
		}

		//while(1 == dxl_read_byte(BROADCAST_ID, P_MOVING));

		sleep(1);
	}

	dxl_terminate();
	printf("Shutting down...\n");
	return 0;
}
