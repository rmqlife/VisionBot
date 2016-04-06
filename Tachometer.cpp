#include "Tachometer.h"
Tachometer::Tachometer(int p0)
{
  pin=p0;
  vChangeCount=0;
}

int Tachometer::getPin(){
  return pin;
}


void Tachometer::reset(){
	vChangeCount=0;
}


void Tachometer::adder()
{
  vChangeCount++;
  if (vChangeCount==1){ //first change
  	startMicros=micros();
  }
  else
  	endMicros=micros();
}


float Tachometer::rpm()
{
  int holesCount=vChangeCount-1;  
  if (holesCount>0){
      float rpm=60000000/((endMicros-startMicros)*HOLE_NUMBER/holesCount);
      return rpm;
  }
  return 0;
}

