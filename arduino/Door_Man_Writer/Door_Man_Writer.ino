/*********************************************************************************/
/*!
 @file     Door_Man_Writer.ino
 @author   Irvin Ponce Pérez
 
 Sketch de la unidad de escritura de Door-Man System.
 
 Encargado de detectar las tarjetas Mifare Classic y realizar la petición GET
 para consultar si hay datos que escribir en la tarjeta.
 
 La unidad de escritura es la encargada de enviar las notificaciones de la 
 cerradura al servidor door-man.appspot.com
 */
/*********************************************************************************/
#include <Ethernet.h>
#include <SPI.h>
#include <JsonParser.h>
#include <Time.h>
#include <DateTimeStrings.h>
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

/*!
 *  Definiciones para el NFC Shield.
 */
#define IRQ (2)
#define RESET (3)

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
uint8_t uidLength;
uint8_t key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/*!
 *  Definición de Pines del LED RGB.
 */
const int PIN_RED = 5;
const int PIN_GREEN = 6;
const int PIN_BLUE = 7;

/*!
 *  Objeto Json para realizar el parse del 
 *  resultado del servidor.
 */
JsonParser<32> parser;

/*!
 *  Definición de objetos de la Arduino Ethernet.
 *   mac : dirección mac de la tarjeta Arduino.
 *   server : dirección del servidor.
 *   ip : IP estatica de la Arduino Ethernet.
 *   port : puerto de comunicación.
 *   client : cliente ethernet para comunicarse con el servidor.
 */
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "door-man.appspot.com";
IPAddress ip (192, 168, 0, 111);
int port = 80;
EthernetClient client;

/*********************************************************************************/
/*!
 *        Inicializacion de la Arduino Ethernet.
 *  1 : Configuración de los pines de salida para el indicador LED.
 *  2 : Inicializacion de la libreria Ethernet.
 *  3 : Test de conexión con el servidor.
 *  4 : Sincronización del tiempo de la unidad de escritura.
 *  5 : Inicialización de la libreria NFC.
 */
/*********************************************************************************/
void setup(){
  Serial.begin(9600);
  //1
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  //2
  if(Ethernet.begin(mac) == 0){
    Ethernet.begin(mac, ip);
    setColor(true, true, false); //Ilumina de color amarillo si establecio la dirección IP de forma estatica.
  } 
  else {
    setColor(false, true, false); //Ilumina el LED de color verde si la dirección IP es dinamica.
  }
  offLED(500);

  //3
  while(!client.connected()){
    client.connect(server, port);
  }
  if (client.connected()){
    setColor(false, true, false);
  }
  offLED(500);

  //4
  if (client.connected()){
    setRequestGET("/sync?board=Arduino1");  
    setTimeArduino(getResponseGET()); 
    setColor(false, true, false);   
  }
  offLED(500);

  //5
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata){
    setColor(true, false, false);
    while (true){}
  } else {
    nfc.SAMConfig();
    setColor(false, true, false);
  }
  offLED(500);
}

/**************************************************************************/
//                Ciclo de la Arduino Ethernet
/**************************************************************************/
void loop(){
  uint8_t success;
  
  
}



/******************************************************************************/
/*!
 Realiza la Petición GET a la URL asignada.
 
 @param URL :  Cadena de Texto de la URL de la petición al 
 servidor.
 */
/******************************************************************************/
void setRequestGET(String URL){
  client.println("GET " + URL + " HTTP/1.1");
  client.println("Host: door-man.appspot.com");   
  client.println("Connection: keep-alive");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println();
}

/******************************************************************************/
/*!
 Lee la respuesta realizada por el servidor y la devuelve como tipo cadena
 
 @return  :  String del JSON devuelto del servidor.
 */
/******************************************************************************/
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

/*****************************************************************/
/*!
 Función para Establecer la fecha hora de la tarjeta.
 
 @param JSON  :  Objeto JSON de tipo String para estbalecer la fecha
 y hora de la tarjeta Arduino.
 */
/*****************************************************************/
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


/*****************************************************************************/
/*!    
 *    Funcion para obtener la fecha 
 *
 *    @return : fecha en formato hh:mm:ss dd mmm yyyy
 */
/*****************************************************************************/
String getDate(){
  String date = "";
  date.concat(getDigits(hour()));
  date.concat(":");
  date.concat(getDigits(minute()));
  date.concat(":");
  date.concat(getDigits(second()));
  date.concat(" ");
  if (isAM()){
    date.concat("AM");
  } 
  else if(isPM()){
    date.concat("PM");
  }
  date.concat(" ");
  date.concat(dayStr(weekday()));
  date.concat(" ");
  date.concat(getDigits(day()));
  date.concat(" ");
  date.concat(monthShortStr(month()));
  date.concat(" ");
  date.concat(year()); 

  return date;
}

String getDigits(int digits){
  String strDigits = "";
  if(digits < 10){
    strDigits.concat("0");
    strDigits.concat(digits);
  } 
  else {
    strDigits.concat(digits);
  }
  return strDigits;
}

/*****************************************************************************/
/*!    
 Establece el color a mostrar, de acuerdo al estatus de la tarjeta
 
 OFF    (false, false, false)  
 RED    (true, false, false)    
 GREEN  (false, true, false)
 BLUE   (false, false, blue)  
 YELLOW (true, true, false)  
 CYAN   (false, true, true)
 PURPLE (true, false, true) 
 WHITE(true, true, true)
 
 @param red  :  valor del pin del LED Rojo.
 @param green  :  valor del pin del LED Verde.
 @param blue :  valor del pin del LED Azul.
 */
/*****************************************************************************/
void setColor(boolean red, boolean green, boolean blue){
  digitalWrite(PIN_RED, red);
  digitalWrite(PIN_GREEN, green);
  digitalWrite(PIN_BLUE, blue);
}

/*****************************************************************************/
/*!    
 Apaga el LED de la unidad de escritura y realiza una espera del tiempo
 necesario.
 
 @param wait : tiempo de espera.
 */
/*****************************************************************************/
void offLED(int wait){
  delay(wait);
  setColor(false, false, false);
  delay(wait);
}

