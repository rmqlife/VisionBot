#include "Cmd.h"
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

ArmCmd::ArmCmd(byte valH=90, byte valV=90)
{
  setVal(valH,valV);  
}

void ArmCmd::setVal(byte valH,byte valV)
{
  this->valH=valH;
  this->valV=valV;
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


int Cmd::parseCmd(char s[], int len)
{
  if (len==0)
    return -1;
  char act = s[0];
  switch(act){
      case 'q': keepStatus(0,0); return 0;
      case 'f': keepStatus(1,1); return 0;
      case 'b': keepStatus(-1,-1); return 0;
      case '[': keepStatus(1,0); return 0;
      case ']': keepStatus(0,1); return 0;
      case '(': keepStatus(-1,0); return 0;
      case ')': keepStatus(0,-1); return 0;
      case '{': keepStatus(1,-1); return 0;
      case '}': keepStatus(-1,1); return 0;
      case 'l': tempStatus(-1,1,100); return 0;
      case 'r': tempStatus(1,-1,100); return 0;
      case 'n': findDirection(180); return 0;
      
      case 'U': armCmd.up(); return 0;
      case 'D': armCmd.down(); return 0;
      case 'R': armCmd.right(); return 0;
      case 'L': armCmd.left(); return 0;
      
      //set arm values
      case 'A':
         if (len != 3)
            return -1;
         armCmd.setVal(int(s[1]), int(s[2]));                 
         return 0;
     //set turn degree
     case 'T':
        if (len != 3)
          return -1;
        if (s[1] == 'r')
          turnDegree(s[2]);
        else
          turnDegree(-s[2]);
        return 0;
     //set find direction
     case 'F':
        if (len != 3)
          return -1;
        if (s[1] == 'r')
          findDirection(s[2]);
        else
          findDirection(-s[2]);
        return 0;
     //keep status
     case 'K':
       if (len != 3)
          return -1;
       keepStatus(s[1]-1,s[2]-1);
       return 0;
     //temp status
     case 'E':
       if (len != 4)
         return -1;
       tempStatus(s[1]-1,s[2]-1,10*s[3]);
       return 0;
  }
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
  motorCmd.freqR=feedbackFreq(rpmR,feedbackR,motorCmd.freqR);
}

int Cmd::updateDir(float feedbackDegree){
  float clockwiseDelta=degree-feedbackDegree;
  // make the delta in the range [-180,+180]
  if (clockwiseDelta<-180)
    clockwiseDelta+=360;
  if (clockwiseDelta>180){
    clockwiseDelta=clockwiseDelta-360;  
  }
  if (abs(clockwiseDelta)<DEGREE_TOLERATE){
    motorCmd.setDir(0,0);
    return 0;
  }
  if (clockwiseDelta>0) //clockwiseDelta is positive, clockwise turn, turn right
    motorCmd.setDir(1,-1);
  else //clockwiseDelta is negative, counterclockwise turn, which means turn left
    motorCmd.setDir(-1,1);

  float strength=sqrt(abs(clockwiseDelta)/180)*0.6+0.4;
  if (strength>1)
    strength=1;
  motorCmd.setFreq(strength,strength);
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
}

int Cmd::updateTurn(float feedbackDegree)
{
  float clockwiseDelta=degree-feedbackDegree;
  
  //no need to turn
  if (abs(clockwiseDelta)<DEGREE_TOLERATE){
      keepStatus(0,0);  //change to stay 
      return 0;
  }
  float strength=0.6;
  if (strength>1)
    strength=1;
  
  if (clockwiseDelta>0){ //clockwiseDelta is positive, clockwise turn, turn right
    motorCmd.setDir(1,-1);
  }
  else{ //clockwiseDelta is negative, counterclockwise turn, which means turn left
    motorCmd.setDir(-1,1);
  }
  
  motorCmd.setFreq(strength,strength);
  //change the clockwiseDelta
  degree=clockwiseDelta;
  return 1;
}

void Cmd::tempStatus(int dirL,int dirR, int timeout)
{
  timeStamp=millis();
  type=TEMP_STATUS;
  motorCmd.setDir(dirL,dirR);  
  motorCmd.setFreq(1,1);
  motorCmd.setTimeout(timeout);
}
