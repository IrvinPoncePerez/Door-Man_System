void setup(){
  Serial.begin(9600);
}

void loop(){
  while(Serial.available()){
    Serial.print((char)Serial.read());
  }
  
  //   #door10/inside_open/Supervisor$
}
