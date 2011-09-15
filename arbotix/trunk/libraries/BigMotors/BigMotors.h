/*
  BigMotors.h - Big Motor Library for ArbotiX using Timer2
  Copyright (c) 2010 Michael E. Ferguson.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Motors_h
#define Motors_h

#if defined(__AVR_ATmega1280__)
  // ArbotiX+
  #define M1_A           28     // PA6 (D28)  -- LEFT SIDE
  #define M1_B           29     // PA7 (D29)
  #define M1_PWM         9      // PH6/OC2B (D9)

  #define M2_A           40     // PG1 (D40)  -- RIGHT SIDE
  #define M2_B           37     // PC0 (D37)
  #define M2_PWM         10     // PB4/OC2A (D10)

  #define M_EN           41     // PG0 (D41)

#elif defined(__AVR_ATmega644P__)
  // ArbotiX/ArbotiX2
  #define M1_A           18     // PC2 (D18)  -- LEFT SIDE
  #define M1_B           2      // PB2 (D2)
  #define M1_PWM         14     // PD6/OC2B (D14)

  #define M2_A           19     // PC3 (D19)  -- RIGHT SIDE
  #define M2_B           1      // PB1 (D1)
  #define M2_PWM         15     // PD7/OC2A (D15)

  #define M_EN           3      // PB3 (D3)
#endif

/* Motors Class */
class BigMotors
{
  public:
	BigMotors();
    // Standard functions found in Motors libraries
    void left(int pwm);
    void right(int pwm);
	void set(int lpwm, int rpwm);
    int getLeft();
    int getRight(); 
    // Only available in BigMotors
    void brakeLeft(int pwm);
    void brakeRight(int pwm);

  private:
    int m1_pwm;
    int m2_pwm;
};

#endif
