
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <XBee.h>
#include <Time.h>

#define XBEE_BDR 38400
#define USB_BDR 38400
#define CRLF "\r\n"
#define STN_ID 1

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();

byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x2B, 0x23 };
byte ip[] = {163, 143, 166, 109};
byte dns[] = {163,143,1,100};
byte gateway[] = {163,143,166,1};
byte netmask[] = {255,255,255,0};
IPAddress server(163,143,166,120); 
char serverName[] = "enformtk.u-aizu.ac.jp";
int serverPort = 8000;

unsigned int localPort = 8888;      // local port to listen for UDP packets
IPAddress timeServer(133, 243, 238, 163); // time.nist.gov NTP server
const int NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
time_t prevDisplay = 0; // when the digital clock was displayed

String xbee_pkt="";
String http_resp ="";
byte resetCounter = 0;
boolean http_resp_required = false;
boolean debug = true;
boolean dhcp_flag = true;

EthernetClient client;

void setup(){
  xbee.begin(XBEE_BDR);
  if (debug) {
    Serial.begin(USB_BDR);
    Serial.println("connecting...");
  }
  begin_ethernet(); 
  begin_timer();
}

void begin_ethernet(){
  if (dhcp_flag) {
    Ethernet.begin(mac);
  } else {
    Ethernet.begin(mac,ip,dns,gateway,netmask);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  if (debug) {print_dhcp_address();}
}

void loop() { 
  
  client.stop();
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()){
    receive_xbee_payload();
    send_by_ethernet();
  }
  delay(1000);
}
  
void send_by_ethernet(){  
  if (client.connect(serverName, serverPort)) {
    put_http_data();
    if (debug) {Serial.println(http_resp);}
  } 
  else {
    if (debug) {
      Serial.println("connection failed");
    }
    resetCounter++;
     if (resetCounter >=5 ) {resetEthernetShield();}
  }
  client.stop();
}

void print_dhcp_address(){
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
}



