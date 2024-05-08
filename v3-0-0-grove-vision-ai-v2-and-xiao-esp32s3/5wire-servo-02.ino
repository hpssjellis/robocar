/*

// my regular servo is having issues moving as fast aws the grove vision shield can let it so I am going to try the original 5 wire servo
//This servo is actually a DC motor that I will need to use my dual motor driver an a potentiometer that tells where the motor is. 
//This motor has a very small turning radius probably about 53 degrees to 127 degrees like the regular servo


## or the 5 wire dc turning motor with potentiometer on the original shredder and Dual motor driver

// red motor B1
// black motor B2 
// white 3V3
// yellow A0
// brown GND

*/

#include <Arduino.h>

// Define the pins
const int myPotPin = A2;  // Analog pin for the potentiometer
const int myMotorDirPin = A1;  // Digital pin for DC motor direction
const int myMotorPin = A3; // PWM pin for the motor

void setup() {
  pinMode(myMotorPin, OUTPUT);
  pinMode(myMotorDirPin, OUTPUT);
  Serial.begin(115200); // Updated baud rate
}

void loop() {
  // Example usage: Turn motor to position 512
  turnMotorToPosition(512);     // need to find out if this is near the middle
  delay(2000); // Delay to see the changes, not needed in actual use
  turnMotorToPosition(450);    
  delay(2000); 
  turnMotorToPosition(570);     
  delay(5000); 

// Function to turn the motor to the desired potentiometer position
void turnMotorToPosition(int desiredPosition) {
  int currentPosition = analogRead(myPotPin); // Read the current position from the potentiometer
  while (abs(currentPosition - desiredPosition) > 10) { // Allow a tolerance of 10 units
    if (currentPosition < desiredPosition) {
      digitalWrite(myMotorDirPin, HIGH); // Set motor direction forward
      analogWrite(myMotorPin, 128); // Adjust PWM value for speed
    } else {
      digitalWrite(myMotorDirPin, LOW); // Set motor direction reverse
      analogWrite(myMotorPin, 128); // Keep same speed, change direction
    }
    delay(100); // Short delay to allow the motor to move.   need to see how short this can be.
    currentPosition = analogRead(myPotPin); // Update the potentiometer reading
  }
  analogWrite(myMotorPin, 0); // Stop the motor when position is reached
  Serial.print("Position reached: ");
  Serial.println(currentPosition);
}
