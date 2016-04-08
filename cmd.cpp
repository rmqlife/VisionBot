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
    // read the incoming byte:
    int code = Serial.read() - 48;
    if (code > -1 && code < 9) {
		  keepStatus(code);
	    return 0;      
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

void Cmd::updateDir(float feedbackDegree){
  if (abs(degree-feedbackDegree)<5){
    motorCmd.setDir(0,0);
    return;
  }
  else if (feedbackDegree>degree)
    motorCmd.setDir(1,-1);
  else 
    motorCmd.setDir(-1,1);
  motorCmd.setTimeout(50);
}

void Cmd::findDirection(float degree)
{
  timeStamp=millis();
  this->degree=degree;
  this->type=FIND_DIRECTION;  
}
