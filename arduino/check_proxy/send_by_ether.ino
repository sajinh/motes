void put_http_data(){
  client.print("POST ");
  //client.print(srv_name);
  //client.print(":");
  //client.print(srv_port);
  client.println("/feeds HTTP/1.0");
  //client.println("Host: enformtk.u-aizu.ac.jp");
  client.print("Content-Length: ");
  client.println("0");
  client.println("Content-Type: text/plain");
  client.println("Connection: close");
  client.println();
  //client.println();
  delay(500);
  resetCounter = 0;
  if (http_resp_required){
    http_resp=read_http_response();
  }
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
  begin_ethernet();
}
