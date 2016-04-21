#include "Arm.h"
Arm::Arm(int pinH, int pinV)
{
	servoH.attach(pinH);
	servoV.attach(pinV);
}

void Arm::set(int valH,int valV)
{
	servoH.write(valH);
	servoV.write(valV);
}