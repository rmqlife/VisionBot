#ifndef RotateSpeedMeter_h 
#define RotateSpeedMeter_h
#include "Arduino.h"

class RotateSpeedMeter
{
	const int HOLE_NUMBER=20;
	int vLastStatus=0;
	int vChangeCount=-1;
	double startMicros=0,endMicros=0;
public:
	const int pin=12;
	void reset();
	void measure();
	float rpm();
};

#endif
