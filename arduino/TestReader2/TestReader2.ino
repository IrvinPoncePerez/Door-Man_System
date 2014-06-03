#include <Servo.h>

Servo myServo;

void setup(){
  myServo.attach(8);
  myServo.write(0);
}

void loop(){
 delay(1000);
 myServo.write(45);
 delay(1000);
 myServo.write(90);
 delay(1000);
 myServo.write(135);
 delay(1000);
 myServo.write(180);
 delay(1000);
 
}
