#ifndef USER_CONFIG_H
#define USER_CONFIG_H

// --------------- Supported Sensors (Uncomment for Enable) -------------------
#define DHT_ON
#define BME280_ON
#define SHT21_ON
#define BH1750_ON
#define PZEM_ON

//------------------DS18X20 Sensors---------------------------------------------
#define DS18X20_ON
#define DS18X20_PIN 2
//------------------------------------------------------------------------------

//------------------MH-Z19 Sensor-----------------------------------------------
#define MHZ19_ON
#define RESPONSE_SIZE 9   // Размер пакета
#define READ_TIMEOUT 300  // Время ожидания ответа от датчика
//------------------------------------------------------------------------------


//#define DEBUG
//#define DEBUG_JSON_CONFIG

//#define RESET_BUTTON_ON   //Функционал сброса конфига по кнопке при загрузке модуля
//#define REBOOT_ON
#define NTP_ON

//#define LCD_ON   //Дисплей с I2C

#define USE_WEBSERVER

#define FADE_PINS 2
#define UP true
#define DOWN false

enum WIFI_MODE_ENUM {AP, STA, AP_STA};
enum WIFI_PHY_MODE_ENUM {B, G, N};
enum WIFI_AUTH_ENUM {OPEN, WPA_PSK, WPA2_PSK, WPA_WPA2_PSK};
enum log_t   {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};


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


#ifdef DS18X20_ON
  struct DS1820_T {
    uint8_t type;
    //float data;
    String dsTemp = "none";
    String addressString = "none";
    byte address[8];
  } dsSensor;

  #define MAX_DS_SENSORS 5

  DS1820_T dsData[MAX_DS_SENSORS];
  uint8_t findDsSensors = 0;
  uint8_t currentDsSensor = 0;
  bool searchDsSensorDone = false;
  bool flag_ds_sensor_read_delay = false;
  enum DS_SENSOR_ENUM {DS18S20, DS18B20, DS1822, UNKNOWN};

  char ds1_buff[MQTTSZ];
  char ds2_buff[MQTTSZ];
  char ds3_buff[MQTTSZ];
  char ds4_buff[MQTTSZ];
  char ds5_buff[MQTTSZ];
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
char pzemVoltage_buff[MQTTSZ];
char pzemCurrent_buff[MQTTSZ];
char pzemPower_buff[MQTTSZ];
char pzemEnergy_buff[MQTTSZ];
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
char pzemReset_buff_sub[MQTTSZ];


#endif
