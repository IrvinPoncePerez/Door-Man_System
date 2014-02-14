#include <Ethernet.h>
#include <SPI.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char serverName[] = "door-man.appspot.com";
int serverPort = 80;
EthernetClient client;
int amarillo = 6;
int rojo = 7;

void setup(){
 pinMode(amarillo, OUTPUT);
 pinMode(rojo, OUTPUT);
 
 Ethernet.begin(mac);
 delay(1000);
}

void loop(){
  
  while(!client.connected()){
    client.connect(serverName, serverPort);
    digitalWrite(amarillo, LOW);  
  }
  digitalWrite(amarillo, HIGH);
  
  digitalWrite(rojo, HIGH);
  delay(1000);
  digitalWrite(rojo,LOW);
  delay(1000);
}
