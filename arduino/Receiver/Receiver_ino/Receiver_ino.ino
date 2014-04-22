#include <Manchester.h>

#define RX_PIN 4

void setup(){
  Serial.begin(9600);
  man.setupReceive(RX_PIN, MAN_4800);
  man.beginReceive();
  Serial.println("Setup");
}

void loop(){
  if (man.receiveComplete()){
    uint16_t data = man.getMessage();
    char c = (char)data;
    Serial.println(c);
    man.beginReceive();
  } 
}


