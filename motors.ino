#include "RotateSpeedMeter.h"
#include "Motor.h"
bool debug=1;

RotateSpeedMeter speedR(2); //right
RotateSpeedMeter speedL(3); //left
MotorSet motorSet(9,6,11,10);//(11, 10, 9, 6);

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}

int currentCmd = 4;

int getCmd() {
  int currentOrder = -1; //wait
  if (Serial.available() > 0) {
    // read the incoming byte:
    int order = Serial.read() - 48;
    if (order > -1 && order < 9) {
      currentOrder = order;
      if (debug){
        Serial.print("Command: ");
        Serial.print(order, DEC);
        Serial.print("   ");
        Serial.print(currentOrder / 3 - 1);
        Serial.print("   ");
        Serial.println(currentOrder % 3 - 1);
      }
    }
  }
  return currentOrder;
}




void loop() {
  //get command
  int cmd = getCmd();
  if (cmd >= 0){ //legal command
    currentCmd = cmd;
    //drive motor by current command
    motorSet.driveFeedback(currentCmd / 3 - 1,0,currentCmd % 3 - 1,0);
  }
  ///velocity
  speedR.measure();
  speedL.measure();
  //show velocity every 100 millisecond
  if (millis() % 100 == 0) {
    float rpmR = speedR.rpm();
    float rpmL = speedL.rpm();
    motorSet.driveFeedback(currentCmd / 3 - 1,rpmL,currentCmd % 3 - 1,rpmR);
    speedR.reset();
    speedL.reset();
    if (debug && (rpmR > 0.1 || rpmL > 0.1)) {
      Serial.print(rpmL);
      Serial.print("\t");
      Serial.print(rpmR); //round per minute;
      Serial.println();
    }
  }
}
