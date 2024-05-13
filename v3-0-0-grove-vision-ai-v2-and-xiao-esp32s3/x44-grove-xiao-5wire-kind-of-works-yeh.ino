/*
 * Connections XIAO-esp32s3 to Grove Vision AI V2 
 * GND to GND
 * 3V3 to 3V3
 * SDA (D4) to SDA grove
 * SCL (D5) to SCL Grove
 * 
 * Connections XIAO-esp32S3 to Servo
 * D2 to Orange wire
 * 
 * Connections XIAO-esp32S3 to Big Motor Driver
 * D0 to top left 1   Digital turn
 * D6 to top left 3   PWM motor speed
 * D1 to top left 6   Digital turn
 * 3V3 to top left 7
 * GND to top left 8
 * 
 * DON'T USE PINS D3 AND D6 ON THE XIAO AS THEY ARE PART OF THE COM WITH THE GROVE.
 */

/*
 * Notes on 5-wire DC turning motor with potentiometer:
 * This servo is actually a DC motor that uses a potentiometer to determine position.
 * The motor has a very small turning radius, approximately 53 degrees to 127 degrees.
 * 
 * Connections:
 * Red - Motor A1
 * Black - Motor A2
 * White - 3V3
 * Yellow - A0
 * Brown - GND
 */

#include <Seeed_Arduino_SSCMA.h>

// Initialize Grove Vision AI
SSCMA AI;

// Define motor and sensor pins
const int myDrivePwmPin  = A0;
const int myTurnPotPin   = A2;
const int myTurnDirPin   = A8;
const int myTurnPwmPin   = A9;

// Define constants and variables for motor control
const int mySlowestSpeed = 53;   
const int myAngleChange  = 10;
const int myMaxAngle     = 4094;   
const int myMinAngle     = 0;      
const int myRealMinX     = 10;     
const int myRealMaxX     = 210;    
const int myNoneMax      = 3;

int myMainSpeed          = 0;  
int myOldSpeed           = 0;   
int myMappedAngle        = 0;
int myOldMappedAngle     = 0;
int myNoneCount          = 0;
int currentPosition      = 0;
int myRandom             = 0;
const int myTurnSpeed    = 255;
const int myTurnOk       = 50;   // allowed error
const int myTurnDelay    = 20;   //15;   // was 9; was OK..... was 5; to short    // time to turn ms

void setup() {
    Serial.begin(115200); 
    pinMode(LED_BUILTIN, OUTPUT);
    
    pinMode(myTurnPwmPin, OUTPUT);
    pinMode(myTurnDirPin, OUTPUT);
    pinMode(myDrivePwmPin, OUTPUT);   


/*

    Serial.println("Ready");   
    delay(5000); 
    Serial.println("Soon");   
    delay(5000); 



       
    Serial.println("motors testing");    
    delay(500);

    // Test motors
    Serial.println("motor testing turn to center");
    //myRandom = random(4000);
    turnMotorToPosition(2045);
    analogWrite(myDrivePwmPin, mySlowestSpeed); // Slowest speed motor test
    delay(500);   
                 
 
    Serial.println("Stop and turn Left");              
    analogWrite(myDrivePwmPin, 0); // Stop motor
   // myRandom = random(4000);
    turnMotorToPosition(100);
    delay(500);     
      
    Serial.println("go and turn right");                
   // myRandom = random(4000);
    turnMotorToPosition(4000);
    delay(500);       
         
    analogWrite(myDrivePwmPin, 0); // Stop motor
    Serial.println("Motor test done");    
    delay(500); 


*/


    Serial.println("Setup Grove AV V2");    
    delay(50);

    AI.begin();  // Initialize Grove Vision AI V2 
    Serial.println("Did it work?");    
    delay(50);
 
}

void loop() {
    if (!AI.invoke()) {
        AI.perf().prepocess;
        AI.perf().inference;
        AI.perf().postprocess;

        Serial.println();
        if (AI.boxes()[0].score > 81) {
           // myMappedAngle = map(AI.boxes()[0].x, myRealMinX, myRealMaxX, myMaxAngle, myMinAngle);
            myMappedAngle = map(AI.boxes()[0].x, myRealMinX, myRealMaxX, myMinAngle, myMaxAngle);
           // Serial.print(" score:" + String(AI.boxes()[0].score) + ", x:" + String(AI.boxes()[0].x) + ", map:" + String(myMappedAngle) + ", old:" + String(myOldMappedAngle));
            Serial.print(" score:" + String(AI.boxes()[0].score) + ", x:" + String(AI.boxes()[0].x) );
            myMainSpeed = mySlowestSpeed;



/*

            if (myOldMappedAngle < myMappedAngle - myAngleChange) {
                myOldMappedAngle += myAngleChange;    
                if (myOldMappedAngle > myMaxAngle) {
                    myOldMappedAngle = myMaxAngle;
                }  
                turnMotorToPosition(myOldMappedAngle);
                Serial.print(" >");   
            } else if (myOldMappedAngle > myMappedAngle + myAngleChange) {
                myOldMappedAngle -= myAngleChange;
                if (myOldMappedAngle < myMinAngle) {
                    myOldMappedAngle = myMinAngle;
                }      
                turnMotorToPosition(myOldMappedAngle);
                Serial.print(" <");  
            }


*/


            turnMotorToPosition(myMappedAngle);

            myNoneCount = 0;
        } else {
            myNoneCount += 1;
            Serial.print(String(AI.boxes()[0].score) + ", None, count:" + String(myNoneCount));
            if (myNoneCount >= myNoneMax) {   
                myMainSpeed = 0;       
                analogWrite(myDrivePwmPin, myMainSpeed);   
                Serial.print(" |"); 
            } 
        }

        if (myOldSpeed != myMainSpeed) {
            analogWrite(myDrivePwmPin, myMainSpeed);   
            Serial.print(" ."); 
            myOldSpeed = myMainSpeed;
        }

        AI.boxes()[0].score = 0;
        AI.boxes().clear();
    }
}

void turnMotorToPosition(int desiredPosition) {
    currentPosition = analogRead(myTurnPotPin);    
    Serial.print(" Want: " + String(desiredPosition) + ", Now: " + String(currentPosition));

    if (abs(currentPosition - desiredPosition) > myTurnOk) {
        if (currentPosition < desiredPosition) {
            digitalWrite(myTurnDirPin, HIGH); 
            analogWrite(myTurnPwmPin, myTurnSpeed); 
            Serial.print(" >");                     // more right
        } else {
            digitalWrite(myTurnDirPin, LOW); 
            analogWrite(myTurnPwmPin, myTurnSpeed);
            Serial.print(" <");                      // more left
        }
        delay(myTurnDelay); 
        analogWrite(myTurnPwmPin, 0); 
    } else {
        analogWrite(myTurnPwmPin, 0); 
        Serial.print(", |");
    }
    currentPosition = analogRead(myTurnPotPin);  
    Serial.print(", after: " + String(currentPosition));
}
