void receive_xbee_payload(){
  xbee_pkt = "";

  if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
    xbee.getResponse().getRx16Response(rx16);
    int length = rx16.getDataLength();
    int offset = rx16.getDataOffset();

    for (int i = offset ; i < (length-2); i++) {
      xbee_pkt += (char) rx16.getData(i+2);
    }
     xbee_pkt.trim();
    if (debug) {Serial.print("payload:"); Serial.println(xbee_pkt);}

  } else {
      xbee_pkt = "E"; //"Error=rcv_xbee_pkt";
  }
}
