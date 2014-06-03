int pin13 = 13;
int pin12 = 12;
int pin11 = 11;
int pin10 = 10;
int pin9 = 9;

void setup(){
  
  pinMode(pin13, OUTPUT);
  pinMode(pin12, OUTPUT);
  pinMode(pin11, OUTPUT);
  pinMode(pin10, OUTPUT);
  pinMode(pin9, OUTPUT);
  
}

void loop(){
  setPin(pin13);
  setPin(pin12);
  setPin(pin11);
  setPin(pin10);
  setPin(pin9);
}

void setPin(int pin){
  digitalWrite(pin, HIGH);
  delay(2000);
  digitalWrite(pin, LOW);
  delay(1000);
}
