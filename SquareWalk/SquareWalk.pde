// Square Walk
// A simple forward-walking program written
// after Mongol was reconstructed in a square
// frame.
// Written by Allen Baker

#include <ax12.h>
#include <BioloidController.h>
#include "ForwardWalk.h"

BioloidController control = BioloidController(1000000);

void assumeThePosition(){
  
  delay(100);
  control.loadPose(START);
  control.readPose();
  control.interpolateSetup(500);
  while(control.interpolating > 0){ 
    control.interpolateStep();
    delay(3);
  }
}

void setup(){
  
  assumeThePosition();
}

int i = 0;
transition_t * seqToPlay;

void loop(){
  
  if(i < 9){
    seqToPlay = FORWARD;
  }
  else{
    seqToPlay = BACKWARD;
  }
  
  control.playSeq(seqToPlay);
  while(control.playing) control.play();
  
  i++;
  if(i == 19){
    assumeThePosition();
    i = 0;
  }
  else if(i == 9){
    assumeThePosition();
  }
}
