const int INSIDE = 0;
const int OUTSIDE = 1;
const int CLOSED = 2;

boolean isInside;
boolean isOutside;
boolean isClosed;

void setup(){
  
  Serial.begin(9600);
  
  isInside = getValue(INSIDE);
  isOutside = getValue(OUTSIDE);
  isClosed = getValue(CLOSED);
  
}

void loop(){
  
  if (isInside != getValue(INSIDE)){
    isInside = !isInside;
    Serial.print("Change inside ");
    Serial.println(isInside);
  }
  
  if (isOutside != getValue(OUTSIDE)){
    isOutside = !isOutside;
    Serial.print("Change outside ");
    Serial.println(isOutside);
  }
  
  if (isClosed != getValue(CLOSED)){
    isClosed = !isClosed;
    Serial.print("Change closed ");
    Serial.println(isClosed);
  }
  
}

boolean getValue(int pin){
  
  int value = map(analogRead(pin), 0, 1023, 0, 2);
  return (boolean)value;
  
}
