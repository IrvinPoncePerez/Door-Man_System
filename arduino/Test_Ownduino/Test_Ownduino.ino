/****Test pines Digitales****/
const int PIN = 5;

void setup(){
  Serial.begin(9600);
  pinMode(PIN, OUTPUT);
}

void loop(){
  digitalWrite(PIN, HIGH);
  delay(500);
  digitalWrite(PIN, LOW);
  delay(500);
}

/*****Test Attach interrupt*****/
//void setup(){
//  Serial.begin(9600);
//  attachInterrupt(1, holaMundo, CHANGE);
//}
//
//void loop(){
//}
//
//void holaMundo(){
//  Serial.println("Change...");
//}


/*****Test analog pins******/
//void loop(){
//  int value = analogRead(5);
//  Serial.println(value);
//  delay(2000);
//}
