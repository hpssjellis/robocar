

#include <Arduino.h> // Only needed by https://platformio.org/
//#include <Servo.h>
//#include <ESP32Servo.h>

//Servo myServo_D2;


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
    digitalWrite(1, LOW);     // set left-motor-direction
    analogWrite(A0, 110);     // left-motor
    digitalWrite(3, LOW);     // set right-motor-direction
    analogWrite(A2, 110);     // right-motor 
    delay(500);
    
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
        if (AI.boxes().size() == 0  || AI.boxes()[0].score < 40 ){
           analogWrite(A2, 0);     // if no objects stop right motor
           analogWrite(A0, 0);     // also stop left motor
           Serial.println("STOP, ");
        } else {
           analogWrite(A0, 110);     // if objects keep the left motor running
           Serial.print("GO, ");
           
           // and adjust the right motor
           // later decide what value range should go straight
           // [0] is hopefully the best reading, possibly later take an average
           if ( AI.boxes()[0].x   < 105 ){  
                 analogWrite(A2, 100);     // right-motor slower
                 Serial.print("RIGHT?, ");
           } else {
                analogWrite(A2, 120);     // right-motor faster
                Serial.print("LEFT?, ");
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
