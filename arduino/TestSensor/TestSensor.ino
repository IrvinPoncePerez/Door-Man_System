const int PIN  = 0;
int value = -1;

void setup(){
  Serial.begin(9600);
}

void loop(){
  boolean result = getValueInterruptor(PIN);
  if (value != result){
    value = result;
    Serial.println(value);
  }
 
}

boolean getValueInterruptor(int pin){
   int val = map(analogRead(pin), 0, 1023, 0, 1); 
   return (boolean)val;
}
