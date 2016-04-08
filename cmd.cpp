#include "cmd.h"
Cmd::Cmd(int dirL,int dirR,float rpmL,float rpmR)
{
  keepStatus(dirL,dirR,rpmL,rpmR);
}

void Cmd::setStop()
{
	keepStatus(0,0);
}

bool Cmd::isStop()
{
  if (type==KEEP_STATUS && dirL==0 && dirR==0)
      return true;
  return false;
}

void Cmd::keepStatus(int dirL,int dirR)
{
	type=KEEP_STATUS;
	timeStamp=millis();
	this->dirL=dirL;
	this->dirR=dirR;
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
    bool abnormalL=(dirL!=0) ^ (feedbackL>RPM_EQUALS_STOP);
    bool abnormalR=(dirR!=0) ^ (feedbackR>RPM_EQUALS_STOP);
    return  abnormalL || abnormalR;
}

void Cmd::updateFreq(float feedbackL,float feedbackR,bool obeyFag){
    if (obeyFag && abs(millis()-timeStamp)>MAX_TOLERANT_STARTUP_MILLIS && abnormalStatus(feedbackL,feedbackR)){
       if (!isStop()) // not stop signal
        setStop();//sets a stop signal
       else
        keepStatus(1,1);
       return;
    }

  freqL=feedbackFreq(rpmL,feedbackL,freqL);  
  freqR=feedbackFreq(rpmR,feedbackR,freqR);
}

void Cmd::updateDir(float feedbackDegree){
  if (abs(degree-feedbackDegree)<5){
    keepStatus(0,0);
    return;
  }
  else if (feedbackDegree>degree)
    keepStatus(1,-1);
  else 
    keepStatus(-1,1);
}

