#include "RotateSpeedMeter.h"
RotateSpeedMeter::RotateSpeedMeter(int p0)
{
  pin=p0;
  pinMode(pin,INPUT);
  vChangeCount=-1;
}


void RotateSpeedMeter::reset()
{
  vChangeCount=-1;
}

void RotateSpeedMeter::measure()
{
  int vStatus=digitalRead(pin);
  if (vStatus!=vLastStatus){
    vChangeCount++;
    if (vChangeCount == 0) //tic
      startMicros=micros();
    else if (vChangeCount % 2==0 && vChangeCount>0 ) //toc
       endMicros=micros();
     vLastStatus=vStatus;
  }
}

float RotateSpeedMeter::rpm()
{
  int holesCount=vChangeCount/2;  
  if (holesCount>0){
      float rpm=60000000/((endMicros-startMicros)*HOLE_NUMBER/holesCount);
      return rpm;
  }
  return 0;
}
