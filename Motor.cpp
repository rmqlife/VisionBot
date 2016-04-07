#include "Motor.h"
void Motor::init(int p0,int p1){
  pin[0]=p0;
  pin[1]=p1;
  pinMode(pin[0],OUTPUT);
  pinMode(pin[1],OUTPUT);
}

void Motor::drive(int dir,float freq)
{
  if (dir>0){
    analogWrite(pin[0],freq*255);
    digitalWrite(pin[1],LOW);
   }
   else if (dir==0){
     digitalWrite(pin[0],LOW);
     digitalWrite(pin[1],LOW);
   }
   else {
     digitalWrite(pin[0],LOW);
     analogWrite(pin[1],freq*255);
   }
}


MotorSet::MotorSet(int pin0,int pin1,int pin2,int pin3)
{
  motorL.init(pin0,pin1);
  motorR.init(pin2,pin3);
}

void MotorSet::drive(int dirL,float freqL,int dirR,float freqR)
{
  motorL.drive(dirL,freqL); //Left wheel
  motorR.drive(dirR,freqR); //Right wheel
}

void MotorSet::driveCmd(Cmd cmd)
{
  if (cmd.type==KEEP_STATUS){
    drive(cmd.dirL,cmd.freqL,cmd.dirR,cmd.freqR);
  }
}
