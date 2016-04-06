#ifndef motor_h
#define motor_h
#include "Arduino.h"
class Motor{
	int pin[2];
	float freq=0.9;
	const float tolerate = 0.05;
	int updateFreq(float observedRPM);
public:
  float rpm=100;
	void init(int,int);//initialize with 2 pins
  void drive(int dir);//PWM version at freq
	void driveFeedback(int dir,float observedRPM);
};

class MotorSet{
public:
  Motor motorL,motorR;
  MotorSet(int,int,int,int); //initialize with 4 pins
	void driveFeedback(int dirL,float observedRPML,int dirR,float observedRPMR);
  void drive(int dirL,int dirR);
};

#endif

