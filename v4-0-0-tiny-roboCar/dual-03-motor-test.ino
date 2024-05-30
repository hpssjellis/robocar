// RC Car servo and big motor driver
// By Jeremy Ellis
// MIT license

// for now reference at https://github.com/hpssjellis/particle.io-photon-high-school-robotics/tree/master/a11-dc-motor-drivers
// although pin names wrong

// You are suppossed to get it working using the web-app
// Draw your circuit diagram first
// This program will just tell you if  the connections are working
// See https://www.pololu.com/product/1451 for assistance

// On motor driver board LED goes red for one direction and green for the other






#include <Arduino.h> // Only needed by https://platformio.org/
//#include <Servo.h>
//#include <ESP32Servo.h>

//Servo myServo_D2;





void setup() {
  
   // myServo_D2.attach(D2); // D2 should do PWM on Portenta

    Serial.begin(115200);
    pinMode(A0, OUTPUT);  // left-motor PWM 0 to 255 
    pinMode(1, OUTPUT);   // left-Motor direction
    
    pinMode(A2, OUTPUT);   // right-motor PWM 0 to 255
    pinMode(3, OUTPUT);   // right-Motor direction

    
                           

    
   // pinMode(LED_BUILTIN, OUTPUT);

    // to connect wires and put car on ground
    delay(1000);
    Serial.println("Just wait");
   // digitalWrite(LED_BUILTIN, LOW);   
    delay(3000);
  //  digitalWrite(LED_BUILTIN, HIGH);    
    delay(2000);
    
}

void loop() {
    
    Serial.println("LED on");
  //  digitalWrite(LED_BUILTIN, LOW);  
      
    Serial.println("left motor");
    digitalWrite(1, HIGH);   // HIGH backqwards, LOW forwards, set left-motor-direction
    analogWrite(A0, 80);   // left-motor
    delay(1000);
    analogWrite(A0, 0);     // stop
    delay(3000); 
    
    Serial.println("right motor");
    digitalWrite(3, HIGH);    // set left-motor-direction
    analogWrite(A2, 100);   // right-motor   
    delay(1000);    
    analogWrite(A2, 0);     // stop
    delay(3000); 

    Serial.println("both fast");
    digitalWrite(1, LOW);     // set left-motor-direction
    analogWrite(A0, 255);     // left-motor
    digitalWrite(3, LOW);     // set left-motor-direction
    analogWrite(A2, 255);     // right-motor 
    delay(1000);
    analogWrite(A0, 0);       // left-motor
    analogWrite(A2, 0);       // right-motor  
    delay(3000);
    
    Serial.println("LED OFF");
   // digitalWrite(LED_BUILTIN, HIGH);  
    delay(9000);            // wait 9 seconds
        
}
