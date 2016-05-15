#include "Tachometer.h"
#include "Motor.h"
#include "Ultrasonic.h"
#include <Wire.h>
#include <Servo.h>
#include "utility.h"
#include "Cmd.h"
#include "Arm.h"
bool show_rpm=0;
bool dist_fag=1;
bool avoid_camera=0;
//sensors and motors
Tachometer speedL(2); //left
Tachometer speedR(3); //right
MotorSet motorSet(9,6,11,10);//(11, 10, 9, 6);
Ultrasonic ultrasonic(22,23); // trig echo
Arm arm(47,46);
// parameters
Cmd currentCmd(0,0,50,50);
float currentAngle=0;
unsigned long timerSpeed=0;
unsigned long timerGyro=0; 
unsigned long timerCheck=0; 
unsigned long timerServo=0; 
float servoDist=0;
unsigned int servoDistCount=0;
//attachInterrupt wrappers
void tachoAdderR(){
  speedR.adder();  
}
void tachoAdderL(){
  speedL.adder();
}
void tachoStart(){
    attachInterrupt(speedR.getPin()-2,tachoAdderR,RISING);
    attachInterrupt(speedL.getPin()-2,tachoAdderL,RISING);
}
//setup
void setup() {
    arm.init(80,90);
    Wire.begin();
    delay(10);
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial3.begin(9600);
    Serial2.begin(9600);
    delay(10);
    tachoStart();
}



void loop() {
  //get command
  currentCmd.getCmd();
  //drive
  motorSet.driveCmd(currentCmd);
  arm.driveCmd(currentCmd);
  //measure tacho by tachometer and feedback control
  if (millis() / CIRCLE_SPEED != timerSpeed) {
    timerSpeed=millis()/CIRCLE_SPEED;
    float feedbackR = speedR.rpm();
    float feedbackL = speedL.rpm();
    //check if it is run in an abnormal way.
    if ( currentCmd.type==KEEP_STATUS ) { 
      currentCmd.updateFreq(feedbackL,feedbackR,true);
      motorSet.driveCmd(currentCmd);
    }
    if (show_rpm && (feedbackR > 0.1 || feedbackL > 0.1)) {
      Serial.print(feedbackL);
      Serial.print("\t");
      Serial.println(feedbackR); //round per minute;
    }
    speedR.reset(); //reset the speedMeter's time circle
    speedL.reset();
  }
  //gyroscope compass and ultrasonic 
  if (millis()/CIRCLE_GYRO != timerGyro){
      timerGyro=millis()/CIRCLE_GYRO;
      // ultrasonic
      float distFront=ultrasonic.Ranging(CM);
      if (distFront<20 && dist_fag && currentCmd.motorCmd.isDir(1,1))
          currentCmd.keepStatus(0,0);
      
      if (avoid_camera && millis()/CIRCLE_SERVO!= timerServo && servoDistCount>=1){
        servoDist=servoDist/servoDistCount;
        timerServo=millis()/CIRCLE_SERVO;
        arm.set(80+100*(1-sqrt(servoDist/51)),90-90*(1-sqrt(servoDist/51)));
        servoDist=0;
        servoDistCount=0;
      }else{
        servoDist+=distFront;  
        servoDistCount++;
      }
      
      
  }
}



