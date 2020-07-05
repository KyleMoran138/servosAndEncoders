#include <Servo.h>
#define inputA 12
#define inputB 13
#define servoPin 9

int counter = 0;
int aState;
int aLastState;
Servo servo;

void setup() {
  pinMode(inputA, INPUT);
  pinMode(inputB, INPUT);
  servo.attach(servoPin);
  Serial.begin(9600);
  aLastState = digitalRead(inputA);
}

void loop() {
  aState = digitalRead(inputA);

  if(aState != aLastState){
    if(aState != digitalRead(inputB)){
      if(counter < 180){
        counter++;  
      }
    }else{
      if(counter > 0){
        counter--;
      }
    }
  }
  aLastState = aState;

  servo.write(counter);

}
