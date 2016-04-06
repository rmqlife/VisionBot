#include "RotateSpeedMeter.h"
#include "Motor.h"
#include "Ultrasonic.h"
#include "GY_85.h"
#include <Wire.h>

bool debug=1;
RotateSpeedMeter speedL(8); //left
RotateSpeedMeter speedR(7); //right
MotorSet motorSet(9,6,11,10);//(11, 10, 9, 6);
Ultrasonic ultrasonic(4,5); // trig echo
GY_85 GY85;     //A5->scl A4->sda

#define MAX_TOLERANT_STARTUP_MILLIS 500
#define RPM_EQUALS_STOP 50

void setup() {
    Wire.begin();
    delay(10);
    Serial.begin(9600);
    delay(10);
    GY85.init();
    delay(10);
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

bool abnormalStatus(int cmdDirL,int cmdDirR,float rpmL,float rpmR)
{
    return  (cmdDirL!=0 && rpmL<RPM_EQUALS_STOP ) || (cmdDirR!=0 && rpmR<RPM_EQUALS_STOP) ;
}

void testGy(){
//    int ax = GY85.accelerometer_x( GY85.readFromAccelerometer() );
//    int ay = GY85.accelerometer_y( GY85.readFromAccelerometer() );
//    int az = GY85.accelerometer_z( GY85.readFromAccelerometer() );
    
    int cx = GY85.compass_x( GY85.readFromCompass() );
    int cy = GY85.compass_y( GY85.readFromCompass() );
    int cz = GY85.compass_z( GY85.readFromCompass() );

//    float gx = GY85.gyro_x( GY85.readGyro() );
//    float gy = GY85.gyro_y( GY85.readGyro() );
//    float gz = GY85.gyro_z( GY85.readGyro() );
//    float gt = GY85.temp  ( GY85.readGyro() );
//    
//    Serial.print  ( "accelerometer" );
//    Serial.print  ( " x:" );
//    Serial.print  ( ax );
//    Serial.print  ( " y:" );
//    Serial.print  ( ay );
//    Serial.print  ( " z:" );
//    Serial.print  ( az );
//    
    Serial.print("compass" );
    float yaw=atan2(cy,cx);
    if (yaw<0)
      yaw+=2*PI;
    if (yaw>2*PI)
      yaw-=2*PI;
    yaw=yaw*180/PI;
    Serial.print(yaw);
    Serial.print("\n");
    
    
//    
//    Serial.print  ( "  gyro" );
//    Serial.print  ( " x:" );
//    Serial.print  ( gx );
//    Serial.print  ( " y:" );
//    Serial.print  ( gy );
//    Serial.print  ( " z:" );
//    Serial.print  ( gz );
//    Serial.print  ( " gyro temp:" );
//    Serial.println( gt );
}

#define CIRCLE_SPEED 100
#define CIRCLE_GYRO 100

unsigned long timerSpeed=0;
unsigned long timerGyro=0; 
unsigned long timerCmd=0;


void loop() {
  //get command
  int cmd = getCmd();
  if (cmd >= 0){ //legal command
    currentCmd = cmd;
    //drive motor by current command
    motorSet.drive(currentCmd / 3 - 1,currentCmd % 3 - 1); //skip the update procedure when start
    timerCmd=millis();
  }
  
  ///velocity
  speedR.measure();
  speedL.measure();
  //show velocity every 100 millisecond
  if (millis() / CIRCLE_SPEED != timerSpeed) {
    timerSpeed=millis()/CIRCLE_SPEED;
    float rpmR = speedR.rpm();
    float rpmL = speedL.rpm();
    
    if (abs(millis()-timerCmd)>MAX_TOLERANT_STARTUP_MILLIS && abnormalStatus(currentCmd / 3 - 1,currentCmd % 3 - 1,rpmL,rpmR))
    {
       currentCmd=4;//sets a stop signal
    }
    
    motorSet.driveFeedback(currentCmd / 3 - 1,rpmL,currentCmd % 3 - 1,rpmR);
    speedR.reset(); //reset the speedMeter's time circle
    speedL.reset();
    
//    if (rpmR > 0.1 || rpmL > 0.1) {
//      Serial.print(rpmL);
//      Serial.print("\t");
//      Serial.print(rpmR); //round per minute;
//      Serial.print("\n");
//    }
  }
  
  if (millis()/CIRCLE_GYRO != timerGyro){
      timerGyro=millis()/CIRCLE_GYRO;
      float distFront=ultrasonic.Ranging(CM);
      if (distFront<20 && false){
          currentCmd=4;
          motorSet.driveFeedback(currentCmd / 3 - 1,0,currentCmd % 3 - 1,0);
      }
//      testGy();
  }
}

