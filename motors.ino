#include "RotateSpeedMeter.h"
#include "Motor.h"

RotateSpeedMeter mspeed(12);
Motor motor(11,10,9,6);

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}

int currentCmd=4;

int getCmd() {
  int currentOrder=-1; //wait
  if (Serial.available() > 0) {
    // read the incoming byte:
    int order = Serial.read()-48;
    if (order>-1 && order<9){
      currentOrder=order;
      Serial.print("Command: ");
      Serial.print(order,DEC);
      Serial.print("   ");
      Serial.print(currentOrder/3-1);
      Serial.print("   ");
      Serial.println(currentOrder%3-1);
    }
  }
 return currentOrder;
}


float freq=0.8;
float targetRpm=95;
const float tolerate=0.05;

void loop() {
  //get command
  int cmd=getCmd();
  if (cmd>=0)//legal command
    currentCmd=cmd;
  //drive motor by current command
  motor.driveLR(currentCmd / 3-1, currentCmd % 3 -1,freq,freq);
  ///velocity
  mspeed.measure();
 
  //show velocity every 100 millisecond 
  if (millis()%100 ==0){
    float rpm=mspeed.rpm();
    if (rpm>0.1){
      Serial.print("rpm: ");
      Serial.println(rpm); //round per minute;
      //rpm feedback control
      if (currentCmd==8 && abs(targetRpm-rpm)>targetRpm*tolerate)
      {
         float freqUpdate=freq*targetRpm/rpm;
         if (freqUpdate<1.0)
          freq=freqUpdate;
      }
    }
    mspeed.reset();
  }
}
