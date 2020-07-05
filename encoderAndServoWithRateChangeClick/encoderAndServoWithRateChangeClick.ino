#include <Servo.h>
#define inputA 12
#define inputB 13
#define rateInput 11
#define servoPin 9

int counter = 0;
int rateChange = 1;
int aState;
int aLastState;
int lastRateState;
int rateState;
Servo servo;

void setup() {
  pinMode(inputA, INPUT);
  pinMode(inputB, INPUT);
  pinMode(rateInput, INPUT);
  servo.attach(servoPin);
  Serial.begin(9600);
  aLastState = digitalRead(inputA);
  lastRateState = LOW;
}

void loop() {
  
  counter = counter + getEncoderChange();
  servo.write(counter);
}

int getEncoderChange(){
  aState = digitalRead(inputA);
  rateState = digitalRead(rateInput);
  int localCounter = 0;
  
  if(aState != aLastState){
    if(aState != digitalRead(inputB)){
      if(counter < 180 && (counter + rateChange) <= 180){
        localCounter+= rateChange;  
      }
    }else{
      if(counter > 0 && (counter - rateChange) >= 0){
        localCounter-= rateChange;
      }
    }
    Serial.print("pos: ");
    Serial.println(counter + localCounter);
  }
  aLastState = aState;

  if(lastRateState != rateState && rateState == HIGH){
    switch(rateChange){
      case(1):
        rateChange = 10;
        break;
      case(10):
        rateChange = 100;
        break;
      default:
        rateChange = 1;
        break;
    }
    Serial.print("Rate changed to: ");
    Serial.println(rateChange);
  }
  lastRateState = rateState;

  return localCounter;
}
