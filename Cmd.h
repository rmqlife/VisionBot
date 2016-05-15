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

class ArmCmd{
  unsigned long expireTime;
  bool timeoutFag;
  const byte STEP=5;
public:
  byte valH,valV;
  ArmCmd(byte,byte);
  void setVal(byte,byte);
  inline void up(){if (valV-STEP>0) valV-=STEP; else valV=0;}
  inline void down(){if (valV+STEP<180) valV+=STEP; else valV=180;}
  inline void right(){if (valH-STEP>0) valH-=STEP; else valH=0;}
  inline void left(){if (valH+STEP<180) valH+=STEP; else valH=180;}
};


class Cmd{
  unsigned long timeStamp;
  bool abnormalStatus(float feedbackL,float feedbackR);
  float rpmL,rpmR; //target, not observed
  float degree;

public:
	int type;
	MotorCmd motorCmd;
  ArmCmd armCmd;
	//init
	Cmd(int dirL,int dirR,float rpmL,float rpmR);
  int parseCmd(char);
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
