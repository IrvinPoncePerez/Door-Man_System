void setup(){
  Serial.begin(9600);
}

void loop(){
  float sensorValue = analogRead(0);
  int value;
    
  value = map(sensorValue, 200, 1023, 0, 100);
  
  if (value > 0){  
    Serial.print("Sensor Value : ");
    Serial.println(sensorValue);
    
    sensorValue /= 1024;
    
    Serial.print("Sensor Value : ");
    Serial.println(sensorValue);
    
    sensorValue *= 3.7;
    
    Serial.print("Sensor Value : ");
    Serial.println(sensorValue);
    
    Serial.print(value);
    Serial.println("%");
    
    delay(60000);
    
    Serial.println();
  }
}
