#include <Manchester.h>

#define RX_PIN 4

void setup(){
  Serial.begin(9600);
  man.setupReceive(RX_PIN, MAN_4800);
  man.beginReceive();
}

void loop(){
  String message = "";
  while (man.receiveComplete()){
    uint16_t c = man.getMessage();
    Serial.println(c);
    message.concat((char)c);
    man.beginReceive();
  }
  if (!message.equals("")){
    Serial.println(message);
  }
 
}

