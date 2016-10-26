#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#include "ESP8266WebServer.h"
extern "C" {
#include "user_interface.h"
}

#include "PubSubClient.h"
#include "json_config.h"
#include "ArduinoJson.h"

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
#include "DHT.h"
#define DHTTYPE DHT22
DHT dht(atoi(JConf.dht_pin), DHTTYPE);
int errorDHTdata = 0;  // количество ошибок чтения датчика DHT
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

ADC_MODE(ADC_VCC);
float voltage_float;

const char *ver                = "1.08"              ;         

const char *lux                = "Lux"               ;        
const char *lightType          = "LightType"         ;              
const char *lightType2         = "LightType2"        ;               
const char *temperature        = "Temp"              ;         
const char *humidity           = "Humidity"          ;             
const char *pressure           = "Pressure"          ;             
const char *altitude           = "Altitude"          ;             
const char *motionSensor       = "MotionSensor"      ;                 
const char *motionsensortimer  = "MotionSensorTimer" ;                      
const char *motionsensortimer2 = "MotionSensorTimer2";                       
const char *version            = "Version"           ;            
const char *freeMemory         = "FreeMemory"        ;               
const char *ip                 = "IP"                ;       
const char *mac                = "MAC"               ;        
const char *errorsDHT          = "ErrorsDHT"         ;              
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
String lightState =        "AUTO";
String lightState2 =       "AUTO";
String pzemVoltageString = "none";
String pzemCurrentString = "none";
String pzemPowerString =   "none";
String pzemEnergyString =  "none";

PubSubClient mqttClient;

long Day=0;
int Hour =0;
int Minute=0;
int Second=0;
int HighMillis=0;
int Rollover=0;

int rebootTimer = 0;
int subscribeTimer = 0;

unsigned long lightOffTimer = 0;
unsigned long lightOffTimer2 = 0;

bool motionDetect = false;
bool wifiSafeMode = false;

WiFiClient espClient;

char topic_buff[120];
char value_buff[120];

String network_html;          // Список доступных Wi-Fi точек

ESP8266WebServer WebServer(80);

int cycleNow[ESP_PINS];
int cycleEnd[ESP_PINS];

unsigned long timerDigitalPin[ESP_PINS];
int delayDigitalPin = 10;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////         HTML SNIPPLETS

const char headerStartP[] PROGMEM = "<html lang='en'><head><title>";
//JConf.module_id
const char headerStart2P[] PROGMEM = "</title><meta charset='utf-8'>";

const char headerRefreshStatusP[] PROGMEM = "<META HTTP-EQUIV='Refresh' CONTENT='20; URL=/'>";

const char headerEndP[] PROGMEM = 
"<meta name='viewport' content='width=device-width, initial-scale=1'>\
<link rel='stylesheet' href='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'>\
<script src='https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js'></script>\
<script src='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js'></script></head>";

const char javaScriptPinControlP[] PROGMEM = 
"<div id='content'></div>\
<div id='pin1'></div>\
<script>\
function show()\
{\
$.ajax({\
url: 'controlstatus',\
cache: false,\
success: function(html){\
$('#content').html(html);\
}\
});\
}\
function Pin1()\
{\
$.ajax({\
type: 'POST',\
url: 'control',\
data: '1=1',\
success: function(data){\
show();\
}\
});\
}\
function Auto1()\
{\
$.ajax({\
type: 'POST',\
url: 'control',\
data: '1=2',\
success: function(data){\
show();\
}\
});\
}\
function Pin2()\
{\
$.ajax({\
type: 'POST',\
url: 'control',\
data: '2=1',\
success: function(data){\
show();\
}\
});\
}\
function Auto2()\
{\
$.ajax({\
type: 'POST',\
url: 'control',\
data: '2=2',\
success: function(data){\
show();\
}\
});\
}\
$(document).ready(function(){\
show();\
setInterval('show()',5000);\
});\
</script>";

const char div1P[] PROGMEM =
"<div class='col-sm-8 col-md-6 col-lg-5'><h2>Control Pins</h2>\
<table class='table table-hover'>\
<tbody>\
  <tr>\
    <td class='active'><h4>Pins</h4></td>\
    <td class='active'></td><td class='active'></td>\
    <td class='active'><h4>Status</h4></td>\
    <td class='active'><h4>Mode</h4></td>\
    <td class='active'><h4>Timer</h4></td>\
  </tr>\
  <tr>\
    <td class='active'><h4>Light1</h4></td>\
    <td class='active'><div onclick='Pin1();'><input id='OnOff' type='submit' class='btn btn-";

const char javaScriptP[] PROGMEM = 
"<SCRIPT>\
var xmlHttp=createXmlHttpObject();\
function createXmlHttpObject(){\
  if(window.XMLHttpRequest){\
    xmlHttp=new XMLHttpRequest();\
  }else{\
    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\
 }\
 return xmlHttp;\
}\
function process(){\
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){\
   xmlHttp.open('PUT','xml',true);\
   xmlHttp.onreadystatechange=handleServerResponse;\
   xmlHttp.send(null);\
 }\
 setTimeout('process()',10000);\
}\
function handleServerResponse(){\
 if(xmlHttp.readyState==4 && xmlHttp.status==200){\
   xmlResponse=xmlHttp.responseXML;";

#if defined(UART_ON)
const char javaScript2P[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('apin0');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin0Id').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('apin1');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin1Id').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('apin2');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin2Id').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('apin3');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin3Id').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('apin4');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin4Id').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('apin5');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('apin5Id').innerHTML=message;\
 }\
}\
</SCRIPT>";
#endif

const char jsTemperatureP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('temperature');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('temperatureId').innerHTML=message;";

const char jsHumidityP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('humidity');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('humidityId').innerHTML=message;";

const char jsIlluminanceP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('illuminance');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('illuminanceId').innerHTML=message;";

const char jsPressureP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('pressure');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('pressureId').innerHTML=message;";


#if defined(PZEM_ON)
const char jsPzemP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('pzemVoltage');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('pzemVoltageId').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('pzemCurrent');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('pzemCurrentId').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('pzemPower');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('pzemPowerId').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('pzemEnergy');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('pzemEnergyId').innerHTML=message;";
#endif


const char jsNtpP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('ntpTime');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('ntpTimeId').innerHTML=message;";

// Длина строки не должна быть больше 1024 символов
const char javaScriptEndP[] PROGMEM = 
"xmldoc = xmlResponse.getElementsByTagName('vcc');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('vccId').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('uptime');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('uptimeId').innerHTML=message;\
xmldoc = xmlResponse.getElementsByTagName('freeMemory');\
message = xmldoc[0].firstChild.nodeValue;\
document.getElementById('freeMemoryId').innerHTML=message;\
 }\
}\
</SCRIPT>";

const char bodyAjaxP[] PROGMEM = "<body onload='process()'>";
const char bodyNonAjaxP[] PROGMEM = "<body>";

const char navbarStartP[] PROGMEM = 
"<nav class='navbar navbar-inverse'><div class='container-fluid'><div class='navbar-header'>\
<a class='navbar-brand' href='/'>";
//JConf.module_id
const char navbarStart2P[] PROGMEM = "</a></div><div><ul class='nav navbar-nav'>";

const char navbarNonActiveP[] PROGMEM = "<li>";
const char navbarActiveP[] PROGMEM = "<li class='active'>";

const char navbarBeginP[] PROGMEM  = 
"<a href='/'><span class='glyphicon glyphicon-dashboard'></span> Status</a></li>\
<li><a href='/pincontrol'><span class='glyphicon glyphicon-tasks'></span> Control Pins</a></li>";

#if defined(UART_ON)
const char navbarUartP[] PROGMEM = 
"<li><a href='/analog'><span class='glyphicon glyphicon-tasks'></span> Analog Pins UART</a></li>";
#endif

const char navbarEndP[] PROGMEM =
"<li class='dropdown'><a class='dropdown-toggle' data-toggle='dropdown' href='#'>\
<span class='glyphicon glyphicon-cog'></span> Configure<span class='caret'></span></a><ul class='dropdown-menu'>\
<li><a href='/wificonf'>Wi-Fi</a></li>\
<li><a href='/sensorsconf'>Sensors</a></li>\
<li><a href='/espconf'>ESP</a></li>\
<li><a href='/mqttconf'>MQTT</a></li>\
<li><a href='/ntpconf'>NTP time</a></li>\
<li><a href='/update'>Update frimware</a></li>\
<li><a href='/reboot'>Reboot ESP</a></li>\
</ul></li></ul></div></div></nav>"; 

const char containerStartP[] PROGMEM    =  "<div class='container'><div class='row'>";
const char containerEndP[] PROGMEM      =  "<div class='clearfix visible-lg'></div></div></div>";
const char siteEndP[] PROGMEM         =  "</body></html>";
  
const char panelHeaderNameP[] PROGMEM     =  "<div class='col-sm-6 col-md-5 col-lg-4'><div class='page-header'><h2>";
const char panelHeaderEndP[] PROGMEM    =  "</h2></div>";
const char panelEndP[] PROGMEM        =  "</div>";
  
const char panelBodySymbolP[] PROGMEM     =  
"<div class='panel panel-default'><div class='panel-body'><span class='glyphicon glyphicon-";

const char panelBodyNameP[] PROGMEM     =  "'></span> ";
const char panelBodyValueP[] PROGMEM    =  "<span class='pull-right'";
const char closingAngleBracketP[] PROGMEM    =  ">";

const char panelBodyEndP[] PROGMEM      =  "</span></div></div>";

const char inputBodyStartP[] PROGMEM    =  
"<form action='' method='POST'><div class='panel panel-default'><div class='panel-body'>";

const char inputBodyNameP[] PROGMEM     =  
"<div class='form-group'><div class='input-group'><span class='input-group-addon' id='basic-addon1'>";

const char inputBodyPOSTP[] PROGMEM     =  "</span><input type='text' name='";
const char inputPlaceHolderP[] PROGMEM  =  "' class='form-control' value='";

const char inputBodyCloseP[] PROGMEM    =  "' aria-describedby='basic-addon1'>";

const char inputBodyUnitStartP[] PROGMEM    =  "<span class='input-group-addon' id='basic-addon1'>";
const char inputBodyUnitEndP[] PROGMEM    =  "</span>";

const char inputBodyCloseDivP[] PROGMEM    =  "</div></div>";

const char inputBodyEndP[] PROGMEM      = 
"</div><div class='panel-footer clearfix'><div class='pull-right'>\
<button type='submit' class='btn btn-default' name='save_conf' value='1'>Save</button></div></div></div></form>";

const char sketchUploadFormP[] PROGMEM  = 
"<div class='col-sm-6 col-md-5 col-lg-4'><div class='page-header'><h2>Update Frimware</h2></div>\
<form method='POST' action='/upload_sketch' enctype='multipart/form-data'>\
<p><input type='file' class='btn btn-primary' name='sketch'></p>\
<h3><small>Выберите файл формата *.bin</small></h3>\
<p><input type='submit' value='Upload' class='btn btn-danger'></p></form></div>";


const char ClassInfoP[] PROGMEM  = "info";
const char ClassDangerP[] PROGMEM  = "danger";
const char ClassDefaultP[] PROGMEM  = "default";
const char ClassSuccessP[] PROGMEM  = "success";

const char AUTOP[] PROGMEM  = "AUTO";
const char ONP[] PROGMEM  = "ON";
const char OFFP[] PROGMEM  = "OFF";
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////         ROOT 

/*
static char* floatToChar(float charester)
{
  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("floatToChar() Start"));
  #endif

 dtostrf(charester, 1, 0, value_buff);
 
  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("floatToChar() Load Time: ")); Serial.println(load_time);
  #endif

 return value_buff;
}
*/


void GetFreeMemory () {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("GetFreeMemory() Start"));
  #endif

  freeMemoryString = String(ESP.getFreeHeap());

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("GetFreeMemory() Load Time: ")); Serial.println(load_time);
  #endif
}



String GetIpString (IPAddress ip) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("GetIpString() Start"));
  #endif

  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("GetIpString() Load Time: ")); Serial.println(load_time);
  #endif

  return ipStr;
}



void GetMacString () {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("GetMacString() Start"));
  #endif

  uint8_t macData[6];
  WiFi.macAddress(macData);
  sprintf_P(value_buff, (const char *)F("%x:%x:%x:%x:%x:%x"), macData[0], macData[1], macData[2], macData[3], macData[4], macData[5]);
  
  macString = String(value_buff);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("GetMacString() Load Time: ")); Serial.println(load_time);
  #endif
}



IPAddress stringToIp (String strIp) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("stringToIp() Start"));
  #endif

  String temp;
  IPAddress ip;

  int count = 0;
  for(int i=0; i <= strIp.length(); i++)
  {
    if(strIp[i] != '.')
    {
      temp += strIp[i];
    }
    else
    {
      if(count < 4)
      {
        ip[count] = atoi(temp.c_str());
        temp = "";
        count++;
      }
    }
    if(i==strIp.length())
    {
      ip[count] = atoi(temp.c_str());
    }
  }

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("stringToIp() Load Time: ")); Serial.println(load_time);
  #endif

  return ip;
}



bool isIPValid(const char * IP) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("isIPValid() Start"));
  #endif

  //limited size
  int internalcount=0;
  int dotcount = 0;
  bool previouswasdot=false;
  char c;

  if (strlen(IP)>15 || strlen(IP)==0) {
    return false;
  }
  //cannot start with .
  if (IP[0]=='.') {
    return false;
  }
  //only letter and digit
  for (int i=0; i < strlen(IP); i++) {
    c = IP[i];
    if (isdigit(c)) {
      //only 3 digit at once
      internalcount++;
      previouswasdot=false;
      if (internalcount>3) {
        return false;
      }
    } else if(c=='.') {
      if (previouswasdot) {   //cannot have 2 dots side by side
        return false;
      }
      previouswasdot=true;
      internalcount=0;
      dotcount++;
    } else {    //if not a dot neither a digit it is wrong
      return false;
    }
  }
  
  if (dotcount!=3) {    //if not 3 dots then it is wrong
    return false;
  }
  //cannot have the last dot as last char
  if (IP[strlen(IP)-1]=='.') {
      return false;
  }

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("isIPValid() Load Time: ")); Serial.println(load_time);
  #endif

  return true;
}



void PWMChange(int pin, int bright) {
  cycleEnd[pin] = bright;

  if ( ( atoi(JConf.light_smooth) == 0 && pin == atoi(JConf.light_pin) )   ||   ( atoi(JConf.light2_smooth) == 0 && pin == atoi(JConf.light2_pin) ) ){
    if (cycleNow[pin] < cycleEnd[pin]){
      cycleNow[pin] = 1022;
    } else if (cycleNow[pin] > cycleEnd[pin]){
      cycleNow[pin] = 1;
    }
  }
}



void FadeSwitchDelay(int pin){
  if (millis() - timerDigitalPin[pin] >= delayDigitalPin && cycleNow[pin] != cycleEnd[pin]){
    timerDigitalPin[pin] = millis();
    if (cycleNow[pin] < cycleEnd[pin]){
      cycleNow[pin] = constrain(cycleNow[pin] + 10, 0, 1023);
    } else if (cycleNow[pin] > cycleEnd[pin]){
      cycleNow[pin] = constrain(cycleNow[pin] - 10, 0, 1023);
    }
    analogWrite(pin, cycleNow[pin]);

    #ifdef DEBUG
    Serial.print(F("PWM pin: ")); Serial.print(pin);
    Serial.print(F("   PWM Value: ")); Serial.println(cycleNow[pin]);
    #endif
  }
}



void FadeSwitchLoop(){
  for ( size_t i = 0; i < ESP_PINS; i++ ){
    FadeSwitchDelay(i);
  }
}



void LightControl() {

  #ifdef DEBUG11
    unsigned long start_time = millis();
    Serial.println(F("LightControl() Start"));
  #endif

  String AUTO;       AUTO += FPSTR(AUTOP);
  String ON;         ON += FPSTR(ONP);
  String OFF;        OFF += FPSTR(OFFP);

  if (lightState == ON && luxString.toInt() < atoi(JConf.lighton_lux)){
    PWMChange(atoi(JConf.light_pin), 1023);
  } else if (lightState == OFF){
    PWMChange(atoi(JConf.light_pin), 0);
  } else if (lightState == AUTO && motionDetect == true && luxString.toInt() < atoi(JConf.lighton_lux)){
    PWMChange(atoi(JConf.light_pin), 1023);
    lightOffTimer = millis();
  } else if (lightState == AUTO && motionDetect == false && cycleEnd[atoi(JConf.light_pin)] != 0){
    if (millis() - lightOffTimer >= atoi(JConf.lightoff_delay) * 60UL * 1000UL){
      PWMChange(atoi(JConf.light_pin), 0);
    }
  }

  if (lightState2 == ON && luxString.toInt() < atoi(JConf.light2on_lux)){
    PWMChange(atoi(JConf.light2_pin), 1023);
  } else if (lightState2 == OFF){
    PWMChange(atoi(JConf.light2_pin), 0);
  } else if (lightState2 == AUTO && motionDetect == true && luxString.toInt() < atoi(JConf.light2on_lux)){
    PWMChange(atoi(JConf.light2_pin), 1023);
    lightOffTimer2 = millis();
  } else if (lightState2 == AUTO && motionDetect == false && cycleEnd[atoi(JConf.light2_pin)] != 0){
    if (millis() - lightOffTimer2 >= atoi(JConf.light2off_delay) * 60UL * 1000UL){
      PWMChange(atoi(JConf.light2_pin), 0);
    }
  }

  #ifdef DEBUG11
    unsigned long load_time = millis() - start_time;
    Serial.print(F("LightControl() Load Time: ")); Serial.println(load_time);
  #endif
}



void scanWiFi(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("scanWiFi() Start"));
  #endif

  int founds = WiFi.scanNetworks();

  #ifdef DEBUG
    Serial.println();  Serial.println(F("scan done"));
    if (founds == 0) {
      Serial.println(F("no networks found"));
    } else {
      Serial.print(founds);  Serial.println(F(" networks found"));
      for (size_t i = 0; i < founds; ++i) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);  Serial.print(F(": "));  Serial.print(WiFi.SSID(i));  Serial.print(F(" ("));  Serial.print(WiFi.RSSI(i));  Serial.print(F(")"));
        Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? F(" ") : F("*"));
        delay(10);
      }
    }
  #endif

  network_html = String(F("<blockquote>"));
  for (size_t i = 0; i < founds; ++i)
  {
    // Print SSID and RSSI for each network found
    network_html += String(F("<p><kbd>"));
    network_html += WiFi.SSID(i);
    network_html += String(F(" ("));
    network_html += WiFi.RSSI(i);
    network_html += String(F(")"));
    network_html += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? F(" ") : F("*");
    network_html += String(F("</kbd></p>"));
  }
  network_html += String(F("</blockquote>"));

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("scanWiFi() Load Time: ")); Serial.println(load_time);
  #endif
}



void wifiAPSettings(){
  if (atoi(JConf.wifi_auth) == OPEN){
    WiFi.softAP(JConf.module_id);
  } else {
    WiFi.softAP(JConf.module_id, JConf.ap_pwd);
  }

  //setup PHY_MODE
  if (atoi(JConf.wifi_phy_mode) == B){
    wifi_set_phy_mode((phy_mode_t)PHY_MODE_11B);    //PHY_MODE_11B,PHY_MODE_11G,PHY_MODE_11N
  } else if (atoi(JConf.wifi_phy_mode) == G){
    wifi_set_phy_mode((phy_mode_t)PHY_MODE_11G);
  } else {
    wifi_set_phy_mode((phy_mode_t)PHY_MODE_11N);
  }

  //get current config
  struct softap_config apconfig;
  wifi_softap_get_config(&apconfig);
  //set the chanel
  apconfig.channel=atoi(JConf.wifi_channel);

  //set Authentification type                      //AUTH_OPEN,AUTH_WPA_PSK,AUTH_WPA2_PSK,AUTH_WPA_WPA2_PSK
  if (atoi(JConf.wifi_auth) == OPEN){
    apconfig.authmode=(AUTH_MODE)AUTH_OPEN;
  } else if (atoi(JConf.wifi_auth) == WPA_PSK){
    apconfig.authmode=(AUTH_MODE)AUTH_WPA_PSK;
  } else if (atoi(JConf.wifi_auth) == WPA2_PSK){
    apconfig.authmode=(AUTH_MODE)AUTH_WPA2_PSK;
  } else {
    apconfig.authmode=(AUTH_MODE)AUTH_WPA_WPA2_PSK;
  }

  //set the visibility of SSID
  apconfig.ssid_hidden=0;
  //no need to add these settings to configuration just use default ones
  //apconfig.max_connection=2;
  //apconfig.beacon_interval=100;
  //apply settings to current and to default
  if (!wifi_softap_set_config(&apconfig) || !wifi_softap_set_config_current(&apconfig)) {
      Serial.println(F("Error Wifi AP_STA!"));
      delay(1000);
  }
}



bool wifiTryConnect(){
  byte i=0;
  while (WiFi.status() != WL_CONNECTED && i<40) {  //try to connect
    switch(WiFi.status()) {
    case 1:
      Serial.println(F("No SSID found!"));
      break;
    case 4:
      Serial.println(F("No Connection!"));
      break;
    default:
      Serial.println(F("Connecting..."));
      break;
    }
    delay(500);
    i++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }
  return true;  
}



void wifiAP() {
  WiFi.mode(WIFI_AP);   //setup Soft AP
  wifiAPSettings();
}



void wifiSTA() {
  WiFi.mode(WIFI_STA);                            //setup station mode
  WiFi.begin(JConf.sta_ssid, JConf.sta_pwd);
  delay(500);
  
  wifi_set_phy_mode((phy_mode_t)PHY_MODE_11N);    //setup PHY_MODE

  wifiTryConnect();

  WiFi.hostname(JConf.module_id);
}



void wifiAP_STA() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(JConf.sta_ssid, JConf.sta_pwd);

  wifiTryConnect();

  wifiAPSettings();
}



bool WiFiSetup()
{
  wifi_set_sleep_type ((sleep_type_t)NONE_SLEEP_T);   // NONE_SLEEP_T,LIGHT_SLEEP_T,MODEM_SLEEP_T
  WiFi.disconnect();

  if (atoi(JConf.wifi_mode) == AP) {
    wifiAP();
  } else if (atoi(JConf.wifi_mode) == STA) {
    wifiSTA();
  } else if (atoi(JConf.wifi_mode) == AP_STA) {
    wifiAP_STA();
  }

  //DHCP or Static IP ?
  if (atoi(JConf.static_ip_enable) == 1) {
    IPAddress staticIP = stringToIp(JConf.static_ip);
    IPAddress staticGateway = stringToIp(JConf.static_gateway);
    IPAddress staticSubnet = stringToIp(JConf.static_subnet);
    //apply according active wifi mode
    if (wifi_get_opmode()==WIFI_STA || wifi_get_opmode()==WIFI_AP_STA) {
      WiFi.config(staticIP, staticGateway, staticSubnet);
    }
  }
  //Get IP
  IPAddress espIP;
  if (wifi_get_opmode()==WIFI_STA || wifi_get_opmode()==WIFI_AP_STA) {
      espIP=WiFi.localIP();
  } else {
      espIP=WiFi.softAPIP();
  }
  ipString = GetIpString(espIP);

  return true;
}



void  WiFiSafeSetup()
{
  WiFi.disconnect();
  //setup Soft AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(JConf.module_id, JConf.ap_pwd);
  delay(500);
  Serial.println(F("Safe mode started"));
  wifiSafeMode = true;
}



void wifiSafeModeReconnect() {
  if (wifiSafeMode == true && WiFiSetup()) {
    wifiSafeMode = false; 
  }
}



#ifdef BH1750_ON
void GetLightSensorData()
{
  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("GetLightSensorData() Start"));
  #endif

  luxString = String(lightSensor.readLightLevel());
    
  #ifdef DEBUG
    Serial.print(F("Lux:   "));  Serial.print(luxString);  Serial.println();
  #endif

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("GetLightSensorData() Load Time: ")); Serial.println(load_time);
  #endif
}
#endif



#ifdef BME280_ON
void GetBmeSensorData()
{ 
  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("GetBmeSensorData() Start"));
  #endif

  temperatureString = String(bmeSensor.readTempC());
  #ifdef DEBUG 
    Serial.print(F("Temperature: "));  Serial.print(temperatureString);  Serial.println(F(" C"));
  #endif

  pressureString = String(bmeSensor.readFloatPressure()/133.3F);
  #ifdef DEBUG
    Serial.print(F("Pressure: "));  Serial.print(pressureString);  Serial.println(F(" mm"));
  #endif

  humidityString = String(bmeSensor.readFloatHumidity());
  #ifdef DEBUG
    Serial.print(F("%RH: "));  Serial.print(humidityString);  Serial.println(F(" %"));
  #endif

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("GetBmeSensorData() Load Time: ")); Serial.println(load_time);
  #endif
}
#endif



#ifdef SHT21_ON
void GetSHT21SensorData(){

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("GetSHT21SensorData() Start"));
  #endif

  myHTU21D.setResolution(HTU21D_RES_RH8_TEMP12);
  temperatureString = String(myHTU21D.readTemperature());
  humidityString = String(myHTU21D.readCompensatedHumidity());

  #ifdef DEBUG
    Serial.println(F(""));  Serial.println(F(""));  Serial.println(F("<<%RH: 8Bit, Temperature - 12Bit>>"));
    
    Serial.println(F(""));  Serial.print(F("Humidity: "));  Serial.println(myHTU21D.readHumidity());
    
    Serial.println(F(""));  Serial.print(F("Compensated Humidity: "));   Serial.println(humidityString);
    
    Serial.println(F(""));  Serial.print(F("Temperature: "));  Serial.print(temperatureString);  Serial.println(F(" C"));

    Serial.println(F(""));  Serial.println(F(""));  Serial.println(F("<<Battery Status>>"));
    
    if (myHTU21D.batteryStatus() == true)
    {
      Serial.println(F("Battery OK. Level > 2.25v"));
    }
    else
    {
      Serial.println(F("Battery LOW. Level < 2.25v"));
    }
    unsigned long load_time = millis() - start_time;
    Serial.print(F("GetSHT21SensorData() Load Time: ")); Serial.println(load_time);
  #endif
}
#endif



#ifdef DHT_ON
void DHT22Sensor()
{
  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("DHT22Sensor() Start"));
  #endif

  float temperatureData = dht.readTemperature();
  float humidityData = dht.readHumidity();
  #ifdef DEBUG
    Serial.print(F("Humidity "));  Serial.println(humidityData);
    Serial.print(F("Temperature "));  Serial.println(temperatureData);
  #endif
  if (isnan(humidityData) || isnan(temperatureData)) {
    errorDHTdata++;
    #ifdef DEBUG
      Serial.print(F("Failed to read from DHT. Number errors: "));  Serial.println(errorDHTdata);
    #endif
  } else {
    temperatureString = String(temperatureData);
    humidityString = String(humidityData);
  }

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("DHT22Sensor() Load Time: ")); Serial.println(load_time);
  #endif
}
#endif


#if defined(PZEM_ON)
bool GetPzemData(float data, String *val) {
  if (data < 0.0){
    return false;
  } else if (pzem_current_read == PZEM_POWER || pzem_current_read == PZEM_ENERGY) {
    data = data * coil_ratio / 1000;
  } else if (pzem_current_read == PZEM_CURRENT) {
    data = data * coil_ratio;
  } 
  *val = String(data);
  return true;
}



void GetPzemSerialRead() { 
  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("GetPzemSerialRead() Start"));
  #endif

  switch (pzem_current_read) {
    case PZEM_VOLTAGE:
      if (GetPzemData(pzem.voltage(ip_pzem), &pzemVoltageString)) {
        pzem_current_read = PZEM_CURRENT;
      }
      break;
    case PZEM_CURRENT:
      if (GetPzemData(pzem.current(ip_pzem), &pzemCurrentString)) {
        pzem_current_read = PZEM_POWER;
      }
      break;
    case PZEM_POWER:
      if (GetPzemData(pzem.power(ip_pzem), &pzemPowerString)) {
        pzem_current_read = PZEM_ENERGY;
      }
      break;
    case PZEM_ENERGY:
      GetPzemData(pzem.energy(ip_pzem), &pzemEnergyString);
      pzem_current_read = PZEM_VOLTAGE;
      break;
    default:
      pzem_current_read = PZEM_VOLTAGE;
  }

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("GetPzemSerialRead() Load Time: ")); Serial.println(load_time);
  #endif
}



void PzemResetEnergy() {

  String ON;         ON += FPSTR(ONP);
  String OFF;        OFF += FPSTR(OFFP);

  switch (pzem_reset_stage) {
    case PZEM_STAGE1:
      lightState = ON;
      LightControl();
      pzem_reset_stage = PZEM_STAGE2;
      timer.setTimeout(6000, PzemResetEnergy);
      break;
    case PZEM_STAGE2:
      lightState = OFF;
      LightControl();
      pzem_reset_stage = PZEM_STAGE3;
      timer.setTimeout(1000, PzemResetEnergy);
      break;
    case PZEM_STAGE3:
      lightState = ON;
      LightControl();
      pzem_reset_stage = PZEM_STAGE4;
      timer.setTimeout(1000, PzemResetEnergy);
      break;
    case PZEM_STAGE4:
      lightState = OFF;
      LightControl();
      pzem_reset_stage = PZEM_STAGE1;
      break;
    default:
      break;
  }
}
#endif


void MotionDetect(){

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("MotionDetect() Start"));
  #endif

  if (digitalRead(atoi(JConf.motion_pin)) == HIGH) {
    #ifdef DEBUG
      Serial.println(F("MotionSensor: movement detected"));
    #endif
    motionDetect = true;
    LightControl();
    if (atoi(JConf.mqtt_enable) == 1 && mqttClient.connected()) {
      sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  motionSensor, JConf.mqtt_name);
      mqttClient.publish(topic_buff, "ON");
    }
  } else {
    motionDetect = false;
  }

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("MotionDetect() Load Time: ")); Serial.println(load_time);
  #endif
}



String GetUptimeData(){

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("GetUptimeData() Start"));
  #endif

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
  #ifdef DEBUG
    Serial.print(F("Uptime: "));  Serial.print(value_buff);  Serial.print(F(":"));  Serial.print(Second/10);  Serial.println(Second%10);
    unsigned long load_time = millis() - start_time;
    Serial.print(F("GetUptimeData() Load Time: ")); Serial.println(load_time);
  #endif

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



// handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("callback() Start"));
  #endif

  timer.restartTimer(subscribeTimer);

  #ifdef REBOOT_ON
    timer.restartTimer(rebootTimer);
  #endif

  size_t i=0;
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    value_buff[i] = payload[i];
  }
  value_buff[i] = '\0';

  #ifdef DEBUG
  Serial.print(F("callback: "));  Serial.print(topic);  Serial.print(F(" payload "));  Serial.println(value_buff);
  #endif

  // Обрабатываем данные о состоянии светодиодной ленты (AUTO, ON, OFF)
  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.command_pub_topic, lightType, JConf.mqtt_name);
  if (strcmp (topic,topic_buff) == 0){

    String AUTO;       AUTO += FPSTR(AUTOP);
    String ON;         ON += FPSTR(ONP);
    String OFF;        OFF += FPSTR(OFFP);

    #ifdef DEBUG
      Serial.print(F("topic: "));  Serial.print(topic);  Serial.print(F(" equals "));  Serial.println(topic_buff);
    #endif
    if (strncmp (value_buff,"1",1) == 0){
      lightState = ON;
    } else if (strncmp (value_buff,"0",1) == 0){
      lightState = OFF;
    } else if (strncmp (value_buff,"2",1) == 0){
      lightState = AUTO;
    }


    #ifdef DEBUG
      Serial.print(F("value_buff: "));  Serial.print(value_buff);  Serial.print(F(" contains "));   Serial.print(lightState);
    #endif

    LightControl();
  }

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.command_pub_topic, lightType2, JConf.mqtt_name);
  if (strcmp (topic,topic_buff) == 0){

    String AUTO;       AUTO += FPSTR(AUTOP);
    String ON;         ON += FPSTR(ONP);
    String OFF;        OFF += FPSTR(OFFP);

    #ifdef DEBUG
      Serial.print(F("topic: "));  Serial.print(topic);  Serial.print(F(" equals "));  Serial.println(topic_buff);
    #endif
    if (strncmp (value_buff,"1",1) == 0){
      lightState2 = ON;
    } else if (strncmp (value_buff,"0",1) == 0){
      lightState2 = OFF;
    } else if (strncmp (value_buff,"2",1) == 0){
      lightState2 = AUTO;
    }


    #ifdef DEBUG
      Serial.print(F("value_buff: "));  Serial.print(value_buff);  Serial.print(F(" contains "));   Serial.print(lightState2);
    #endif

    LightControl();
  }


  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.command_pub_topic, motionsensortimer, JConf.mqtt_name);
    if (strcmp (topic,topic_buff) == 0){
      #ifdef DEBUG
        Serial.print(F("topic: "));  Serial.print(topic);  Serial.print(F(" equals "));  Serial.println(topic_buff);
      #endif
      sprintf_P(JConf.lightoff_delay, (const char *)F("%s"), value_buff);
      JConf.saveConfig();
    }

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.command_pub_topic, motionsensortimer2, JConf.mqtt_name);
    if (strcmp (topic,topic_buff) == 0){
      #ifdef DEBUG
        Serial.print(F("topic: "));  Serial.print(topic);  Serial.print(F(" equals "));  Serial.println(topic_buff);
      #endif
      sprintf_P(JConf.light2off_delay, (const char *)F("%s"), value_buff);
      JConf.saveConfig();
    }


  #ifdef PZEM_ON
  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.command_pub_topic, pzemReset, JConf.mqtt_name);
    if (strcmp (topic,topic_buff) == 0){
      #ifdef DEBUG
        Serial.print(F("topic: "));  Serial.print(topic);  Serial.print(F(" equals "));  Serial.println(topic_buff);
      #endif
      if (strncmp (value_buff,"ON",1) == 0){
        PzemResetEnergy();
      }
    }
  #endif

  #ifdef DEBUG
    Serial.print(F("topic: "));  Serial.println(topic);  Serial.print(F("value_buff: "));  Serial.println(value_buff);
    Serial.println();
  #endif

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("callback() Load Time: ")); Serial.println(load_time);
  #endif
}



bool MqttPubLightState(){

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("MqttPubLightState() Start"));
  #endif

  if (!mqttClient.connected()){
    #ifdef DEBUG
      Serial.print(F("MQTT server not connected"));  Serial.println();
      unsigned long load_time = millis() - start_time;
      Serial.print(F("MqttPubLightState() Load Time: ")); Serial.println(load_time);
    #endif
    return false;
  }

  String ON;         ON += FPSTR(ONP);
  String OFF;        OFF += FPSTR(OFFP);

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  lightType, JConf.mqtt_name);
  String lightStateNum;
  if (lightState == ON){
    lightStateNum = String(F("1"));
  } else if (lightState == OFF){
    lightStateNum = String(F("0"));
  } else {
    lightStateNum = String(F("2"));
  }
  mqttClient.publish(topic_buff, lightStateNum.c_str());

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  lightType2, JConf.mqtt_name);
  if (lightState2 == ON){
    lightStateNum = String(F("1"));
  } else if (lightState2 == OFF){
    lightStateNum = String(F("0"));
  } else {
    lightStateNum = String(F("2"));
  }
  mqttClient.publish(topic_buff, lightStateNum.c_str());

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("MqttPubLightState() Load Time: ")); Serial.println(load_time);
  #endif

  return true;
}



bool MqttPubLightOffDelay() {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("MqttPubLightOffDelay() Start"));
  #endif


  if (!mqttClient.connected()){
    #ifdef DEBUG
      Serial.print(F("MQTT server not connected"));  Serial.println();
      unsigned long load_time = millis() - start_time;
      Serial.print(F("MqttPubLightOffDelay() Load Time: ")); Serial.println(load_time);
    #endif

    return false;
  }

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  motionsensortimer, JConf.mqtt_name);
  mqttClient.publish(topic_buff, JConf.lightoff_delay);

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  motionsensortimer2, JConf.mqtt_name);
  mqttClient.publish(topic_buff, JConf.light2off_delay);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("MqttPubLightOffDelay() Load Time: ")); Serial.println(load_time);
  #endif

  return true;
}



bool MqttPubData() {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("MqttPubData() Start"));
  #endif

  if (mqttClient.state() != 0){
    #ifdef DEBUG
      Serial.print(F("MQTT server not connected"));  Serial.println();
      unsigned long load_time = millis() - start_time;
      Serial.print(F("MqttPubData() Load Time: ")); Serial.println(load_time);
    #endif
    return false;
  }

  Serial.print(F("MQTT data send"));  Serial.println();

  if (atoi(JConf.bh1750_enable) == 1){
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  lux, JConf.mqtt_name);
    mqttClient.publish(topic_buff, luxString.c_str());
  }

  if (atoi(JConf.bme280_enable) == 1  ||  atoi(JConf.sht21_enable) == 1 ||  atoi(JConf.dht_enable) == 1){
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic, temperature, JConf.mqtt_name);
    mqttClient.publish(topic_buff, temperatureString.c_str());
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  humidity, JConf.mqtt_name);
    mqttClient.publish(topic_buff, humidityString.c_str());
  }

  if (atoi(JConf.bme280_enable) == 1){
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic, pressure, JConf.mqtt_name);
    mqttClient.publish(topic_buff, pressureString.c_str());
  }

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  freeMemory, JConf.mqtt_name);
  mqttClient.publish(topic_buff, freeMemoryString.c_str());

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic, uptime, JConf.mqtt_name);
  mqttClient.publish(topic_buff, uptimeString.c_str());

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  version, JConf.mqtt_name);
  mqttClient.publish(topic_buff, ver);

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  ip, JConf.mqtt_name);
  mqttClient.publish(topic_buff, ipString.c_str());

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  mac, JConf.mqtt_name);
  mqttClient.publish(topic_buff, macString.c_str());

 
  #ifdef DHT_ON
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  errorsDHT, JConf.mqtt_name);
    sprintf_P(value_buff, (const char *)F("%d"), errorDHTdata);  
    mqttClient.publish(topic_buff, value_buff);
  #endif

  #ifdef PZEM_ON
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  pzemVoltage, JConf.mqtt_name);
    mqttClient.publish(topic_buff, pzemVoltageString.c_str());

    sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  pzemCurrent, JConf.mqtt_name);
    mqttClient.publish(topic_buff, pzemCurrentString.c_str());

    sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  pzemPower, JConf.mqtt_name);
    mqttClient.publish(topic_buff, pzemPowerString.c_str());

    sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.publish_topic,  pzemEnergy, JConf.mqtt_name);
    mqttClient.publish(topic_buff, pzemEnergyString.c_str());
  #endif


  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("MqttPubData() Load Time: ")); Serial.println(load_time);
  #endif

  return true;
}



bool MqttSubscribePrint(char *sub_buff)
{
  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("MqttSubscribePrint() Start"));
  #endif

  delay(50);
  if (!mqttClient.connected()){
    #ifdef DEBUG
      Serial.print(F("MQTT server not connected"));  Serial.println();
    #endif
    return false;
  }

  if (mqttClient.subscribe(sub_buff)) {
    #ifdef DEBUG
      Serial.print(F("subscribe: "));  Serial.println(sub_buff);
    #endif
  } else {
    #ifdef DEBUG
      mqttClient.disconnect();
      Serial.print(F("ERROR subscribe: "));  Serial.println(sub_buff);
    #endif
  }

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("MqttSubscribePrint() Load Time: ")); Serial.println(load_time);
  #endif

  return true;
}



bool MqttSubscribe(){

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("MqttSubscribe() Start"));
  #endif

  if (!mqttClient.connected()){
    #ifdef DEBUG
      Serial.print(F("MQTT server not connected"));  Serial.println();
    #endif
    return false;
  }

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.command_pub_topic, motionsensortimer, JConf.mqtt_name);
  MqttSubscribePrint(topic_buff);

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.command_pub_topic, lightType, JConf.mqtt_name);
  MqttSubscribePrint(topic_buff);

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.command_pub_topic, lightType2, JConf.mqtt_name);
  MqttSubscribePrint(topic_buff);

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.subscribe_topic, uptime, JConf.mqtt_name);
  MqttSubscribePrint(topic_buff);

  #ifdef PZEM_ON
  sprintf_P(topic_buff, (const char *)F("%s%s%s"), JConf.command_pub_topic, pzemReset, JConf.mqtt_name);
  MqttSubscribePrint(topic_buff);
  #endif

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("MqttSubscribe() Load Time: ")); Serial.println(load_time);
  #endif

  return true;
}



void TestMQTTPrint()
{
  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("TestMQTTPrint() Start"));
  #endif

  int state = mqttClient.state();

  switch (state) {
    case -4:
      Serial.println(F("MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time"));
      break;
    case -3:
      Serial.println(F("MQTT_CONNECTION_LOST - the network connection was broken"));
      break;
    case -2:
      Serial.println(F("MQTT_CONNECT_FAILED - the network connection failed"));
      break;
    case -1:
      Serial.println(F("MQTT_DISCONNECTED - the client is disconnected cleanly"));
      break;
    case 0:
      Serial.println(F("MQTT_CONNECTED - the cient is connected"));    
      break;
    case 1:
      Serial.println(F("MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT"));    
      break;
    case 2:
      Serial.println(F("MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier"));    
      break;
    case 3:
      Serial.println(F("MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection"));    
      break;
    case 4:
      Serial.println(F("MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected"));    
      break;
    case 5:
      Serial.println(F("MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect"));    
      break;
  }

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("TestMQTTPrint() Load Time: ")); Serial.println(load_time);
  #endif
}



void TestSystemPrint()
{
  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("TestSystemPrint() Start"));
  #endif

  Serial.println(F("----------------"));

  if (atoi(JConf.mqtt_enable) == 1){
    TestMQTTPrint();
  }

  Serial.println(__TIMESTAMP__);

  Serial.print(F("Version "));  Serial.println(ver);

  Serial.print(F("IP address: "));  Serial.println(ipString);

  Serial.print(F("Sketch size: "));  Serial.println(ESP.getSketchSize());

  Serial.print(F("Free size: "));  Serial.println(ESP.getFreeSketchSpace());

  Serial.print(F("Free memory: "));  Serial.println(freeMemoryString);

  Serial.print(F("WiFi status: "));  Serial.println(WiFi.status());

  Serial.print(F("Chip Id: "));  Serial.println(ESP.getChipId());

  Serial.print(F("Flash Chip Id: "));  Serial.println(ESP.getFlashChipId());

  Serial.print(F("Flash Chip Size: ")); Serial.println(ESP.getFlashChipSize());

  Serial.print(F("Flash Chip Speed: "));  Serial.println(ESP.getFlashChipSpeed());

  Serial.println(F("----------------"));

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("TestSystemPrint() Load Time: ")); Serial.println(load_time);
  #endif
}




//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////   WEB PAGES  Start  //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////



void WebRoot(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebRoot() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String javaScript;            javaScript += FPSTR(javaScriptP);
  String jsTemperature;         jsTemperature += FPSTR(jsTemperatureP);
  String jsHumidity;            jsHumidity += FPSTR(jsHumidityP);
  String jsPressure;            jsPressure += FPSTR(jsPressureP);
  String jsIlluminance;         jsIlluminance += FPSTR(jsIlluminanceP);
  #ifdef PZEM_ON
  String jsPzem;                jsPzem += FPSTR(jsPzemP);
  #endif
  String jsNtp;                 jsNtp += FPSTR(jsNtpP);
  String javaScriptEnd;         javaScriptEnd += FPSTR(javaScriptEndP);
  String bodyAjax;              bodyAjax += FPSTR(bodyAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarActive;          navbarActive += FPSTR(navbarActiveP);

  navbarActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);
  String panelEnd;              panelEnd += FPSTR(panelEndP);
  String panelBodySymbol;       panelBodySymbol += FPSTR(panelBodySymbolP);
  String panelBodyName;         panelBodyName += FPSTR(panelBodyNameP);
  String panelBodyValue;        panelBodyValue += FPSTR(panelBodyValueP);
  String closingAngleBracket; closingAngleBracket += FPSTR(closingAngleBracketP);
  String panelBodyEnd;          panelBodyEnd += FPSTR(panelBodyEndP);

  String title1       = panelHeaderName + String(F("Sensor Data"))   + panelHeaderEnd;

  if (atoi(JConf.bme280_enable) == 1 || atoi(JConf.sht21_enable) == 1 || atoi(JConf.dht_enable) == 1 ){

    title1           += panelBodySymbol + String(F("fire"))          + panelBodyName + String(F("Temperature")) + panelBodyValue + String(F(" id='temperatureId'")) + closingAngleBracket   + panelBodyEnd;
    title1           += panelBodySymbol + String(F("tint"))          + panelBodyName + String(F("Humidity"))    + panelBodyValue + String(F(" id='humidityId'")) + closingAngleBracket      + panelBodyEnd;
  }

  #ifdef BME280_ON
    if (atoi(JConf.bme280_enable) == 1){
      title1         += panelBodySymbol + String(F("cloud"))         + panelBodyName + String(F("Pressure"))    + panelBodyValue + String(F(" id='pressureId'")) + closingAngleBracket      + panelBodyEnd;
    }
  #endif

  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
      title1         += panelBodySymbol + String(F("flash"))         + panelBodyName + String(F("Voltage"))    + panelBodyValue + String(F(" id='pzemVoltageId'")) + closingAngleBracket   + panelBodyEnd;
      title1         += panelBodySymbol + String(F("flash"))         + panelBodyName + String(F("Current"))    + panelBodyValue + String(F(" id='pzemCurrentId'")) + closingAngleBracket   + panelBodyEnd;
      title1         += panelBodySymbol + String(F("flash"))         + panelBodyName + String(F("Power"))      + panelBodyValue + String(F(" id='pzemPowerId'"))   + closingAngleBracket   + panelBodyEnd;
      title1         += panelBodySymbol + String(F("flash"))         + panelBodyName + String(F("Energy"))     + panelBodyValue + String(F(" id='pzemEnergyId'"))  + closingAngleBracket   + panelBodyEnd;
    }
  #endif


  if (atoi(JConf.bh1750_enable) == 1){
    title1           += panelBodySymbol + String(F("asterisk"))      + panelBodyName + String(F("illuminance")) + panelBodyValue + String(F(" id='illuminanceId'")) + closingAngleBracket   + panelBodyEnd;
  }
  
  String title2       = panelHeaderName + String(F("Settings"))      + panelHeaderEnd;
  //title2             += panelBodySymbol + String(F("signal"))        + panelBodyName + String(F("Wi-Fi SSID"))  + panelBodyValue + closingAngleBracket + JConf.sta_ssid    + panelBodyEnd;
  title2             += panelBodySymbol + String(F("globe"))         + panelBodyName + String(F("IP Address"))  + panelBodyValue + closingAngleBracket + ipString          + panelBodyEnd;
  //title2             += panelBodySymbol + String(F("scale"))         + panelBodyName + String(F("MAC Address")) + panelBodyValue + closingAngleBracket + macString         + panelBodyEnd;
  //title2             += panelBodySymbol + String(F("tag"))           + panelBodyName + String(F("MQTT Prefix")) + panelBodyValue + closingAngleBracket + JConf.mqtt_name   + panelBodyEnd;

  title2             += panelBodySymbol + String(F("time"))          + panelBodyName + String(F("Uptime"))      + panelBodyValue + String(F(" id='uptimeId'"))     + closingAngleBracket  + panelBodyEnd;

  if (atoi(JConf.ntp_enable) == 1) {
    title2           += panelBodySymbol + String(F("time"))          + panelBodyName + String(F("NTP time"))    + panelBodyValue + String(F(" id='ntpTimeId'"))    + closingAngleBracket  + panelBodyEnd;
  }

  title2             += panelBodySymbol + String(F("flash"))         + panelBodyName + String(F("Voltage"))     + panelBodyValue + String(F(" id='vccId'"))        + closingAngleBracket  + panelBodyEnd;
  title2             += panelBodySymbol + String(F("flash"))         + panelBodyName + String(F("Free Memory")) + panelBodyValue + String(F(" id='freeMemoryId'")) + closingAngleBracket  + panelBodyEnd;
  //title2             += panelBodySymbol + String(F("flag"))          + panelBodyName + String(F("Version"))     + panelBodyValue + closingAngleBracket + String(ver)                      + panelBodyEnd;

  String data = headerStart + JConf.module_id + headerStart2 + headerEnd + javaScript;

  if (atoi(JConf.bme280_enable) == 1 || atoi(JConf.sht21_enable) == 1 || atoi(JConf.dht_enable) == 1) {
    data += jsTemperature + jsHumidity;
  }
  if (atoi(JConf.bme280_enable) == 1) {
    data += jsPressure;
  }
  if (atoi(JConf.bh1750_enable) == 1) {
    data += jsIlluminance;
  }

  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
      data += jsPzem;
    }
  #endif

  if (atoi(JConf.ntp_enable) == 1) {
    data += jsNtp;
  }
  data += javaScriptEnd + bodyAjax + navbarStart + JConf.module_id + navbarStart2 +navbarActive + navbarEnd + containerStart + title1 + panelEnd + title2 + panelEnd +  containerEnd + siteEnd;

  WebServer.send ( 200, "text/html", data);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebRoot() Load Time: ")); Serial.println(load_time);
  #endif
}



void WebReboot(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebReboot() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerRefreshStatus;   headerRefreshStatus += FPSTR(headerRefreshStatusP);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);

  String data = String(F("<div class='col-sm-6 col-md-5 col-lg-4'><div class='page-header'><h2>Reboot ESP</h2></div><div class='alert alert-info' role='alert'><a href='#' class='alert-link'>Rebooting...</a></div></div>"));
  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerRefreshStatus + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebReboot() Load Time: ")); Serial.println(load_time);
  #endif

  ESP.restart();
}



void WebUpdate(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebUpdate() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String sketchUploadForm;      sketchUploadForm += FPSTR(sketchUploadFormP);

  WebServer.send(200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + sketchUploadForm + containerEnd + siteEnd);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebUpdate() Load Time: ")); Serial.println(load_time);
  #endif
}



void WebFileUpload(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebFileUpload() Start"));
  #endif

  if (WebServer.uri() != "/upload_sketch") return;
  HTTPUpload& upload = WebServer.upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.setDebugOutput(true);
    WiFiUDP::stopAll();
    #ifdef DEBUG
    Serial.printf("Sketch: %s\n", upload.filename.c_str());
    #endif
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if (!Update.begin(maxSketchSpace)) { //start with max available size
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) { //true to set the size to the current progress
      #ifdef DEBUG
      Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      #endif
    } else {
      Update.printError(Serial);
    }
    Serial.setDebugOutput(false);
  }
  yield();

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebFileUpload() Load Time: ")); Serial.println(load_time);
  #endif
}



void WebUploadSketch(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebUploadSketch() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerRefreshStatus;   headerRefreshStatus += FPSTR(headerRefreshStatusP);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);

  String varDataString = String(F("<div class='col-sm-6 col-md-5 col-lg-4'><div class='page-header'><h2>Update Frimware</h2></div><div class='alert alert-success'>")) + ((Update.hasError()) ? String(F("FAIL")) : String(F("Update Frimware: OK"))) + String(F("</div></div>"));


  WebServer.send(200, "text/html", headerStart + JConf.module_id + headerStart2 + headerRefreshStatus + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + varDataString + containerEnd + siteEnd);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebUploadSketch() Load Time: ")); Serial.println(load_time);
  #endif

  ESP.restart();
}



void WebWiFiConf(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebWiFiConf() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);
  String panelEnd;              panelEnd += FPSTR(panelEndP);
  String panelBodySymbol;       panelBodySymbol += FPSTR(panelBodySymbolP);
  String panelBodyName;         panelBodyName += FPSTR(panelBodyNameP);
  String panelBodyValue;        panelBodyValue += FPSTR(panelBodyValueP);
  String panelBodyEnd;          panelBodyEnd += FPSTR(panelBodyEndP);
  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  bool config_changed = false;
  bool enable = false;
  String payload = "";

  payload=WebServer.arg("module_id");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.module_id, sizeof(JConf.module_id));
    config_changed = true;
  }

  payload=WebServer.arg("wifi_mode");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.wifi_mode, sizeof(JConf.wifi_mode));
    config_changed = true;
  }

  payload=WebServer.arg("wifi_phy_mode");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.wifi_phy_mode, sizeof(JConf.wifi_phy_mode));
    config_changed = true;
  }
  payload=WebServer.arg("wifi_channel");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.wifi_channel, sizeof(JConf.wifi_channel));
    config_changed = true;
  }
  payload=WebServer.arg("wifi_auth");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.wifi_auth, sizeof(JConf.wifi_auth));
    config_changed = true;
  }

  payload=WebServer.arg("sta_ssid");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.sta_ssid, sizeof(JConf.sta_ssid));
    config_changed = true;
  }

  payload=WebServer.arg("sta_pwd");
  if (payload.length() > 7 &&  payload != String(F("********"))) {
    payload.toCharArray(JConf.sta_pwd, sizeof(JConf.sta_pwd));
    config_changed = true;
  }

  payload=WebServer.arg("ap_pwd");
  if (payload.length() > 7 &&  payload != String(F("********"))) {
    payload.toCharArray(JConf.ap_pwd, sizeof(JConf.ap_pwd));
    config_changed = true;
  }

  payload=WebServer.arg("static_ip_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.static_ip_enable, sizeof(JConf.static_ip_enable));
    config_changed = true;
    enable = true;
  } 

  payload=WebServer.arg("static_ip");
  if (payload.length() > 6 ) {
    payload.toCharArray(JConf.static_ip, sizeof(JConf.static_ip));
    config_changed = true;
  }

  payload=WebServer.arg("static_gateway");
  if (payload.length() > 6 ) {
    payload.toCharArray(JConf.static_gateway, sizeof(JConf.static_gateway));
    config_changed = true;
  }

  payload=WebServer.arg("static_subnet");
  if (payload.length() > 6 ) {
    payload.toCharArray(JConf.static_subnet, sizeof(JConf.static_subnet));
    config_changed = true;
  }

  if (config_changed){
    if (!enable){
      JConf.static_ip_enable[0] = '0';
      JConf.static_ip_enable[1] = '\0';
    }
    JConf.saveConfig();
  }

  String data = panelHeaderName + String(F("Wi-Fi Configuration")) + panelHeaderEnd;
  data += inputBodyStart;

  data += inputBodyName + String(F("Module ID")) + inputBodyPOST + String(F("module_id"))  + inputPlaceHolder + JConf.module_id + inputBodyClose + inputBodyCloseDiv;

  data += String(F("<div class='form-group'><div class='input-group'><span class='input-group-addon'>AP type</span>"));
  if (atoi(JConf.wifi_mode) == STA){
    data += String(F("<select class='form-control' name='wifi_mode'><option value='0'>AP</option><option value='1' selected>STA</option><option value='2'>AP_STA</option></select></div></div>"));
  } else if (atoi(JConf.wifi_mode) == AP_STA){
    data += String(F("<select class='form-control' name='wifi_mode'><option value='0'>AP</option><option value='1'>STA</option><option value='2' selected>AP_STA</option></select></div></div>"));
  } else {
    data += String(F("<select class='form-control' name='wifi_mode'><option value='0' selected>AP</option><option value='1'>STA</option><option value='2'>AP_STA</option></select></div></div>"));
  }

  if ( atoi(JConf.wifi_mode) != STA){
    data += String(F("<div class='form-group'><div class='input-group'><span class='input-group-addon'>AP mode</span>"));
    if (atoi(JConf.wifi_phy_mode) == G){
      data += String(F("<select class='form-control' name='wifi_phy_mode'><option value='0'>11B</option><option value='1' selected>11G</option><option value='2'>11N</option></select></div></div>"));
    } else if (atoi(JConf.wifi_phy_mode) == N){
      data += String(F("<select class='form-control' name='wifi_phy_mode'><option value='0'>11B</option><option value='1'>11G</option><option value='2' selected>11N</option></select></div></div>"));
    } else {
      data += String(F("<select class='form-control' name='wifi_phy_mode'><option value='0' selected>11B</option><option value='1'>11G</option><option value='2'>11N</option></select></div></div>"));
    }

    data += inputBodyName + String(F("AP Channel")) + inputBodyPOST + String(F("wifi_channel"))  + inputPlaceHolder + JConf.wifi_channel + inputBodyClose + inputBodyCloseDiv;

    data += String(F("<div class='form-group'><div class='input-group'><span class='input-group-addon'>AP auth</span>"));
    if (atoi(JConf.wifi_auth) == WPA_PSK){
      data += String(F("<select class='form-control' name='wifi_auth'><option value='0'>OPEN</option><option value='1' selected>WPA_PSK</option><option value='2'>WPA2_PSK</option><option value='3'>WPA_WPA2_PSK</option></select></div></div>"));
    } else if (atoi(JConf.wifi_auth) == WPA2_PSK){
      data += String(F("<select class='form-control' name='wifi_auth'><option value='0'>OPEN</option><option value='1'>WPA_PSK</option><option value='2' selected>WPA2_PSK</option><option value='3'>WPA_WPA2_PSK</option></select></div></div>"));
    } else if (atoi(JConf.wifi_auth) == WPA_WPA2_PSK){
      data += String(F("<select class='form-control' name='wifi_auth'><option value='0'>OPEN</option><option value='1'>WPA_PSK</option><option value='2'>WPA2_PSK</option><option value='3' selected>WPA_WPA2_PSK</option></select></div></div>"));
    } else {
      data += String(F("<select class='form-control' name='wifi_auth'><option value='0' selected>OPEN</option><option value='1'>WPA_PSK</option><option value='2'>WPA2_PSK</option><option value='3'>WPA_WPA2_PSK</option></select></div></div>"));
    }

    if (atoi(JConf.wifi_auth) != OPEN){
      data += inputBodyName + String(F("AP Password")) + String(F("</span><input type='password' name='")) + String(F("ap_pwd")) + inputPlaceHolder + String(F("********")) + inputBodyClose + inputBodyCloseDiv;
    }
  }

  if ( atoi(JConf.wifi_mode) != AP){
    data += inputBodyName + String(F("STA SSID")) + inputBodyPOST + String(F("sta_ssid"))  + inputPlaceHolder + JConf.sta_ssid + inputBodyClose + inputBodyCloseDiv;
    data += inputBodyName + String(F("STA Password")) + String(F("</span><input type='password' name='")) + String(F("sta_pwd")) + inputPlaceHolder + String(F("********")) + inputBodyClose + inputBodyCloseDiv;

    if (atoi(JConf.static_ip_enable) == 1){
      data += String(F("<div class='checkbox'><label><input type='checkbox' name='static_ip_enable' value='1' checked='true'>Static IP Mode</label></div>"));
      data += inputBodyName + String(F("Static IP"))      + inputBodyPOST + String(F("static_ip"))      + inputPlaceHolder + JConf.static_ip      + inputBodyClose + inputBodyCloseDiv;
      data += inputBodyName + String(F("Static Gateway")) + inputBodyPOST + String(F("static_gateway")) + inputPlaceHolder + JConf.static_gateway + inputBodyClose + inputBodyCloseDiv;
      data += inputBodyName + String(F("Static Subnet"))  + inputBodyPOST + String(F("static_subnet"))  + inputPlaceHolder + JConf.static_subnet  + inputBodyClose + inputBodyCloseDiv;
    } else {
      data += String(F("<div class='checkbox'><label><input type='checkbox' name='static_ip_enable' value='1'>Static IP Mode</label></div>"));
    }
  }

  data += inputBodyEnd;
  data += String(F("</div>"));

  data += panelHeaderName + String(F("Scan AP")) + panelHeaderEnd;
  data += network_html;

  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebWiFiConf() Load Time: ")); Serial.println(load_time);
  #endif
}



void WebSensorsConf(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebSensorsConf() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);

  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  String data;
  data += panelHeaderName;
  data += String(F("Sensors Configuration"));
  data += panelHeaderEnd;
  data += inputBodyStart;

  bool config_changed = false;

  bool dht_enable = false;
  bool bme280_enable = false;
  bool sht21_enable = false;
  bool bh1750_enable = false;
  bool motion_sensor_enable = false;
  bool pzem_enable = false;

  String payload = "";

  payload=WebServer.arg("save_conf");
  if (payload.length() > 0) {
    config_changed = true;
  } 

  payload=WebServer.arg("bme280_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.bme280_enable, sizeof(JConf.bme280_enable));
    bme280_enable = true;
  } 

  payload=WebServer.arg("sht21_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.sht21_enable, sizeof(JConf.sht21_enable));
    sht21_enable = true;
  } 

  payload=WebServer.arg("dht_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.dht_enable, sizeof(JConf.dht_enable));
    dht_enable = true;
  } 

  payload=WebServer.arg("bh1750_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.bh1750_enable, sizeof(JConf.bh1750_enable));
    bh1750_enable = true;
  } 

  payload=WebServer.arg("motion_sensor_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.motion_sensor_enable, sizeof(JConf.motion_sensor_enable));
    motion_sensor_enable = true;
  } 

  payload=WebServer.arg("pzem_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.pzem_enable, sizeof(JConf.pzem_enable));
    pzem_enable = true;
  } 


  if (config_changed){
    if (!bme280_enable){
      JConf.bme280_enable[0] = '0';
      JConf.bme280_enable[1] = '\0';
    }
    if (!sht21_enable){
      JConf.sht21_enable[0] = '0';
      JConf.sht21_enable[1] = '\0';
    }
    if (!dht_enable){
      JConf.dht_enable[0] = '0';
      JConf.dht_enable[1] = '\0';
    }
    if (!bh1750_enable){
      JConf.bh1750_enable[0] = '0';
      JConf.bh1750_enable[1] = '\0';
    }
    if (!motion_sensor_enable){
      JConf.motion_sensor_enable[0] = '0';
      JConf.motion_sensor_enable[1] = '\0';
    }
    if (!pzem_enable){
      JConf.pzem_enable[0] = '0';
      JConf.pzem_enable[1] = '\0';
    }
    JConf.saveConfig();
  }

  if (atoi(JConf.bme280_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='bme280_enable' value='1' checked='true'>BME280 Enable</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='bme280_enable' value='1'>BME280 Enable</label></div>"));
  }

  if (atoi(JConf.sht21_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='sht21_enable' value='1' checked='true'>SHT21 Enable</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='sht21_enable' value='1'>SHT21 Enable</label></div>"));
  }

  if (atoi(JConf.dht_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='dht_enable' value='1' checked='true'>DHT Enable</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='dht_enable' value='1'>DHT Enable</label></div>"));
  }

  if (atoi(JConf.bh1750_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='bh1750_enable' value='1' checked='true'>BH1750 Enable</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='bh1750_enable' value='1'>BH1750 Enable</label></div>"));
  }

  if (atoi(JConf.motion_sensor_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='motion_sensor_enable' value='1' checked='true'>Motion Sensor Enable</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='motion_sensor_enable' value='1'>Motion Sensor Enable</label></div>"));
  }

  if (atoi(JConf.pzem_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='pzem_enable' value='1' checked='true'>Energy Monitor Enable</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='pzem_enable' value='1'>Energy Monitor Enable</label></div>"));
  }

  data += inputBodyEnd;

  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebSensorsConf() Load Time: ")); Serial.println(load_time);
  #endif
}



void WebEspConf(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebEspConf() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);
  String panelEnd;              panelEnd += FPSTR(panelEndP);
  String panelBodySymbol;       panelBodySymbol += FPSTR(panelBodySymbolP);
  String panelBodyName;         panelBodyName += FPSTR(panelBodyNameP);
  String panelBodyValue;        panelBodyValue += FPSTR(panelBodyValueP);
  String panelBodyEnd;          panelBodyEnd += FPSTR(panelBodyEndP);
  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  bool config_changed = false;
  bool enable_light_smooth = false;
  bool enable_light2_smooth = false;
  String payload = "";

  payload=WebServer.arg("light_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light_pin, sizeof(JConf.light_pin));
    config_changed = true;
  }

  payload=WebServer.arg("lightoff_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.lightoff_delay, sizeof(JConf.lightoff_delay));
    if (atoi(JConf.mqtt_enable) == 1){
      MqttPubLightOffDelay();
    }
    config_changed = true;
  }

  payload=WebServer.arg("lighton_lux");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.lighton_lux, sizeof(JConf.lighton_lux));
    config_changed = true;
  }

  payload=WebServer.arg("light_smooth");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light_smooth, sizeof(JConf.light_smooth));
    config_changed = true;
    enable_light_smooth = true;
  }

  payload=WebServer.arg("light2_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light2_pin, sizeof(JConf.light2_pin));
    config_changed = true;
  }

  payload=WebServer.arg("light2off_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light2off_delay, sizeof(JConf.light2off_delay));
    if (atoi(JConf.mqtt_enable) == 1){
      MqttPubLightOffDelay();
    }
    config_changed = true;
  }

  payload=WebServer.arg("light2on_lux");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light2on_lux, sizeof(JConf.light2on_lux));
    config_changed = true;
  }

  payload=WebServer.arg("light2_smooth");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.light2_smooth, sizeof(JConf.light2_smooth));
    config_changed = true;
    enable_light2_smooth = true;
  }

  payload=WebServer.arg("motion_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.motion_pin, sizeof(JConf.motion_pin));
    config_changed = true;
  }

  payload=WebServer.arg("dht_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.dht_pin, sizeof(JConf.dht_pin));
    config_changed = true;
  }

  payload=WebServer.arg("get_data_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.get_data_delay, sizeof(JConf.get_data_delay));
    config_changed = true;
  }

  payload=WebServer.arg("motion_read_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.motion_read_delay, sizeof(JConf.motion_read_delay));
    config_changed = true;
  }

  payload=WebServer.arg("reboot_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.reboot_delay, sizeof(JConf.reboot_delay));
    config_changed = true;
  }

  if (config_changed){
    if (!enable_light_smooth){
      JConf.light_smooth[0] = '0';
      JConf.light_smooth[1] = '\0';
    }
    if (!enable_light2_smooth){
      JConf.light2_smooth[0] = '0';
      JConf.light2_smooth[1] = '\0';
    }
    JConf.saveConfig();
  }

  String data = panelHeaderName + String(F("ESP Configuration")) + panelHeaderEnd;
  data += inputBodyStart;

  data += String(F("<h4>Light 1</h4>"));
  data += inputBodyName + String(F("Pin")) + inputBodyPOST + String(F("light_pin")) + inputPlaceHolder + JConf.light_pin + inputBodyClose + inputBodyCloseDiv;
  data += inputBodyName + String(F("Off Delay")) + inputBodyPOST + String(F("lightoff_delay")) + inputPlaceHolder + JConf.lightoff_delay + inputBodyClose + inputBodyUnitStart + String(F("min")) + inputBodyUnitEnd + inputBodyCloseDiv;
  data += inputBodyName + String(F("On Lux")) + inputBodyPOST + String(F("lighton_lux")) + inputPlaceHolder + JConf.lighton_lux + inputBodyClose + inputBodyUnitStart + String(F("Lux")) + inputBodyUnitEnd + inputBodyCloseDiv;

  if (atoi(JConf.light_smooth) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='light_smooth' value='1' checked='true'>Smooth Enable</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='light_smooth' value='1'>Smooth Enable</label></div>"));
  }
  data += String(F("<hr>"));

  data += String(F("<h4>Light 2</h4>"));
  data += inputBodyName + String(F("Pin")) + inputBodyPOST + String(F("light2_pin")) + inputPlaceHolder + JConf.light2_pin + inputBodyClose + inputBodyCloseDiv;
  data += inputBodyName + String(F("Off Delay")) + inputBodyPOST + String(F("light2off_delay")) + inputPlaceHolder + JConf.light2off_delay + inputBodyClose + inputBodyUnitStart + String(F("min")) + inputBodyUnitEnd + inputBodyCloseDiv;
  data += inputBodyName + String(F("On Lux")) + inputBodyPOST + String(F("light2on_lux")) + inputPlaceHolder + JConf.light2on_lux + inputBodyClose + inputBodyUnitStart + String(F("Lux")) + inputBodyUnitEnd + inputBodyCloseDiv;

  if (atoi(JConf.light2_smooth) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='light2_smooth' value='1' checked='true'>Smooth Enable</label></div>"));
  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='light2_smooth' value='1'>Smooth Enable</label></div>"));
  }
  data += String(F("<hr>"));

  data += inputBodyName + String(F("Motion Pin")) + inputBodyPOST + String(F("motion_pin")) + inputPlaceHolder + JConf.motion_pin + inputBodyClose + inputBodyCloseDiv;
  data += inputBodyName + String(F("DHT Pin")) + inputBodyPOST + String(F("dht_pin")) + inputPlaceHolder + JConf.dht_pin + inputBodyClose + inputBodyCloseDiv;
  data += String(F("<br>"));

  data += inputBodyName + String(F("Update Data Delay")) + inputBodyPOST + String(F("get_data_delay")) + inputPlaceHolder + JConf.get_data_delay + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;
  data += inputBodyName + String(F("Motion Read Delay")) + inputBodyPOST + String(F("motion_read_delay")) + inputPlaceHolder + JConf.motion_read_delay + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;
  data += inputBodyName + String(F("Reboot Delay")) + inputBodyPOST + String(F("reboot_delay")) + inputPlaceHolder + JConf.reboot_delay + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;

  data += inputBodyEnd;

  WebServer.send( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebEspConf() Load Time: ")); Serial.println(load_time);
  #endif
}



void WebMqttConf(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebMqttConf() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);

  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  String data;
  data += panelHeaderName;
  data += String(F("MQTT Configuration"));
  data += panelHeaderEnd;
  data += inputBodyStart;

  bool config_changed = false;
  bool enable = false;
  String payload = "";

  payload=WebServer.arg("mqtt_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.mqtt_enable, sizeof(JConf.mqtt_enable));
    config_changed = true;
    enable = true;
  } 

  payload=WebServer.arg("mqtt_server");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.mqtt_server, sizeof(JConf.mqtt_server));
    config_changed = true;
  }

  payload=WebServer.arg("mqtt_port");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.mqtt_port, sizeof(JConf.mqtt_port));
    config_changed = true;
  }

  payload=WebServer.arg("mqtt_user");
  if (payload.length() > 0 ) {
    if (payload == "0"){
      String data = "";
      data.toCharArray(JConf.mqtt_user, sizeof(JConf.mqtt_user));
      data.toCharArray(JConf.mqtt_pwd, sizeof(JConf.mqtt_pwd));
    } else {
      payload.toCharArray(JConf.mqtt_user, sizeof(JConf.mqtt_user));
    }
    config_changed = true;
  }

  payload=WebServer.arg("mqtt_pwd");
  if (payload.length() > 0 ) {
    if (payload == "0"){
      String data = "";
      data.toCharArray(JConf.mqtt_user, sizeof(JConf.mqtt_user));
      data.toCharArray(JConf.mqtt_pwd, sizeof(JConf.mqtt_pwd));
    } else {
      payload.toCharArray(JConf.mqtt_pwd, sizeof(JConf.mqtt_pwd));
    }
    config_changed = true;
  } 

  payload=WebServer.arg("mqtt_name");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.mqtt_name, sizeof(JConf.mqtt_name));
    config_changed = true;
  }

  payload=WebServer.arg("publish_topic");
  if (payload.length() > 0 ) {
    payload.replace("%2F", String(F("/")));
    payload.toCharArray(JConf.publish_topic, sizeof(JConf.publish_topic));
    config_changed = true;
  }

  payload=WebServer.arg("subscribe_topic");
  if (payload.length() > 0 ) {
    payload.replace("%2F", String(F("/")));
    payload.toCharArray(JConf.subscribe_topic, sizeof(JConf.subscribe_topic));
    config_changed = true;
  }

  payload=WebServer.arg("publish_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.publish_delay, sizeof(JConf.publish_delay));
    config_changed = true;
  }

  payload=WebServer.arg("subscribe_delay");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.subscribe_delay, sizeof(JConf.subscribe_delay));
    config_changed = true;
  }

  if (config_changed){
    if (!enable){
      JConf.mqtt_enable[0] = '0';
      JConf.mqtt_enable[1] = '\0';
    }
    JConf.saveConfig();
  }

  if (atoi(JConf.mqtt_enable) == 1){

    data += String(F("<div class='checkbox'><label><input type='checkbox' name='mqtt_enable' value='1' checked='true'>MQTT Enable</label></div>"));
    data += inputBodyName + String(F("Server MQTT")) + inputBodyPOST + String(F("mqtt_server")) + inputPlaceHolder + JConf.mqtt_server + inputBodyClose + inputBodyCloseDiv;
    data += inputBodyName + String(F("Port MQTT")) + inputBodyPOST + String(F("mqtt_port")) + inputPlaceHolder + JConf.mqtt_port + inputBodyClose + inputBodyCloseDiv;

    if (strlen(JConf.mqtt_user) != 0){
      data += String(F("<div><input type='hidden' name='mqtt_user' value='0'></div>"));
    } 
    data += inputBodyName + String(F("MQTT User")) + inputBodyPOST + String(F("mqtt_user")) + inputPlaceHolder + JConf.mqtt_user + inputBodyClose + inputBodyCloseDiv;

    if (strlen(JConf.mqtt_pwd) != 0){
      data += String(F("<div><input type='hidden' name='mqtt_pwd' value='0'></div>"));
    } 
    data += inputBodyName + String(F("MQTT Password")) + inputBodyPOST + String(F("mqtt_pwd")) + inputPlaceHolder + JConf.mqtt_pwd + inputBodyClose + inputBodyCloseDiv;

    data += inputBodyName + String(F("MQTT Prefix")) + inputBodyPOST + String(F("mqtt_name")) + inputPlaceHolder + JConf.mqtt_name + inputBodyClose + inputBodyCloseDiv;
    data += inputBodyName + String(F("Publish Topic")) + inputBodyPOST + String(F("publish_topic")) + inputPlaceHolder + JConf.publish_topic + inputBodyClose + inputBodyCloseDiv;
    data += inputBodyName + String(F("Subscribe Topic")) + inputBodyPOST + String(F("subscribe_topic")) + inputPlaceHolder + JConf.subscribe_topic + inputBodyClose + inputBodyCloseDiv;
    data += inputBodyName + String(F("Publish Delay")) + inputBodyPOST + String(F("publish_delay")) + inputPlaceHolder + JConf.publish_delay + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;
    data += inputBodyName + String(F("Subscribe Delay")) + inputBodyPOST + String(F("subscribe_delay")) + inputPlaceHolder + JConf.subscribe_delay + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;

  } else {
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='mqtt_enable' value='1'>MQTT Enable</label></div>"));
  }

  data += inputBodyEnd;

  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebMqttConf() Load Time: ")); Serial.println(load_time);
  #endif
}



void WebNTPConf(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebNTPConf() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);

  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  String data;
  data += panelHeaderName;
  data += String(F("NTP Configuration"));
  data += panelHeaderEnd;
  data += inputBodyStart;

  bool config_changed = false;
  bool enable = false;
  String payload = "";

  payload=WebServer.arg("ntp_enable");
  if (payload.length() > 0) {
    payload.toCharArray(JConf.ntp_enable, sizeof(JConf.ntp_enable));
    config_changed = true;
    enable = true;
  } 

  payload=WebServer.arg("ntp_server");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.ntp_server, sizeof(JConf.ntp_server));
    config_changed = true;
  }

  payload=WebServer.arg("my_time_zone");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.my_time_zone, sizeof(JConf.my_time_zone));
    config_changed = true;
  }

  if (config_changed){
    if (!enable){
      JConf.ntp_enable[0] = '0';
      JConf.ntp_enable[1] = '\0';
    }
    JConf.saveConfig();
    #ifdef NTP_ON
      NTPSettingsUpdate();
    #endif
  }

  if (atoi(JConf.ntp_enable) == 1){
    data += String(F("<div class='checkbox'><label><input type='checkbox' name='ntp_enable' value='1' checked='true'>NTP Enable</label></div>"));

    data += inputBodyName + String(F("Server NTP")) + inputBodyPOST + String(F("ntp_server")) + inputPlaceHolder + JConf.ntp_server + inputBodyClose + inputBodyCloseDiv;

    data += inputBodyName + String(F("Time Zone")) + inputBodyPOST + String(F("my_time_zone")) + inputPlaceHolder + JConf.my_time_zone + inputBodyClose + inputBodyCloseDiv;

  } else {

    data += String(F("<div class='checkbox'><label><input type='checkbox' name='ntp_enable' value='1'>NTP Enable</label></div>"));
  }

  data += inputBodyEnd;


  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebNTPConf() Load Time: ")); Serial.println(load_time);
  #endif
}



void handleControl(){

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("handleControl() Start"));
  #endif

  String AUTO;       AUTO += FPSTR(AUTOP);
  String ON;         ON += FPSTR(ONP);
  String OFF;        OFF += FPSTR(OFFP);

  String last_light_state = lightState;
  String last_light_state2 = lightState2;

  if (WebServer.args() > 0 ) {
    for ( size_t i = 0; i < WebServer.args(); i++ ) {
      if (WebServer.argName(i) == "1" && WebServer.arg(i) == "1") {
        if (cycleEnd[atoi(JConf.light_pin)] != 0){
          lightState = OFF;
        } else {
          lightState = ON;
        }
      }
      if (WebServer.argName(i) == "1" && WebServer.arg(i) == "2") {
        lightState = AUTO;
      }

      if (WebServer.argName(i) == "2" && WebServer.arg(i) == "1") {
        if (cycleEnd[atoi(JConf.light2_pin)] != 0){
          lightState2 = OFF;
        } else {
          lightState2 = ON;
        }
      }
      if (WebServer.argName(i) == "2" && WebServer.arg(i) == "2") {
        lightState2 = AUTO;
      }
      #ifdef DEBUG
      Serial.println(WebServer.argName(i));
      Serial.println(WebServer.arg(i));
      #endif
      if (last_light_state != lightState || last_light_state2 != lightState2){
        LightControl();
        if (atoi(JConf.mqtt_enable) == 1) {
          MqttPubLightState();
        }
      }
    }
  }

  WebServer.send ( 200, "text/html", "OK");

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("handleControl() Load Time: ")); Serial.println(load_time);
  #endif
}



void WebPinControl(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebPinControl() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);

  String javaScriptPinControl;               javaScriptPinControl += FPSTR(javaScriptPinControlP);

  String pinControl = headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + javaScriptPinControl + containerEnd + siteEnd;

  WebServer.send ( 200, "text/html", pinControl);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebPinControl() Load Time: ")); Serial.println(load_time);
  #endif
}



void WebPinControlStatus(void) {

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebPinControlStatus() Start"));
  #endif

  LightControl();

  bool pinState;
  bool pinState2;

  String ClassInfo;       ClassInfo += FPSTR(ClassInfoP);
  String ClassDanger;     ClassDanger += FPSTR(ClassDangerP);
  String ClassDefault;    ClassDefault += FPSTR(ClassDefaultP);
  String ClassSuccess;    ClassSuccess += FPSTR(ClassSuccessP);
  String AUTO;            AUTO += FPSTR(AUTOP);
  String ON;              ON += FPSTR(ONP);
  String OFF;             OFF += FPSTR(OFFP);

  if (cycleEnd[atoi(JConf.light_pin)] != 0){
    pinState = true;
  } else {
    pinState = false;
  }

  if (cycleEnd[atoi(JConf.light2_pin)] != 0){
    pinState2 = true;
  } else {
    pinState2 = false;
  }

  String mode;
  if (lightState == AUTO){
    mode = ClassSuccess;
  } else if (lightState == ON) {
    mode = ClassInfo;
  } else {
    mode = ClassDanger;
  }

  String mode2;
  if (lightState2 == AUTO){
    mode2 = ClassSuccess;
  } else if (lightState2 == ON) {
    mode2 = ClassInfo;
  } else {
    mode2 = ClassDanger;
  }

  unsigned long timeOff = 0;
  if (millis() - lightOffTimer < atoi(JConf.lightoff_delay) * 60 * 1000){
    timeOff = atoi(JConf.lightoff_delay) * 60 * 1000 - (millis() - lightOffTimer);
    timeOff = timeOff/1000;
  }

  unsigned long timeOff2 = 0;
  if (millis() - lightOffTimer2 < atoi(JConf.light2off_delay) * 60 * 1000){
    timeOff2 = atoi(JConf.light2off_delay) * 60 * 1000 - (millis() - lightOffTimer2);
    timeOff2 = timeOff2/1000;
  }

  String data;    data += FPSTR(div1P);
  
  if (lightState == AUTO) { data+=ClassDefault; } else if (pinState) { data+=ClassDanger; } else { data+=ClassInfo; }
  data+=String(F("' value='"));
  if (pinState) { data+=String(F("Turn Off")); } else { data+=String(F("Turn On")); }
  data+=String(F("'></div></td><td class='active'><div onclick='Auto1();'><input id='Auto' type='submit' class='btn btn-"));
  if (lightState == AUTO) { data+=ClassDanger; } else { data+=ClassDefault; }
  data+=String(F("' value='Auto'></div></td><td class='"));
  if (pinState) { data+=ClassInfo; } else { data+=ClassDanger; }
  data+=String(F("'><h4>"));
  if (pinState) { data+=ON; } else { data+=OFF; }
  data+=String(F("</h4></td><td class='"));
  data+=mode;    
  data+=String(F("'><h4>"));
  data+=lightState;
  data+=String(F("</h4></td><td class='"));
  data+=String(F("active"));
  data+=String(F("'><h4>"));
  data+=String(timeOff);
  data+=String(F("</h4></td></tr>"));

  data+=String(F("<tr><td class='active'><h4>Light2</h4></td><td class='active'><div onclick='Pin2();'><input id='OnOff2' type='submit' class='btn btn-"));
  if (lightState2 == AUTO) { data+=ClassDefault; } else if (pinState2) { data+=ClassDanger; } else { data+=ClassInfo; }
  data+=String(F("' value='"));
  if (pinState2) { data+=String(F("Turn Off")); } else { data+=String(F("Turn On")); }
  data+=String(F("'></div></td><td class='active'><div onclick='Auto2();'><input id='Auto2' type='submit' class='btn btn-"));
  if (lightState2 == AUTO) { data+=ClassDanger; } else { data+=ClassDefault; }
  data+=String(F("' value='Auto'></div></td><td class='"));
  if (pinState2) { data+=ClassInfo; } else { data+=ClassDanger; }
  data+=String(F("'><h4>"));
  if (pinState2) { data+=ON; } else { data+=OFF; }
  data+=String(F("</h4></td><td class='"));
  data+=mode2;    
  data+=String(F("'><h4>"));
  data+=lightState2;
  data+=String(F("</h4></td><td class='"));
  data+=String(F("active"));
  data+=String(F("'><h4>"));
  data+=String(timeOff2);
  data+=String(F("</h4></td></tr>"));
  data+=String(F("</tbody></table></div>"));

  WebServer.send ( 200, "text/html", data);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebPinControlStatus() Load Time: ")); Serial.println(load_time);
  #endif
}



#if defined(UART_ON)
void WebAnalogUart(void) {
  #ifdef DEBUG
    Serial.print(F("WebAnalogUart()"));  Serial.println();
  #endif

    String headerStart;           headerStart += FPSTR(headerStartP);
    String headerStart2;          headerStart2 += FPSTR(headerStart2P);
    String headerEnd;             headerEnd += FPSTR(headerEndP);
    String javaScript;            javaScript += FPSTR(javaScriptP);
    String javaScript2;           javaScript2 += FPSTR(javaScript2P);
    String bodyAjax;              bodyAjax += FPSTR(bodyAjaxP);
    String navbarStart;           navbarStart += FPSTR(navbarStartP);
    String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
    String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

    navbarNonActive += FPSTR(navbarBeginP);
    navbarNonActive += FPSTR(navbarUartP);

    String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
    String containerStart;        containerStart += FPSTR(containerStartP);
    String containerEnd;          containerEnd += FPSTR(containerEndP);
    String siteEnd;               siteEnd += FPSTR(siteEndP);
    String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
    String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);
    String panelEnd;              panelEnd += FPSTR(panelEndP);
    String panelBodySymbol;       panelBodySymbol += FPSTR(panelBodySymbolP);
    String panelBodyName;         panelBodyName += FPSTR(panelBodyNameP);
    String panelBodyValue;        panelBodyValue += FPSTR(panelBodyValueP);
    String closingAngleBracket;   closingAngleBracket += FPSTR(closingAngleBracketP);
    String panelBodyEnd;          panelBodyEnd += FPSTR(panelBodyEndP);

    String title1  = panelHeaderName + String(F("Analog Pins value"))   + panelHeaderEnd;

    String ApinValue = panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 0")) + panelBodyValue + String(F(" id='apin0Id'")) + closingAngleBracket + panelBodyEnd;
    ApinValue       += panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 1")) + panelBodyValue + String(F(" id='apin1Id'")) + closingAngleBracket + panelBodyEnd;
    ApinValue       += panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 2")) + panelBodyValue + String(F(" id='apin2Id'")) + closingAngleBracket + panelBodyEnd;
    ApinValue       += panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 3")) + panelBodyValue + String(F(" id='apin3Id'")) + closingAngleBracket + panelBodyEnd;
    ApinValue       += panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 4")) + panelBodyValue + String(F(" id='apin4Id'")) + closingAngleBracket + panelBodyEnd;
    ApinValue       += panelBodySymbol + String(F("fire")) + panelBodyName + String(F("Analog pin 5")) + panelBodyValue + String(F(" id='apin5Id'")) + closingAngleBracket + panelBodyEnd;

    
    String title2 = panelHeaderName + String(F("Analog Pins delay"))  + panelHeaderEnd;

    String ApinDelay = panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 0")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin0 + panelBodyEnd;
    ApinDelay       += panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 1")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin1 + panelBodyEnd;
    ApinDelay       += panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 2")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin2 + panelBodyEnd;
    ApinDelay       += panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 3")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin3 + panelBodyEnd;
    ApinDelay       += panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 4")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin4 + panelBodyEnd;
    ApinDelay       += panelBodySymbol + String(F("time")) + panelBodyName + String(F("Analog pin 5")) + panelBodyValue + closingAngleBracket + JConf.uart_delay_analog_pin5 + panelBodyEnd;
    
    WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + javaScript + javaScript2 + bodyAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + title1 + ApinValue + panelEnd + title2 + ApinDelay + panelEnd + containerEnd + siteEnd);
}
#endif



void WebGreenhouse(void) {
  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebGreenhouse() Start"));
  #endif

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerStart2;          headerStart2 += FPSTR(headerStart2P);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarStart2;          navbarStart2 += FPSTR(navbarStart2P);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);

  navbarNonActive += FPSTR(navbarBeginP);
  #ifdef UART_ON
    navbarNonActive += FPSTR(navbarUartP);
  #endif

  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);
  String panelHeaderName;       panelHeaderName += FPSTR(panelHeaderNameP);
  String panelHeaderEnd;        panelHeaderEnd += FPSTR(panelHeaderEndP);

  String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
  String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
  String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
  String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
  String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
  String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
  String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
  String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
  String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

  String data;
  data += panelHeaderName;
  data += String(F("Подсветка"));
  data += panelHeaderEnd;
  data += inputBodyStart;

  String payload=WebServer.arg("green_light_on");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_light_on, sizeof(JConf.green_light_on));
  }
  data += inputBodyName + String(F("Время включения")) + inputBodyPOST + String(F("green_light_on")) + inputPlaceHolder + JConf.green_light_on + inputBodyClose + inputBodyCloseDiv;

  payload=WebServer.arg("green_light_off");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_light_off, sizeof(JConf.green_light_off));
  }
  data += inputBodyName + String(F("Время выключения")) + inputBodyPOST + String(F("green_light_off")) + inputPlaceHolder + JConf.green_light_off + inputBodyClose + inputBodyCloseDiv;

  payload=WebServer.arg("green_light_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_light_pin, sizeof(JConf.green_light_pin));
  }
  data += inputBodyName + String(F("Пин лампы")) + inputBodyPOST + String(F("green_light_pin")) + inputPlaceHolder + JConf.green_light_pin + inputBodyClose + inputBodyCloseDiv;

  data += inputBodyEnd;

  data += String(F("<div class='page-header'><h2>"));
  data += String(F("Влажность почвы"));
  data += panelHeaderEnd;
  data += inputBodyStart;

  payload=WebServer.arg("green_humidity_threshold_up");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_humidity_threshold_up, sizeof(JConf.green_humidity_threshold_up));
  }
  data += inputBodyName + String(F("Верхний порог")) + inputBodyPOST + String(F("green_humidity_threshold_up")) + inputPlaceHolder + JConf.green_humidity_threshold_up + inputBodyClose + inputBodyCloseDiv;

  payload=WebServer.arg("green_humidity_threshold_down");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_humidity_threshold_down, sizeof(JConf.green_humidity_threshold_down));
  }
  data += inputBodyName + String(F("Нижний порог")) + inputBodyPOST + String(F("green_humidity_threshold_down")) + inputPlaceHolder + JConf.green_humidity_threshold_down + inputBodyClose + inputBodyCloseDiv;

  payload=WebServer.arg("green_humidity_sensor_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_humidity_sensor_pin, sizeof(JConf.green_humidity_sensor_pin));
  }
  data += inputBodyName + String(F("Пин датчика")) + inputBodyPOST + String(F("green_humidity_sensor_pin")) + inputPlaceHolder + JConf.green_humidity_sensor_pin + inputBodyClose + inputBodyCloseDiv;

  payload=WebServer.arg("green_pump_pin");
  if (payload.length() > 0 ) {
    payload.toCharArray(JConf.green_pump_pin, sizeof(JConf.green_pump_pin));
  }
  data += inputBodyName + String(F("Пин насоса")) + inputBodyPOST + String(F("green_pump_pin")) + inputPlaceHolder + JConf.green_pump_pin + inputBodyClose + inputBodyCloseDiv;

  data += inputBodyEnd;

  JConf.saveConfig();

  WebServer.send ( 200, "text/html", headerStart + JConf.module_id + headerStart2 + headerEnd + bodyNonAjax + navbarStart + JConf.module_id + navbarStart2 +navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebGreenhouse() Load Time: ")); Serial.println(load_time);
  #endif
}
///////////////////////////////////   WEB PAGES  End  //////////////////////////////////////////////



void handleXML(){
  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("handleXML() Start"));
  #endif
  String XML;
  XML=String(F("<?xml version='1.0'?>"));
  XML+=String(F("<Donnees>")); 
  XML+=String(F("<temperature>"));
  XML+=temperatureString;
  XML+=String(F(" °C"));
  XML+=String(F("</temperature>"));
  XML+=String(F("<humidity>"));
  XML+=humidityString;
  XML+=String(F(" %"));
  XML+=String(F("</humidity>"));
  XML+=String(F("<pressure>"));
  XML+=pressureString;
  XML+=String(F(" mm"));
  XML+=String(F("</pressure>"));
  XML+=String(F("<illuminance>"));
  XML+=luxString;
  XML+=String(F(" lux"));
  XML+=String(F("</illuminance>"));

  #ifdef PZEM_ON
  XML+=String(F("<pzemVoltage>"));
  XML+=pzemVoltageString;
  XML+=String(F(" V"));
  XML+=String(F("</pzemVoltage>"));

  XML+=String(F("<pzemCurrent>"));
  XML+=pzemCurrentString;
  XML+=String(F(" A"));
  XML+=String(F("</pzemCurrent>"));

  XML+=String(F("<pzemPower>"));
  XML+=pzemPowerString;
  XML+=String(F(" kW"));
  XML+=String(F("</pzemPower>"));

  XML+=String(F("<pzemEnergy>"));
  XML+=pzemEnergyString;
  XML+=String(F(" kWh"));
  XML+=String(F("</pzemEnergy>"));
  #endif

  XML+=String(F("<uptime>"));
  XML+=uptimeString;
  XML+=String(F("</uptime>"));

  if (atoi(JConf.ntp_enable) == 1) {
    XML+=String(F("<ntpTime>"));
    XML+=ntpTimeString;
    XML+=String(F("</ntpTime>"));
  }

  XML+=String(F("<vcc>"));
  XML+=String(voltage_float);
  XML+=String(F(" V"));
  XML+=String(F("</vcc>"));
  XML+=String(F("<freeMemory>"));
  XML+=freeMemoryString;
  XML+=String(F("</freeMemory>"));

  #ifdef UART_ON
  for (int i = 0; i < ANALOG_PINS; i++){
    XML+=String(F("<apin"));  XML+=String(i);  XML+=String(F(">"));
    XML+=String(Uart.valueAnalogPin[i]);
    XML+=String(F("</apin")); XML+=String(i);  XML+=String(F(">"));
  }
  #endif

  XML+=String(F("</Donnees>")); 

  WebServer.send(200,"text/xml",XML);

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("handleXML() Load Time: ")); Serial.println(load_time);
  #endif
}



void WebServerInit()
{

  #ifdef DEBUG
    unsigned long start_time = millis();
    Serial.println(F("WebServerInit() Start"));
  #endif

  // Prepare webserver pages
  WebServer.on("/", WebRoot);
  WebServer.on("/reboot", WebReboot);
  WebServer.on("/update", WebUpdate);
  WebServer.onFileUpload(WebFileUpload);
  WebServer.on("/upload_sketch", WebUploadSketch);
  WebServer.on("/wificonf", WebWiFiConf);
  WebServer.on("/sensorsconf", WebSensorsConf);
  WebServer.on("/espconf", WebEspConf);
  WebServer.on("/mqttconf", WebMqttConf);
  WebServer.on("/ntpconf", WebNTPConf);
  WebServer.on("/control", handleControl);
  WebServer.on("/pincontrol", WebPinControl);
  WebServer.on("/controlstatus", WebPinControlStatus);

#ifdef UART_ON
  WebServer.on("/analog", WebAnalogUart);
#endif

  WebServer.on("/greenhouse", WebGreenhouse);

  WebServer.on("/xml",handleXML);

/*
  WebServer.on("/upload", HTTP_GET, handle_upload);
  WebServer.on("/upload", HTTP_POST, handle_upload_post, handleFileUpload);
  WebServer.onNotFound(handleNotFound);

  if (ESP.getFlashChipRealSize() > 524288)
    httpUpdater.setup(&WebServer);
*/
  WebServer.begin();

  #ifdef DEBUG
    unsigned long load_time = millis() - start_time;
    Serial.print(F("WebServerInit() Load Time: ")); Serial.println(load_time);
  #endif
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
      DHT22Sensor();
    }
  #endif

  #ifdef PZEM_ON
    if (atoi(JConf.pzem_enable) == 1){
      GetPzemSerialRead();
    }
  #endif

  GetUptimeData();
  GetFreeMemory();
  GetMacString();

  #ifdef DEBUG
    TestSystemPrint();
  #endif

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



void restartESP() {
  ESP.restart();
}



void deleteConfigFile() {
  pinMode(atoi(JConf.reset_pin), INPUT); 
  if (digitalRead(atoi(JConf.reset_pin)) == LOW) {
    delay(3000);
    if (digitalRead(atoi(JConf.reset_pin)) == LOW) {
      #ifdef DEBUG
        Serial.println(F("Reset pin pressed. Delete config file."));
      #endif
      JConf.deleteConfig();
    }
  } 
}



void setup() {

  #ifdef DEBUG
    Serial.begin(115200);
    delay(10);
    Serial.println();
  #endif

  #ifdef PZEM_ON
    Serial.begin(9600);
    delay(500);
    Serial.println();
  #endif
  
  if (!SPIFFS.begin()) {
    #ifdef DEBUG
      Serial.println(F("Failed to mount file system"));
    #endif

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
    #ifdef DEBUG
      Serial.println(F("Failed to load config"));
    #endif
  } else {
    #ifdef DEBUG
      Serial.println(F("Config loaded"));
    #endif
  }
  JConf.printConfig();

  pinMode(atoi(JConf.light_pin), OUTPUT);
  pinMode(atoi(JConf.light2_pin), OUTPUT);
  pinMode(atoi(JConf.motion_pin), INPUT);

  digitalWrite(atoi(JConf.light_pin), LOW);
  digitalWrite(atoi(JConf.light2_pin), LOW);

  #ifdef PZEM_ON
    pzem.setAddress(ip_pzem);
    pzem.setReadTimeout(500);
  #endif

  #ifdef DHT_ON
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

  scanWiFi();  // scan Access Points

  if (!WiFiSetup()) {
    WiFiSafeSetup();
  }
  delay(1000);

  if (atoi(JConf.mqtt_enable) == 1) {
    mqttClient.setClient(espClient);

    if (isIPValid(JConf.mqtt_server)){
      IPAddress mqtt_ip = stringToIp(JConf.mqtt_server);
      mqttClient.setServer(mqtt_ip, atoi(JConf.mqtt_port));
    } else {
      mqttClient.setServer(JConf.mqtt_server, atoi(JConf.mqtt_port));
    }
    
    mqttClient.setCallback (callback);
  }

  WebServerInit();

  #ifdef NTP_ON
    if (atoi(JConf.ntp_enable) == 1) {
      timeClient.setUpdateServer(JConf.ntp_server);
      timeClient.setTimeOffset(atoi(JConf.my_time_zone) * 60 * 60);
      timeClient.setUpdateInterval(60*60*1000);
      timeClient.begin();
    }
  #endif

  timer.setInterval(atoi(JConf.get_data_delay) * 1000, getData);
  timer.setInterval(atoi(JConf.publish_delay) * 1000, MqttPubData);

  if (atoi(JConf.motion_sensor_enable) == 1){
    timer.setInterval(atoi(JConf.motion_read_delay) * 1000, MotionDetect);
  }

  subscribeTimer = timer.setInterval(atoi(JConf.subscribe_delay) * 1000, MqttSubscribe);
  timer.setInterval(600000, wifiSafeModeReconnect);

  #ifdef REBOOT_ON
    rebootTimer = timer.setInterval(atoi(JConf.reboot_delay) * 1000, restartESP);
  #endif
}



void loop() {

  WebServer.handleClient();  // handle web server

  timer.run();

  if (atoi(JConf.motion_sensor_enable) == 1 && motionDetect == false){
    MotionDetect();
  }

  String AUTO;   AUTO += FPSTR(AUTOP);
  if (lightState == AUTO){
    LightControl();
  }

  if (WiFi.status() != WL_CONNECTED && atoi(JConf.wifi_mode) != AP && wifiSafeMode == false) {
    #ifdef DEBUG
      Serial.print(F("Connecting "));
      Serial.println(F("..."));
    #endif

    WiFiSetup();
  }

  if (WiFi.status() == WL_CONNECTED) {

    if (atoi(JConf.mqtt_enable) == 1) {

      if (!mqttClient.connected()) {
        if (JConf.mqtt_user != "" && JConf.mqtt_pwd != ""){
          mqttClient.connect(JConf.mqtt_name, JConf.mqtt_user, JConf.mqtt_pwd);
        } else {
          mqttClient.connect(JConf.mqtt_name);
        }
      } else {
        mqttClient.loop();
      }
    }
  }

  #ifdef UART_ON
    Uart.serialEvent();
  #endif

  FadeSwitchLoop();
}