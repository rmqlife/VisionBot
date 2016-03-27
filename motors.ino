#include "RotateSpeedMeter.h"
#include "Motor.h"

RotateSpeedMeter mspeed(12);
Motor motor(11,10,9,8);

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

void loop() {
  //get command
  int cmd=getCmd();
  if (cmd>0)//legal command
    currentCmd=cmd;
  //drive motor by current command
  motor.driveBoth(currentCmd / 3-1, currentCmd % 3 -1);
  ///velocity
  mspeed.measure();
 
  //show velocity every 100 millisecond 
  if (millis()%100 ==0){
    float rpm=mspeed.rpm();
    if (rpm>0.1){
      Serial.print("rpm: ");
      Serial.println(rpm); //round per minute;
    }
    mspeed.reset();
  }
}
