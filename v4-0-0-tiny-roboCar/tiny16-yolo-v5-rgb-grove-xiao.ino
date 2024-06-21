

#include <Arduino.h> // Only needed by https://platformio.org/
//#include <Servo.h>
//#include <ESP32Servo.h>

//Servo myServo_D2;

// Global Variables
int myCountStop = 0;

#include <Seeed_Arduino_SSCMA.h>

SSCMA AI;

void setup(){
    AI.begin();
    Serial.begin(9600);    
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);   
    delay(3000);
    digitalWrite(LED_BUILTIN, HIGH);    
    delay(2000);  
    Serial.println("Start");
    
    pinMode(A0, OUTPUT);  // left-motor PWM 0 to 255 
    pinMode(1, OUTPUT);   // left-Motor direction
    
    pinMode(A2, OUTPUT);   // right-motor PWM 0 to 255
    pinMode(3, OUTPUT);   // right-Motor direction


// motor test both wheels moving briefly
    Serial.println("both fast");
    
    digitalWrite(LED_BUILTIN, LOW);   
    digitalWrite(1, LOW);     // set left-motor-direction
    analogWrite(A0, 110);     // left-motor
    digitalWrite(3, LOW);     // set right-motor-direction
    analogWrite(A2, 110);     // right-motor 
    delay(1000);
    
    analogWrite(A0, 0);     // left-motor off
    analogWrite(A2, 0);     // right-motor off
    digitalWrite(LED_BUILTIN, HIGH);   


    //just going to have left motor run continuously
     analogWrite(A0, 110); 
}

void loop(){
    if (!AI.invoke()){

   
       // Serial.println("invoke success");
        Serial.print("perf: prepocess=");
        Serial.print(AI.perf().prepocess);
        Serial.print(", inference=");
        Serial.print(AI.perf().inference);
        Serial.print(", postpocess=");
        Serial.print(AI.perf().postprocess);
        Serial.print(", ");



        // what about stopping
        if (AI.boxes().size() == 0  || AI.boxes()[0].score < 30 ){

            if (myCountStop > 20){   // stop both motors
              Serial.print("Motor stopped, ");  
              analogWrite(A2, 0);     // if no objects stop right motor
              analogWrite(A0, 0);     // also stop left motor
             //myCountStop = 0;  // don't reset until a detection was made
              digitalWrite(LED_BUILTIN, HIGH);
            } 

           Serial.println("No objects, #: " + String(myCountStop)); 
           myCountStop ++;


           
        } else {
           myCountStop = 0;
           analogWrite(A0, 110); // startup the left motor again

          // analogWrite(A0, 110); // perma set    // if objects, keep the left motor running
           Serial.print("#: "+String(AI.boxes().size()) + " detection, GO, " );
           
           // and adjust the right motor
           // later decide what value range should go straight
           // [0] is hopefully the best reading, possibly later take an average
           if ( AI.boxes()[0].x   < 145 ){  
                 analogWrite(A2, 50);     // right-motor slower should turn right
                 Serial.print("RIGHT?, ");
                 digitalWrite(LED_BUILTIN, HIGH);    
           } else if ( AI.boxes()[0].x   < 152 ) {
                analogWrite(A2, 110);     // right-motor same speed as left go straight
                Serial.print("Straight?, ");
                digitalWrite(LED_BUILTIN, LOW);    
           } else {
                analogWrite(A2, 170);     // right-motor faster should turn left
                Serial.print("LEFT?, ");
                digitalWrite(LED_BUILTIN, HIGH);    
           }
        }


        for (int i = 0; i < AI.boxes().size(); i++)
        {
            Serial.print("Box[");
            Serial.print(i);
            Serial.print("] target=");
            Serial.print(AI.boxes()[i].target);
            Serial.print(", score=");
            Serial.print(AI.boxes()[i].score);
            Serial.print(", x=");
            Serial.print(AI.boxes()[i].x);
            Serial.print(", y=");
            Serial.print(AI.boxes()[i].y);
            Serial.print(", w=");
            Serial.print(AI.boxes()[i].w);
            Serial.print(", h=");
            Serial.println(AI.boxes()[i].h);

        }
        
    }
}
