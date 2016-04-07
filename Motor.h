#ifndef motor_h
#define motor_h
#include "Arduino.h"
#include "Cmd.h"
class Motor{
	int pin[2];
public:
	void init(int,int);//initialize with 2 pins
  void drive(int dir,float freq);//PWM version at freq
};

class MotorSet{
public:
  Motor motorL,motorR;
  MotorSet(int,int,int,int); //initialize with 4 pins
	void drive(int dirL,float freqL,int dirR,float freqR);
  void driveCmd(Cmd);
};

#endif

