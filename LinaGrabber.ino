#include <AccelStepper.h>
#include <Servo.h>

// Define motor pins

#define ENABLE_PIN 8

#define X_STEP_PIN 2
#define X_DIR_PIN 5

#define Y_STEP_PIN 3
#define Y_DIR_PIN 6

#define Z_STEP_PIN 4
#define Z_DIR_PIN 7

#define A_STEP_PIN 12
#define A_DIR_PIN 13

#define SERVO1_PIN 40

// Define input pins
#define X_UP 22
#define X_DOWN 25
#define Y_UP 23
#define Y_DOWN 24
#define Z_UP 26
#define Z_DOWN 27

#define SERVO1_CLOSE_PIN 28
#define SERVO1_OPEN_PIN 29

#define X_LIMIT 9
#define Y_LIMIT 10
#define Z_LIMIT 11
#define A_LIMIT 33

#define X_MAX 18100
#define Y_MAX 28500
#define Z_MAX 8500

#define DEBUG true

AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper stepperA(AccelStepper::DRIVER, A_STEP_PIN, A_DIR_PIN);

Servo servo1;
int servo1Pos = 90; // Starting position for servo1

int xPos = -1; 
int yPos = -1; 
int y2Pos = -1; 
int zPos = -1; 



void setup() {
  Serial.begin(115200);
  pinMode(ENABLE_PIN, OUTPUT);
  
  pinMode(X_UP, INPUT_PULLUP);
  pinMode(X_DOWN, INPUT_PULLUP);
  pinMode(Y_UP, INPUT_PULLUP);
  pinMode(Y_DOWN, INPUT_PULLUP);
  pinMode(Z_UP, INPUT_PULLUP);
  pinMode(Z_DOWN, INPUT_PULLUP);
  pinMode(SERVO1_OPEN_PIN, INPUT_PULLUP);
  pinMode(SERVO1_CLOSE_PIN, INPUT_PULLUP);

  pinMode(X_LIMIT, INPUT_PULLUP);
  pinMode(Y_LIMIT, INPUT_PULLUP);
  pinMode(Z_LIMIT, INPUT_PULLUP);
  pinMode(A_LIMIT, INPUT_PULLUP);

  for (int i = 1; i <= 100; i++) {
      digitalWrite(ENABLE_PIN, HIGH);
      delay(30); // Sleep for 5 seconds
      digitalWrite(ENABLE_PIN, LOW);
      delay(10); // Sleep for 5 seconds
  }

  for (int i = 1; i <= 100; i++) {
    stepperA.setSpeed(6000);
    stepperA.runSpeed();
  }
  
  stepperX.setMaxSpeed(6000);
  stepperY.setMaxSpeed(6000);
  stepperZ.setMaxSpeed(6000);
  stepperA.setMaxSpeed(6000);

  servo1.attach(SERVO1_PIN);
  servo1.write(servo1Pos); 

}

void loop() {
  controlAxis(stepperX, X_UP, X_DOWN, X_LIMIT);
  controlAxis(stepperY, Y_UP, Y_DOWN, Y_LIMIT);
  controlAxis(stepperZ, Y_UP, Y_DOWN, Z_LIMIT);
  controlAxis(stepperA, Z_UP, Z_DOWN, X_LIMIT);
  
  controlServo(servo1, SERVO1_OPEN_PIN, SERVO1_CLOSE_PIN, servo1Pos);
}

void controlAxis(AccelStepper &stepper, int upPin, int downPin, int limitPin) {
  
  bool max=false;
  if( &stepper == &stepperX){
    if(digitalRead(limitPin) == LOW)
    { 
      xPos=0;
    }

    if(xPos>=0){
      if(digitalRead(downPin) == LOW){
        if(xPos>=X_MAX){
          max=true;
          if(DEBUG) Serial.print("xMAX:");
          if(DEBUG) Serial.println(xPos);
        } else{
          xPos+=1;
          if(DEBUG) Serial.print("x:");
          if(DEBUG) Serial.println(xPos);
        }
      }
      if(digitalRead(upPin) == LOW){
        xPos-=1;
        if(DEBUG) Serial.print("x:");
        if(DEBUG) Serial.println(xPos);
      }

    }    
  } else if ( &stepper == &stepperY ) {
       if(digitalRead(limitPin) == LOW)
    { 
      yPos=0;
    }

    if(yPos>=0){
      if(digitalRead(downPin) == LOW){
        if(yPos>=Y_MAX){
          max=true;
          if(DEBUG) Serial.print("y1MAX:");
          if(DEBUG) Serial.println(yPos);
        } else{
          yPos+=1;
          if(DEBUG) Serial.print("y:");
          if(DEBUG) Serial.println(yPos);
        }
      }
      if(digitalRead(upPin) == LOW){
        yPos-=1;
        if(DEBUG) Serial.print("y:");
        if(DEBUG) Serial.println(yPos);
      }     
    } 
  } else if ( &stepper == &stepperZ) {
    if(digitalRead(limitPin) == LOW)
    { 
      y2Pos=0;
    }

    if(y2Pos>=0){
      if(digitalRead(downPin) == LOW){
        if(y2Pos>=Y_MAX){
          max=true;
          if(DEBUG) Serial.print("y2MAX:");
          if(DEBUG) Serial.println(y2Pos);
        } else{
          y2Pos+=1;
          if(DEBUG) Serial.print("y2:");
          if(DEBUG) Serial.println(y2Pos);
        }
      }
      if(digitalRead(upPin) == LOW){
        y2Pos-=1;
        if(DEBUG) Serial.print("y2:");
        if(DEBUG) Serial.println(y2Pos);
      }     
    } 
  } else if ( &stepper == &stepperA) {
    
      if(digitalRead(downPin) == LOW){
        if(zPos>=Y_MAX){
          max=true;
          if(DEBUG) Serial.print("ZMAX:");
          if(DEBUG) Serial.println(zPos);
        } else{
          zPos+=1;
          if(DEBUG) Serial.print("Z:");
          if(DEBUG) Serial.println(zPos);
        }
      }
      if(digitalRead(upPin) == LOW){
        zPos-=1;
        if(DEBUG) Serial.print("Z:");
        if(DEBUG) Serial.println(zPos);
      }     
    
  } 
  if(max==false){
    if (digitalRead(upPin) == LOW && digitalRead(limitPin) == HIGH) {
      stepper.setSpeed(6000);
      stepper.runSpeed();
    } else if (digitalRead(downPin) == LOW ) {
      stepper.setSpeed(-6000);
      stepper.runSpeed();
    } else {
      stepper.setSpeed(0);
    }
  }

}

void controlServo(Servo &servo, int upPin, int downPin, int &servoPos) {
  if (digitalRead(upPin) == LOW && servoPos < 180) {
    servoPos++; // Increment position slowly
    servo.write(servoPos);
    delay(10); // Adjust speed of movement
  } else if (digitalRead(downPin) == LOW && servoPos > 0) {
    servoPos--; // Decrement position slowly
    servo.write(servoPos);
    delay(10); // Adjust speed of movement
  }
}


