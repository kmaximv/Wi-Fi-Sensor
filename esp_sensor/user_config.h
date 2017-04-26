#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include "json_config.h"
JsonConf JConf;

// --------------- Supported Sensors (Uncomment for Enable) -------------------
#define DHT_ON
#define BME280_ON
#define SHT21_ON
#define BH1750_ON

//------------------PZEM Sensor-------------------------------------------------
#define PZEM_ON

#ifdef PZEM_ON
  bool pzemAlive = false;
  float coil_ratio = 1.84; // Если используем разные катушки, подбираем коэффициент
#endif //PZEM_ON
//------------------------------------------------------------------------------

//------------------DS18X20 Sensors---------------------------------------------
#define DS18X20_ON

#ifdef DS18X20_ON
  #define DS18X20_PIN 14
  #define MAX_DS_SENSORS 2
#endif //DS18X20_ON
//------------------------------------------------------------------------------

//------------------MH-Z19 Sensor-----------------------------------------------
#define MHZ19_ON

#define RESPONSE_SIZE 9   // Размер пакета
#define READ_TIMEOUT 300  // Время ожидания ответа от датчика
//------------------------------------------------------------------------------


//#define DEBUG

//#define RESET_BUTTON_ON   //Функционал сброса конфига по кнопке при загрузке модуля

//#define REBOOT_ON

//#define LCD_ON   //Дисплей с I2C

//#define BOILER_ON   //Котел отопления


//------------------NTP Client--------------------------------------------------
#define NTP_ON

#ifdef NTP_ON
  #define NTP_ERROR_TIME 30000 // Если за это время не смогли обновить время, засыпаем
  #define NTP_TIME_SLEEP 600000 // Время сна
  int ntpTimer = 0;
  unsigned long ntpLastUpdateTime = 0;
#endif //NTP_ON
//------------------------------------------------------------------------------

//------------------Encoder-----------------------------------------------------
//#define ENCODER_ON                              // Включить поддержку энкодера

#ifdef ENCODER_ON
  uint8_t encoderDirection = 0;                 // Направление поворота энкодера
  bool encoderFlagA = false;
  bool encoderFlagB = false;
  int encoderResetTimer = 0;
  int encoderResetInterval = 2000;                     // Интервал сброса флагов
#endif //ENCODER_ON
//------------------------------------------------------------------------------

#define USE_WEBSERVER

#define FADE_PINS 2
#define UP true
#define DOWN false


#define TOPSZ                  60           // Max number of characters in topic string
#define MESSZ                  128          // Max number of characters in message string
#define LOGSZ                  128          // Max number of characters in log string
#define MQTTSZ                 50           // Max number of characters in MQTT topic string

#define USE_WEBSERVER                       // Enable web server and wifi manager
#define MAX_LOG_LINES          30           // Max number of lines in weblog


#define SUB_PREFIX             "cmnd"       // Sonoff devices subscribe to:- cmnd/MQTT_TOPIC and cmnd/MQTT_GRPTOPIC


String Log[MAX_LOG_LINES];            // Web log buffer
byte logidx = 0;                      // Index in Web log buffer

struct TIME_T {
  uint8_t       Second;
  uint8_t       Minute;
  uint8_t       Hour;
  uint8_t       Wday;      // day of week, sunday is day 1
  uint8_t       Day;
  uint8_t       Month;
  char          MonthName[4];
  uint16_t      Year;
  unsigned long Valid;
} rtcTime;

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

#ifdef PZEM_ON
  Adafruit_MQTT_Publish pubTopicPzemVoltage = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
  Adafruit_MQTT_Publish pubTopicPzemCurrent = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
  Adafruit_MQTT_Publish pubTopicPzemPower = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
  Adafruit_MQTT_Publish pubTopicPzemEnergy = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);

  Adafruit_MQTT_Subscribe subTopicPzemReset = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);
#endif //PZEM_ON

Adafruit_MQTT_Publish pubTopicMhz19ppm = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);

Adafruit_MQTT_Publish pubTopicFreeMemory = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicUptime = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicVersion = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicIp = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
Adafruit_MQTT_Publish pubTopicMac = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);

Adafruit_MQTT_Subscribe subTopicMotionSensorTimer = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);
Adafruit_MQTT_Subscribe subTopicMotionSensorTimer2 = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);
Adafruit_MQTT_Subscribe subTopicLightType = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);
Adafruit_MQTT_Subscribe subTopicLightType2 = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);
Adafruit_MQTT_Subscribe subTopicUptime = Adafruit_MQTT_Subscribe(&mqtt, JConf.command_pub_topic);

#ifdef BOILER_ON
  Adafruit_MQTT_Publish pubTopicBoilerGetTemp = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
  Adafruit_MQTT_Publish pubTopicBoilerSetTemp = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);

  Adafruit_MQTT_Subscribe subTopicBoilerGetTemp = Adafruit_MQTT_Subscribe(&mqtt, JConf.publish_topic);
#endif //BOILER_ON


#ifdef BOILER_ON
  struct BOILER_T {
    String dataStr = "none";
    char mqttBuff[MQTTSZ];
    unsigned long updated = 0;
  };

  BOILER_T boilerGetTemp;
  BOILER_T boilerPubGetTemp;
  BOILER_T boilerSetTemp;
#endif //BOILER_ON


#ifdef DS18X20_ON
  struct DS1820_T {
    uint8_t type;
    //float data;
    String dsTemp = "none";
    String addressString = "none";
    byte address[8];
    Adafruit_MQTT_Publish pubTopic = Adafruit_MQTT_Publish(&mqtt, JConf.publish_topic);
    char ds_buff[MQTTSZ];
  } dsSensor;

  DS1820_T dsData[MAX_DS_SENSORS];
  uint8_t findDsSensors = 0;
  uint8_t currentDsSensor = 0;
  bool searchDsSensorDone = false;
  bool flag_ds_sensor_read_delay = false;
  enum DS_SENSOR_ENUM {DS18S20, DS18B20, DS1822, UNKNOWN};
#endif //DS18X20_ON


const char *ver                = "1.12"              ;

const char *lux                = "Lux"               ;
const char *lightType          = "LightType"         ;
const char *lightType2         = "LightType2"        ;
const char *temperature        = "Temp"              ;
const char *humidity           = "Humidity"          ;
const char *pressure           = "Pressure"          ;
const char *altitude           = "Altitude"          ;
const char *motionSensor       = "MotionSensor"      ;
const char *motionSensorTimer  = "MotionSensorTimer" ;
const char *motionSensorTimer2 = "MotionSensorTimer2";
const char *version            = "Version"           ;
const char *freeMemory         = "FreeMemory"        ;
const char *ip                 = "IP"                ;
const char *mac                = "MAC"               ;
const char *uptime             = "Uptime"            ;
const char *pzemVoltage        = "pzemVoltage"       ;
const char *pzemCurrent        = "pzemCurrent"       ;
const char *pzemPower          = "pzemPower"         ;
const char *pzemEnergy         = "pzemEnergy"        ;
const char *pzemReset          = "pzemReset"         ;
const char *mhz19ppm           = "mhz19ppm"          ;
const char *getTemp            = "getTemp"           ;
const char *setTemp            = "setTemp"           ;

const char sec[] PROGMEM = "sec";

String temperatureString = "none";
String pressureString =    "none";
String humidityString =    "none";
String luxString =         "none";
String ipString =          "none";
String macString =         "none";
String uptimeString =      "none";
String ntpTimeString =     "none";
String freeMemoryString =  "none";
String lightState =        "OFF";
String lightState2 =       "OFF";
String pzemVoltageString = "none";
String pzemCurrentString = "none";
String pzemPowerString =   "none";
String pzemEnergyString =  "none";
String mhz19PpmString =    "none";

long Day=0;
int Hour =0;
int Minute=0;
int Second=0;
int HighMillis=0;
int Rollover=0;

int wifiReconnectTimer = 0;
int rebootTimer = 0;
int subscribeTimer = 0;

unsigned long lightOffTimer = 0;
unsigned long lightOffTimer2 = 0;

bool motionDetect = false;
bool wifiSafeMode = false;


char topic_buff[120];
char value_buff[120];

char lightType_buff[MQTTSZ];
char lightType2_buff[MQTTSZ];
char motionSensor_buff[MQTTSZ];
char motionSensorTimer_buff[MQTTSZ];
char motionSensorTimer2_buff[MQTTSZ];
char lux_buff[MQTTSZ];
char temperature_buff[MQTTSZ];
char humidity_buff[MQTTSZ];
char pressure_buff[MQTTSZ];
#ifdef PZEM_ON
  char pzemVoltage_buff[MQTTSZ];
  char pzemCurrent_buff[MQTTSZ];
  char pzemPower_buff[MQTTSZ];
  char pzemEnergy_buff[MQTTSZ];
#endif //PZEM_ON
char mhz19ppm_buff[MQTTSZ];
char freeMemory_buff[MQTTSZ];
char uptime_buff[MQTTSZ];
char version_buff[MQTTSZ];
char ip_buff[MQTTSZ];
char mac_buff[MQTTSZ];

char motionSensorTimer_buff_sub[MQTTSZ];
char motionSensorTimer2_buff_sub[MQTTSZ];
char lightType_buff_sub[MQTTSZ];
char lightType2_buff_sub[MQTTSZ];
char uptime_buff_sub[MQTTSZ];
#ifdef PZEM_ON
  char pzemReset_buff_sub[MQTTSZ];
#endif //PZEM_ON


const char *AP = "AP";
const char *STA = "STA";
const char *AP_STA = "AP_STA";

const char *B = "11B";
const char *G = "11G";
const char *N = "11N";

const char *OPEN = "OPEN";
const char *WPA_PSK = "WPA_PSK";
const char *WPA2_PSK = "WPA2_PSK";
const char *WPA_WPA2_PSK = "WPA_WPA2_PSK";

enum log_t   {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};


#endif
