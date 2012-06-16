#include <string.h>
#include <ax12.h>
#include <BioloidController.h>
#include "MongolMotion.h"
#include "MongolSerial.h"

// Serial communication helper functions
void reconnect (void);
void init_serial (void);
int extract_msg (packet* msg, char* inbuf);
int read_serial (packet* msg);
void write_serial (packet* msg);
void say_hello (void);
void say_ready (void);
void close_serial (void);

packet cur_pkt;
packet prev_pkt;

byte cur_move;              // Current movement mode variable
byte cur_turn;              // Current turning mode variable
byte cur_h_aim;             // Current horizontal aim mode variable
byte cur_v_aim;             // Current vertical aim mode variable
byte cur_fire;              // Current firing mode variable
byte cur_l_strf;            // Current left-strafing mode variable
byte cur_r_strf;            // Current right-strafing mode variable

int mov_fwd_indx;
int mov_bkd_indx;
int trn_l_indx;
int trn_r_indx;
int intpol_spd = 67;

BioloidController servos = BioloidController(1000000);

void setup(){
  
    // Initialize serial communications (includes first HELLO packet)
    init_serial();

    cur_move = MOV_STOP;
    cur_turn = TRN_NONE;
    cur_h_aim = AIM_H_STRGHT;
    cur_v_aim = AIM_V_STRGHT;
    cur_fire = FIRE_OFF;
    cur_l_strf = STRF_L_OFF;
    cur_r_strf = STRF_R_OFF;
    mov_fwd_indx = 1;
    mov_bkd_indx = 1;
    trn_l_indx = 1;
    trn_r_indx = 1;

}

// Main loop of program
void loop(){
    
    say_ready();
    while(read_serial(&cur_pkt) < 0)
        delay(10);      // Allow time for a complete packet to arrive

    if(cur_pkt.type == PKT_STDBY)
        cur_pkt = prev_pkt;
    
    switch(cur_pkt.type){

        case PKT_HELLO:
        case PKT_RDY:
            break;

        case PKT_MOVE:
            switch(cur_pkt.value){

                case MOV_STOP:
                    cur_move = MOV_STOP;
                    interpolate(START);
                    mov_fwd_indx = 1;
                    mov_bkd_indx = 1;
                    break;

                case MOV_FWD:
                    cur_move = MOV_FWD;
                    if(mov_fwd_indx == 5)
                        mov_fwd_indx = 1;
                    interpolate(FORWARD[mov_fwd_indx ++].pose);
                    break;
                
                case MOV_BKD:
                    cur_move = MOV_BKD;
                    if(mov_bkd_indx == 5)
                        mov_bkd_indx =1;
                    interpolate(BACKWARD[mov_bkd_indx ++].pose);
                    break;
            }
            break;

        case PKT_TURN:
            if(cur_move != MOV_STOP)
                break;

            switch(cur_pkt.value){

                case TRN_NONE:
                    cur_turn = TRN_NONE;
                    interpolate(START);
                    trn_l_indx = 1;
                    trn_r_indx = 1;
                    break;

                case TRN_LEFT:
                    cur_turn = TRN_LEFT;
                    if(trn_l_indx == 5)
                        trn_l_indx = 1;
                    interpolate(TURN_LEFT[trn_l_indx ++].pose);
                    break;

                case TRN_RIGHT:
                    cur_turn = TRN_RIGHT;
                    if(trn_r_indx == 5)
                        trn_r_indx = 1;
                    interpolate(TRN_RIGHT[trn_r_indx ++].pose);
                    break;
            }
            break;

        case PKT_AIM_H:
            if((cur_move != MOV_STOP) &&
               (cur_turn != TRN_NONE))
                break;

            switch(cur_pkt.value){

                case AIM_H_STRGHT:
                    break;

                case AIM_H_LEFT:
                    SetPosition(GUN_H,
                                ax12GetRegister(GUN_H, AX_PRESENT_POSITION_L, 2) + 10);
                    while(ax12GetRegister(GUN_H, AX_MOVING, 1));
                    break;
                
                case AIM_H_RIGHT:
                    SetPosition(GUN_H,
                                ax12GetRegister(GUN_H, AX_PRESENT_POSITION_L, 2) - 10);
                    while(ax12GetRegister(GUN_H, AX_MOVING, 1));
                    break;
            }
            break;
        
        case PKT_AIM_V:
            if((cur_move != MOV_STOP) &&
               (cur_turn != TRN_NONE))
                break;

            switch(cur_pkt.value){

                case AIM_V_STRGHT:
                    break;

                case AIM_V_DWN:
                    SetPosition(GUN_V,
                                ax12GetRegister(GUN_H, AX_PRESENT_POSITION_L, 2) + 10);
                    while(ax12GetRegister(GUN_V, AX_MOVING, 1));
                    break;
                
                case AIM_V_UP:
                    SetPosition(GUN_V,
                                ax12GetRegister(GUN_V, AX_PRESENT_POSITION_L, 2) - 10);
                    while(ax12GetRegister(GUN_V, AX_MOVING, 1));
                    break;
            }
            break;

        case PKT_FIRE:
            if (cur_pkt.value == FIRE_ON)
                digitalWrite(GUN_FIRE, HIGH);
            else
                digitalWrite(GUN_FIRE, LOW);
            break;

        case PKT_STRF_L:
        case PKT_STRF_R:
            break;
    
        case PKT_GDBY:
            reconnect();
            break;
    }
}

/*** SERVO MANIPULATION FUNCTIONS ***/

void interpolate(const unsigned int * pose){
    
    servos.loadPose(pose);
    servos.interpolateSetup(intpol_spd);
    while(servos.interpolating > 0){
        servos.interpolateStep();
        delay(1);
    }

    return;
}

/*** SERIAL COMMUNICATION FUNCTIONS ***/

void reconnect(){
    
    packet response;
    
    do{
        say_hello();
        delay(1000);
    } while((read_serial(&response) < 0) ||
            (response.type != PKT_HELLO));
            
    return;
}
    
void init_serial (){

    Serial.begin(38400);
    reconnect();
    return;
}

int extract_msg (packet* msg, char* inbuf){

    char* start;
    char* end;

    if ((start = strchr(inbuf, PKT_BND)) &&
        (end = strchr((start+1), PKT_BND))){
        *(end+1) = 0;
        strcpy((char*)msg, start);
        return strlen(start);
    }
    else
        return -1;
}

int read_serial (packet* msg){
    
    char inbuf [SER_BUFSIZ];
    int inbuf_len = 0;
    int inmsg_len = 0;
    byte nxt_byte;

    while(Serial.available() &&
          (inbuf_len < SER_BUFSIZ) &&
          ((nxt_byte = Serial.read()) > 0)){
    
        inbuf[inbuf_len++] = nxt_byte;
    }
    inbuf[inbuf_len] = 0;
    
    if((inmsg_len = extract_msg(msg, inbuf)) < PKT_LEN)
        return -1;
        
    return inmsg_len;
}

void write_serial (packet * msg){
    
    char outbuf [SER_BUFSIZ];

    if(msg && (msg->type)){
    
        msg->front_bnd = PKT_BND;
        msg->end_bnd = PKT_BND;

        // These packet types have no defined values, but must be filled anyways
        if((msg->type == PKT_HELLO) ||
           (msg->type == PKT_GDBY) ||
           (msg->type == PKT_STDBY) ||
           (msg->type == PKT_RDY))
            msg->value = PKT_FIL;

        msg->null_term = 0;
        
        strcpy(outbuf, (char*)msg);
        Serial.write((byte*)outbuf,PKT_LEN);
        Serial.flush();
        
        return;
    }
}

void say_hello(){

    packet hello;
    hello.type = PKT_HELLO;
    write_serial(&hello);
    return;
}

void say_ready(){

    packet ready;
    ready.type = PKT_RDY;
    write_serial(&ready);
    return;
}

void close_serial (){

    Serial.end();
}
