void send_data_by_air(){
  xbee802.ON();
  delay(1000);
  value_temperature *=100.0;
  value_humidity *=100.0;
  temperature=(int) value_temperature;
  humidity=(int) value_humidity;
  Serial.println(temperature);
  Serial.println(humidity);
  sprintf(data,"T=%d&H=%d&B=%d",temperature,humidity,battery_level);
  paq_sent=(packetXBee*) calloc(1,sizeof(packetXBee)); 
  paq_sent->mode=UNICAST;
  paq_sent->packetID=0x52;
  paq_sent->opt=0x00; 
  xbee802.hops=0;
  xbee802.setPAN(PANID);
  xbee802.setMacMode(2); // set mode to 802.15.4 header
  xbee802.setOriginParams(paq_sent, MY_XB_ID, MY_TYPE);
  xbee802.setDestinationParams(paq_sent, MY_XB_DL, data, MY_TYPE, DATA_ABSOLUTE);
  xbee802.sendXBee(paq_sent);
  free(paq_sent); 
  paq_sent=NULL;
  delay(2000); 
  xbee802.OFF();
}



void print_via_USB(){
  USB.print("Temp: ");
  USB.println(value_temperature);
  USB.print("Hum: ");
  USB.println(value_humidity);
  USB.print("Battery level: ");
  USB.println(battery_level, DEC);
}
