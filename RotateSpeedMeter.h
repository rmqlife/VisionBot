#ifndef RotateSpeedMeter_h 
#define RotateSpeedMeter_h
#include "Arduino.h"

class RotateSpeedMeter
{
	const int HOLE_NUMBER=20;
	int vLastStatus=0;
	int vChangeCount=-1;
	double startMicros=0,endMicros=0;
        int pin;
public:
        RotateSpeedMeter(int);
	void reset();
	void measure();
	float rpm();
};

#endif
