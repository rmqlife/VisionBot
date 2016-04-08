#include "cmd.h"
MotorCmd::MotorCmd(int dirL=0,int dirR=0,float freqL=0.9,float freqR=0.9)
{
  this->dirL=dirL;
  this->dirR=dirR;
  this->freqL=freqL;
  this->freqR=freqR;
  timeoutFag=false;
}

void MotorCmd::setDir(int dirL,int dirR)
{
  this->dirL=dirL;
  this->dirR=dirR;
  timeoutFag=false;
}

bool MotorCmd::isDir(int dirL,int dirR)
{
  return (this->dirL==dirL && this->dirR==dirR);  
}

void MotorCmd::setFreq(float freqL,float freqR)
{
  this->freqR=freqR;
  this->freqL=freqL;
  timeoutFag=false;
}

void MotorCmd::setTimeout(unsigned long timeout)
{
  timeoutFag=true;
  expireTime=timeout+millis();
}
bool MotorCmd::available()
{
  return ( (!timeoutFag) || millis()<expireTime);
}

Cmd::Cmd(int dirL,int dirR,float rpmL,float rpmR)
{
  keepStatus(dirL,dirR,rpmL,rpmR);
}


void Cmd::keepStatus(int dirL,int dirR)
{
	type=KEEP_STATUS;
	timeStamp=millis();
  motorCmd.setDir(dirL,dirR);
}

void Cmd::keepStatus(int code)
{
	keepStatus(code / 3 - 1, code % 3 - 1);
}

void Cmd::keepStatus(int dirL,int dirR,float rpmL,float rpmR)
{
	keepStatus(dirL,dirR);
	this->rpmL=rpmL;
	this->rpmR=rpmR;
}

int Cmd::getCmd()
{
  if (Serial.available() > 0) {
    char raw=(char)Serial.read();
    Serial.println(raw);
    switch(raw){
      case 'q': keepStatus(0,0); return 0;
      case 'f': keepStatus(1,1); return 0;
      case 'b': keepStatus(-1,-1); return 0;
      case 'l': turnDegree(+20); return 0;
      case 'r': turnDegree(-20); return 0;
      case 'n': findDirection(180); return 0;
      case 's': findDirection(0); return 0;
      case 'e': findDirection(270); return 0;
      case 'w': findDirection(90); return 0;
    }
  }
  return -1;
}


bool Cmd::abnormalStatus(float feedbackL, float feedbackR)
{
    bool abnormalL=(motorCmd.dirL!=0) ^ (feedbackL>RPM_EQUALS_STOP);
    bool abnormalR=(motorCmd.dirR!=0) ^ (feedbackR>RPM_EQUALS_STOP);
    return  abnormalL || abnormalR;
}

void Cmd::updateFreq(float feedbackL,float feedbackR,bool obeyFag){
    if (obeyFag && abs(millis()-timeStamp)>MAX_TOLERANT_STARTUP_MILLIS && abnormalStatus(feedbackL,feedbackR)){
       if (!motorCmd.isDir(0,0)) // not stop signal
        keepStatus(0,0);//sets a stop signal
       else
        keepStatus(1,1);
       return;
    }
  motorCmd.freqL=feedbackFreq(rpmL,feedbackL,motorCmd.freqL);  
  motorCmd.freqR=feedbackFreq(rpmR,feedbackR,motorCmd.freqL);
}

int Cmd::updateDir(float feedbackDegree){
  float clockwiseDelta=feedbackDegree-degree;
  if (clockwiseDelta<0)
    clockwiseDelta+=360;
  if (clockwiseDelta>180){
    clockwiseDelta=clockwiseDelta-360;  
  }
  if (abs(clockwiseDelta)<3){
    motorCmd.setDir(0,0);
    return 0;
  }
  if (clockwiseDelta>0) //clockwiseDelta is positive, clockwise turn, turn right
    motorCmd.setDir(1,-1);
  else //clockwiseDelta is negative, counterclockwise turn, which means turn left
    motorCmd.setDir(-1,1);

  float strength=abs(clockwiseDelta)/180;
  motorCmd.setTimeout(60*strength+40);
  return 1;
}

void Cmd::findDirection(float degree)
{
  timeStamp=millis();
  this->degree=degree;
  this->type=FIND_DIRECTION;  
}

void Cmd::turnDegree(float clockwiseDegree)
{
  timeStamp=millis();
  degree=clockwiseDegree;
  type=TURN_DEGREE;
  //turningInitFag 
  turningInitFag=false;
}

int Cmd::updateTurn(float feedbackDegree)
{
  if (!turningInitFag)
  {
    degree=degreeAdd(feedbackDegree,degree);  
    turningInitFag=true;
    return -2;
  }
  if ( updateDir(feedbackDegree)==0) // ready
    keepStatus(0,0);
}
