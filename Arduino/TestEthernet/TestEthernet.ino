#include <SPI.h>
#include <Ethernet.h>

//Dirección MAC de la Unidad de escritura.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192, 168, 0, 200};

void setup(){
 Ethernet.begin(mac, ip);
 pinMode(8, OUTPUT);
 
}

void loop(){
  digitalWrite(8, HIGH);
}
