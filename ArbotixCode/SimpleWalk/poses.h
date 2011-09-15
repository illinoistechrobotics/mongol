#ifndef MONGOLWALK_POSES
#define MONGOLWALK_POSES

#include <avr/pgmspace.h>

PROGMEM prog_uint16_t Start[] = {8, 511, 511, 731, 291, 511, 511, 291, 731};
PROGMEM prog_uint16_t RFDown[] = {8, 511, 511, 631, 631, 511, 511, 631, 631};
PROGMEM prog_uint16_t LFLift[] = {8, 511, 371, 631, 391, 371, 511, 391, 631};
PROGMEM prog_uint16_t LFDown[] = {8, 511, 511, 631, 391, 511, 511, 391, 631};
PROGMEM prog_uint16_t RFLift[] = {8, 651, 511, 631, 631, 511, 651, 631, 631};

PROGMEM transition_t Walk[] = {{0,4} ,{LFLift,500} ,{LFDown,500} ,{RFLift,500} ,{RFDown,500} };

#endif
