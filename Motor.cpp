#include "Motor.h"
void Motor::init(int p0,int p1){
  pin[0]=p0;
  pin[1]=p1;
  pinMode(pin[0],OUTPUT);
  pinMode(pin[1],OUTPUT);
}



void Motor::drive(int dir)
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


int Motor::updateFreq(float observedRPM)
{
  if (abs(rpm - observedRPM) > rpm * tolerate) //can not tolerate, need to update
  {
    if (observedRPM>0.1){// can not divide by 0
      float freqUpdate = freq * rpm / observedRPM;
      if (freqUpdate < 1.0){
        freq = freqUpdate;
        return 0;// success
      }
    }
    freq=1.0;
    return 1;//restart motor
  }
  return -1; //not need to update
}

void Motor::driveFeedback(int dir, float observedRPM)
{
  if (abs(dir)==0){//command to stop
    drive(dir);
  } else{ // forward or backward, need to update frequency
    updateFreq(observedRPM);
    drive(dir);
  }
}

MotorSet::MotorSet(int pin0,int pin1,int pin2,int pin3)
{
  motorL.init(pin0,pin1);
  motorR.init(pin2,pin3);
}

void MotorSet::driveFeedback(int dirL,float observedRPML,int dirR,float observedRPMR)
{
  motorL.driveFeedback(dirL,observedRPML); //Left wheel
  motorR.driveFeedback(dirR,observedRPMR); //Right wheel
}



