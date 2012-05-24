
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


