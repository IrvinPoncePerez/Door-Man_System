const int PIN_INSIDE = 0;
const int PIN_OUTSIDE = 1;

void setup(){
  Serial.begin(9600);
  attachInterrupt(PIN_INSIDE, insideChange, CHANGE);
  attachInterrupt(PIN_OUTSIDE, outsideChange, CHANGE);
}

void loop(){
  
}

void insideChange(){
  Serial.println("Inside");
}

void outsideChange(){
  Serial.println("Outside");
}
