boolean value = false;

void setup(){
  Serial.begin(9600);
}

void loop(){
  boolean result = getSwitch(0);
  if (value != result){
    value = result;
    Serial.println(value);
  }
}

boolean getSwitch(int pin){
  int value = map(analogRead(pin), 0, 1023, 0, 2);
  return (boolean)value;
}
