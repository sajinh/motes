
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


#define SENSIRION_STARTUP_TIME 100
#define SENSIRION_REST_TIME  1000
#define XBEE_OFF_ON_WAIT 500
#define MY_XB_ID "0001"
#define MY_XB_DL "0002"


void setup()
{  
  // Switch off unecessary modules
  GPS.setMode(GPS_OFF);
  PWR.setSensorPower(SENS_3V3, SENS_OFF);
  PWR.setSensorPower(SENS_5V, SENS_OFF);
  PWR.closeI2C();
  PWR.setLowBatteryThreshold(3.0);
  SensorAgr.setBoardMode(SENS_ON);
  xbee802.init(XBEE_802_15_4, FREQ2_4G, NORMAL);
  RTC.ON(); 
  RTC.setTime("09:10:21:04:14:25:00"); 
  RTC.OFF();
 // USB.begin();
}

void loop()
{
   PWR.deepSleep("00:00:10:00",RTC_OFFSET,RTC_ALM1_MODE2,ALL_OFF); 
  if( intFlag & RTC_INT ) 
  { 
    Utils.blinkLEDs(1000);
    Utils.blinkLEDs(1000);
    Utils.blinkLEDs(1000);
    sample_data();
    intFlag &= ~(RTC_INT); 
  } 
  PWR.clearInts();
} 


void sample_data(){
  switch_on_sensirion();
 // USB.println("Sensor On");
  read_sensirion_temp_hum();
  switch_off_sensirion();
  battery_level=PWR.getBatteryLevel();
  send_data_by_air();
}


void send_data_by_air(){
  XBee.setMode(XBEE_ON);
  xbee802.ON();
  delay(XBEE_OFF_ON_WAIT);
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
  delay(XBEE_OFF_ON_WAIT); 
  xbee802.OFF();
  XBee.setMode(XBEE_OFF);
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

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

