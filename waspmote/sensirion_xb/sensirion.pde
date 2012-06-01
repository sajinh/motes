  
void switch_on_sensirion(){
  SensorAgr.setSensorMode(SENS_ON, SENS_AGR_SENSIRION);
  delay(SENSIRION_STARTUP_TIME);
}

void switch_off_sensirion(){
  SensorAgr.setSensorMode(SENS_OFF, SENS_AGR_SENSIRION);
}

void read_sensirion_temp_hum(){
  value_temperature = SensorAgr.readValue(SENS_AGR_SENSIRION, SENSIRION_TEMP);
  delay(SENSIRION_REST_TIME);
 value_humidity = SensorAgr.readValue(SENS_AGR_SENSIRION, SENSIRION_HUM);
}
