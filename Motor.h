#ifndef motor_h
#define motor_h
#include "Arduino.h"

class Motor{
	int pin[4]; 
public:
        Motor(int,int,int,int);
	void drive(int pinA,int pinB, int dir);
	void drive(int pinA, int pinB, int dir, float freq);
	void driveLR(int dir1,int dir2,float freq1,float freq2);
};

#endif
