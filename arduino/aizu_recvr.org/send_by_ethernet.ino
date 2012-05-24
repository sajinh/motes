void send_by_ethernet(){
  String payload="";
  int length = rx16.getDataLength();
  int offset = rx16.getDataOffset();
  for (int i = offset; i < length; i++){
    payload += (char) rx16.getData(i);
  }
  payload.trim();
  String data="T=5555&H=3333&B=9999";
  //data = "GET /"+data+" HTTP/1.0";
  Serial.print(payload);
    if (client.connect(serverName, 8000)) {
      Serial.println("connected");
  
      client.println("POST /update HTTP/1.1");

    //client.print("Host: enformtk.u-aizu.ac.jp\n");
    client.print("Connection: close\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(data.length());
    client.println();
    client.println(); 
    client.print(data);
    client.println("");
    //  client.println(data);
    Serial.print("client.status = ");
   Serial.println(client.status(),DEC);
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
    Serial.print("client.status = ");
   Serial.println(client.status(),DEC);
  }
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  client.stop();
} 
