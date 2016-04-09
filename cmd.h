#ifndef CMD_H
#define CMD_H
#include "Arduino.h"
#include "utility.h"
class MotorCmd{
	unsigned long expireTime;
	bool timeoutFag;
public:
	MotorCmd(int,int,float,float);
	float freqL,freqR;
	int dirL,dirR;
	void setDir(int,int);
  bool isDir(int,int);
	void setFreq(float,float);
	void setTimeout(unsigned long);
	bool available();
};

class Cmd{
  unsigned long timeStamp;
  bool abnormalStatus(float feedbackL,float feedbackR);
  float rpmL,rpmR; //target, not observed
  bool turningInitFag;
public:
	int type;
	MotorCmd motorCmd;
  float degree;

	//init
	Cmd(int dirL,int dirR,float rpmL,float rpmR);
	int getCmd(); // return -1 if no available cmd received
  	//set the status and time stamp, change exactly this Cmd
	void keepStatus(int,int);
	void keepStatus(int code); // compressed code
	void keepStatus(int,int,float,float);
  void tempStatus(int dirL,int dirR,int timeout);
	void findDirection(float degree);
  void turnDegree(float clockwiseDegree);
	//change motorCmd, and maybe its time stamp
  	void updateFreq(float feedbackL,float feedbackR, bool obeyFag);
  	int updateDir(float feedbackDegree);
    int updateTurn(float feedbackDegree);
};
#endif
