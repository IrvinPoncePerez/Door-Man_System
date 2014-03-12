#include <Ethernet.h>
#include <SPI.h>

//pines.
int pinEthernet = 5;
int pinServer = 6;
int pinRed = 7;

//Dirección MAC de la tarjeta Ethernet.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress server(127, 0, 0, 1); //IP del servidor local.
char server[] = "192.168.0.106";
//char server[] = {127, 0, 0, 1};
IPAddress ip(192, 168, 0 , 111); //IP de la tarjeta.
int port = 80;

//Inicializa la libreria EthernetClient
EthernetClient client;

void setup(){
  Serial.begin(9600);
  
  //Configuracion de Pines.
  pinMode(pinEthernet, OUTPUT);
  pinMode(pinServer, OUTPUT);
  pinMode(pinRed, OUTPUT);
  
  if (Ethernet.begin(mac) == 0){
    //Intentar la configuracion usando una IP estatica.
    Serial.println("Tarjeta Ethernet configurada, IP estatica.");
    Ethernet.begin(mac, ip);
    digitalWrite(pinEthernet, HIGH);
  } else {
    Serial.println("Tarjeta Ethernet configurada, usando DHCP.");
    digitalWrite(pinEthernet, HIGH);
  }
  
  Serial.println(Ethernet.localIP());
  delay(1000);
  Serial.println("Conectando...");
   
}

void loop(){
  
   while(!client.connected()){
    client.connect(server, port);
    digitalWrite(pinServer, LOW);  
  }
  digitalWrite(pinServer, HIGH);
  
  if (client.connected()) {
    Serial.println("Enviando POST");
    String data = "door=door1&battery=90&activity=outside_open&hour=11:35 am";
    client.println("POST /estatus HTTP/1.1");
    client.println("Host: 192.168.0.106");
    client.println("Connection: keep-alive");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println("Cache-Control: max-age=0");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Accept-Language: en-US,en;q=0.8");
    client.println();
    client.print(data);
    client.println();
    client.stop();
  }
  
}
