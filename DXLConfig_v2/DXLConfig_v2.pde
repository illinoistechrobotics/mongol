// Mongol terminal program
// Written by Allen Baker
//     for Illinois Tech Robotics

#include <string.h>
#include <ax12.h>
#include <BioloidController.h>
#include "MongolMotion.h"

#define CMD_BUFSIZ    128
#define NUM_CMDS      3
#define MAX_ARGS      5

#define SER_BAUD      38400
#define DXL_BAUD      1000000

char* cmd_str_list [NUM_CMDS+1] = {
    "PA",
    "PDXL",
    "CH",
    NULL
};
                                   
int (*cmd_sub_list [NUM_CMDS+1])(void) = {
    print_all,
    print_dxl,
    change_id,
    NULL
};

char cur_cmd_str [CMD_BUFSIZ];
char* cur_cmd_args [MAX_ARGS];

/*** SERIAL COMMUNICATION FUNCTIONS ***/

int (*get_next_cmd(char* next_cmd, char** next_cmd_args))(void){

    int char_indx = 0;
    while(!(Serial.available()));
    for(int char_indx = 0;
        (Serial.available() > 0) && char_indx < CMD_BUFSIZ; 
        char_indx ++){
            
        next_cmd[char_indx] = Serial.read();
        delay(5);
    }
        
    Serial.print("Received: ");
    Serial.println(next_cmd);
    
    char** cmd_str_list_p;
    int(**cmd_sub_list_p)(void);
    
    *cmd_sub_list_p = cmd_sub_list[0];
    for(*cmd_str_list_p = cmd_str_list[0]; 
        *cmd_str_list_p != NULL;
        cmd_str_list_p ++){
    
        if(strcmp(next_cmd, *cmd_str_list_p) == 0){
            
            while((*next_cmd_args = strtok(next_cmd, " ")))
                next_cmd_args ++;
                
            return *cmd_sub_list_p;
        }
            
        cmd_sub_list_p ++;
    }
    
    return NULL;
}

/*** COMMAND SUBROUTINES ***/

int print_all(){
    
    for(int i = 1; i < AX12_MAX_SERVOS; i ++){
        
        Serial.print(i);
        Serial.print('\t');
        Serial.print(ax12GetRegister(i, AX_PRESENT_POSITION_L, 2));
        Serial.println();
    }
    return 0;
}

int print_dxl(){
    
    int id = int(cur_cmd_args[1]);
    Serial.print(id);
    Serial.print('\t');
    Serial.print(ax12GetRegister(id, AX_PRESENT_POSITION_L, 2));
    Serial.println();
    return 0;
}

int change_id(){
    
    int cur_id = int(cur_cmd_args[1]);
    int new_id = int(cur_cmd_args[2]);
    ax12SetRegister(cur_id, AX_ID, new_id);
    Serial.print("DXL "); Serial.print(cur_id);
    Serial.print(" changed to "); Serial.print(new_id);
    Serial.println();
    return 0;
}

int change_pos(){
    
    int id = int(cur_cmd_args[1]);
    int new_pos = int(cur_cmd_args[2]);
    ax12SetRegister2(id, AX_GOAL_POSITION_L, new_pos);
    return 0;
}

/*** MAIN PROGRAM ***/

void setup(){
    
    ax12Init(DXL_BAUD);
    Serial.begin(SER_BAUD);
}

void loop(){ 
    
    int (*cur_cmd_sub)(void);
    int retval;

    Serial.print("Mongol > ");
    
    if((cur_cmd_sub = get_next_cmd(cur_cmd_str, cur_cmd_args)))
        retval = (*cur_cmd_sub)();
    else
        Serial.println("\nERROR: Unrecognized command.");
}
