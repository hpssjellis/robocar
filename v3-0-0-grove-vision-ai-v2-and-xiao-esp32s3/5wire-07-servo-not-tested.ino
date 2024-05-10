/*

// My regular servo is having issues moving as fast aws the grove vision shield can let it so I am going to try the original 5 wire servo
// This servo is actually a DC motor that I will need to use my dual motor driver an a potentiometer that tells where the motor is. 
// This motor has a very small turning radius probably about 53 degrees to 127 degrees like the regular servo


## for the 5 wire dc turning motor with potentiometer on the original shredder and Dual motor driver

// red motor A1      
// black motor A2 
// white 3V3
// yellow A0
// brown GND

Dual Motor Driver
https://github.com/hpssjellis/maker100-eco/blob/main/media/b-b-f-small-dual-dc-motor-driver.png

myPotPin      = A2;       // Analog pin for the potentiometer
myMotorDirPin = A4;       // Digital pin for DC motor direction
myMotorPin    = A5;       // PWM pin for the motor

Note: the potentiometer goes from 0 to 4095 for the servo which goes about 40 degrees to 150 ish
*/


#include <Arduino.h>

// Define the pins
const int myPotPin      = A2;   // Analog pin for the potentiometer
const int myMotorDirPin = A4;   // Digital pin for DC motor direction
const int myMotorPin    = A5;   // PWM pin for the motor
int currentPosition     = 0;
int myRandom            = 2000; // start near the middle 0-4095
int myTurnSpeed         = 255;  // might as well turn as fast as we can 0-255
int myTurnOk            = 20;   // range that we don't have to turn the motor
int myTurnDelay         = 7;    // time to let the motor get somewhere.

void setup() {
  pinMode(myMotorPin, OUTPUT);
  pinMode(myMotorDirPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); // Updated baud rate
  
  Serial.println("Wait...");
  delay(6000);
  Serial.println("Starting setup...");

}

void loop() {

  /// imitate the data flow from the grove-vision-ai-v2-xiao
  int myNew = random(400);
  myRandom  += myNew - 200;
  
  if ( myRandom < 0) {    myRandom = 2000;}
  if ( myRandom > 4095) { myRandom = 2000;}

  turnMotorToPosition(myRandom);
  delay(100);

}


void turnMotorToPosition(int desiredPosition) {

  currentPosition = analogRead(myPotPin);    
  Serial.println("Want: "+ String(desiredPosition) + ", Now: "+ String(currentPosition));

  if (abs(currentPosition - desiredPosition) > myTurnOk ) {

    if (currentPosition < desiredPosition) {
      digitalWrite(myMotorDirPin, HIGH); // Set motor direction forward
      analogWrite(myMotorPin, myTurnSpeed); // Adjust PWM value for speed
    } else {
      digitalWrite(myMotorDirPin, LOW); // Set motor direction reverse
      analogWrite(myMotorPin, myTurnSpeed); // Keep same speed, change direction
    }
    delay(myTurnDelay); // Short delay to allow the motor to move

  } else {
    analogWrite(myMotorPin, 0); // Stop the motor when position is reached
    // no delay needed since motor is stopped
  }
}
