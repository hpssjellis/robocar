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
 *   Summary don't use pins D3 and D6!

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

    
 * 
 * 
 * 
 * 
 * 
 */


#include <Arduino.h>
#include <Seeed_Arduino_SSCMA.h>
#include <ESP32Servo.h>   // for XIAO-ESP32S3-Sense


TaskHandle_t myTaskHandle = NULL;



// Global Variables
SSCMA AI;
Servo myServo_D2;

int myDrivePwmPin = D0;        // PWM pin for the big motor driver.
int myLowestSpeed = 53;        //  was 60;  37;   // slowest speed that the car moves on a charged battery. Typically between about 20 and 50 depending on your DC motor  (It will "hum" if not enough)
int myHighestSpeed = 53;       // no speed control here all the same speed
int myNewChosenSpeed = 0;         // speed the car should be going
int myOldChosenSpeed = 0;         // speed the car should be going


int myMinServo = 60;           // careful if too low,  strain on servo motor gear  
int myCenterServo = 90;           // careful if too low,  strain on servo motor gear  
int myMaxServo = 120;          // careful if too high, strain on servo motor gear 
int myNewServoDegrees = 0;        // Changes but the turning degrees from 60 to 120 where 90 is striaght
int myOldServoDegrees = 0;        // Changes but the turning degrees from 60 to 120 where 90 is striaght

//int myMultipleObjects = 0;     // zero the object count
//int myObjectMax = 5;           // maximum number of objects to increase speed
//int mySpeedMultiplier =  3;   // object speed mulitplier
int myMotorDelay  =   30;     // was 30 was 150;  delay makes motor respond longer, but try to match classification
float myAverageX = 0;
int myIntAverageX = 0;
int myCount = 0;
bool myAnalysis = false;
int myAnalysisCount = 0;
int myStopCount = 0;

/*
void myTask(void *parameter) {
  while (true) {
    if (myChosenSpeed > 255){myChosenSpeed = 255;}
    if (myChosenSpeed < 0){myChosenSpeed = 0;}
    if (myMultipleObjects > myObjectMax){  myMultipleObjects = myObjectMax; }
    if (myMultipleObjects > 1){ myChosenSpeed = myChosenSpeed + ( myMultipleObjects * mySpeedMultiplier );  }  // make it go faster if it sees multiple objects
    
    analogWrite(myDrivePwmPin, myChosenSpeed);
    Serial.print(" Speed:");
    Serial.print(myChosenSpeed);
   // if (myChosenSpeed == myLowestSpeed){delay(700);}   // extra delay when nothing found
   // if (myChosenSpeed == 0){delay(700);}               // extra delay when nothing found
   // delay(myMotorDelay);  // just to give the motor a bit of time to react
    vTaskDelay(pdMS_TO_TICKS(myMotorDelay)); 
  }
}

*/


void setup(){
    Serial.begin(115200);
  
    pinMode(myDrivePwmPin, OUTPUT);   // PWM 0 to 255
    
    myServo_D2.attach(D2); // D2 should do PWM on XIOA
    // note the two drive pins on the big motor driver are just connected to GND and 3V3 respectively.





    // Grove Vision AI V2 and zero at least one value

    /*
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
  */

}

void loop(){
    if (!AI.invoke() ){
  
     AI.perf().prepocess;
     AI.perf().inference;
     AI.perf().postprocess;


 
     Serial.println();    
     if (AI.boxes()[0].score > 80 ){
        myNewChosenSpeed = myLowestSpeed;  // set the speed if analysis

        
       // myMultipleObjects = 0;
       // Serial.print(" Score[0]:"); 
       // Serial.print(String(AI.boxes()[0].score)+", ");
   
       // 
       // myMultipleObjects = AI.boxes().size();
      //  if (myMultipleObjects > 1){
       //    Serial.print(" Multiple objects: " + String(myMultipleObjects));
     // }
         myAverageX=0;
         for (myCount=0; myCount < AI.boxes().size(); myCount++){
            myAverageX += AI.boxes()[myCount].x;
            Serial.print(",  X[" + String(myCount) + "]:" + String(AI.boxes()[myCount].x) );            
         }
         myIntAverageX = (int)(myAverageX / myCount) ;


     
      // 320 x 320 width and height 
      if( myIntAverageX < 90){  // if x average less than 80
           // Serial.println(" Right ");
            myNewServoDegrees = myMaxServo; // turn Right
      }
      else if(myIntAverageX >= 90 && myIntAverageX <= 120 ){ // x average between

           // Serial.print(" Center ");
            myNewServoDegrees = myCenterServo; // turn center
      }

      else if (myIntAverageX > 120) {    // x average greater than 120
           // Serial.print(" Left ");
            myNewServoDegrees = myMinServo; // turn left   
      }

     }
      else {
      //  Serial.print(" Score[0]:");        
       // Serial.print(AI.boxes()[0].score);
      //  Serial.print(",  None ");
      //  analogWrite(myDrivePwmPin, 0);   // No objects detected so stop
          myNewChosenSpeed = 0;   // stop the car!
      // I don't like the delay here so I tried to run the motor in it's own thread
       // delay(500);   //  hmmmm  does not feel like the correct way to deal with this.
      }

      if (myNewChosenSpeed > myHighestSpeed){myNewChosenSpeed = myHighestSpeed;}
      if (myNewChosenSpeed < 0){myNewChosenSpeed = 0;}
  
      if (myNewServoDegrees < myMinServo){myNewServoDegrees = myMinServo;}   // min servo angle
      if (myNewServoDegrees > myMaxServo){myNewServoDegrees = myMaxServo;} // max servo angle

      if ( myOldChosenSpeed != myNewChosenSpeed) {
          analogWrite(myDrivePwmPin, myNewChosenSpeed);   // go slow or stop 
          myOldChosenSpeed = myNewChosenSpeed;         // reset the speed to the new value slow or stop only if different
      }
      if (myOldServoDegrees != myNewServoDegrees){
          myServo_D2.write(myNewServoDegrees);            // turn if needed
          myOldServoDegrees = myNewServoDegrees;       // reset the degrtees to the new value only if different
      }
     
     //AI.boxes()[0].score = 0;  // pre-zero the score for next loop
     AI.boxes().clear();


    }

}
