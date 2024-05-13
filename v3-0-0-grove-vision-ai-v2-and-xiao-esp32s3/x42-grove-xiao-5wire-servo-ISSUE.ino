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
 * DON'T USE PINS D3 AND D6 ON THE XIAO AS THEY ARE PART OF THE COM WITH THE GROVE.
 * 
 */
/*

// my regular servo is having issues moving as fast aws the grove vision shield can let it so I am going to try the original 5 wire servo
//This servo is actually a DC motor that I will need to use my dual motor driver an a potentiometer that tells where the motor is. 
//This motor has a very small turning radius probably about 53 degrees to 127 degrees like the regular servo


## or the 5 wire dc turning motor with potentiometer on the original shredder and Dual motor driver

// red motor A1      
// black motor A2 
// white 3V3
// yellow A0
// brown GND

Dual Motor Driver
https://github.com/hpssjellis/maker100-eco/blob/main/media/b-b-f-small-dual-dc-motor-driver.png

myTurnPotPin = A2;       // Analog pin for the potentiometer
myTurnDirPin = A4;  // Digital pin for DC motor direction
myTurnPwmPin = A5;     // PWM pin for the motor

Note: the potentiometer goes from 0 to 4095
*/



#include <Seeed_Arduino_SSCMA.h>
//#include <ESP32Servo.h>   // for XIAO-ESP32S3-Sense


SSCMA AI;
//Servo myServo_D2;
int myDrivePwmPin = D0;
int mySlowestSpeed = 53;   // slowest speed that the car moves on a charged battery
int myMainSpeed = 0;  
int myOldSpeed = 0;   

int myMappedAngle = 0;
int myOldMappedAngle = 0;
int myAngleChange = 10;  // was 5 the degrees the servfo will move each time it moves
int myMaxAngle = 4094;   // 133;    // was 127 max right turn on my car
int myMinAngle = 0;      // 53;     // was 53 max left turn on my car
int myRealMinX = 10;     // Should be 0 but is the lowest x reading your vision model gets
int myRealMaxX = 210;    // should be 320 but is the real max reading your vision model gets

int myNoneCount = 0;
int myNoneMax = 3;


// Define the pins for 5 wire turning motor
const int myTurnPotPin = A2;  // Analog pin for the potentiometer
const int myTurnDirPin = A8;  // A4;  // Digital pin for DC motor direction
const int myTurnPwmPin = A9;   // A5; // PWM pin for the motor
int currentPosition    = 0;
int myRandom           = 0;
int myTurnSpeed        = 255;
int myTurnOk           = 50;   // range that we don't have to turn the motor
int myTurnDelay        = 5;    // time to let the motor get somewhere.


void setup(){
    pinMode(myTurnPwmPin, OUTPUT);
    pinMode(myTurnDirPin, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200); // Updated baud rate
    pinMode(myDrivePwmPin, OUTPUT);   // PWM 0 to 255
   // myServo_D2.attach(D2); // D2 should do PWM on XIOA
    // note the two drive pins on the big motor driver are just connected to GND and 3V3 respectively.

    
    Serial.println("Ready");   
    delay(5000); 
    Serial.println("Soon");   
    delay(5000); 
    
    Serial.println("Setup Grove AV V2");    
    delay(50);
    Serial.println("Did it work?");    
    delay(50);

    AI.begin();  // Grove Vision AI V2 

    Serial.println("motors testing");    
    delay(500);
   // test motors
    Serial.println("motor testing turn to center");
   // myServo_D2.write(90);  
    myRandom = random(4000);
    turnMotorToPosition(myRandom);
    delay(500);
    Serial.println("Go Left");
    analogWrite(myDrivePwmPin, mySlowestSpeed); // slowest speed motor test
   // myServo_D2.write(myMinAngle);
    delay(500);                
    Serial.println("stop turn center");                // wait a bit
    analogWrite(myDrivePwmPin, 0);             // stop motor
    myRandom = random(4000);
    turnMotorToPosition(myRandom);
   // myServo_D2.write(90);
    delay(500);       
    Serial.println("turn right");                // wait a bit
    myRandom = random(4000);
    turnMotorToPosition(myRandom);
   // myServo_D2.write(myMaxAngle);
    delay(500);       
    Serial.println("Motor test done");    
        

}

void loop(){
    if (!AI.invoke() ){

     AI.perf().prepocess;
     AI.perf().inference;
     AI.perf().postprocess;

     Serial.println();
     if (AI.boxes()[0].score > 81 ){
      
      myMappedAngle = map(AI.boxes()[0].x, myRealMinX, myRealMaxX, myMaxAngle, myMinAngle); // x location to angle note reverse numbers 
      Serial.print(" score:"+String(AI.boxes()[0].score) + ", x:" + String(AI.boxes()[0].x) + ", map:" + String(myMappedAngle) + ", old:" + String(myOldMappedAngle) );
      myMainSpeed = mySlowestSpeed;   


      
        
      if (myOldMappedAngle < myMappedAngle - myAngleChange ){
           myOldMappedAngle += myAngleChange;    
           if (myOldMappedAngle > myMaxAngle){myOldMappedAngle = myMaxAngle;}  // protect from maxTurn
           //myServo_D2.write(myOldMappedAngle);                   // turn towards new bigger angle
           turnMotorToPosition(myOldMappedAngle);
          Serial.print(" >");   
          
      } else if (myOldMappedAngle > myMappedAngle + myAngleChange ){
         myOldMappedAngle -= myAngleChange;
         if (myOldMappedAngle < myMinAngle){myOldMappedAngle = myMinAngle;}      // protect from minimum turn
        // myServo_D2.write(myOldMappedAngle - 5);                 // turn towards new smaller angle
         turnMotorToPosition(myOldMappedAngle);
         Serial.print(" <");  
      }  // note: angles  myAngleChange < oldmapped < myAngleChange should not change servo at all





      myNoneCount = 0;  // reset none count since an object found
     } else {
        myNoneCount += 1;
        Serial.print(String( AI.boxes()[0].score)+", None" + ", count:" + String(myNoneCount));
        if (myNoneCount >= myNoneMax) {   // other wise ignore it.
          myMainSpeed = 0;       
          analogWrite(myDrivePwmPin, myMainSpeed);   // No objects detected so stop
          Serial.print(" |"); 
        } 
     }

      if ( myOldSpeed != myMainSpeed){     // test if speed is different
         analogWrite(myDrivePwmPin, myMainSpeed);   
         Serial.print(" ."); 
         myOldSpeed = myMainSpeed;
      }

      AI.boxes()[0].score = 0;  // pre-zero for next loop
      AI.boxes().clear();
    }
  
}




void turnMotorToPosition(int desiredPosition) {

  currentPosition = analogRead(myTurnPotPin);    
  Serial.print("Want: "+ String(desiredPosition) + ", Now: "+ String(currentPosition));

  if (abs(currentPosition - desiredPosition) > myTurnOk ) {

    if (currentPosition < desiredPosition) {
      digitalWrite(myTurnDirPin, HIGH); // Set motor direction forward
      analogWrite(myTurnPwmPin, myTurnSpeed); // Adjust PWM value for speed
    } else {
      digitalWrite(myTurnDirPin, LOW); // Set motor direction reverse
      analogWrite(myTurnPwmPin, myTurnSpeed); // Keep same speed, change direction
    }
    delay(myTurnDelay); // Short delay to allow the motor to move
    analogWrite(myTurnPwmPin, 0); // Stop the turn motor after the delay

  } else {
    analogWrite(myTurnPwmPin, 0); // Stop the motor when position is reached
    Serial.print(", |");
    // no delay needed since motor is stopped
  }
    currentPosition = analogRead(myTurnPotPin);  
    Serial.println(", after: "+ String(currentPosition));
}
