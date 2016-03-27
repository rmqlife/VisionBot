#ifndef motor_h
#define motor_h
#include "Arduino.h"

class Motor{
	int pin[4];
	int freq=0;
	const int freqTotal=1000;
public:
    Motor(int,int,int,int);
	void driveSimple(int pinA,int pinB, int dir);
	void driveSimpleV(int pinA, int pinB, int dir, int freqUp);
 	void driveSingle(int right,int dir);
	void driveBoth(int dir1,int dir2);
};

#endif
