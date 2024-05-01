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
 * Do not use D3 and D6 on XIAO as they are part of the Grove communication
 * 
 */


#include <Arduino.h>
#include <Seeed_Arduino_SSCMA.h>
#include <ESP32Servo.h>   // for XIAO-ESP32S3-Sense

TaskHandle_t myMotorTaskHandle = NULL;
TaskHandle_t myTurnTaskHandle  = NULL;
//int myPWMSpeed = 0; // Global variable to store PWM speed


// Global Variables
SSCMA AI;
Servo myServo_D2;
int myLowestSpeed = 53;          //  was 60;  37;   // slowest speed that the car moves on a charged battery. Typically between about 20 and 50 depending on your DC motor  (It will "hum" if not enough)
int myChosenSpeed = 0;           // speed the car should be going
int myOldChosenSpeed = 0;        // speed the car should be going

int myAngle = 0;
int myOldAngle = 0;

int myDrivePwmPin = D0;          // PWM pin for the big motor driver.
//int myMultipleObjects = 0;     // zero the object count
int myObjectMax = 5;             // maximum number of objects to increase speed
int mySpeedMultiplier =  3;      // object speed mulitplier
int myMotorDelay  =   130;        // was 200
int myTurnDelay  =   300;         // was 200

void myMotorTask(void *parameter) {
  while (true) {

   if (myOldChosenSpeed != myChosenSpeed){   // if speed changed
      myOldChosenSpeed = myChosenSpeed;
      if (myChosenSpeed > 255){myChosenSpeed = 255;}
      if (myChosenSpeed < 0){myChosenSpeed = 0;}
       
      analogWrite(myDrivePwmPin, myChosenSpeed);                         
      Serial.print(".");   // signal speed change
   }

   vTaskDelay(pdMS_TO_TICKS(myMotorDelay));   
         
  } 
}


void myTurnTask(void *parameter) {
  while (true) {

   if (myOldAngle != myAngle){
     if (myAngle > 127) {myAngle = 127;}
     if (myAngle < 53) {myAngle = 53;}

      myServo_D2.write(myAngle); // turn  
      myOldAngle = myAngle;
      Serial.print("|");  //signal turn change   
      
    }
      
    vTaskDelay(pdMS_TO_TICKS(myTurnDelay));   
    
  } 
}



void setup(){
    Serial.begin(115200);
    pinMode(myDrivePwmPin, OUTPUT);   // PWM 0 to 255 
    myServo_D2.attach(D2); // D2 should do PWM on XIOA
    
    // note the two drive pins on the big motor driver are just connected to GND and 3V3 respectively.

   // test motor
    analogWrite(myDrivePwmPin, myLowestSpeed); // slowest speed motor test
    delay(500);                                // wait a bit
    analogWrite(myDrivePwmPin, 0);             // stop motor

    // Grove Vision AI V2 and zero at least one value
    
    AI.begin();
    
    xTaskCreatePinnedToCore(
      myMotorTask,         // Function to be executed
      "myMotorTask",       // Name of the task
      2048,                // Stack size in words
      NULL,                // Task input parameter
      0,                   // Priority of the task 0 to 10  was 1
      &myMotorTaskHandle,  // Task handle
      0                    // Core where the task should run
  );


    
    xTaskCreatePinnedToCore(
      myTurnTask,         // Function to be executed
      "myTurnTask",       // Name of the task
      2048,               // Stack size in words
      NULL,               // Task input parameter
      0,                  // Priority of the task 0 to 10  was 1
      &myTurnTaskHandle,  // Task handle
      0                   // Core where the task should run
  );

}

void loop(){
    if (!AI.invoke() ){
  
     AI.perf().prepocess;
     AI.perf().inference;
     AI.perf().postprocess;
     
     Serial.println();   
     if (AI.boxes()[0].score > 85 ){
        Serial.print(" Score[0]:"); 
        Serial.print(String(AI.boxes()[0].score)+", ");
        myChosenSpeed = myLowestSpeed;   // only one speed or stop

      // 320 x 320 width and height 
       if( AI.boxes()[0].x < 120){
            Serial.print(" Right ");
            myAngle = 127;
           // myServo_D2.write(127); // turn Right
       }
       else if(AI.boxes()[0].x >= 120 && AI.boxes()[0].x <= 140 ){
            Serial.print(" Center ");
            myAngle = 90;
         //   myServo_D2.write(90); // turn center
       }

       else if (AI.boxes()[0].x > 140) {
            Serial.print(" Left ");
            myAngle = 53;
          //  myServo_D2.write(53); // turn left
       }

     }
      else {
        Serial.print(" Score[0]:");        
        Serial.print(AI.boxes()[0].score);
        Serial.print(",  None ");
        myChosenSpeed = 0;   // stop the car!

      }
      
      Serial.print(" Speed:");
      Serial.print(myChosenSpeed);
      AI.boxes()[0].score = 0;  // pre-zero the score for next loop
      AI.boxes().clear();

    }

}
