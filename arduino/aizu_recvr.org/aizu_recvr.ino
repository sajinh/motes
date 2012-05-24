
#include <SPI.h>
#include <Ethernet.h>

#include <XBee.h>

#define XBEE_BDR 38400
#define USB_BDR 38400

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();

byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x2B, 0x23 };
byte ip[] = {163, 143, 166, 109};
byte dns[] = {163,143,1,100};
byte gateway[] = {163,143,166,1};
byte netmask[] = {255,255,255,0};
IPAddress server(163,143,166,120); // Google
char serverName[] = "enformtk.u-aizu.ac.jp";
//char serverName[] = "www.google.co.jp";
int serverPort = 8000;

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;


void setup(){
  xbee.begin(XBEE_BDR);
  Serial.begin(USB_BDR);
   Ethernet.begin(mac,ip,dns,gateway,netmask);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
}

void loop() { 
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()){
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
      int len = rx16.getDataLength();
      int off = rx16.getDataOffset();
      int k = 0;
      String xbee_data = "";
      for (int i = off; i < len; i++){
        char val=rx16.getData(i);
        xbee_data += (char) rx16.getData(i);
        k += 1;
      }
      //Serial.print(xbee_data);
      send_by_ethernet();
    } else {
      Serial.println("Error");
    }
  }
  delay(10000);
}



