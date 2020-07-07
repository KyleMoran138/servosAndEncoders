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

#define recordPin 2
#define playPin 4
#define recordLed 10
#define playLed 8
#define maxFrames 200

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

int xRecording[maxFrames];
int yRecording[maxFrames];
int timeRecording[maxFrames];
long timeInbetween = 0;
int currentFrame = 0;
int totalFrames = 0;
bool isRecording = false;
bool isPlaying = false;

void setup() {
  pinMode(xinputA, INPUT);
  pinMode(xinputB, INPUT);
  pinMode(xrateInput, INPUT);
  xservo.attach(xservoPin);
  pinMode(yinputA, INPUT);
  pinMode(yinputB, INPUT);
  pinMode(yrateInput, INPUT);

  pinMode(recordPin, INPUT);
  pinMode(recordLed, OUTPUT);
  pinMode(playPin, INPUT);
  pinMode(playLed, OUTPUT);
  
  digitalWrite(playLed, LOW);
  digitalWrite(recordLed, LOW);
  
  yservo.attach(yservoPin);
  Serial.begin(9600);
  xaLastState = digitalRead(xinputA);
  xlastRateState = LOW;
  yaLastState = digitalRead(yinputA);
  ylastRateState = LOW;
}

void loop() {
  int xChange = getXEncoderChange();
  int yChange = getYEncoderChange();
  bool change = (xChange != 0 || yChange != 0);

  if(digitalRead(recordPin) == HIGH){
    isPlaying = false;
    if(isRecording){
      isRecording = false;
      currentFrame = 0;
    }else{
      isRecording = true;
      totalFrames = 0;
    }
  }

  if(digitalRead(playPin) == HIGH){
    isRecording = false;
    if(isPlaying){
      digitalWrite(playLed, false);
      isPlaying = false;
    }else{
      digitalWrite(playLed, true);
      isPlaying = true;
      currentFrame = 0;
      xpos = xRecording[1];
      ypos = yRecording[1];
      xservo.write(xpos);
      yservo.write(ypos);
      delay(3000);
    }
  }

  digitalWrite(recordLed, isRecording);
  digitalWrite(playLed, isPlaying);
  
  //If currently recording and we reach the end of the buffer, stop.
  if(currentFrame+1 >= maxFrames && isRecording){
    isRecording = false;
    totalFrames = currentFrame;
    currentFrame = 0;
  }else if(currentFrame+1 >= maxFrames && isPlaying){
     currentFrame = 0;
  }else if(change && isRecording == true){
    currentFrame++;
  }

  if(!isPlaying){
    xpos += xChange;
    ypos += yChange;
    xservo.write(xpos);
    yservo.write(ypos);
  }else{
    xpos += xRecording[currentFrame];
    ypos += yRecording[currentFrame];
    xservo.write(xpos);
    yservo.write(ypos);
    delay(timeRecording[currentFrame]/10);
    currentFrame++;
  }

  if(isRecording && change){
    xRecording[currentFrame] = xChange;
    yRecording[currentFrame] = yChange;
    timeRecording[currentFrame] = timeInbetween;
    if(currentFrame == 1){
      timeRecording[currentFrame] = 0;
    }
    totalFrames++;
  }

  if(change){
    Serial.print("time: ");
    Serial.println(timeInbetween);
    timeInbetween = 0;
  }else{
    timeInbetween++;
  }
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
