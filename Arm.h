#ifndef ARM_H
#define ARM_H
#include "servo.h"
#include "Arduino.h"
class Arm{
//horizontal, vertical
Servo servoH,servoV;
public:
	Arm(int pinH,int pinV);
	void set(int valH,int valV);
};

#endif
