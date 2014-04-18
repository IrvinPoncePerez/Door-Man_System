#include <Manchester.h>

#define RX_PIN 4

void setup(){
  Serial.begin(9600);
  man.setupReceive(RX_PIN, MAN_4800);
  man.beginReceive();
  Serial.println("Setup");
}

void loop(){
  String message = "";
  if (man.receiveComplete()){
    uint16_t c = man.getMessage();
    message.concat((char)c);
    man.beginReceive();
    Serial.print((char)c);
  }
 
}

