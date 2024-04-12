#include <Arduino.h>
#include <Seeed_Arduino_SSCMA.h>
#include <ESP32Servo.h>   // for XIAO-ESP32S3-Sense

#define DEBUG   // comment out if you want the code to run a little faster

TaskHandle_t myTaskHandle = NULL;
volatile bool mainLoopActive = false;  // Heartbeat signal from the main loop

// Global Variables
SSCMA AI;
Servo myServo_D2;
int myLowestSpeed = 58;
int myMaxSpeed = 75;
volatile int myChosenSpeed = 0;  // Made volatile since it's accessed from multiple tasks
int myDrivePwmPin = D0;
int myMultipleObjects = 0;
int myObjectMax = 5;
int mySpeedMultiplier =  3;
int myMotorDelay  =   40;
bool groveConnected = false;  // Flag to indicate if Grove module is connected

void myTask(void *parameter) {
  while (true) {
    if (!mainLoopActive || !groveConnected) {
      // Stop the motor if the main loop is inactive or the Grove module is disconnected
      analogWrite(myDrivePwmPin, 0);
      delay(500);  // Check again after a delay
      continue;
    }

    // Motor speed control logic
    if (myChosenSpeed > 255) { myChosenSpeed = 255; }
    if (myChosenSpeed < 0) { myChosenSpeed = 0; }
    if (myMultipleObjects > myObjectMax) { myMultipleObjects = myObjectMax; }
    if (myMultipleObjects > 1) { myChosenSpeed = myChosenSpeed + (myMultipleObjects * mySpeedMultiplier); }

    myMultipleObjects = 0;  // Reset object count

    if (myChosenSpeed > myMaxSpeed) { myChosenSpeed = myMaxSpeed; }
    analogWrite(myDrivePwmPin, myChosenSpeed);

#ifdef DEBUG    
    Serial.print(" Speed:");
    Serial.print(myChosenSpeed);
#endif    

    delay(myMotorDelay);  // Motor response delay
  }
}

void setup() {
    Serial.begin(115200);
    pinMode(myDrivePwmPin, OUTPUT);
    myServo_D2.attach(D2);

    groveConnected = AI.begin();  // Initialize Grove module and check connection
    if (!groveConnected) {
      Serial.println("Failed to connect to Grove Vision AI V2 module.");
      while (true) { delay(1000); }  // Halt if connection fails
    }

    xTaskCreatePinnedToCore(
      myTask,
      "MyTask",
      2048,
      NULL,
      1,
      &myTaskHandle,
      0
    );
}

void loop() {
    mainLoopActive = true;  // Update heartbeat signal

    if (!AI.invoke()) {
      groveConnected = false;  // Set flag if Grove module is not responding
      delay(500);  // Wait before retrying
      groveConnected = AI.begin();  // Attempt to reconnect
      if (!groveConnected) {
        Serial.println("Grove module connection lost.");
        continue;
      }
    }

    groveConnected = true;  // Set flag if Grove module is responding
    // Object detection and motor speed logic...

    mainLoopActive = false;  // Reset heartbeat signal
}
