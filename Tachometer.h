#ifndef Tachometer_h 
#define Tachometer_h
#include "Arduino.h"

class Tachometer
{
	const int HOLE_NUMBER=20;
  const float THRESH_DURATION=10;
	int vChangeCount=-1;
	double startMicros=0,endMicros=0;
  int pin;
  bool debug=0;
  float durations[20];
public:
  void adder();
  Tachometer(int);
  void reset();
	float rpm();
  int getPin();
};

#endif


