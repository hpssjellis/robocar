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
 * Connections XIAO-esp32S3 to Big Motor Driver
 * D0 to top left 1   Digital turn
 * D6 to top left 3    PWM motor speed
 * D1 to top left 6     digital turn
 * 3V3 to top left 7
 * GND to top left 8
 * 
 * 
 * Don't use D6 or D3 they are both used to communicate with the Grove
 * 
 */


#include <Seeed_Arduino_SSCMA.h>
#include <ESP32Servo.h>   // for XIAO-ESP32S3-Sense


#define DEBUG

SSCMA AI;
Servo myServo_D2;

int myMinSpeed = 46;   // slowest speed that the car moves on a charged battery
int myMaxSpeed = 46;   // slowest speed that the car moves on a charged battery

int myNewMainSpeed = 37;   // slowest speed that the car moves on a charged battery
int myOldMainSpeed = 0;   // slowest speed that the car moves on a charged battery
int myDrivePwmPin = D0;

void setup(){
    Serial.begin(115200);
  
    pinMode(myDrivePwmPin, OUTPUT);   // PWM 0 to 255
    
    myServo_D2.attach(D2); // D2 should do PWM on XIOA
    // note the two drive pins on the big motor driver are just connected to GND and 3V3 respectively.

    analogWrite(myDrivePwmPin, 40);   // show motor slow 
    delay(500); 
    analogWrite(myDrivePwmPin, 0);   // stop motor  

    // Grove Vision AI V2 and zero at least one value
    AI.begin();

}

void loop(){
    if (!AI.invoke() ){
  
     AI.perf().prepocess;
     AI.perf().inference;
     AI.perf().postprocess;

     if (AI.boxes()[0].score > 85 ){   // 82 is often not an item
#ifdef DEBUG      
      Serial.println();   // reset the println
       ("Score: " + String(AI.boxes()[0].score)+", ");
#endif      
      myNewMainSpeed = myMinSpeed;
      if (myOldMainSpeed != myNewMainSpeed){
         analogWrite(myDrivePwmPin, myNewMainSpeed);   // go medium  
#ifdef DEBUG            
         Serial.print("  Driving  ");
#endif         
         myOldMainSpeed = myNewMainSpeed;
      }

      // 320 x 320 width and height 
      if( AI.boxes()[0].x < 120){
#ifdef DEBUG           
            Serial.print("Right");
#endif            
            myServo_D2.write(110); // turn Right
      }
      else if(AI.boxes()[0].x >= 120 && AI.boxes()[0].x <= 140 ){
#ifdef DEBUG   
            Serial.print("Center");
#endif            
            myServo_D2.write(90); // turn center
      }

      else if (AI.boxes()[0].x > 140) {
#ifdef DEBUG           
            Serial.print("Left");
#endif
            myServo_D2.write(70); // turn left
      }

     }
      else {
#ifdef DEBUG   
        Serial.println();
        Serial.print(String(AI.boxes()[0].score)+", None");
#endif        
        if (myOldMainSpeed != 0){
           analogWrite(myDrivePwmPin, 0);   // No objects detected so stop
#ifdef DEBUG              
           Serial.print("  Stopped  ");
#endif           
           myOldMainSpeed = 0;
           myNewMainSpeed = 0;
        }
        
     } // end may if statement
#ifdef DEBUG        
     Serial.print(", Speed, Old: " + String(myOldMainSpeed)+", New: "+String(myNewMainSpeed)+", ");
#endif
     AI.boxes().clear(); // pre-zero for next loop
    }
 
}
