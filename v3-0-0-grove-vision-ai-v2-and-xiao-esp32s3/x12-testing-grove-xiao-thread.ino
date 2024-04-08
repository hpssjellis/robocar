/*
 * Connections XIAO-esp32s3 to Grove Vision AI V2 
 * GND to GND
 * 3V3 to 3V3
 * SDA (D4) to SDA grove
 * SCL (D5) to SCL Grove
 * 
 * 
 * Coonections XIAO-esp32S3 to Servo
 * D2 to Orange wire
 * 
 * Connections XIAO-esp32S3 to Big Motor Driver Pololu 1451 VNH5019
 * https://github.com/hpssjellis/maker100-eco/blob/main/media/b-b-g-big-dc-motor-driver.png
 * GND to top left 1   Digital turn
 * D0 to top left 3    PWM motor speed
 * 3V3 to top left 6   Digital turn
 * 3V3 to top left 7
 * GND to top left 8
 * 
 * 
 * 
 * 
 */


#include <Arduino.h>
#include <Seeed_Arduino_SSCMA.h>
#include <ESP32Servo.h>   // for XIAO-ESP32S3-Sense


TaskHandle_t myTaskHandle = NULL;
//int myPWMSpeed = 0; // Global variable to store PWM speed


// Global Variables
SSCMA AI;
Servo myServo_D2;
int myMainSpeed = 37;   // slowest speed that the car moves on a charged battery
int myChosenSpeed = 0;  // speed the car should be going
int myDrivePwmPin = D0;   // PWM pin for the big motor driver.



void myTask(void *parameter) {
  while (true) {
    //Serial.print("PWM Speed: ");
    //Serial.println(myChosenSpeed);
    //delay(1000); // Print the speed every second
    analogWrite(myDrivePwmPin, myChosenSpeed);
    delay(30);  // just to give the motor a bit of time to react
  }
}


void setup(){
    Serial.begin(115200);
  
    pinMode(myDrivePwmPin, OUTPUT);   // PWM 0 to 255
    
    myServo_D2.attach(D2); // D2 should do PWM on XIOA
    // note the two drive pins on the big motor driver are just connected to GND and 3V3 respectively.



    // PWM research on XIAO-ESP32S3 pins with Grove Vision AI V2 board:
    // Note on the grove board we only have PINS 3V3, GND, D4, D5 set

    // D0  PWM works no startup issue
    // D1  PWM works no startup issue
    // D2  PWM works no startup issue
    // D3  JUST don't use! When both boards connected, D3 is the reset pin.    
    // D4 (SDA) which is used for I2C communication with the Grove board
    // D5 (SCL) which is used for I2C communication with the Grove board
    // D6  TX UART pin, PWM works after a brief HIGH, shorter HIGH duration if pins set before AI.begin();
    
    // D7  RX UART pin  PWM works no startup issue
    // D8  PWM works no startup issue,  SCK SPI connection for Image data if needed from Grove Board
    // D9  PWM works no startup issue,  MISO SPI connection for Image data if needed from Grove Board 
    // D10 PWM works no startup issue,  MOSI SPI connection for Image data if needed from Grove Board

    



    // Grove Vision AI V2 and zero at least one value
    
    AI.begin();
    xTaskCreatePinnedToCore(
      myTask,         // Function to be executed
      "MyTask",       // Name of the task
      2048,           // Stack size in words
      NULL,           // Task input parameter
      1,              // Priority of the task
      &myTaskHandle,  // Task handle
      0               // Core where the task should run
  );

}

void loop(){
    if (!AI.invoke() ){
  
     AI.perf().prepocess;
     AI.perf().inference;
     AI.perf().postprocess;


     if (AI.boxes()[0].score > 85 ){
      Serial.print(String(AI.boxes()[0].score)+", ");
     // analogWrite(myDrivePwmPin, myMainSpeed);   // go medium  
      myChosenSpeed = myMainSpeed;

      // 320 x 320 width and height 
      if( AI.boxes()[0].x < 100){
            Serial.println("Right");
            myServo_D2.write(110); // turn Right
      }
      else if(AI.boxes()[0].x >= 100 && AI.boxes()[0].x <= 150 ){

            Serial.println("Center");
            myServo_D2.write(90); // turn center
      }

      else if (AI.boxes()[0].x > 150) {
            Serial.println("Left");
            myServo_D2.write(70); // turn left
      }

     }
      else {
        Serial.println(String(AI.boxes()[0].score)+", None");
      //  analogWrite(myDrivePwmPin, 0);   // No objects detected so stop
        myChosenSpeed = 0;   // stop the car!
      // I don't like the delay here so I tried to run the motor in it's own thread
       // delay(500);   //  hmmmm  does not feel like the correct way to deal with this.
      }

     AI.boxes()[0].score = 0;  // pre-zero the score for next loop

    }

}
