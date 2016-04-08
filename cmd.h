#ifndef CMD_H
#define CMD_H
#include "Arduino.h"
#include "utility.h"

class Cmd{
  unsigned long timeStamp;
public:
  Cmd(int dirL,int dirR,float rpmL,float rpmR);
	float rpmL,rpmR; //target, not observed
  float freqL=0.9,freqR=0.9;
	int dirL,dirR;
	int type;
	float degree;
  void findDirection();
	void keepStatus(int,int);
	void keepStatus(int code); // compressed code
	void keepStatus(int,int,float,float);
  bool abnormalStatus(float feedbackL,float feedbackR);
	void setStop();
  bool isStop();
	int getCmd(); // return -1 if no available cmd received
  void updateFreq(float feedbackL,float feedbackR, bool obeyFag);
  void updateDir(float feedbackDegree);
};
#endif
