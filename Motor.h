#ifndef motor_h
#define motor_h
#include "Arduino.h"
class Motor{
	int pin[2];
	float freq=0.9;
	float rpm=100;
	static const float tolerate = 0.05;
	void drive(int dir);//PWM version at freq
	int updateFreq(float observedRPM);
public:
	void init(int,int);//initialize with 2 pins
	void driveFeedback(int dir,float observedRPM);
};

class MotorSet{
	Motor motorL,motorR;
public:
    MotorSet(int,int,int,int); //initialize with 4 pins
	void driveFeedback(int dirL,float observedRPML,int dirR,float observedRPMR);
};

#endif

