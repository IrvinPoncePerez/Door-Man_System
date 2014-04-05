#include <VirtualWire.h>

void setup(){
  Serial.begin(9600);
  
  vw_set_ptt_inverted(true);
  vw_setup(4800);
  vw_set_tx_pin(4);
}

void loop(){
  char *msg = "Hola Tu";
  if (vw_send((uint8_t *)msg, strlen(msg))){
    vw_wait_tx();
  
    delay(3000);
    Serial.println("Send Message!");
  } 
}
