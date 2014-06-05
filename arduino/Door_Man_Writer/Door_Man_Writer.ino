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
#include <Wire.h>
#include <Adafruit_NFCShield.h>
#include <Time.h>

/*!
 *  Definiciones para el NFC Shield.
 */
#define IRQ (2)
#define RESET (3)

Adafruit_NFCShield nfc(IRQ, RESET);

uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
uint8_t uidLength;
uint8_t key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/*!
 *  Definición de Pines.
 */
const int PIN_RED = 7;
const int PIN_GREEN = 5;
const int PIN_BLUE = 6;

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
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "192.168.0.106";
//char server[] = "door-man.appspot.com";

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
 *  6 : Configuracion del receptor de Datos.
 */
/*********************************************************************************/
void setup(){
  
  //1
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  setColor(false, false, false);
   
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
  while (!client.connected()){
    client.connect(server, port);
    setColor(true, false, false);
  }
  if (client.connected()){
    setColor(false, true, false);
  }
  offLED(500);

  //4
  if (client.connected()){
    doGET("/sync?board=Arduino1");  
    setTimeArduino(getResponse()); 
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
   
  //6
  Serial.begin(9600);
  
}

/**************************************************************************/
//                Ciclo de la Arduino Ethernet
//  1 : Detección y escritura de las tarjetas.
//  2 : Recepcion de actividades de la cerradura.
/**************************************************************************/
void loop(){
  
   //1
  uint8_t success;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success){
    setColor(false, false, true);
    offLED(200);
    doGET("/writecard?writer=Arduino1");
    String userId = writeCard(getResponse());

    if (userId.length() > 0){
      doPOST("/writecard", "function=write&writer=Arduino1&userId=" + userId);
      client.stop();
    }
    offLED(500);
  }
  else if (!success){
    offLED(0);
  }  

  //2
  if(Serial.available()){
    char c = Serial.read();
    if (c == '#'){
      receiveMessage();
    }
  }
  
}

/******************************************************************************/
/*!
 Realiza la Petición GET a la URL asignada.
 
 @param URL :  Cadena de Texto de la URL de la petición al 
 servidor.
 */
/******************************************************************************/
void doGET(String URL){
  while(!client.connected()){
    client.connect(server, port);
  }
  
  if (client.connected()){
    client.println("GET " + URL + " HTTP/1.1");
    client.println("Host: 192.168.0.106");   
    client.println("Connection: keep-alive");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println();
  }
}

/******************************************************************************/
/*!
 Realiza la Petición POST a la URL asignada.
 
 @param URL :  Cadena de Texto de la URL de la petición al 
 servidor.
 */
/******************************************************************************/
void doPOST(String URL, String data){

  while(!client.connected()){
    client.connect(server, port);
  }
  
  if (client.connected()){
    client.println("POST " + URL + " HTTP/1.1");
    client.println("Host: 192.168.0.106");   
    client.println("Connection: keep-alive");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println();
    client.print(data);
    client.println();
  }  
}

/******************************************************************************/
/*!
 *    Metodo para recibir la actividad de la cerradura.
 *
 *      @return String del mensaje recibido.
 */
/******************************************************************************/
void receiveMessage(){
  String message = "";
  String hour = getHour();
  
  while (true){
    if (Serial.available()){
      char c = Serial.read();
      if (c == '$'){
        break;
      } else {
        message.concat(c);
      }
    } else {
      break;
    }
  }
  
  message += "/" + hour;
  
  doPOST("/estatus", "status=" + message);
  client.stop();

}

/*****************************************************************************/
/*!    
 *    Funcion para obtener la hora de la actividad. 
 *
 *    @return : Hora hh:mm am o pm.
 */
/*****************************************************************************/
String getHour(){
  String date = "";
  
  date.concat(getDigits(hour()));
  date.concat(":");
  date.concat(getDigits(minute()));
  date.concat(" ");
  if (isAM()){
    date.concat("AM");
  } 
  else if(isPM()){
    date.concat("PM");
  }
  
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

/*****************************************************************/
/*!
 *    Obtiene los valores del String JSON para ser escrito en 
 *    los bloques de la tarjeta.
 *
 *    @param JSON : String JSON de el response.
 *    @return String : String del userId para realizar el POST
 *                     y el servidor pueda notficar al canal adecuado.
 */
/*****************************************************************/
String writeCard(String JSON){

  if (JSON.equals("")){
    setColor(true, false, true);
    offLED(200);
    return "";
  } else {
    int length = JSON.length();  
    char buffer[length];
    JSON.toCharArray(buffer, length +1); 

    //JSON Parse.
    JsonHashTable objJson = parser.parseHashTable(buffer);
    if (objJson.success()){
      writeData(objJson.getString("doorId"), 4);
      writeData(objJson.getString("card"), 5);
    } else {
      setColor(true, false, false);
      offLED(200);
    }
    return objJson.getString("userId");
  }
  
}

/*****************************************************************************/
/*!    
 *    Escribe los datos pasados en el bloque indicado, notifica enciendo el 
 *    LED de color rojo si no se pueden escribir los datos en la tarjeta o 
 *    blanco si la escritura se realizo correctamente.
 *
 *      @param value : datos a escribir en el bloque.
 *      @param block : bloque en el que seran escrito los datos.
 */
/*****************************************************************************/
void writeData(String value, uint8_t block){
  uint8_t success;
  uint8_t data[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < value.length(); i++){
    data[i] = value[i];
  }
  
  success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, block, 0 , key);
  if (success){
    success = nfc.mifareclassic_WriteDataBlock(block, data);
    if (success){
      setColor(true, true, true);
    } else if (!success){
      setColor(true, false, false);
    }
    offLED(300);
  } else if (!success){
    setColor(true, false, false);
    offLED(300);
  }

}

/******************************************************************************/
/*!
 Lee la respuesta realizada por el servidor y la devuelve como tipo cadena
 
 @return  :  String del JSON devuelto del servidor.
 */
/******************************************************************************/
String getResponse(){
  while (!client.available()) {
  }

  String response = "";
  while (client.available()) {
    char character = client.read();
    response.concat(character);
  }    
  client.stop();

  int index = response.indexOf("{");
  response = response.substring(index);
  Serial.println(response);
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
  if (objJson.success()){
    int Year = objJson.getLong("year");
    int Month = objJson.getLong("month");
    int Day = objJson.getLong("day");
    int Hour = objJson.getLong("hour");
    int Minute = objJson.getLong("minute");
    int Second = objJson.getLong("second");
    setTime(Hour, Minute, Second, Day, Month, Year);

    setColor(false, true, false);
  } else {
    setColor(true, true, false);
  }
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
  digitalWrite(PIN_RED, !red);
  digitalWrite(PIN_GREEN, !green);
  digitalWrite(PIN_BLUE, !blue);
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
