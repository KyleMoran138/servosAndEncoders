#include <Servo.h>
#define xinputA 12
#define xinputB 13
#define xrateInput 11
#define xservoPin 9
//
#define yinputA 5
#define yinputB 6
#define yrateInput 7 
#define yservoPin 3

int xpos = 0;
int xrateChange = 1;
int xaState;
int xaLastState;
int xlastRateState;
int xrateState;
Servo xservo;
//
int ypos = 0;
int yrateChange = 1;
int yaState;
int yaLastState;
int ylastRateState;
int yrateState;
Servo yservo;

void setup() {
  pinMode(xinputA, INPUT);
  pinMode(xinputB, INPUT);
  pinMode(xrateInput, INPUT);
  xservo.attach(xservoPin);
  pinMode(yinputA, INPUT);
  pinMode(yinputB, INPUT);
  pinMode(yrateInput, INPUT);
  yservo.attach(yservoPin);
  Serial.begin(9600);
  xaLastState = digitalRead(xinputA);
  xlastRateState = LOW;
  yaLastState = digitalRead(yinputA);
  ylastRateState = LOW;
}

void loop() {
  
  xpos = xpos + getXEncoderChange();
  xservo.write(xpos);
  ypos = ypos + getYEncoderChange();
  yservo.write(ypos);
}

int getXEncoderChange(){
  xaState = digitalRead(xinputA);
  xrateState = digitalRead(xrateInput);
  int localCounter = 0;
  
  if(xaState != xaLastState){
    if(xaState != digitalRead(xinputB)){
      if(xpos < 180 && (xpos + xrateChange) <= 180){
        localCounter+= xrateChange;  
      }
    }else{
      if(xpos > 0 && (xpos - xrateChange) >= 0){
        localCounter-= xrateChange;
      }
    }
    Serial.print("xpos: ");
    Serial.println(xpos + localCounter);
  }
  xaLastState = xaState;

  if(xlastRateState != xrateState && xrateState == HIGH){
    switch(xrateChange){
      case(1):
        xrateChange = 5;
        break;
      case(5):
        xrateChange = 10;
        break;
      default:
        xrateChange = 1;
        break;
    }
    Serial.print("x Rate changed to: ");
    Serial.println(xrateChange);
  }
  xlastRateState = xrateState;

  return localCounter;
}


int getYEncoderChange(){
  yaState = digitalRead(yinputA);
  yrateState = digitalRead(yrateInput);
  int localCounter = 0;
  
  if(yaState != yaLastState){
    if(yaState != digitalRead(yinputB)){
      if(ypos < 180 && (ypos + yrateChange) <= 180){
        localCounter+= yrateChange;  
      }
    }else{
      if(ypos > 0 && (ypos - yrateChange) >= 0){
        localCounter-= yrateChange;
      }
    }
    Serial.print("ypos: ");
    Serial.println(ypos + localCounter);
  }
  yaLastState = yaState;

  if(ylastRateState != yrateState && yrateState == HIGH){
    switch(yrateChange){
      case(1):
        yrateChange = 5;
        break;
      case(5):
        yrateChange = 10;
        break;
      default:
        yrateChange = 1;
        break;
    }
    Serial.print("y Rate changed to: ");
    Serial.println(yrateChange);
  }
  ylastRateState = yrateState;

  return localCounter;
}
