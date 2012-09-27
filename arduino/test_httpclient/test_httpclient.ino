#include <SPI.h>
#include <Ethernet.h>
#include <HTTPClient.h>

//  Ethernet Variables
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x2B, 0x23 };
byte ip[] = {163, 143, 166, 109};
byte dom_ns[] = {163,143,1,100};
byte gateway[] = {163,143,166,1};
byte netmask[] = {255,255,255,0};

byte server[] ={ 163,143,166,120 };

EthernetClient client;

#define FEED_URI            "/feeds" // eg  "/v2/feeds/19321.csv"
  
boolean lastConnected = false;       // state of the connection last time through the main loop

// buffer to store send string
char data[40];


void setup() {
 
  // start the ethernet connection and serial port:
  Ethernet.begin(mac,ip,dom_ns,gateway,netmask);
  Serial.begin(9600);
  
  // give the ethernet module time to boot up:
  delay(1000);
  
  Serial.println("HTTPClient Library");
  Serial.println("ready");
}

void loop() {
  // read the analog sensor:
  int sensorReading = 55;  

  // if you're not connected, and Metro has expired then connect again and send data:
  if(!client.connected() ) {

    Serial.print("sending ");
    Serial.println(sensorReading);
  
                        // Pachube API_v2 STREAMS are updated with the following pattern:
                        // <streamID>,<value>                                                             (for a single STREAM within a FEED) 
                        // <streamID>,<value> NEWLINE <streamID>,<value> ... NEWLINE <streamID>,<value>   (for multiple STREAM within a FEED)
                        
    // use this for a single STREAM
    sprintf(data, "%d,%d ", 0, sensorReading);  // updates 1 STREAM: Stream 0 set to sensorReading  
    Serial.println(data);
    
    // create HTTPClient
    HTTPClient client( "enformtk.u-aizu.ac.jp", server, 8000);
      
    // FILE is the return STREAM type of the HTTPClient
    FILE* result = client.putURI( FEED_URI, NULL, data );
  
    int returnCode = client.getLastReturnCode();
    
    if (result!=NULL) {
      client.closeStream(result);  // this is very important -- be sure to close the STREAM
    } 
    else {
      Serial.println("failed to connect");
    }
    
    if (returnCode==200) {
      Serial.println("data uploaded");
    } 
    else {
      Serial.print("ERROR: Server returned ");
      Serial.println(returnCode);
    }
  }
  
  
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
  delay(5000);
}


