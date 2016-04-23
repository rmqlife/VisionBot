#include "Arm.h"
Arm::Arm(int pinH, int pinV)
{
  this->pinH=pinH;
  this->pinV=pinV;  
}

void Arm::init(int valH, int valV)
{
	servoH.attach(pinH);
	servoV.attach(pinV);
  set(valH,valV);
}

void Arm::set(int valH,int valV)
{
	servoH.write(valH);
	servoV.write(valV);
}

void Arm::driveCmd(Cmd & cmd)
{
  ArmCmd armCmd=cmd.armCmd;
  set(armCmd.valH,armCmd.valV);  
}
