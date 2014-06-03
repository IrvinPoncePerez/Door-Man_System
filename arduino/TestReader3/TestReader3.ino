const int PIN = 2;

boolean IsBool = false;

void setup(){
  Serial.begin(9600);
  
  IsBool = getValue(PIN);
}

void loop(){
  
  if (IsBool != getValue(PIN)){
    IsBool = getValue(PIN);
    Serial.print("Is change... ");
    Serial.println(IsBool);
  }
 
}

boolean getValue(int pin){
  int value = map(analogRead(pin), 0, 1023, 0, 2);
  return (boolean)value;
}
