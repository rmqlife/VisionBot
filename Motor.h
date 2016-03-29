#ifndef motor_h
#define motor_h
#include "Arduino.h"

class Motor{
	int pin[4]; //pin0=L1,pin1=L2,pin2=R1,pin3=R2
public:
        Motor(int,int,int,int);
	void drive(int pinA,int pinB, int dir);
	void drive(int pinA, int pinB, int dir, float freq);
	void driveLR(int dirL,int dirR,float freqL,float freqR);
};

#endif
