
#include <Ethernet.h>
#include <SPI.h>
#include <Time.h>

//Pines LED RGB
const int PIN_RED = 5;
const int PIN_GREEN = 6;
const int PIN_BLUE = 7;


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Direccion MAC Arduino Ethernet.
char server[] = "192.168.0.106";                     //URL del Servidor.
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
  Serial.println("Conectado a 192.168.0.116...");
  if (client.connected()){
    setColor(false, false, true);
    delay(500);
  }
  
  //*****Peticion del Tiempo del Servidor para Sincronizacion de la Arduino.
  if (client.connected()){

    client.println("GET /sync?board=Arduino1 HTTP/1.1");
    client.println("Host: 192.168.0.106");
    client.println();
    
    while (!client.available()) {
    }
    
    String strTime = "";
    while (client.available()) {
      char character = client.read();
      strTime.concat(character);
      Serial.println(strTime);
    }
    delay(500);
    
    client.stop();
  }
  Serial.println("Sincronizando Arduino Board...");
  setTime(1394585131542);
  
  
}

void loop(){
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
