
/*  *  *  *  *  *  *  *  *  *
 * Controls the Sensirion sensor,
 * reads temperature and humidity
 * and sends data by XBee
 *
 * GPL Copyright (C) 2012 Saji N Hameed
 *
 *  *  *  *  *  *  *  *  *  */

#include "WProgram.h"
void setup();
void loop();
void sample_data();
void send_data_by_air();
void print_via_USB();
void switch_on_sensirion();
void switch_off_sensirion();
void read_sensirion_temp_hum();
float value_temperature = 0.0;
float value_humidity = 0.0;
char data[17];
unsigned int temperature;
unsigned int humidity;

uint8_t PANID[2]={0x11,0x11};
uint8_t battery_level=0;
packetXBee* paq_sent;


#define READ_TEMP_WAIT_TIME 1000
#define READ_HUM_WAIT_TIME  1000
#define MY_XB_ID "0001"
#define MY_XB_DL "0002"


void setup()
{  
  SensorAgr.setBoardMode(SENS_ON);
  xbee802.init(XBEE_802_15_4, FREQ2_4G, NORMAL);
  USB.begin();
}

void loop()
{
   RTC.ON(); 
   RTC.setTime("09:10:21:04:14:25:00"); 
   PWR.deepSleep("00:00:10:00",RTC_OFFSET,RTC_ALM1_MODE1,ALL_OFF); 
  if( intFlag & RTC_INT ) 
  { 
    sample_data();
    intFlag &= ~(RTC_INT); 
  } 
  PWR.clearInts();
} 


void sample_data(){
  switch_on_sensirion();
  USB.println("Sensor On");
  read_sensirion_temp_hum();
  switch_off_sensirion();
  USB.println("Sensor Off");
  battery_level=PWR.getBatteryLevel();
  print_via_USB();
  send_data_by_air();
}


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

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

