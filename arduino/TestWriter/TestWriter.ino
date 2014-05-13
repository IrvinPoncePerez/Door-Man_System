const int RED = 7;
const int GREEN = 5;
const int BLUE = 6;

void setup(){
  Serial.begin(9600);
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  setColor(false, false, false);
}

void loop(){
  delay(2000);
  setColor(true, false, false); //Rojo
  delay(2000);
  setColor(false, true, false); //Verde
  delay(2000);
  setColor(false, false, true); //Azul
  
  if (Serial.available()){
    while (Serial.available()){
      Serial.print(Serial.read());
    }
  }
}

void setColor(boolean red, boolean green, boolean blue){
  digitalWrite(RED, !red);
  digitalWrite(GREEN, !green);
  digitalWrite(BLUE, !blue);
}
