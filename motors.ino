#include "RotateSpeedMeter.h"
#include "Motor.h"

RotateSpeedMeter speedR(2); //right
RotateSpeedMeter speedL(3); //left
Motor motor(11, 10, 9, 6);

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
      Serial.print("Command: ");
      Serial.print(order, DEC);
      Serial.print("   ");
      Serial.print(currentOrder / 3 - 1);
      Serial.print("   ");
      Serial.println(currentOrder % 3 - 1);
    }
  }
  return currentOrder;
}


float freqL = 0.7, freqR = 0.7;
float targetRpm = 130;
const float tolerate = 0.05;
float updateFreq(float freq, float targetRpm, float rpm)
{
  if (abs(targetRpm - rpm) > targetRpm * tolerate && rpm>0.1)
  {
    float freqUpdate = freq * targetRpm / rpm;
    if (freqUpdate < 1.0){
      freq = freqUpdate;
      return freqUpdate;
    }
  }
  return freq;
}




void loop() {
  //get command
  int cmd = getCmd();
  if (cmd >= 0) //legal command
    currentCmd = cmd;
  //drive motor by current command
  motor.driveLR(currentCmd / 3 - 1, currentCmd % 3 - 1, freqL, freqR);
  ///velocity
  speedR.measure();
  speedL.measure();

  //show velocity every 100 millisecond
  if (millis() % 100 == 0) {
    float rpmR = speedR.rpm();
    float rpmL = speedL.rpm();
    if (rpmR > 0.1 || rpmL > 0.1) {
      Serial.print(rpmL);
      Serial.print("\t");
      Serial.print(rpmR); //round per minute;
      //rpm feedback control
      if (currentCmd == 8 && true)
      {
        freqL = updateFreq(freqL, targetRpm, rpmL);
        freqR = updateFreq(freqR, targetRpm, rpmR);
        Serial.print("\t");
        Serial.print(freqL);
        Serial.print("\t");
        Serial.print(freqR);
      }
      Serial.println();
    }
    speedR.reset();
    speedL.reset();
  }
}
