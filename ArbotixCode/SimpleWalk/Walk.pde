#include <ax12.h>
#include <BioloidController.h>
#include "walk.h"

BioloidController bioloid = BioloidController(1000000);

void setup(){
    // stand up slowly
    delay(100);                    // recommended pause
    bioloid.loadPose(start);
    bioloid.readPose();
    bioloid.interpolateSetup(500);
    while(bioloid.interpolating > 0){
        bioloid.interpolateStep();
        delay(3);
    }
    
    // start our walking
    bioloid.playSeq(Walk);
}

void loop(){
    bioloid.play();
}
