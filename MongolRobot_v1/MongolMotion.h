#ifndef MOVETEST_POSES
#define MOVETEST_POSES

#include <avr/pgmspace.h>

// horizontal rotation constants
#define FR_H_FWD    460
#define FR_H_MID    358
#define FR_H_BKD    256
    
#define BR_H_FWD    767
#define BR_H_MID    665
#define BR_H_BKD    563
    
#define FL_H_FWD    BR_H_BKD
#define FL_H_MID    BR_H_MID
#define FL_H_BKD    BR_H_FWD
    
#define BL_H_FWD    FR_H_BKD
#define BL_H_MID    FR_H_MID
#define BL_H_BKD    FR_H_FWD

//gun constants
#define GUN_H 13
#define GUN_V 14
#define GUN_FIRE 2

// vertical rotation constants
#define FR_V_DN     665
#define FR_V_UP     818
        
#define BR_V_DN     FR_V_DN
#define BR_V_UP     FR_V_UP
        
#define FL_V_UP     205
#define FL_V_DN     358
        
#define BL_V_UP     FL_V_UP
#define BL_V_DN     FL_V_DN

// foot rotation constants
#define FR_F_DN     808

#define BR_F_DN     FR_F_DN

#define FL_F_DN     215

#define BL_F_DN     FL_F_DN

// turret constants

#define GUN_H_STRGHT    512
#define GUN_V_STRGHT    512

//starting pose
PROGMEM prog_uint16_t START[]=          {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_DN, FL_V_DN, BL_V_DN, BR_V_DN, FR_H_MID, FL_H_MID, BL_H_MID, BR_H_MID, GUN_H_STRGHT, GUN_V_STRGHT};

// forward and backward walking poses
PROGMEM prog_uint16_t TO_FL_FWD[]=      {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_DN, FL_V_UP, BL_V_DN, BR_V_UP, FR_H_MID, FL_H_MID, BL_H_MID, BR_H_MID, GUN_H_STRGHT, GUN_V_STRGHT};
PROGMEM prog_uint16_t FL_FWD[]=         {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_DN, FL_V_DN, BL_V_DN, BR_V_DN, FR_H_BKD, FL_H_FWD, BL_H_BKD, BR_H_FWD, GUN_H_STRGHT, GUN_V_STRGHT};
PROGMEM prog_uint16_t TO_FR_FWD[]=      {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_UP, FL_V_DN, BL_V_UP, BR_V_DN, FR_H_MID, FL_H_MID, BL_H_MID, BR_H_MID, GUN_H_STRGHT, GUN_V_STRGHT};
PROGMEM prog_uint16_t FR_FWD[]=         {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_DN, FL_V_DN, BL_V_DN, BR_V_DN, FR_H_FWD, FL_H_BKD, BL_H_FWD, BR_H_BKD, GUN_H_STRGHT, GUN_V_STRGHT};

// right turning poses
PROGMEM prog_uint16_t TO_FR_FWD_RT[]=   {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_DN, FL_V_UP, BL_V_DN, BR_V_UP, FR_H_MID, FL_H_MID, BL_H_MID, BR_H_MID, GUN_H_STRGHT, GUN_V_STRGHT};
PROGMEM prog_uint16_t FR_FWD_RT[]=      {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_DN, FL_V_DN, BL_V_DN, BR_V_DN, FR_H_FWD, FL_H_FWD, BL_H_BKD, BR_H_BKD, GUN_H_STRGHT, GUN_V_STRGHT};
PROGMEM prog_uint16_t TO_FL_FWD_RT[]=   {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_UP, FL_V_DN, BL_V_UP, BR_V_DN, FR_H_MID, FL_H_MID, BL_H_MID, BR_H_MID, GUN_H_STRGHT, GUN_V_STRGHT};
PROGMEM prog_uint16_t FL_FWD_RT[]=      {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_DN, FL_V_DN, BL_V_DN, BR_V_DN, FR_H_BKD, FL_H_BKD, BL_H_FWD, BR_H_FWD, GUN_H_STRGHT, GUN_V_STRGHT};

// left turning poses
PROGMEM prog_uint16_t TO_FL_FWD_LT[]=   {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_DN, FL_V_UP, BL_V_DN, BR_V_UP, FR_H_MID, FL_H_MID, BL_H_MID, BR_H_MID, GUN_H_STRGHT, GUN_V_STRGHT};
PROGMEM prog_uint16_t FL_FWD_LT[]=      {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_DN, FL_V_DN, BL_V_DN, BR_V_DN, FR_H_BKD, FL_H_BKD, BL_H_FWD, BR_H_FWD, GUN_H_STRGHT, GUN_V_STRGHT};
PROGMEM prog_uint16_t TO_FR_FWD_LT[]=   {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_UP, FL_V_DN, BL_V_UP, BR_V_DN, FR_H_MID, FL_H_MID, BL_H_MID, BR_H_MID, GUN_H_STRGHT, GUN_V_STRGHT};
PROGMEM prog_uint16_t FR_FWD_LT[]=      {12, FR_F_DN, FL_F_DN, BL_F_DN, BR_F_DN, FR_V_DN, FL_V_DN, BL_V_DN, BR_V_DN, FR_H_FWD, FL_H_FWD, BL_H_BKD, BR_H_BKD, GUN_H_STRGHT, GUN_V_STRGHT};

// old eight-servo poses

// forward and backward walking poses
//PROGMEM prog_uint16_t TO_FL_UP[]=       {8, BR_V_UP, FR_V_DN, FR_H_MID, BL_H_MID, BL_V_DN, FL_V_UP, BR_H_MID, FL_H_MID};
//PROGMEM prog_uint16_t FL_UP[]=          {8, BR_V_DN, FR_V_DN, FR_H_BKD, BL_H_BKD, BL_V_DN, FL_V_DN, BR_H_FWD, FL_H_FWD};
//PROGMEM prog_uint16_t TO_FR_UP[]=       {8, BR_V_DN, FR_V_UP, FR_H_MID, BL_H_MID, BL_V_UP, FL_V_DN, BR_H_MID, FL_H_MID};
//PROGMEM prog_uint16_t FR_UP[]=          {8, BR_V_DN, FR_V_DN, FR_H_FWD, BL_H_FWD, BL_V_DN, FL_V_DN, BR_H_BKD, FL_H_BKD};

// right turning poses
// PROGMEM prog_uint16_t TO_FR_FWD_RT[]=   {8, BR_V_UP, FR_V_DN, FR_H_MID, BL_H_MID, BL_V_DN, FL_V_UP, BR_H_MID, FL_H_MID};
// PROGMEM prog_uint16_t FR_FWD_RT[]=      {8, BR_V_DN, FR_V_DN, FR_H_FWD, BL_H_BKD, BL_V_DN, FL_V_DN, BR_H_BKD, FL_H_FWD};
// PROGMEM prog_uint16_t TO_FL_FWD_RT[]=   {8, BR_V_DN, FR_V_UP, FR_H_MID, BL_H_MID, BL_V_UP, FL_V_DN, BR_H_MID, FL_H_MID};
// PROGMEM prog_uint16_t FL_FWD_RT[]=      {8, BR_V_DN, FR_V_DN, FR_H_BKD, BL_H_FWD, BL_V_DN, FL_V_DN, BR_H_FWD, FL_H_BKD};

//left turning poses
// PROGMEM prog_uint16_t TO_FL_FWD_LT[]=   {8, BR_V_UP, FR_V_DN, FR_H_MID, BL_H_MID, BL_V_DN, FL_V_UP, BR_H_MID, FL_H_MID};
// PROGMEM prog_uint16_t FL_FWD_LT[]=      {8, BR_V_DN, FR_V_DN, FR_H_BKD, BL_H_FWD, BL_V_DN, FL_V_DN, BR_H_FWD, FL_H_BKD};
// PROGMEM prog_uint16_t TO_FR_FWD_LT[]=   {8, BR_V_DN, FR_V_UP, FR_H_MID, BL_H_MID, BL_V_UP, FL_V_DN, BR_H_MID, FL_H_MID};
// PROGMEM prog_uint16_t FR_FWD_LT[]=      {8, BR_V_DN, FR_V_DN, FR_H_FWD, BL_H_BKD, BL_V_DN, FL_V_DN, BR_H_BKD, FL_H_FWD};

//forward walking sequence
PROGMEM transition_t FORWARD[] =        {{0,4} ,{TO_FL_FWD,250} ,{FL_FWD,250} ,{TO_FR_FWD,250} ,{FR_FWD,250} };

//backward walking sequence
PROGMEM transition_t BACKWARD[] =       {{0,4} ,{FL_FWD,250} ,{TO_FL_FWD,250} ,{FR_FWD,250} ,{TO_FR_FWD,250} };

//right turning sequence
PROGMEM transition_t TURN_RIGHT[] =     {{0,4} ,{TO_FR_FWD_RT,250} ,{FR_FWD_RT,250} ,{TO_FL_FWD_RT,250} ,{FL_FWD_RT,250} };

//left turning sequence
PROGMEM transition_t TURN_LEFT[] =      {{0,4} ,{TO_FL_FWD_LT,250} ,{FL_FWD_LT,250} ,{TO_FR_FWD_LT,250} ,{FR_FWD_LT,250} };

//next up are strafing sequences :0
//stay tuned!

#endif
