
#include <SPI.h>
#include <Ethernet.h>
#define USB_BDR 38400
#define CRLF "\r\n"


byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x2B, 0x23 };
byte ip[] = {163, 143, 166, 109};
byte dns[] = {163,143,1,100};
byte gateway[] = {163,143,166,1};
byte netmask[] = {255,255,255,0};
char serverName[] = "enformtk.u-aizu.ac.jp";
int serverPort = 8000;
boolean lastConnected = false;
boolean debug = true;
EthernetClient client;
byte resetCounter = 0;


void setup(){
   if (debug) {
  Serial.begin(USB_BDR);
  Serial.println("connecting...");
   }
   Ethernet.begin(mac,ip,dns,gateway,netmask);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
}

void loop() { 
 

  if (!client.connected() && lastConnected) {
    if (debug) {
      Serial.println();
      Serial.println("disconnecting.");
    }
    client.stop();
  }
  
  send_by_ethernet();
  delay(10000);
  lastConnected = client.connected();
}



