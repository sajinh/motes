
/*  *  *  *  *  *  *  *  *  *
 * Controls the Sensirion sensor,
 * reads temperature and humidity
 * and sends data by XBee
 *
 * GPL Copyright (C) 2012 Saji N Hameed
 *
 *  *  *  *  *  *  *  *  *  */

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


