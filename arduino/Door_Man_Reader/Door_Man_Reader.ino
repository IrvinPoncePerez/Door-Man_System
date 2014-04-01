/*!
 *  @file    Door_Man_Reader.ino
 *  @autor   Irvin Ponce Pérez
 *
 *  Sketch de la unidad de lectura de la cerradura de Door-Man System.
 *
 *  Encargado de detectar las tarjetas Mifare Classic y realizar el desbloqueo
 *  de la cerradura.
 *
 *  La unidad de Lectura es la encargada de realizar el envió de la actividad
 *  reaizada en la puerta, leer los cambios en los optointeruptores y enviar
 *  esa información a la unidad de escritura.
 *
 *  Conexion de Pines RFID-RC522.
 *
 *  SDA   : Pin 10
 *  SCK   : Pin 13
 *  MOSI  : Pin 11
 *  MISO  : Pin 12
 *  IRQ   :
 *  GND   :  GND
 *  RST   : Pin 9
 *  3.3V  :  3.3V
 */
 
 #include <SPI.h>
 #include <MFRC522.h>
 #include <Servo.h>
 
 /*!
  *   Definiciones para el lector RFID MF-RC522
  */

 #define SDA_PIN 10
 #define RST_PIN 9
 
 MFRC522 mfrc522(SDA_PIN, RST_PIN);
 MFRC522::MIFARE_Key key = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
 const byte TRAILER_BLOCK = 7;
 const byte DOOR_BLOCK = 4;
 const byte TYPE_BLOCK = 5;
 
 /*!
  *  Definición de Pines.
  */
 const int PIN_RED = 2;
 const int PIN_GREEN = 3;
 const int PIN_BLUE = 4;
 const int PIN_SERVO = 8; 
 
 /*!
  *  Definición de Variables de las Tarjetas.
  */
 String door;
 String type;
 
 const String DOOR = "door10";
 const String TYPE = "Cliente";
 
 /*!
  * Otras declaraciones.
  */
 Servo servo;
 
 /*****************************************************************/
 /*!
  *      Inicialización de la Arduino UNO
  *  1  :  Inicialización de los pines del LED RGB
  *  2  :  Inicialización del lector RFID-RC522.
  *  3  :  Reseteo del Servo a 0 grados.
  */ 
 /*****************************************************************/
void setup(){
  Serial.begin(9600);
  
  servo.attach(PIN_SERVO);
  //1
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  //2
  SPI.begin();
  mfrc522.PCD_Init();
  setColor(false, true, false);
  offLED(500);
  
  //3
  setServo(0);
}

/**************************************************************************/
//                Ciclo de la Arduino Ethernet
//  1 : Detección y escritura de las tarjetas.
/**************************************************************************/
void loop(){

  //1
  if (mfrc522.PICC_IsNewCardPresent()){
    if (mfrc522.PICC_ReadCardSerial()){
      setColor(true, true, true);
      delay(500);
      if (readTag()){
        if (DOOR.equals(door)){
          setColor(false, true, false);
          setServo(180);
        } else {
          setColor(true, false, false);
          setServo(0);
        }
      } else {
        setColor(true, true, false);
      }
      mfrc522.PCD_StopCrypto1();
    }
    offLED(500);
  }
}

/**************************************************************************/
/*!
 *     Desbloquea el pasador girando el servo en los grados dados.
 */
/**************************************************************************/
void setServo(int degree){
  servo.attach(PIN_SERVO);
  servo.write(degree);
  delay(500);
  servo.detach();
}

/**************************************************************************/
/*!
 *     Lectura de la tarjeta
 */
/**************************************************************************/
boolean readTag(){
   byte authentication = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, TRAILER_BLOCK, &key, &(mfrc522.uid));
   
   if(authentication != MFRC522::STATUS_OK){
     authentication = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, TRAILER_BLOCK, &key, &(mfrc522.uid));
     if(authentication != MFRC522::STATUS_OK){
       return false;
     } else {
       return readed();
     }
   } else {
     return readed();
   }
}

/**************************************************************************/
/*!
 *     Realiza la lectura de los bloques 4 y 5 y asigna el valor recuperado
 *     a las variables door y type
 *       @return  : Booleano correspondiente a la lectura satisfactoria de 
                    los dos bloques leidos. 
 */
/**************************************************************************/
boolean readed(){
  
   byte readDoor;
   byte readType;
   byte buffer[18];
   byte size = sizeof(buffer);
   
   door = "";
   type = "";
   
   readDoor = mfrc522.MIFARE_Read(DOOR_BLOCK, buffer, &size);
   if (readDoor == MFRC522::STATUS_OK){
     for (int i = 0; i < 16; i++){
       if (buffer[i] != 0){
         char c = buffer[i];
         door.concat(c);
       }
     }
     Serial.println();
   }
      
   readType = mfrc522.MIFARE_Read(TYPE_BLOCK, buffer, &size);
   if (readType == MFRC522::STATUS_OK){
     for (int i = 0; i < 16; i++){
       if (buffer[i] != 0){
         char c = buffer[i];       
         type.concat(c);
       }
     }
     Serial.println();
   }
   
   if (readDoor == MFRC522::STATUS_OK && readType == MFRC522::STATUS_OK){
     return true;
   } else {
     door = "";
     type = "";
     return false;
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
 
