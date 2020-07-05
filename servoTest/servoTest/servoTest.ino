#include <Servo.h>

Servo myservo;
int pos = 0;

void setup() {
  myservo.attach(9);

}

void loop() {
  delay(5000);
  myservo.write(180);
  delay(6000);
  myservo.write(90);
  delay(6000);
  myservo.write(0);
  delay(1000);
}
