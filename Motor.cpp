#include "Motor.h"

Motor::Motor(int p0,int p1,int p2,int p3){
 pin[0]=p0;
 pin[1]=p1;
 pin[2]=p2;
 pin[3]=p3;
 for (int i=0;i<4;i++)
   pinMode(pin[i],OUTPUT);
}

void Motor::drive(int pinA,int pinB, int dir)
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

void Motor::drive(int pinA, int pinB, int dir, float freq)
{
  if (dir>0){
    analogWrite(pinA,freq*255);
    digitalWrite(pinB,LOW);
   }
   else if (dir==0){
     digitalWrite(pinA,LOW);
     digitalWrite(pinB,LOW);
   }
   else {
     digitalWrite(pinA,LOW);
     analogWrite(pinB,freq*255);
   }
}

void Motor::driveLR(int dirL,int dirR,float freqL,float freqR)
{
  drive(pin[0],pin[1],dirL,freqL); //Left wheel
  drive(pin[2],pin[3],dirR,freqR); //Right wheel
}
