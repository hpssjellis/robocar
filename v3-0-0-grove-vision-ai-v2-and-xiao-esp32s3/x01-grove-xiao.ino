
#include <Seeed_Arduino_SSCMA.h>
#include <ESP32Servo.h>   // for XIAO-ESP32S3-Sense


SSCMA AI;


void setup(){
    AI.begin();
    Serial.begin(115200);
    
    // Stay away from pins D4 (SDA) and D5 (SCL) whiuch are used for I2C communication with the Grove board
    // D3 when both are connected is the reset pin so that messes things up.

  
}

void loop()
{
    if (!AI.invoke() ){

   
     AI.perf().prepocess;
     AI.perf().inference;
     AI.perf().postprocess;

     if (AI.boxes()[0].score > 75 ){
      Serial.print(String(AI.boxes()[0].score)+", ");
     
    }

  }
}
