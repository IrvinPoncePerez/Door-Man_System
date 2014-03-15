#include <Ethernet.h>
#include <SPI.h>
#include <JsonParser.h>
#include <Time.h>
#include <DateTimeStrings.h>

//Pines LED RGB
const int PIN_RED = 5;
const int PIN_GREEN = 6;
const int PIN_BLUE = 7;

//Objeto Json para realizar el json.parse();
JsonParser<32> parser;


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Direccion MAC Arduino Ethernet.
char server[] = "door-man.appspot.com";                     //URL del Servidor.
IPAddress ip (192, 168, 0, 111);                     //IP de la Arduino Ethernet.
int port = 80;                                       //Puerto de comunicacion.
EthernetClient client;                               //Inicializacion de la libreria.

//----------------------------------------------------------
//      Inicializacion de la Arduino Ethernet.
//----------------------------------------------------------
void setup(){
  Serial.begin(9600);
  
 
  //******Configuración de Pines Entrada y Salida.******
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  
  //*****Configuración de la Dirección IP y MAC de la Arduino Ethernet.*****
  if(Ethernet.begin(mac) == 0){
    Serial.println("Tarjeta Ethernet configurada, usando IP estatica.");
    Ethernet.begin(mac, ip);
  } else {
    Serial.println("Tarjeta Ethernet configurada, usando DHCP.");
  }
  setColor(true, false, false);
  delay(500);  
  
  
  //*****Conexión con el Servidor door-man.appspot.com*****
  while(!client.connected()){
    client.connect(server, port);
  }
  if (client.connected()){
    setColor(false, false, true);
    delay(500);
  }
  
  //*****Peticion del Tiempo del Servidor para Sincronizacion de la Arduino.
  if (client.connected()){
    setRequestGET("/sync?board=Arduino1");  
    setTimeArduino(getResponseGET());    
  }
  delay(500);
  setColor(false, false, false);
}

//**************************************************************************
//                Ciclo de la Arduino Ethernet
//**************************************************************************
void loop(){
  delay(1000);
  printDate();
}

//*****************************************************************************
//    Establece el color a mostrar, de acuerdo al estatus de la tarjeta
//  OFF(false, false, false)  RED(true, false, false)    GREEN(false, true, false)
//  BLUE(false, false, blue)  YELLOW(true, true, false)  CYAN(false, true, true)
//  PURPLE(true, false, true) WHITE(true, true, true)
//*****************************************************************************
void setColor(boolean red, boolean green, boolean blue){
  digitalWrite(PIN_RED, red);
  digitalWrite(PIN_GREEN, green);
  digitalWrite(PIN_BLUE, blue);
}

//******************************************************************************
//  Realiza la Petición GET a la URL asignada.
//******************************************************************************
void setRequestGET(String URL){
   client.println("GET " + URL + " HTTP/1.1");
   client.println("Host: door-man.appspot.com");   
   client.println("Connection: keep-alive");
   client.println("Content-Type: application/x-www-form-urlencoded");
   client.println();
}

//******************************************************************************
//  Lee la respuesta realizada por el servidor y la devuelve como tipo cadena
//******************************************************************************
String getResponseGET(){
      while (!client.available()) {
    }
    
    String response = "";
    while (client.available()) {
      char character = client.read();
      response.concat(character);
    }    
    client.stop();
    
    int indexOf = response.indexOf("{");
    response = response.substring(indexOf);
    
    return response;
}

//-----------------------------------------------------------------
//    Función para Establecer la fecha hora de la tarjeta
//-----------------------------------------------------------------
void setTimeArduino(String JSON){
  int lenght = JSON.length();  
  char buffer[lenght];
  JSON.toCharArray(buffer, lenght +1); 

  //JSON Parse.
  JsonHashTable objJson = parser.parseHashTable(buffer);
  
  int Year = objJson.getLong("year");
  int Month = objJson.getLong("month");
  int Day = objJson.getLong("day");
  int Hour = objJson.getLong("hour");
  int Minute = objJson.getLong("minute");
  int Second = objJson.getLong("second");
  setTime(Hour, Minute, Second, Day, Month, Year);
  
  setColor(false, true, false);
}

void printDate(){
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  if (isAM()){
    Serial.print("AM");
  } else if(isPM()){
    Serial.print("PM");
  }
  Serial.print(" ");
  Serial.print(dayStr(weekday()));
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(monthShortStr(month()));
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
