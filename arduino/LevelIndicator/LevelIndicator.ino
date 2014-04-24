void setup(){
  Serial.begin(9600);
}

void loop(){
  delay(5000);
  Serial.println(analogRead(0));
}
