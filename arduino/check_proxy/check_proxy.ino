
#include <SPI.h>
#include <Ethernet.h>

#define USB_BDR 38400
#define CRLF "\r\n"
#define STN_ID 1


byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x2B, 0x23 };
byte ip[] = {163, 143, 166, 109};
byte dom_ns[] = {163,143,1,100};
byte gateway[] = {163,143,166,1};
byte netmask[] = {255,255,255,0};
IPAddress server(163,143,166,120); 
char srv_name[] = "enformtk.u-aizu.ac.jp";
int srv_port = 8000;
IPAddress ipx_server(163,143,166,120);
char px_server[] = "enformtk.u-aizu.ac.jp";
int px_port = 3128;


String http_resp ="";
byte resetCounter = 0;
boolean http_resp_required = true;
boolean debug = true;
boolean dhcp_flag = false;

EthernetClient client;

void setup(){
  if (debug) {
    Serial.begin(USB_BDR);
    Serial.println("connecting...");
  }
  begin_ethernet(); 
}

void begin_ethernet(){
  if (dhcp_flag) {
    Ethernet.begin(mac);
  } else {
    Ethernet.begin(mac,ip,dom_ns,gateway,netmask);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  if (debug) {print_dhcp_address();}
}

void loop() { 
  
  client.stop();
 send_by_ethernet();
  delay(3000);
}
  
void send_by_ethernet(){  
  if (client.connect(srv_name, srv_port)) {
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

