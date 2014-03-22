#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

//---------------------------------------------------------
//    Definiciones para NFC Shield.
//---------------------------------------------------------
#define IRQ (2)
#define RESET (3)

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
uint8_t uidLength;
uint8_t keya[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//---------------------------------------------------------
//      Definicion de Pines.
//---------------------------------------------------------
const int RED = 5;
const int GREEN = 6;
const int BLUE = 7;

//---------------------------------------------------------
//      DEfinición de Variables
//---------------------------------------------------------


void setup(){
  Serial.begin(9600);
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if(! versiondata){
    setColor(true, false, false);
  } else {
    setColor(false, true, false);
  }
  
  nfc.SAMConfig();
  
  delay(1000);
  setColor(false, false, false);
}

void loop(){
  uint8_t success;
  
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success){
    setColor(false, false, true);
    delay(1000);
    setColor(false, false, false);
    
    writeData(4, "door1");
    writeData(5, "supervisor");
    
  } 
  else if(!success){
    setColor(false, false, false);
  }
  
  delay(1000);
}

//*****************************************************************************
//    Establece el color a mostrar, de acuerdo al estatus de la tarjeta
//  OFF(false, false, false)  RED(true, false, false)    GREEN(false, true, false)
//  BLUE(false, false, blue)  YELLOW(true, true, false)  CYAN(false, true, true)
//  PURPLE(true, false, true) WHITE(true, true, true)
//*****************************************************************************
void setColor(boolean red, boolean green, boolean blue){
  digitalWrite(RED, red);
  digitalWrite(GREEN, green);
  digitalWrite(BLUE, blue);
}

void writeData(uint32_t block, String data){
  uint8_t success;
  uint8_t dataArray[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  for (int i = 0; i < data.length(); i++){
    dataArray[i] = data[i];
  }
  
  success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, block, 0, keya);
  if (success){
    setColor(true, true, false);
    success = nfc.mifareclassic_WriteDataBlock(block, dataArray);
    if (success){
      setColor(true, true, true);
      delay(500);
      setColor(false, false, false);
    }
    else if(!success){
      setColor(true, false, false);
      delay(500);
      setColor(false, false, false);
    }
  }
  else if(!success){
    setColor(true, false, false);
  }
  delay(1000);
  setColor(false, false, false);
}
