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
 * Note: On the XIAO don't use D3 or D6 as the system is using them
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
int myLowestSpeed = 53;        //  was 60;  37;   // slowest speed that the car moves on a charged battery. Typically between about 20 and 50 depending on your DC motor  (It will "hum" if not enough)
int myHighestSpeed = 53;       // must be below 255
int myChosenSpeed = 0;         // speed the car should be going
int myServoDegrees = 0;        // Changes but the turning degrees from 60 to 120 where 90 is striaght
int myDrivePwmPin = D0;        // PWM pin for the big motor driver.
int myMultipleObjects = 0;     // zero the object count
int myObjectMax = 5;           // maximum number of objects to increase speed
int mySpeedMultiplier =  3;    // object speed mulitplier
int myMotorDelay  =   30;      // was 30 was 150;  delay makes motor respond longer, but try to match classification
float myAverageX = 0;
int myIntAverageX = 0;
int myCount = 0;
bool myAnalysis = false;
int myStopCount = 0;

void myTask(void *parameter) {
  while (true) {
    if (myChosenSpeed > myHighestSpeed){myChosenSpeed = myHighestSpeed;}
    if (myChosenSpeed < 0){myChosenSpeed = 0;}

    Serial.print(" Speed:");
    Serial.print(myChosenSpeed);
  
    if (myAnalysis){
      analogWrite(myDrivePwmPin, myChosenSpeed);
      myServo_D2.write(myServoDegrees);  // turn the car between 60 and 120
    }
    vTaskDelay(pdMS_TO_TICKS(myMotorDelay)); 


  }
}


void setup(){
    Serial.begin(115200);
    pinMode(myDrivePwmPin, OUTPUT);   // PWM 0 to 255
    myServo_D2.attach(D2); // D2 should do PWM on XIOA
    // note the two drive pins on the big motor driver are just connected to GND and 3V3 respectively.
    
    AI.begin();
    xTaskCreatePinnedToCore(
      myTask,         // Function to be executed
      "MyTask",       // Name of the task
      2048,           // Stack size in words
      NULL,           // Task input parameter
      0,              // Priority of the task. 0 = low 10 higher priority
      &myTaskHandle,  // Task handle
      0               // Core where the task should run
  );

}

void loop(){
  myAnalysis = false;
    if (!AI.invoke() ){
     myAnalysis = true;
     AI.perf().prepocess;
     AI.perf().inference;
     AI.perf().postprocess;

     Serial.println();    
     if (AI.boxes()[0].score > 80 ){
        myStopCount = 0;     // reset the stop count
        myMultipleObjects = 0;
        Serial.print(" Score[0]:"); 
        Serial.print(String(AI.boxes()[0].score)+", ");
       // analogWrite(myDrivePwmPin, myLowestSpeed);   // go medium  
        myChosenSpeed = myLowestSpeed;
        myMultipleObjects = AI.boxes().size();
    
        if (myMultipleObjects > 1){
           Serial.print(" Multiple objects: " + String(myMultipleObjects));
           myAverageX=0;
           for (myCount=0; myCount < myMultipleObjects; myCount++){
              myAverageX += AI.boxes()[0].x;
              Serial.print(",  X[" + String(myCount) + "]:" + String(AI.boxes()[0].x) );
              
           }
           myIntAverageX = (int)(myAverageX / myCount) ;
      }  else {
         myIntAverageX = AI.boxes()[0].x;
        }
      // 320 x 320 width and height 
      if( myIntAverageX < 90){  // just base the turn on the first object for now
            Serial.println(" Right ");
            myServoDegrees = 120; // turn Right
      }
      else if(myIntAverageX >= 90 && AI.boxes()[0].x <= 120 ){

            Serial.print(" Center ");
            myServoDegrees = 90; // turn center
      }

      else if (myIntAverageX > 120) {   
            Serial.print(" Left ");
            myServoDegrees = 60; // turn left
      }

     }
      else {
        Serial.print(" Score[0]:");        
        Serial.print(AI.boxes()[0].score);
        Serial.print(",  None ");
        //  analogWrite(myDrivePwmPin, 0);   // No objects detected so stop
        myStopCount +=1;
        if (myStopCount > 2){    // helps detect false positives
           myChosenSpeed = 0;   // stop the car!
           myStopCount = 0;   //reset it as well
        }
        // I don't like the delay here so I tried to run the motor in it's own thread
        // delay(500);   //  hmmmm  does not feel like the correct way to deal with this.
      }

     //AI.boxes()[0].score = 0;  // pre-zero the score for next loop
     AI.boxes().clear();       // probabluy does the same thing.


    }

}
