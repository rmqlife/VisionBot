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
  if (vChangeCount>0){ //second change 
    durations[vChangeCount-1]=(micros()-startMicros)/1000.0;
  }
  startMicros=micros();    
  vChangeCount++;
}


float Tachometer::rpm()
{
  if (vChangeCount<2)
    return 0;
  
  int holesCount=0;
  float totalDuration=0;
  for (int i=0;i<vChangeCount-1;i++){
      if (debug){
        Serial.print(durations[i]);
        Serial.print("\t");
      }
      if (durations[i]>THRESH_DURATION){
        holesCount++;
        totalDuration+=durations[i];
      }
  }
  if (debug)
    Serial.println(holesCount);
  //attention: the unit is millisecond
  if (holesCount>0){
      float rpm=60000/(totalDuration*HOLE_NUMBER/holesCount);
      return rpm;
  }
  return 0;
}

