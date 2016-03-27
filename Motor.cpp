#include "Motor.h"

Motor::Motor(int p0,int p1,int p2,int p3){
 pin[0]=p0;
 pin[1]=p1;
 pin[2]=p2;
 pin[3]=p3;
 for (int i=0;i<4;i++)
   pinMode(pin[i],OUTPUT);
}

void Motor::driveSimple(int pinA,int pinB, int dir)
{
  if (dir>0){
    digitalWrite(pinA,HIGH);
    digitalWrite(pinB,LOW);
   }
   else if (dir==0){
     digitalWrite(pinA,LOW);
     digitalWrite(pinB,LOW);
   }
   else {
     digitalWrite(pinA,LOW);
     digitalWrite(pinB,HIGH);
   }
}

void Motor::driveSimpleV(int pinA, int pinB, int dir, int freqUp)
{
  freq++;
  if (freq<freqUp)
      driveSimple(pinA,pinB,dir);
  else if (freq<freqTotal)
      driveSimple(pinA,pinB,0);
  else
       freq=0;
}

void Motor::driveSingle(int right,int dir)
{
  if (right>0) //right wheel
    driveSimple(pin[0],pin[1],dir);
  else //left wheel
    driveSimple(pin[2],pin[3],dir);
}

void Motor::driveBoth(int dir1,int dir2)
{
  driveSingle(-1,dir1);
  driveSingle(1,dir2);
}
