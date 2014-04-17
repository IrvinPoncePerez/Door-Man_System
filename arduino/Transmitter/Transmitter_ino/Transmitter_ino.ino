#include <Manchester.h>

#define TX_PIN 4

void setup(){
  Serial.begin(9600);
  man.setupTransmit(TX_PIN, MAN_4800);
}

void loop(){
  sendMessage("Hola Mundo");
}

void sendMessage(String message){
  
  for (int i = 0; i < message.length(); i++){
    uint16_t data = message[i];
    man.transmit(data);
    Serial.println(data);
    delay(10);
  }
  
}
