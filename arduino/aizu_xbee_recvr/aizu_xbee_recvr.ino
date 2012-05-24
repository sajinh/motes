

#include <XBee.h>

#define XBEE_BDR 38400
#define USB_BDR 38400


XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();
char xbee_pkt[30];


void setup(){
  xbee.begin(XBEE_BDR);
  delay(1000);
  Serial.begin(USB_BDR);
  Serial.println("Hello world");
  //send_by_ethernet();
}

void loop() { 
  xbee.readPacket();
  //Serial.println("reading Xbee");
  if (xbee.getResponse().isAvailable()){
    Serial.println("XBee ok");
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx16);
      int len = rx16.getDataLength();
      int off = rx16.getDataOffset();
      int k = 0;
      String xbee_data = "";
      for (int i = off; i < len; i++){
        char val=rx16.getData(i);
        xbee_data += (char) rx16.getData(i+1);
        xbee_pkt[k] = (char) rx16.getData(i);
        k += 1;
      }
      xbee_pkt[k] = 0;
      Serial.print(xbee_data);
      Serial.print("xbee_char : ");
      Serial.println(xbee_pkt);
      //send_by_ethernet();
    } else {
      Serial.println("Error");
    }
  }
  //delay(1000);
}


