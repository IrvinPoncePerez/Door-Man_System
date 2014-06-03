const int PIN_RED = 5;
const int PIN_GREEN = 7;
const int PIN_BLUE = 6;

void setup(){
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  setColor(false, false, false);
}

void loop(){
  setColor(true, false, false);
  delay(1000);
  setColor(false, true, false);
  delay(1000);
  setColor(false, false, true);
  delay(1000);
  setColor(true, true, true);
  delay(1000);
  
}

void setColor(boolean red, boolean green, boolean blue){
 digitalWrite(PIN_RED, !red);
 digitalWrite(PIN_GREEN, !green);
 digitalWrite(PIN_BLUE, !blue); 
}
