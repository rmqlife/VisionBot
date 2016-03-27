#include "RotateSpeedMeter.h"
const int wheel[4]={11,10,9,8};

RotateSpeedMeter  mspeed;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
 for (int i=0;i<4;i++){
   pinMode(wheel[i],OUTPUT);
 }
 pinMode(mspeed.pin,INPUT);
}

void driveSimple(int pinA,int pinB, int ahead)
{
  if (ahead>0){
    digitalWrite(pinA,HIGH);
    digitalWrite(pinB,LOW);
   }
   else if (ahead==0){
     digitalWrite(pinA,LOW);
     digitalWrite(pinB,LOW);
   }
   else {
     digitalWrite(pinA,LOW);
     digitalWrite(pinB,HIGH);
   }
}

int freq=0;
const int freqTotal=1000;
void driveSimpleV(int pinA, int pinB, int ahead, int freqUp)
{
  freq++;
  if (freq<freqUp)
      driveSimple(pinA,pinB,ahead);
  else if (freq<freqTotal)
      driveSimple(pinA,pinB,0);
  else
       freq=0;
}
void driveSingle(int right,int ahead)
{
  if (right>0) //right wheel
    driveSimple(wheel[0],wheel[1],ahead);
  else //left wheel
    driveSimple(wheel[2],wheel[3],ahead);
}

void driveBoth(int ahead1,int ahead2)
{
  driveSingle(-1,ahead1);
  driveSingle(1,ahead2);
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
  driveBoth(currentCmd / 3-1, currentCmd % 3 -1);
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
