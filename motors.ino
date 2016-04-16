#include "Tachometer.h"
#include "Motor.h"
#include "Ultrasonic.h"
#include "GY_85.h"
#include <Wire.h>
#include "utility.h"
#include "cmd.h"
bool show_rpm=0;
bool dist_fag=1;
//sensors and motors
Tachometer speedL(2); //left
Tachometer speedR(3); //right
MotorSet motorSet(9,6,11,10);//(11, 10, 9, 6);
Ultrasonic ultrasonic(22,18); // trig echo
GY_85 GY85;     //A5->scl A4->sda
// parameters
Cmd currentCmd(0,0,50,50);
float currentAngle=0;
unsigned long timerSpeed=0;
unsigned long timerGyro=0; 
unsigned long timerCheck=0; 

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
    Wire.begin();
    delay(10);
    Serial.begin(9600);
//    Serial1.begin(9600);
    Serial2.begin(9600);
//    Serial3.begin(9600);
    delay(10);
    GY85.init();
    delay(10);
    tachoStart();
//    currentCmd.findDirection(degreeAdd(compass(),-10));
}

float compass()
{
    int cx = GY85.compass_x( GY85.readFromCompass() );
    int cy = GY85.compass_y( GY85.readFromCompass() );
    int cz = GY85.compass_z( GY85.readFromCompass() );
    float yaw=atan2(cy,cx);
    if (yaw<0)
      yaw+=2*PI;
    if (yaw>2*PI)
      yaw-=2*PI;
    yaw=yaw*180/PI;
    return yaw;
}


float gyroscope()
{
    float gx = GY85.gyro_x( GY85.readGyro() );
    float gy = GY85.gyro_y( GY85.readGyro() );
    float gz = GY85.gyro_z( GY85.readGyro() );
    float gt = GY85.temp  ( GY85.readGyro() ); 
    return gz;
}

void loop() {
  //get command
  currentCmd.getCmd();
  //drive
  motorSet.driveCmd(currentCmd);
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

      //compass
      float d=compass();
      if (currentCmd.type==FIND_DIRECTION)
          currentCmd.updateDir(d);


      //gyroscope
      float g=gyroscope()*CIRCLE_GYRO/1000; //in Second/ in angle
      if (currentCmd.type==TURN_DEGREE)
          currentCmd.updateTurn(g);
      currentAngle+=g;
      if (currentAngle>180)
        currentAngle-=360;
      if (currentAngle<-180)
        currentAngle+=360;
  }
}



