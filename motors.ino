const int wheel[4]={11,10,9,8};
const int velocityPin=12;
const long MAXINT=32760;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
 for (int i=0;i<4;i++){
   pinMode(wheel[i],OUTPUT);
 }
 pinMode(velocityPin,INPUT);
}

void driveSimple(int pinA,int pinB, int ahead)
{
  if (ahead>0)
  {
    digitalWrite(pinA,HIGH);
    digitalWrite(pinB,LOW);
   }
   else if (ahead==0)
   {
     digitalWrite(pinA,LOW);
     digitalWrite(pinB,LOW);
   }
   else {
     digitalWrite(pinA,LOW);
     digitalWrite(pinB,HIGH);
   }
}



int freq=0;
const int freqUp=1000;
const int freqTotal=1000;
void driveSingle(int right,int ahead)
{
  if (right>0) //right wheel
    driveSimple(wheel[0],wheel[1],ahead);
  else //left wheel
  {
    freq++;
    if (freq<freqUp)
      driveSimple(wheel[2],wheel[3],ahead);
    else if (freq<freqTotal)
      driveSimple(wheel[2],wheel[3],0);
    else
       freq=0;
      
  }
}



void driveBoth(int ahead1,int ahead2)
{
  
  driveSingle(-1,ahead1);
  driveSingle(1,ahead2);
}

int currentOrder = 4;


int vLastStatus=0;
int vInteval=0;
int vIntevalCount=0;
int vIntevalSeq[100];
int moveStatus=1;

unsigned long time,lastTime;


void loop() {
  // send data only when you receive data:
   if (Serial.available() > 0) {
                // read the incoming byte:
                int order = Serial.read()-48;
                // say what you got:
                Serial.print("I received: ");
                Serial.print(order,DEC);
                Serial.print("   ");

                if (order>-1 && order<9)
                   currentOrder=order;
                Serial.print(currentOrder/3-1);
                Serial.print("   ");
                Serial.print(currentOrder%3-1);

                Serial.println();

  }
  
  driveBoth(currentOrder / 3-1, currentOrder % 3 -1);

  
  ///count velocity
  int vStatus=digitalRead(velocityPin);
  if (vStatus!=vLastStatus){
    vIntevalSeq[vIntevalCount]=vInteval;
    vIntevalCount++;
    vInteval=0;
    vLastStatus=vStatus;
  }else if (vInteval<MAXINT)
    vInteval++;
  
  
  ///time test
  if (millis()-lastTime==100){
    lastTime=millis();
    long intSum=0;
    vIntevalCount=vIntevalCount-vIntevalCount%2;
    if (vIntevalCount>0){
      for (int i=0;i<vIntevalCount;i++)
        intSum=intSum+vIntevalSeq[i];
      
      int vAvg=intSum/vIntevalCount;
      Serial.print(vIntevalCount);
      Serial.print(' ');
      Serial.println(vAvg);
      moveStatus=1;
      
    }else if (moveStatus!=0){ //now is change to stop
        Serial.println("stop");
        moveStatus=0;  
    } 
    vIntevalCount=0;
  }
}
