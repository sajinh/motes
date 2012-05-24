  
void switch_on_sensirion(){
  SensorAgr.setSensorMode(SENS_ON, SENS_AGR_SENSIRION);
}

void switch_off_sensirion(){
  SensorAgr.setSensorMode(SENS_OFF, SENS_AGR_SENSIRION);
}

void read_sensirion_temp_hum(){
  delay(READ_TEMP_WAIT_TIME);
  value_temperature = SensorAgr.readValue(SENS_AGR_SENSIRION, SENSIRION_TEMP);
  delay(READ_HUM_WAIT_TIME);
 value_humidity = SensorAgr.readValue(SENS_AGR_SENSIRION, SENSIRION_HUM);
}
