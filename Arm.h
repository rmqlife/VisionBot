#ifndef ARM_H
#define ARM_H
#include "Servo.h"
#include "Arduino.h"
#include "Cmd.h"
class Arm{
//horizontal, vertical
Servo servoH,servoV;
int pinH,pinV;
public:
	Arm(int pinH,int pinV);
  void init(int valH, int valV);
	void set(int valH,int valV);
  void driveCmd(Cmd &);
};

#endif
