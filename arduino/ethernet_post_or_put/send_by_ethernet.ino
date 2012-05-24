void send_by_ethernet(){
  String data="T=5555&H=3333&B=88";
  if (client.connect(serverName, serverPort)) {
    put_http_data(data);
    delay(500);
    resetCounter = 0;
    if (debug) {
      Serial.println("connected");
      Serial.println(read_http_response());
    }
  } 
  else {
    if (debug) {
      Serial.println("connection failed");
    }
    resetCounter++;
     if (resetCounter >=5 ) {resetEthernetShield();}
  }

  if (debug) {Serial.println("Stop");}
  client.stop();
} 

void put_http_data(String data){
  client.println("PUT /update HTTP/1.1");
  client.println("Host: cpr-224b.u-aizu.ac.jp");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Connection: close");
  client.print(CRLF);
  client.println(data);
}

String read_http_response(){
  String http_response="";
  while (client.available()) {
    char c = client.read();
    http_response += c;
  }
  return(http_response);
}

void resetEthernetShield()
{
  if (debug) {
    Serial.println("Resetting Ethernet Shield.");   
    Serial.println();
  }
  client.stop();
  delay(1000);
  
  Ethernet.begin(mac, ip, dns, gateway, netmask);
  delay(1000);
}
