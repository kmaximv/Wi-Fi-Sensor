#ifndef USER_CONFIG_H
#define USER_CONFIG_H



// --------------- Supported Sensors (Uncomment for Enable) -------------------
#define DHT_ON
#define BME280_ON
#define SHT21_ON
#define BH1750_ON
#define PZEM_ON
//-----------------------------------------------------------------------------

//#define DEBUG
//#define DEBUG_JSON_CONFIG

//#define RESET_BUTTON_ON   //Функционал сброса конфига по кнопке при загрузке модуля
//#define REBOOT_ON
#define NTP_ON



#define ESP_PINS 17
#define UP true 
#define DOWN false

enum WIFI_MODE_ENUM {AP, STA, AP_STA};
enum WIFI_PHY_MODE_ENUM {B, G, N};
enum WIFI_AUTH_ENUM {OPEN, WPA_PSK, WPA2_PSK, WPA_WPA2_PSK};






#define TOPSZ                  60           // Max number of characters in topic string
#define MESSZ                  128          // Max number of characters in message string (Domoticz string)
#define LOGSZ                  128          // Max number of characters in log string


// Syslog
#define SYS_LOG_HOST           "domus1"
#define SYS_LOG_PORT           514
#define SYS_LOG_LEVEL          LOG_LEVEL_NONE
#define SERIAL_LOG_LEVEL       LOG_LEVEL_ALL
#define WEB_LOG_LEVEL          LOG_LEVEL_INFO

#define TELE_PERIOD            300          // Telemetry (0 = disable, 2 - 3600 seconds)


#define USE_WEBSERVER                       // Enable web server and wifi manager (+37k code, +2k mem) - Disable by //
#define MAX_LOG_LINES          30           // Max number of lines in weblog





char Hostname[33];                    // Composed Wifi hostname
String Log[MAX_LOG_LINES];            // Web log buffer
byte logidx = 0;                      // Index in Web log buffer



struct SYSCFG {
  byte          weblog_level;
  byte          seriallog_level;
  byte          syslog_level;
  char          syslog_host[32];
  uint16_t      syslog_port;
  uint16_t      tele_period;
} sysCfg;

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

enum log_t   {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};


const char *ver                = "1.09"              ;         

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

char lightType_buff[50];
char lightType2_buff[50];
char motionSensor_buff[50];
char motionSensorTimer_buff[50];
char motionSensorTimer2_buff[50];
char lux_buff[50];
char temperature_buff[50];
char humidity_buff[50];
char pressure_buff[50];
char pzemVoltage_buff[50];
char pzemCurrent_buff[50];
char pzemPower_buff[50];
char pzemEnergy_buff[50];
char freeMemory_buff[50];
char uptime_buff[50];
char version_buff[50];
char ip_buff[50];
char mac_buff[50];

char motionSensorTimer_buff_sub[50];
char motionSensorTimer2_buff_sub[50];
char lightType_buff_sub[50];
char lightType2_buff_sub[50];
char uptime_buff_sub[50];
char pzemReset_buff_sub[50];

int cycleNow[ESP_PINS];
int cycleEnd[ESP_PINS];

unsigned long timerDigitalPin[ESP_PINS];
int delayDigitalPin = 10;


#endif