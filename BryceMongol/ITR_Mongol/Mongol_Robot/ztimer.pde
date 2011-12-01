//    timer.c - generates timer events
//    Copyright (C) 2011  Illinois Institute of Technology Robotics
//	  <robotics@iit.edu>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

void timer(robot_queue *q) {
  static unsigned long  last_sent_1hz = 0;
  static unsigned long  last_sent_10hz = 0;
  static unsigned long  last_sent_25hz = 0;
  static unsigned long  last_sent_50hz = 0;
  static unsigned long  last_sent_100hz = 0;
  static unsigned int power_led_state = 0;
  
  robot_event evt;
  evt.command = ROBOT_EVENT_TIMER;
  evt.value = 0;
  //1hz and 10hz timers used by default to use other timer define them in robot.h
  if(millis() - last_sent_1hz >= 1000){   //1 hertz 1000 millis
    last_sent_1hz= millis();
    send_alive_signal();
  #ifdef TIMER_1HZ_
    evt.index = 1;
    on_1hz_timer(&evt);
  #endif
  }
  // used for failsafe mode
  if(millis() - last_sent_10hz >= 100){    //10 hertz 100 millis
    last_sent_10hz = millis();
    comm_watchdog();
  #ifdef TIMER_10HZ_
    evt.index = 2;
    on_10hz_timer(&evt);  
  #endif
  #ifdef WATCHDOG_
    wdt_reset(); //watchdog timer reset 
  #endif
  #ifdef POWER_LED_ //flash led
    if(failsafeMode){
      digitalWrite(POWER_LED_PIN,HIGH);
    }
    else{
      power_led_state++;
      power_led_state&=B00000001; //more effecient than taking modolus
      digitalWrite(POWER_LED_PIN,power_led_state);
    }
  #endif
  }
#ifdef TIMER_25HZ_  
  if(millis() - last_sent_25hz >= 40){
    last_sent_25hz = millis();
    evt.index = 3;
    on_25hz_timer(&evt);
  }
#endif
#ifdef TIMER_50HZ_  
  if(millis() - last_sent_50hz >= 20){
    last_sent_50hz = millis();
    evt.index = 4;
    on_50hz_timer(&evt);
  }
#endif
#ifdef TIMER_100HZ_
  if(millis() - last_sent_100hz >= 10){   //100 hertz 10 millis
    last_sent_100hz= millis();
    evt.index = 5;
    on_100hz_timer(&evt);
  }
#endif  
}


