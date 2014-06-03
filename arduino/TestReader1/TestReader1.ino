void setup(){
  Serial.begin(9600);
  Serial.println("setup");
}

void loop(){
  Serial.println("Hola");
  while(Serial.available()){
    Serial.println(Serial.read());
  }
}
