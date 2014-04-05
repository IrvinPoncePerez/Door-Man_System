#include <VirtualWire.h>

void setup(){
  Serial.begin(9600);
  
  vw_set_ptt_inverted(true);
  vw_setup(4800);
  vw_set_rx_pin(2);
  
  vw_rx_start();
}

void loop(){
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if(vw_get_message(buf, &buflen)){
    Serial.println("Receiving Message!");
    for (int i = 0; i < buflen; i++){
      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }

}
