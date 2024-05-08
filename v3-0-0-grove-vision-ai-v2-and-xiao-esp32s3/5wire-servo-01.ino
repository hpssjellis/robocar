#include <Arduino.h>

// Define the pins
const int myPotPin = A2;  // Analog pin for the potentiometer
const int myMotorPin = A3; // PWM pin for the motor

void setup() {
  pinMode(myMotorPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // Example usage: Turn motor to position 512
  turnMotorToPosition(400);
  delay(2000); // Delay to see the changes, not needed in actual use
  turnMotorToPosition(512);
  delay(2000); // Delay to see the changes, not needed in actual use
  turnMotorToPosition(600);
  delay(5000); // Delay to see the changes, not needed in actual use
}

// Function to turn the motor to the desired potentiometer position
void turnMotorToPosition(int desiredPosition) {
  int currentPosition = analogRead(myPotPin); // Read the current position from the potentiometer
  while (abs(currentPosition - desiredPosition) > 10) { // Allow a tolerance of 10 units
    if (currentPosition < desiredPosition) {
      analogWrite(myMotorPin, 128); // Adjust PWM value for speed/direction
    } else {
      analogWrite(myMotorPin, -128); // Reverse direction if needed
    }
    delay(100); // Short delay to allow the motor to move
    currentPosition = analogRead(myPotPin); // Update the potentiometer reading
  }
  analogWrite(myMotorPin, 0); // Stop the motor when position is reached
  Serial.print("Position reached: ");
  Serial.println(currentPosition);
}
