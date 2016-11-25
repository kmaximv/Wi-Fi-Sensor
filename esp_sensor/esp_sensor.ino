#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#include "ESP8266WebServer.h"
extern "C" {
#include "user_interface.h"
}

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "json_config.h"
#include "ArduinoJson.h"

#include "user_config.h"

#include "SimpleTimer.h"
SimpleTimer timer;

#if defined(NTP_ON)
  #include "NTPClient.h"
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP);
#endif

#if defined(UART_ON)
  #include "MY_ESP_UART.h"
  Espuart Uart;
#endif

JsonConf JConf;

#if defined(DHT_ON)
  #include <DHT.h>
  // Uncomment the type of sensor in use:
  //#define DHTTYPE           DHT11     // DHT 11 
  #define DHTTYPE           DHT22     // DHT 22 (AM2302)
  //#define DHTTYPE           DHT21     // DHT 21 (AM2301)
  DHT dht(atoi(JConf.dht_pin), DHTTYPE);
#endif

#if defined(DS18X20_ON)
  #include "OneWire.h"
  OneWire ds(DS18X20_PIN);
#endif


#if defined(BH1750_ON)
  #include "BH1750.h"
  BH1750 lightSensor;
#endif

#if defined(BME280_ON)
  #include "SparkFunBME280.h"
  BME280 bmeSensor;
#endif

#if defined(SHT21_ON)
  #include "HTU21D.h"
  HTU21D myHTU21D;
#endif

#if defined(PZEM_ON)
  #include "PZEM004T.h"
  PZEM004T pzem(&Serial);
  IPAddress ip_pzem(192,168,1,1);
  float coil_ratio = 1.84; // Если используем разные катушки, подбираем коэффициент
  enum PZEM_ENUM {PZEM_VOLTAGE, PZEM_CURRENT, PZEM_POWER, PZEM_ENERGY};
  PZEM_ENUM pzem_current_read = PZEM_VOLTAGE;
  enum PZEM_RESET_ENUM {PZEM_STAGE1, PZEM_STAGE2, PZEM_STAGE3, PZEM_STAGE4};
  PZEM_RESET_ENUM pzem_reset_stage = PZEM_STAGE1;
#endif

WiFiUDP portUDP;                      // UDP Syslog


ADC_MODE(ADC_VCC);
float voltage_float;

String network_html;          // Список доступных Wi-Fi точек

ESP8266WebServer WebServer(80);

WiFiClient espClient;


Adafruit_MQTT_Client mqtt = Adafruit_MQTT_Client(&espClient, JConf.mqtt_server, atoi(JConf.mqtt_port), JConf.mqtt_user, JConf.mqtt_pwd);

Adafruit_MQTT_Publish pubTopicLightType = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicLightType2 = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicMotionSensor = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicMotionSensorTimer = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicMotionSensorTimer2 = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);

Adafruit_MQTT_Publish pubTopicLux = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicTemperature = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicHumidity = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicPressure = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);

Adafruit_MQTT_Publish pubTopicPzemVoltage = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicPzemCurrent = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicPzemPower = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicPzemEnergy = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);

Adafruit_MQTT_Publish pubTopicFreeMemory = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicUptime = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicVersion = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicIp = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicMac = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);

#ifdef DS18X20_ON
Adafruit_MQTT_Publish pubTopic_ds1 = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopic_ds2 = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopic_ds3 = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopic_ds4 = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopic_ds5 = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
#endif //DS18X20_ON


Adafruit_MQTT_Subscribe subTopicMotionSensorTimer = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);
Adafruit_MQTT_Subscribe subTopicMotionSensorTimer2 = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);

Adafruit_MQTT_Subscribe subTopicLightType = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);
Adafruit_MQTT_Subscribe subTopicLightType2 = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);

Adafruit_MQTT_Subscribe subTopicUptime = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);

Adafruit_MQTT_Subscribe subTopicPzemReset = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);

struct FADING_T
{
  int pin;
  int cycleNow;
  int cycleEnd;
  unsigned long timerFade;
  int delayFade;
}fading[2] = {
   {atoi(JConf.light_pin),0,0,0,20},
   {atoi(JConf.light2_pin),0,0,0,20}
};

 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////         ROOT 

/*
static char* floatToChar(float charester)
{
 dtostrf(charester, 1, 0, value_buff);
 return value_buff;
}
*/

bool MqttConnect();



void LightControl() {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: LightControl Start");

  String AUTO = "AUTO";
  String ON = "ON";
  String OFF = "OFF";

  if (lightState == ON){
    PWMChange(0, 1023);
  } else if (lightState == OFF){
    PWMChange(0, 0);
  } else if (lightState == AUTO && motionDetect == true && luxString.toInt() < atoi(JConf.lighton_lux)){
    PWMChange(0, 1023);
    lightOffTimer = millis();
  } else if (lightState == AUTO && motionDetect == false && fading[0].cycleEnd != 0){
    if (millis() - lightOffTimer >= atoi(JConf.lightoff_delay) * 60UL * 1000UL){
      PWMChange(0, 0);
    }
  }

  if (lightState2 == ON){
    PWMChange(1, 1023);
  } else if (lightState2 == OFF){
    PWMChange(1, 0);
  } else if (lightState2 == AUTO && motionDetect == true && luxString.toInt() < atoi(JConf.light2on_lux)){
    PWMChange(1, 1023);
    lightOffTimer2 = millis();
  } else if (lightState2 == AUTO && motionDetect == false && fading[1].cycleEnd != 0){
    if (millis() - lightOffTimer2 >= atoi(JConf.light2off_delay) * 60UL * 1000UL){
      PWMChange(1, 0);
    }
  }

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: LightControl load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



#ifdef BH1750_ON
void GetLightSensorData()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: GetLightSensorData Start");

  luxString = String(lightSensor.readLightLevel());
    
  snprintf_P(log, sizeof(log), PSTR("GetLightSensorData: Lux: %s"), luxString.c_str());
  addLog(LOG_LEVEL_INFO, log);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: GetLightSensorData load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}
#endif



#ifdef BME280_ON
void GetBmeSensorData()
{ 
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: GetBmeSensorData Start");

  temperatureString = String(bmeSensor.readTempC());
  snprintf_P(log, sizeof(log), PSTR("GetBmeSensorData: Temperature: %s C"), temperatureString.c_str());
  addLog(LOG_LEVEL_INFO, log);

  pressureString = String(bmeSensor.readFloatPressure()/133.3F);
  snprintf_P(log, sizeof(log), PSTR("GetBmeSensorData: Pressure: %s"), pressureString.c_str());
  addLog(LOG_LEVEL_INFO, log);


  humidityString = String(bmeSensor.readFloatHumidity());
  snprintf_P(log, sizeof(log), PSTR("GetBmeSensorData: Humidity: %s %"), humidityString.c_str());
  addLog(LOG_LEVEL_INFO, log);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: GetBmeSensorData load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}
#endif



#ifdef SHT21_ON
void GetSHT21SensorData(){

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: GetSHT21SensorData Start");

  myHTU21D.setResolution(HTU21D_RES_RH8_TEMP12);

  temperatureString = String(myHTU21D.readTemperature());
  snprintf_P(log, sizeof(log), PSTR("GetSHT21SensorData: Temperature: %s C"), temperatureString.c_str());
  addLog(LOG_LEVEL_INFO, log);

  humidityString = String(myHTU21D.readCompensatedHumidity());
  snprintf_P(log, sizeof(log), PSTR("GetSHT21SensorData: Humidity: %s %"), humidityString.c_str());
  addLog(LOG_LEVEL_INFO, log);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: GetSHT21SensorData load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}
#endif



#ifdef DHT_ON
void GetDhtSensorData()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: GetDhtSensorData Start");

  float humidityData = dht.readHumidity();
  float temperatureData = dht.readTemperature();

  if (isnan(humidityData) || isnan(temperatureData)) {
    addLog_P(LOG_LEVEL_ERROR, "GetDhtSensorData: Error reading DHT!");
    return;
  } else {
    temperatureString = String(temperatureData);
    snprintf_P(log, sizeof(log), PSTR("GetDhtSensorData: Temperature: %s C"), temperatureString.c_str());
    addLog(LOG_LEVEL_INFO, log);

    humidityString = String(humidityData);
    snprintf_P(log, sizeof(log), PSTR("GetDhtSensorData: Humidity: %s %"), humidityString.c_str());
    addLog(LOG_LEVEL_INFO, log);

  }

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: GetDhtSensorData load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}
#endif


#ifdef DS18X20_ON
void SearchDS18x20Sensors() {

  if (searchDsSensorDone){
    return;
  }

  char log[LOGSZ];

  if (!ds.search(dsData[currentDsSensor].address)) {
    searchDsSensorDone = true;
    currentDsSensor = 0;
    ds.reset_search();
    MqttInitDS();
    return;
  } else if (!searchDsSensorDone) {
    findDsSensors ++;
  }

  if (OneWire::crc8(dsData[currentDsSensor].address, 7) != dsData[currentDsSensor].address[7]) {
    addLog_P(LOG_LEVEL_ERROR, "DS Sensor Address CRC is not valid!");
    return;
  }

  String addr = "";
  for (size_t i = 0; i < 8; i++)
  {
    addr += String(dsData[currentDsSensor].address[i], HEX);
  }
  dsData[currentDsSensor].addressString = addr;

  switch (dsData[currentDsSensor].address[0])
  {
    case 0x10:
        dsData[currentDsSensor].type = DS18S20;
        break;
    case 0x28:
        dsData[currentDsSensor].type = DS18B20;
        break;
    case 0x22:
        dsData[currentDsSensor].type = DS1822;
        break;
    default:
        dsData[currentDsSensor].type = UNKNOWN;
        return;
  }
  currentDsSensor ++;

  snprintf_P(log, sizeof(log), PSTR("DS: currentDsSensor:%d  findDsSensors:%d"), currentDsSensor, findDsSensors);
  addLog(LOG_LEVEL_INFO, log);
  SearchDS18x20Sensors();
}



void GetDS18x20SensorData(){
  byte i;
  byte data[12];
  byte address[8];

  if (!flag_ds_sensor_read_delay){
    if (findDsSensors == 0){
      addLog_P(LOG_LEVEL_ERROR, "DS Sensors Not Found!");
    }
    flag_ds_sensor_read_delay = true;
    ds.reset();
    ds.select(dsData[currentDsSensor].address);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end
    timer.setTimeout(800, GetDS18x20SensorData);
    return;
  } else {
    flag_ds_sensor_read_delay = false;
  }

  ds.reset();
  ds.select(dsData[currentDsSensor].address);
  ds.write(0xBE);         // Read Scratchpad

  for (i = 0; i < 9; i++)
  {           // we need 9 bytes
      data[i] = ds.read();
  }

  if (OneWire::crc8(data, 8) != data[8]) {
    addLog_P(LOG_LEVEL_ERROR, "DS Sensor: Data CRC is not valid!");
    return;
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (dsData[currentDsSensor].type == DS18S20) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else if (dsData[currentDsSensor].type == DS18B20 || dsData[currentDsSensor].type == DS1822) {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  } else {
    addLog_P(LOG_LEVEL_ERROR, "Device is not a DS18x20 family device!");
  }

  //float celsius = (float) raw / 16.0;

  dsData[currentDsSensor].dsTemp = String((float) raw / 16.0);

  dsDataPrint();
  if (findDsSensors == currentDsSensor+1){
    currentDsSensor = 0;
  } else {
    currentDsSensor ++;
  }
}



void dsDataPrint(){

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: dsDataPrint Start");

  String dsType = " ";

    switch (dsData[currentDsSensor].address[0])
    {
        case 0x10:
            dsType = "DS18S20";  // or old DS1820
            break;
        case 0x28:
            dsType = "DS18B20";
            break;
        case 0x22:
            dsType = "DS1822";
            break;
        default:
            addLog_P(LOG_LEVEL_ERROR, "Device is not a DS18x20 family device!");
            return;
    }

  snprintf_P(log, sizeof(log), PSTR("DS type:%s  addr:%s  temp:%sC"), dsType.c_str(), dsData[currentDsSensor].addressString.c_str(), dsData[currentDsSensor].dsTemp.c_str());
  addLog(LOG_LEVEL_INFO, log);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: dsDataPrint load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}
#endif //DS18X20_ON



#if defined(PZEM_ON)
bool GetPzemData(float data, String *val) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: GetPzemData Start");

  if (data < 0.0){
    addLog_P(LOG_LEVEL_ERROR, "GetPzemData: Error reading data!");
    pzem.setAddress(ip_pzem);
    pzem.setReadTimeout(500);
    return false;
  } else if (pzem_current_read == PZEM_POWER || pzem_current_read == PZEM_ENERGY) {
    data = data * coil_ratio / 1000;
  } else if (pzem_current_read == PZEM_CURRENT) {
    data = data * coil_ratio;
  } 
  *val = String(data);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: GetPzemData load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  return true;
}



void GetPzemSerialRead() { 

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: GetPzemSerialRead Start");

  switch (pzem_current_read) {
    case PZEM_VOLTAGE:
      if (GetPzemData(pzem.voltage(ip_pzem), &pzemVoltageString)) {
        snprintf_P(log, sizeof(log), PSTR("GetPzemSerialRead: Voltage: %s V"), pzemVoltageString.c_str());
        addLog(LOG_LEVEL_INFO, log);
        pzem_current_read = PZEM_CURRENT;
      }
      break;
    case PZEM_CURRENT:
      if (GetPzemData(pzem.current(ip_pzem), &pzemCurrentString)) {
        snprintf_P(log, sizeof(log), PSTR("GetPzemSerialRead: Current: %s A"), pzemCurrentString.c_str());
        addLog(LOG_LEVEL_INFO, log);
        pzem_current_read = PZEM_POWER;
      }
      break;
    case PZEM_POWER:
      if (GetPzemData(pzem.power(ip_pzem), &pzemPowerString)) {
        snprintf_P(log, sizeof(log), PSTR("GetPzemSerialRead: Power: %s W"), pzemPowerString.c_str());
        addLog(LOG_LEVEL_INFO, log);
        pzem_current_read = PZEM_ENERGY;
      }
      break;
    case PZEM_ENERGY:
      if (GetPzemData(pzem.energy(ip_pzem), &pzemEnergyString)){
        snprintf_P(log, sizeof(log), PSTR("GetPzemSerialRead: Energy: %s Wh"), pzemEnergyString.c_str());
        addLog(LOG_LEVEL_INFO, log);
      }
      pzem_current_read = PZEM_VOLTAGE;
      break;
    default:
      pzem_current_read = PZEM_VOLTAGE;
  }

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: GetPzemSerialRead load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void PzemResetEnergy() {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: PzemResetEnergy Start");

  switch (pzem_reset_stage) {
    case PZEM_STAGE1:
      lightState = "ON";
      LightControl();
      pzem_reset_stage = PZEM_STAGE2;
      timer.setTimeout(6000, PzemResetEnergy);
      break;
    case PZEM_STAGE2:
      lightState = "OFF";
      LightControl();
      pzem_reset_stage = PZEM_STAGE3;
      timer.setTimeout(1000, PzemResetEnergy);
      break;
    case PZEM_STAGE3:
      lightState = "ON";
      LightControl();
      pzem_reset_stage = PZEM_STAGE4;
      timer.setTimeout(1000, PzemResetEnergy);
      break;
    case PZEM_STAGE4:
      lightState = "OFF";
      LightControl();
      pzem_reset_stage = PZEM_STAGE1;
      break;
    default:
      break;
  }

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: PzemResetEnergy load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}
#endif


void MotionDetect(){

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: MotionDetect Start");

  if (digitalRead(atoi(JConf.motion_pin)) == HIGH) {
    addLog_P(LOG_LEVEL_INFO, "MotionDetect: movement detected");
    motionDetect = true;
    LightControl();
    if (atoi(JConf.mqtt_enable) == 1 && mqtt.connected()) {
      pubTopicMotionSensor.publish("ON");
    }
  } else {
    motionDetect = false;
  }

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: MotionDetect load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



String GetUptimeData(){

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: GetUptimeData Start");

  //** Making Note of an expected rollover *****//   
  if(millis()>=3000000000){ 
    HighMillis=1;
  }
  //** Making note of actual rollover **//
  if(millis()<=100000&&HighMillis==1){
    Rollover++;
    HighMillis=0;
  }

  long secsUp = millis()/1000;

  Second = secsUp%60;
  Minute = (secsUp/60)%60;
  Hour = (secsUp/(60*60))%24;
  Day = (Rollover*50)+(secsUp/(60*60*24));  //First portion takes care of a rollover [around 50 days]

  sprintf_P(value_buff, (const char *)F("%dd %02d:%02d"), Day, Hour, Minute);
  uptimeString = String(value_buff);

  snprintf_P(log, sizeof(log), PSTR("GetUptimeData: Uptime: %s:%d%d"), uptimeString.c_str(), Second/10, Second%10);
  addLog(LOG_LEVEL_INFO, log);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: GetUptimeData load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  return value_buff;
}



#ifdef NTP_ON
void NTPSettingsUpdate(){
  if (atoi(JConf.ntp_enable) == 1) {
    timeClient.setUpdateServer(JConf.ntp_server);
    timeClient.setTimeOffset(atoi(JConf.my_time_zone) * 60 * 60);
  }
}
#endif



bool MqttConnect() {

  if (atoi(JConf.mqtt_enable) != 1) {
    return false;
  }

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: MqttConnect Start");

  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return true;
  }

  addLog_P(LOG_LEVEL_INFO, "MqttConnect: Connecting to MQTT...");

  if ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    snprintf_P(log, sizeof(log), PSTR("MqttConnect: Error: %s"), mqtt.connectErrorString(ret));
    addLog(LOG_LEVEL_ERROR, log);
    mqtt.disconnect();
    return false;
  }

  addLog_P(LOG_LEVEL_INFO, "MqttConnect: MQTT Connected");

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: MqttConnect load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  return true;
}



void MqttInit() {

  if (atoi(JConf.mqtt_enable) != 1) {
    return;
  }

  //Publish Topics
  sprintf(lightType_buff, "%s%s%s", JConf.publish_topic, lightType, JConf.mqtt_name);
  pubTopicLightType = Adafruit_MQTT_Publish(&mqtt, lightType_buff);

  sprintf(lightType2_buff, "%s%s%s", JConf.publish_topic, lightType2, JConf.mqtt_name);
  pubTopicLightType2 = Adafruit_MQTT_Publish(&mqtt, lightType2_buff);

  sprintf(motionSensor_buff, "%s%s%s", JConf.publish_topic, motionSensor, JConf.mqtt_name);
  pubTopicMotionSensor = Adafruit_MQTT_Publish(&mqtt, motionSensor_buff);

  sprintf(motionSensorTimer_buff, "%s%s%s", JConf.publish_topic, motionSensorTimer, JConf.mqtt_name);
  pubTopicMotionSensorTimer = Adafruit_MQTT_Publish(&mqtt, motionSensorTimer_buff);

  sprintf(motionSensorTimer2_buff, "%s%s%s", JConf.publish_topic, motionSensorTimer2, JConf.mqtt_name);
  pubTopicMotionSensorTimer2 = Adafruit_MQTT_Publish(&mqtt, motionSensorTimer2_buff);

  sprintf(lux_buff, "%s%s%s", JConf.publish_topic, lux, JConf.mqtt_name);
  pubTopicLux = Adafruit_MQTT_Publish(&mqtt, lux_buff);

  sprintf(temperature_buff, "%s%s%s", JConf.publish_topic, temperature, JConf.mqtt_name);
  pubTopicTemperature = Adafruit_MQTT_Publish(&mqtt, temperature_buff);

  sprintf(humidity_buff, "%s%s%s", JConf.publish_topic, humidity, JConf.mqtt_name);
  pubTopicHumidity = Adafruit_MQTT_Publish(&mqtt, humidity_buff);

  sprintf(pressure_buff, "%s%s%s", JConf.publish_topic, pressure, JConf.mqtt_name);
  pubTopicPressure = Adafruit_MQTT_Publish(&mqtt, pressure_buff);

  sprintf(pzemVoltage_buff, "%s%s%s", JConf.publish_topic, pzemVoltage, JConf.mqtt_name);
  pubTopicPzemVoltage = Adafruit_MQTT_Publish(&mqtt, pzemVoltage_buff);

  sprintf(pzemCurrent_buff, "%s%s%s", JConf.publish_topic, pzemCurrent, JConf.mqtt_name);
  pubTopicPzemCurrent = Adafruit_MQTT_Publish(&mqtt, pzemCurrent_buff);

  sprintf(pzemPower_buff, "%s%s%s", JConf.publish_topic, pzemPower, JConf.mqtt_name);
  pubTopicPzemPower = Adafruit_MQTT_Publish(&mqtt, pzemPower_buff);

  sprintf(pzemEnergy_buff, "%s%s%s", JConf.publish_topic, pzemEnergy, JConf.mqtt_name);
  pubTopicPzemEnergy = Adafruit_MQTT_Publish(&mqtt, pzemEnergy_buff);

  sprintf(freeMemory_buff, "%s%s%s", JConf.publish_topic, freeMemory, JConf.mqtt_name);
  pubTopicFreeMemory = Adafruit_MQTT_Publish(&mqtt, freeMemory_buff);

  sprintf(uptime_buff, "%s%s%s", JConf.publish_topic, uptime, JConf.mqtt_name);
  pubTopicUptime = Adafruit_MQTT_Publish(&mqtt, uptime_buff);

  sprintf(version_buff, "%s%s%s", JConf.publish_topic, version, JConf.mqtt_name);
  pubTopicVersion = Adafruit_MQTT_Publish(&mqtt, version_buff);

  sprintf(ip_buff, "%s%s%s", JConf.publish_topic, ip, JConf.mqtt_name);
  pubTopicIp = Adafruit_MQTT_Publish(&mqtt, ip_buff);

  sprintf(mac_buff, "%s%s%s", JConf.publish_topic, mac, JConf.mqtt_name);
  pubTopicMac = Adafruit_MQTT_Publish(&mqtt, mac_buff);

  //Subscribe Topics
  sprintf(motionSensorTimer_buff_sub, "%s%s%s", JConf.command_pub_topic, motionSensorTimer, JConf.mqtt_name);
  subTopicMotionSensorTimer = Adafruit_MQTT_Subscribe(&mqtt, motionSensorTimer_buff_sub);

  sprintf(motionSensorTimer2_buff_sub, "%s%s%s", JConf.command_pub_topic, motionSensorTimer2, JConf.mqtt_name);
  subTopicMotionSensorTimer2 = Adafruit_MQTT_Subscribe(&mqtt, motionSensorTimer2_buff_sub);

  sprintf(lightType_buff_sub, "%s%s%s", JConf.command_pub_topic, lightType, JConf.mqtt_name);
  subTopicLightType = Adafruit_MQTT_Subscribe(&mqtt, lightType_buff_sub);

  sprintf(lightType2_buff_sub, "%s%s%s", JConf.command_pub_topic, lightType2, JConf.mqtt_name);
  subTopicLightType2 = Adafruit_MQTT_Subscribe(&mqtt, lightType2_buff_sub);

  sprintf(uptime_buff_sub, "%s%s%s", JConf.command_pub_topic, uptime, JConf.mqtt_name);
  subTopicUptime = Adafruit_MQTT_Subscribe(&mqtt, uptime_buff_sub);

  sprintf(pzemReset_buff_sub, "%s%s%s", JConf.command_pub_topic, pzemReset, JConf.mqtt_name);
  subTopicPzemReset = Adafruit_MQTT_Subscribe(&mqtt, pzemReset_buff_sub);
}



#ifdef DS18X20_ON
void MqttInitDS() {

  if (atoi(JConf.mqtt_enable) != 1) {
    return;
  }
  sprintf(ds1_buff, "%s%s%s", JConf.publish_topic, dsData[0].addressString.c_str(), JConf.mqtt_name);
  pubTopic_ds1 = Adafruit_MQTT_Publish(&mqtt, ds1_buff);
  sprintf(ds2_buff, "%s%s%s", JConf.publish_topic, dsData[1].addressString.c_str(), JConf.mqtt_name);
  pubTopic_ds2 = Adafruit_MQTT_Publish(&mqtt, ds2_buff);
  sprintf(ds3_buff, "%s%s%s", JConf.publish_topic, dsData[2].addressString.c_str(), JConf.mqtt_name);
  pubTopic_ds3 = Adafruit_MQTT_Publish(&mqtt, ds3_buff);
  sprintf(ds4_buff, "%s%s%s", JConf.publish_topic, dsData[3].addressString.c_str(), JConf.mqtt_name);
  pubTopic_ds4 = Adafruit_MQTT_Publish(&mqtt, ds4_buff);
  sprintf(ds5_buff, "%s%s%s", JConf.publish_topic, dsData[4].addressString.c_str(), JConf.mqtt_name);
  pubTopic_ds5 = Adafruit_MQTT_Publish(&mqtt, ds5_buff);
}
#endif //DS18X20_ON




bool MqttPubLightState(){

  if (atoi(JConf.mqtt_enable) != 1) {
    return false;
  }

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: MqttPubLightState Start");

  if (!mqtt.connected()){
    addLog_P(LOG_LEVEL_ERROR, "MqttPubLightState: MQTT not connected!");
    return false;
  }

  String lightStateNum;
  if (lightState == "ON"){
    lightStateNum = String(F("1"));
  } else if (lightState == "OFF"){
    lightStateNum = String(F("0"));
  } else {
    lightStateNum = String(F("2"));
  }
  pubTopicLightType.publish(lightStateNum.c_str());

  if (lightState2 == "ON"){
    lightStateNum = String(F("1"));
  } else if (lightState2 == "OFF"){
    lightStateNum = String(F("0"));
  } else {
    lightStateNum = String(F("2"));
  }
  pubTopicLightType2.publish(lightStateNum.c_str());

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: MqttPubLightState load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  return true;
}



bool MqttPubLightOffDelay() {

  if (atoi(JConf.mqtt_enable) != 1) {
    return false;
  }

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: MqttPubLightState Start");

  if (!mqtt.connected()){
    addLog_P(LOG_LEVEL_ERROR, "MqttPubLightOffDelay: MQTT not connected!");
    return false;
  }

  pubTopicMotionSensorTimer.publish(JConf.lightoff_delay);

  pubTopicMotionSensorTimer2.publish(JConf.light2off_delay);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: MqttPubLightOffDelay load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  return true;
}



bool MqttPubData() {

  if (atoi(JConf.mqtt_enable) != 1) {
    return false;
  }

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: MqttPubData Start");

  if (!mqtt.connected()){
    addLog_P(LOG_LEVEL_ERROR, "MqttPubData: MQTT not connected!");
    return false;
  }

  if (atoi(JConf.bh1750_enable) == 1){
    pubTopicLux.publish(luxString.c_str());
  }

  if (atoi(JConf.bme280_enable) == 1  ||  atoi(JConf.sht21_enable) == 1 ||  atoi(JConf.dht_enable) == 1){
    pubTopicTemperature.publish(temperatureString.c_str());
    pubTopicHumidity.publish(humidityString.c_str());
  }

  if (atoi(JConf.bme280_enable) == 1){
    pubTopicPressure.publish(pressureString.c_str());
  }

  pubTopicFreeMemory.publish(freeMemoryString.c_str());
  pubTopicUptime.publish(uptimeString.c_str());
  pubTopicVersion.publish(ver);
  pubTopicIp.publish(ipString.c_str());
  pubTopicMac.publish(macString.c_str());

  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
      pubTopicPzemVoltage.publish(pzemVoltageString.c_str());
      pubTopicPzemCurrent.publish(pzemCurrentString.c_str());
      pubTopicPzemPower.publish(pzemPowerString.c_str());
      pubTopicPzemEnergy.publish(pzemEnergyString.c_str());
    }
  #endif

  #ifdef DS18X20_ON
    if (atoi(JConf.ds18x20_enable) == 1){
      pubTopic_ds1.publish(dsData[0].dsTemp.c_str());
      pubTopic_ds2.publish(dsData[1].dsTemp.c_str());
      pubTopic_ds3.publish(dsData[2].dsTemp.c_str());
      pubTopic_ds4.publish(dsData[3].dsTemp.c_str());
      pubTopic_ds5.publish(dsData[4].dsTemp.c_str());
    }
  #endif //DS18X20_ON


  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: MqttPubData load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  return true;
}



void CallbackMotionSensorTimer(char *data, uint16_t len) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: CallbackMotionSensorTimer Start");

  sprintf_P(JConf.lightoff_delay, (const char *)F("%s"), data);
  JConf.saveConfig();

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: CallbackMotionSensorTimer load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void CallbackMotionSensorTimer2(char *data, uint16_t len) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: CallbackMotionSensorTimer2 Start");

  sprintf_P(JConf.light2off_delay, (const char *)F("%s"), data);
  JConf.saveConfig();

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: CallbackMotionSensorTimer2 load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void CallbackLightType(char *data, uint16_t len) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: CallbackLightType Start");

  if (strncmp (data,"1",1) == 0){
    lightState = "ON";
  } else if (strncmp (data,"0",1) == 0){
    lightState = "OFF";
  } else if (strncmp (data,"2",1) == 0){
    lightState = "AUTO";
  }

  LightControl();

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: CallbackLightType load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void CallbackLightType2(char *data, uint16_t len) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: CallbackLightType2 Start");

  if (strncmp (data,"1",1) == 0){
    lightState2 = "ON";
  } else if (strncmp (data,"0",1) == 0){
    lightState2 = "OFF";
  } else if (strncmp (data,"2",1) == 0){
    lightState2 = "AUTO";
  }

  LightControl();

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: CallbackLightType2 load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



#ifdef PZEM_ON
void CallbackPzemReset(char *data, uint16_t len) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: CallbackPzemReset Start");

  if (strncmp (data,"ON",1) == 0){
    PzemResetEnergy();
  }

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: CallbackPzemReset load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}
#endif



void CallbackUptime(char *data, uint16_t len) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: CallbackUptime Start");

  timer.restartTimer(subscribeTimer);

  #ifdef REBOOT_ON
    timer.restartTimer(rebootTimer);
  #endif

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: CallbackUptime load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void MqttSubscribe(){

  if (atoi(JConf.mqtt_enable) != 1) {
    return;
  }

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: MqttSubscribe Start");

  subTopicMotionSensorTimer.setCallback(CallbackMotionSensorTimer);
  subTopicMotionSensorTimer2.setCallback(CallbackMotionSensorTimer2);
  subTopicLightType.setCallback(CallbackLightType);
  subTopicLightType2.setCallback(CallbackLightType2);
  subTopicUptime.setCallback(CallbackUptime);

  mqtt.subscribe(&subTopicMotionSensorTimer);
  mqtt.subscribe(&subTopicMotionSensorTimer2);
  mqtt.subscribe(&subTopicLightType);
  mqtt.subscribe(&subTopicLightType2);
  mqtt.subscribe(&subTopicUptime);

  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
      subTopicPzemReset.setCallback(CallbackPzemReset);
      mqtt.subscribe(&subTopicPzemReset);
    }
  #endif

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: MqttSubscribe load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void TestSystemPrint()
{
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: TestSystemPrint Start");

  snprintf_P(log, sizeof(log), PSTR("ESP: Version: %s"), ver);
  addLog(LOG_LEVEL_DEBUG, log);

  snprintf_P(log, sizeof(log), PSTR("ESP: IP address: %s"), ipString.c_str());
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  snprintf_P(log, sizeof(log), PSTR("ESP: Sketch size: %d"), ESP.getSketchSize());
  addLog(LOG_LEVEL_DEBUG, log);

  snprintf_P(log, sizeof(log), PSTR("ESP: Free size: %d"), ESP.getFreeSketchSpace());
  addLog(LOG_LEVEL_DEBUG, log);

  snprintf_P(log, sizeof(log), PSTR("ESP: Free memory: %s"), freeMemoryString.c_str());
  addLog(LOG_LEVEL_INFO, log);

  snprintf_P(log, sizeof(log), PSTR("ESP: Flash Chip Size: %d"), ESP.getFlashChipSize());
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  snprintf_P(log, sizeof(log), PSTR("ESP: Flash Chip Speed: %d"), ESP.getFlashChipSpeed());
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: TestSystemPrint load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}









void getData(){

  #ifdef NTP_ON
    if (atoi(JConf.ntp_enable) == 1) {
      timeClient.update();
      ntpTimeString = timeClient.getFormattedTime();
    }
  #endif

  int voltage = ESP.getVcc();
  voltage_float = voltage / 1000.0;

  #ifdef BH1750_ON
    if (atoi(JConf.bh1750_enable) == 1){
      GetLightSensorData();
    }
  #endif

  #ifdef BME280_ON
    if (atoi(JConf.bme280_enable) == 1){
      GetBmeSensorData();
    }
  #endif

  #ifdef SHT21_ON
    if (atoi(JConf.sht21_enable) == 1){
      GetSHT21SensorData();
    }
  #endif

  #ifdef DHT_ON
    if (atoi(JConf.dht_enable) == 1){
      GetDhtSensorData();
    }
  #endif

  #ifdef DS18X20_ON
  if (atoi(JConf.ds18x20_enable) == 1){
    if (searchDsSensorDone){
      GetDS18x20SensorData();
    } else {
      SearchDS18x20Sensors();
    }
  }
  #endif //DS18X20_ON

  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
      GetPzemSerialRead();
    }
  #endif

  GetUptimeData();
  GetFreeMemory();
  TestSystemPrint();

  #ifdef UART_ON
  for (int i = 0; i < ANALOG_PINS; i++){
    if (millis() - Uart.timerAnalogPin[i] >= 60000){
      Uart.valueAnalogPin[i] = 0;
      Uart.SetAnalogReadCycle(i, 10, "s");
    } else {

    }
  }
  #endif
}



void setup() {

  Serial.begin(115200);
  delay(100);
  Serial.println();



  if (!SPIFFS.begin()) {
    addLog_P(LOG_LEVEL_NONE, "setup: Failed to mount file system");
    return;
  } else {
    #ifdef RESET_BUTTON_ON
      deleteConfigFile();
    #endif
  }
/*
  if (!JConf.saveConfig()) {
    Serial.println("Failed to save config");
  } else {
    Serial.println("Config saved");
  }
*/
  if (!JConf.loadConfig()) {
    addLog_P(LOG_LEVEL_NONE, "setup: Failed to load config");
  } else {
    addLog_P(LOG_LEVEL_NONE, "setup: Config loaded");
  }

  if (atoi(JConf.pzem_enable)==1){
    JConf.serial_log_level[0] = '0'; // Отключаем serial log
    JConf.serial_log_level[1] = '\0';
    Serial.end();
    Serial.begin(9600);
    delay(100);
    Serial.println();
  } else {
    JConf.printConfig();
  }

  pinMode(atoi(JConf.light_pin), OUTPUT);
  pinMode(atoi(JConf.light2_pin), OUTPUT);
  pinMode(atoi(JConf.motion_pin), INPUT);

  digitalWrite(atoi(JConf.light_pin), LOW);
  digitalWrite(atoi(JConf.light2_pin), LOW);

  fading[0].pin = atoi(JConf.light_pin);
  fading[1].pin = atoi(JConf.light2_pin);

  scanWiFi();  // scan Access Points

  if (!WiFiSetup()) {
    WiFiSafeSetup();
  }
  delay(300);

  #ifdef PZEM_ON
    pzem.setAddress(ip_pzem);
    pzem.setReadTimeout(500);
  #endif

  #ifdef DHT_ON
    dht = DHT(atoi(JConf.dht_pin), DHTTYPE);
    dht.begin();
  #endif


  #ifdef BME280_ON
  if (atoi(JConf.bme280_enable) == 1) {
    bmeSensor.settings.commInterface = I2C_MODE;
    bmeSensor.settings.I2CAddress = 0x76;
    bmeSensor.settings.runMode = 3;
    bmeSensor.settings.tStandby = 0;
    bmeSensor.settings.filter = 4;
    bmeSensor.settings.tempOverSample = 5;
    bmeSensor.settings.pressOverSample = 5;
    bmeSensor.settings.humidOverSample = 5;
    bmeSensor.begin();
  }
  #endif

  #ifdef BH1750_ON
    if (atoi(JConf.bh1750_enable) == 1) {
      lightSensor.begin();
    }
  #endif

  if (atoi(JConf.bme280_enable) == 1 || atoi(JConf.bh1750_enable) == 1 || atoi(JConf.sht21_enable) == 1) {
    Wire.setClock(100000);
  }

  #ifdef SHT21_ON
    myHTU21D.begin(4, 5);  //SDA=4, SCL=5
  #endif


  if (atoi(JConf.mqtt_enable) == 1) {
    if (atoi(JConf.mqtt_auth_enable) == 1){
      mqtt = Adafruit_MQTT_Client(&espClient, JConf.mqtt_server, atoi(JConf.mqtt_port), JConf.mqtt_user, JConf.mqtt_pwd);
    } else {
      mqtt = Adafruit_MQTT_Client(&espClient, JConf.mqtt_server, atoi(JConf.mqtt_port));
    }
    MqttInit();
    MqttSubscribe();
    MqttConnect();
  }

  #ifdef USE_WEBSERVER
    WebServerInit();
  #endif // USE_WEBSERVER


  #ifdef NTP_ON
    if (atoi(JConf.ntp_enable) == 1) {
      timeClient.setUpdateServer(JConf.ntp_server);
      timeClient.setTimeOffset(atoi(JConf.my_time_zone) * 60 * 60);
      timeClient.setUpdateInterval(60*60*1000);
      timeClient.begin();
    }
  #endif

  wifiReconnectTimer = timer.setInterval(10000, wifiReconnect);
  timer.setInterval(atoi(JConf.get_data_delay) * 1000, getData);

  timer.setInterval(60000, MqttConnect);
  timer.setInterval(atoi(JConf.publish_delay) * 1000, MqttPubData);

  if (atoi(JConf.motion_sensor_enable) == 1){
    timer.setInterval(atoi(JConf.motion_read_delay) * 1000, MotionDetect);
  }

  subscribeTimer = timer.setInterval(atoi(JConf.subscribe_delay) * 1000, MqttSubscribe);
  timer.setInterval(600000, wifiSafeModeReconnect);

  #ifdef REBOOT_ON
    rebootTimer = timer.setInterval(atoi(JConf.reboot_delay) * 1000, restartESP);
  #endif

  GetMacString();

  CFG_Default();
}



void loop() {

  if (fading[0].cycleEnd != fading[0].cycleNow || fading[1].cycleEnd != fading[1].cycleNow) {
    FadeSwitchLoop();
  } else {
    #ifdef USE_WEBSERVER
      WebServer.handleClient();  // handle web server
    #endif  // USE_WEBSERVER

    timer.run();

    if (WiFi.status() == WL_CONNECTED && atoi(JConf.mqtt_enable) == 1) {
      if (mqtt.connected()){
        mqtt.processPackets(100);
      }
    }

    if (atoi(JConf.motion_sensor_enable) == 1 && motionDetect == false){
      MotionDetect();
    }

    if (lightState == "AUTO"){
      LightControl();
    }

    #ifdef UART_ON
      Uart.serialEvent();
    #endif

  }
  yield();
}