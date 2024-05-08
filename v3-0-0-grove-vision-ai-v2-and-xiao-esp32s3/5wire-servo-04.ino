#include <Arduino.h>

// Define the pins
const int myPotPin = A2;  // Analog pin for the potentiometer
const int myMotorDirPin = A4;  // Digital pin for DC motor direction
const int myMotorPin = A5; // PWM pin for the motor
int currentPosition = 0;

void setup() {
  pinMode(myMotorPin, OUTPUT);
  pinMode(myMotorDirPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); // Updated baud rate
  delay(2000);
  Serial.println("Starting setup...");

  // Run initial direction test
  directionTest();
  delay(3000); // Wait for 3 seconds after the direction test

  // Run incremental position test
  incrementalPositionTest();
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Starting loop...");

  // Cycle through different positions
  turnMotorToPosition(2000);
  delay(2000);
  turnMotorToPosition(1800);
  delay(2000);
  turnMotorToPosition(2200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);

  currentPosition = analogRead(myPotPin);
  Serial.print("Current Position: ");
  Serial.println(currentPosition);
  
  Serial.println("Completed loop. Restarting in 5 seconds...");
  delay(5000); // Pause before restarting the loop
}

// Function to turn the motor to the desired potentiometer position
void turnMotorToPosition(int desiredPosition) {
  int safetyCounter = 0;
  while (abs(currentPosition - desiredPosition) > 10 && safetyCounter < 100) {
    if (currentPosition < desiredPosition) {
      digitalWrite(myMotorDirPin, HIGH); // Set motor direction forward
      analogWrite(myMotorPin, 70); // Adjust PWM value for speed
    } else {
      digitalWrite(myMotorDirPin, LOW); // Set motor direction reverse
      analogWrite(myMotorPin, 70); // Keep same speed, change direction
    }
    delay(100); // Short delay to allow the motor to move
    currentPosition = analogRead(myPotPin); // Update the potentiometer reading
    safetyCounter++;
  }
  analogWrite(myMotorPin, 0); // Stop the motor when position is reached or safety counter reached
  if (safetyCounter >= 100) {
    Serial.println("Safety stop activated due to no position match.");
  } else {
    Serial.print("Position reached: ");
    Serial.println(currentPosition);
  }
}

void directionTest() {
  Serial.println("Testing motor direction. Moving forward.");
  digitalWrite(myMotorDirPin, HIGH); // Assuming HIGH is forward
  analogWrite(myMotorPin, 70);
  delay(2000); // Move forward for 2 seconds

  Serial.println("Reversing direction. Moving backward.");
  digitalWrite(myMotorDirPin, LOW); // Assuming LOW is backward
  analogWrite(myMotorPin, 70);
  delay(2000); // Move backward for 2 seconds

  analogWrite(myMotorPin, 0); // Stop the motor
  Serial.println("Direction test completed.");
}

void incrementalPositionTest() {
  int testPositions[] = {1000, 1500, 2000, 2500, 3000, 3500}; // Example positions across the range
  for (int i = 0; i < sizeof(testPositions) / sizeof(testPositions[0]); i++) {
    Serial.print("Testing position: ");
    Serial.println(testPositions[i]);
    turnMotorToPosition(testPositions[i]);
    delay(3000); // Allow some time to observe before moving to the next position
  }
}
