#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "SPI.h"
#include <BH1750.h>
#include <ArduinoJson.h>
#include "FS.h"


//#define DEBUG
//#define DHT_ON
#define BME280_ON
//#define SHT21_ON


#if defined(DHT_ON)
#include "DHT.h"
#define DHTTYPE DHT22
DHT dht(ConfDevice.dht_pin, DHTTYPE);
int errorDHTdata = 0;  // количество ошибок чтения датчика DHT
#endif


#if defined(BME280_ON)
#include "SparkFunBME280.h"
BME280 bmeSensor;
#endif


#if defined(SHT21_ON)
#include "HTU21D.h"
HTU21D myHTU21D;
#endif


char staticIpStr[16] = "192.168.1.220";
char staticGatewayStr[16] = "192.168.1.1";
char staticSubnetStr[16] = "255.255.255.0";
uint8_t staticIpMode = 0;

char mqttServerIpStr[16] = "192.168.1.200";


const char *ver = "1.06";

const char *lux = "Lux";
const char *lightType = "LightType";
const char *temperature = "Temp";
const char *humidity = "Humidity";
const char *pressure = "Pressure";
const char *altitude = "Altitude";
const char *motionSensor = "MotionSensor";
const char *motionsensortimer = "MotionSensorTimer";
const char *version = "Version";
const char *freeMemory = "FreeMemory";
const char *ip = "IP";
const char *mac = "MAC";
const char *errorsDHT = "ErrorsDHT";
const char *uptime = "Uptime";

const char sec[] PROGMEM = "sec";

struct ConfDeviceStruct {
  char sta_ssid[32];
  char sta_pwd[64];
  char module_id[32];
  char mqtt_name[32];
  char publish_topic[32];
  char subscribe_topic[32];
  char commandPub_topic[32];
  uint8_t light_pin;
  uint8_t motion_pin;
  uint8_t dht_pin;
  unsigned long publish_delay;
  unsigned long subscribe_delay;
  unsigned long motion_read_delay;
  unsigned long reboot_delay;
  unsigned long get_data_delay;
  bool ver_send;
  bool ip_send;
  bool mac_send;


} ConfDevice = {
  "HomeNET",
  "Asdf1234",
  "",
  "_BedM",
  "/stateSub/",
  "/statePub/",
  "/commandPub/",
  12,
  14,
  13,
  10000,
  60000,
  10000,
  180000,
  10000,
  false,
  false,
  false
};

struct StringDataStruct {
  String temperatureString;
  String pressureString;
  String humidityString;
  String luxString;
  String ipString;
  String macString;
  String uptimeString;
  String freeMemoryString;
  String lightState;
  String lightOffTimerStting;

} StringData = {
  "None",
  "None",
  "None",
  "None",
  "None",
  "None",
  "None",
  "None",
  "AUTO",
  "5"
};

BH1750 lightSensor;
PubSubClient client;

long Day=0;
int Hour =0;
int Minute=0;
int Second=0;
int HighMillis=0;
int Rollover=0;

unsigned long getDataTimer = 0;
unsigned long publishTimer = 2000;
unsigned long motionTimer = 4000;
unsigned long rebootTimer = 6000;
unsigned long subscribeTimer = ConfDevice.subscribe_delay - 5000;
unsigned long lightOffTimer = 0;

boolean run = false;

WiFiClient espClient;

char topic_buff[120];
char value_buff[120];

String network_html;

ESP8266WebServer server(80);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////         HTML SNIPPLETS

/*
String headerStart;           headerStart += FPSTR(headerStartP);
String headerRefreshStatus;   headerRefreshStatus += FPSTR(headerRefreshStatusP);
String headerEnd;             headerEnd += FPSTR(headerEndP);
String javaScript;            javaScript += FPSTR(javaScriptP);
String javaScriptEnd;         javaScriptEnd += FPSTR(javaScriptEndP);
String bodyAjax;              bodyAjax += FPSTR(bodyAjaxP);
String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
String navbarStart;           navbarStart += FPSTR(navbarStartP);
String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);
String navbarActive;          navbarActive += FPSTR(navbarActiveP);
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

String inputBodyStart;        inputBodyStart += FPSTR(inputBodyStartP);
String inputBodyName;         inputBodyName += FPSTR(inputBodyNameP);
String inputBodyPOST;         inputBodyPOST += FPSTR(inputBodyPOSTP);
String inputPlaceHolder;      inputPlaceHolder += FPSTR(inputPlaceHolderP);
String inputBodyClose;        inputBodyClose += FPSTR(inputBodyCloseP);
String inputBodyCloseDiv;     inputBodyCloseDiv += FPSTR(inputBodyCloseDivP);
String inputBodyUnitStart;    inputBodyUnitStart += FPSTR(inputBodyUnitStartP);
String inputBodyUnitEnd;      inputBodyUnitEnd += FPSTR(inputBodyUnitEndP);
String inputBodyEnd;          inputBodyEnd += FPSTR(inputBodyEndP);

String sketchUploadForm;      sketchUploadForm += FPSTR(sketchUploadFormP);

String pinControlStart;       pinControlStart += FPSTR(pinControlStartP);
String pinControlOn;          pinControlOn += FPSTR(pinControlOnP);
String pinControlOff;         pinControlOff += FPSTR(pinControlOffP);
String pinControlBody1;       pinControlBody1 += FPSTR(pinControlBody1P);

String pinControlClassInfo;       pinControlClassInfo += FPSTR(pinControlClassInfoP);
String pinControlClassDanger;     pinControlClassDanger += FPSTR(pinControlClassDangerP);
String pinControlClassDefault;    pinControlClassDefault += FPSTR(pinControlClassDefaultP);
String pinControlClassSuccess;    pinControlClassSuccess += FPSTR(pinControlClassSuccessP);

String pinControlBody2;       pinControlBody2 += FPSTR(pinControlBody2P);

String pinControlTxtOn;       pinControlTxtOn += FPSTR(pinControlTxtOnP);
String pinControlTxtOff;      pinControlTxtOff += FPSTR(pinControlTxtOffP);

String pinControlBody3;       pinControlBody3 += FPSTR(pinControlBody3P);
String pinControlBody4;       pinControlBody4 += FPSTR(pinControlBody4P);
String pinControlBody5;       pinControlBody5 += FPSTR(pinControlBody5P);

String pinControlStatusOff;   pinControlStatusOff += FPSTR(pinControlStatusOffP);
String pinControlStatusOn;    pinControlStatusOn += FPSTR(pinControlStatusOnP);
String pinControlStatusAuto;  pinControlStatusAuto += FPSTR(pinControlStatusAutoP);

String pinControlEnd;         pinControlEnd += FPSTR(pinControlEndP);

*/


const char headerStartP[] PROGMEM = "<html lang='en'><head><title>ESP8266</title><meta charset='utf-8'>";
const char headerRefreshStatusP[] PROGMEM = "<META HTTP-EQUIV='Refresh' CONTENT='20; URL=/'>";
const char headerEndP[] PROGMEM = "<meta name='viewport' content='width=device-width, initial-scale=1'><link rel='stylesheet' href='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'><script src='https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js'></script><script src='http://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js'></script></head>";

const char javaScriptP[] PROGMEM = "<SCRIPT>\
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


const char javaScriptEndP[] PROGMEM = "\
   xmldoc = xmlResponse.getElementsByTagName('temperature');\
   message = xmldoc[0].firstChild.nodeValue;\
   document.getElementById('temperatureId').innerHTML=message;\
   xmldoc = xmlResponse.getElementsByTagName('humidity');\
   message = xmldoc[0].firstChild.nodeValue;\
   document.getElementById('humidityId').innerHTML=message;\
   xmldoc = xmlResponse.getElementsByTagName('illuminance');\
   message = xmldoc[0].firstChild.nodeValue;\
   document.getElementById('pressureId').innerHTML=message;\
   xmldoc = xmlResponse.getElementsByTagName('pressure');\
   message = xmldoc[0].firstChild.nodeValue;\
   document.getElementById('illuminanceId').innerHTML=message;\
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

const char navbarStartP[] PROGMEM = "<nav class='navbar navbar-inverse'><div class='container-fluid'><div class='navbar-header'><a class='navbar-brand' href='/'>ESP8266</a></div><div><ul class='nav navbar-nav'>";
const char navbarNonActiveP[] PROGMEM = "<li>";
const char navbarActiveP[] PROGMEM = "<li class='active'>";
const char navbarEndP[] PROGMEM  = "<a href='/'><span class='glyphicon glyphicon-dashboard'></span> Status</a></li><li><a href='/pincontrol'><span class='glyphicon glyphicon-tasks'></span> Control Pins</a></li><li class='dropdown'><a class='dropdown-toggle' data-toggle='dropdown' href='#'><span class='glyphicon glyphicon-cog'></span> Configure<span class='caret'></span></a><ul class='dropdown-menu'>\
                   <li><a href='/espconf'>Configure ESP</a></li>\
                   <li><a href='/mqttconf'>Configure MQTT</a></li>\
                   <li><a href='/update'>Update frimware</a></li>\
                   <li><a href='/reboot'>Reboot ESP</a></li>\
                   </ul></li></ul></div></div></nav>"; 

const char containerStartP[] PROGMEM    =  "<div class='container'><div class='row'>";
const char containerEndP[] PROGMEM      =  "<div class='clearfix visible-lg'></div></div></div>";
const char siteEndP[] PROGMEM         =  "</body></html>";
  
const char panelHeaderNameP[] PROGMEM     =  "<div class='col-md-4'><div class='page-header'><h1>";
const char panelHeaderEndP[] PROGMEM    =  "</h1></div>";
const char panelEndP[] PROGMEM        =  "</div>";
  
const char panelBodySymbolP[] PROGMEM     =  "<div class='panel panel-default'><div class='panel-body'><span class='glyphicon glyphicon-";
const char panelBodyNameP[] PROGMEM     =  "'></span> ";
const char panelBodyValueP[] PROGMEM    =  "<span class='pull-right'";
const char closingAngleBracketP[] PROGMEM    =  ">";

const char panelBodyEndP[] PROGMEM      =  "</span></div></div>";

const char inputBodyStartP[] PROGMEM    =  "<form action='' method='POST'><div class='panel panel-default'><div class='panel-body'>";
const char inputBodyNameP[] PROGMEM     =  "<div class='form-group'><div class='input-group'><span class='input-group-addon' id='basic-addon1'>";

const char inputBodyPOSTP[] PROGMEM     =  "</span><input type='text' name='";
const char inputPlaceHolderP[] PROGMEM  =  "' class='form-control' value='";

const char inputBodyCloseP[] PROGMEM    =  "' aria-describedby='basic-addon1'>";

const char inputBodyUnitStartP[] PROGMEM    =  "<span class='input-group-addon' id='basic-addon1'>";
const char inputBodyUnitEndP[] PROGMEM    =  "</span>";

const char inputBodyCloseDivP[] PROGMEM    =  "</div></div>";


const char inputBodyEndP[] PROGMEM      =  "</div><div class='panel-footer clearfix'><div class='pull-right'><button type='submit' class='btn btn-default'>Save</button></div></div></div></form>";

const char sketchUploadFormP[] PROGMEM  = 
    "<div class='col-md-4'><div class='page-header'><h1>Update Frimware</h1></div><form method='POST' action='/upload_sketch' enctype='multipart/form-data'>\
    <p><input type='file' class='btn btn-primary' name='sketch'></p>\
    <h3><small>Выберите файл формата *.bin</small></h3>\
    <p><input type='submit' value='Upload' class='btn btn-danger'></p></form></div>";




const char pinControlStartP[] PROGMEM  = "<div class='row'><div class='col-md-5'><h1>Control Pins</h1><table class='table table-hover'><tbody>\
    <tr><td class='active'><h4>Pins</h4></td><td class='active'></td><td class='active'></td><td class='active'><h4>Status</h4></td><td class='active'><h4>Mode</h4></td></tr>\
    <tr><td class='active'><h4>Led Strip</h4></td><td class='active'><a href='/pincontrol/";
const char pinControlOnP[] PROGMEM  = "button1on";
const char pinControlOffP[] PROGMEM  = "button1off";
const char pinControlBody1P[] PROGMEM  = "' class='btn btn-";

const char pinControlClassInfoP[] PROGMEM  = "info";
const char pinControlClassDangerP[] PROGMEM  = "danger";
const char pinControlClassDefaultP[] PROGMEM  = "default";
const char pinControlClassSuccessP[] PROGMEM  = "success";


const char pinControlBody2P[] PROGMEM  = "' role='button'>";

const char pinControlTxtOnP[] PROGMEM  = "Turn On";
const char pinControlTxtOffP[] PROGMEM  = "Turn Off";

const char pinControlBody3P[] PROGMEM  = "</a></td>   <td class='active'><a href='/pincontrol/button1auto' class='btn btn-";
// put class
const char pinControlBody4P[] PROGMEM  = "' role='button'>Auto</a></td> <td class='";
// put class
const char pinControlStatusOffP[] PROGMEM  = "'><h4>Off";
const char pinControlStatusOnP[] PROGMEM  = "'><h4>On";
const char pinControlStatusAutoP[] PROGMEM  = "'><h4>Auto";

const char pinControlBody5P[] PROGMEM  = "</h4></td><td class='";
// put class
// put pinControlStatus
const char pinControlEndP[] PROGMEM  = "</h4></td></tr></tbody></table></div></div>";
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////         ROOT 



void LightControl(bool motion = false)
{
  if (StringData.lightState == "ON"){
    digitalWrite(ConfDevice.light_pin, HIGH);
  } else if (StringData.lightState == "OFF"){
    digitalWrite(ConfDevice.light_pin, LOW);
  } else if (StringData.lightState == "AUTO" && motion == true ){
      digitalWrite(ConfDevice.light_pin, HIGH);
      lightOffTimer = millis();
  } else if (StringData.lightState == "AUTO" && motion == false && digitalRead(ConfDevice.light_pin) == HIGH){
    if (millis() - lightOffTimer >= atoi(StringData.lightOffTimerStting.c_str())*60*1000){
      digitalWrite(ConfDevice.light_pin, LOW);
    }
  }
}



void scanWiFi(void) {
  int founds = WiFi.scanNetworks();
  #ifdef DEBUG
  Serial.println();  Serial.println(F("scan done"));
  #endif
  if (founds == 0) {
    #ifdef DEBUG
    Serial.println(F("no networks found"));
    #endif
  } else {
    #ifdef DEBUG
    Serial.print(founds);  Serial.println(F(" networks found"));
    for (int i = 0; i < founds; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);  Serial.print(F(": "));  Serial.print(WiFi.SSID(i));  Serial.print(F(" ("));  Serial.print(WiFi.RSSI(i));  Serial.print(F(")"));
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
    #endif
  }
  network_html = "<blockquote>";
  for (int i = 0; i < founds; ++i)
  {
    // Print SSID and RSSI for each network found
    network_html += "<p><kbd>";
    network_html += WiFi.SSID(i);
    network_html += " (";
    network_html += WiFi.RSSI(i);
    network_html += ")";
    network_html += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    network_html += "</kbd></p>";
  }
  network_html += "</blockquote>";
}



int waitConnected(void) {
  int wait = 0;
  #ifdef DEBUG
  Serial.println();  Serial.println(F("Waiting for WiFi to connect"));
  #endif
  while ( wait < 20 ) {
    if (WiFi.status() == WL_CONNECTED) {
      #ifdef DEBUG
      Serial.println(F(""));  Serial.println(F("WiFi connected"));
      #endif
      return (1);
    }
    delay(500);
    #ifdef DEBUG
    Serial.print(WiFi.status());
    #endif
    wait++;
    yield();
  }
  #ifdef DEBUG
  Serial.println(F(""));  Serial.println(F("Connect timed out"));
  #endif
  return (0);
}



void GetLightSensorData()
{
  StringData.luxString = String(lightSensor.readLightLevel());
    
  #ifdef DEBUG
    Serial.print(F("Lux:   "));  Serial.print(StringData.luxString);  Serial.println();
  #endif
}



#ifdef BME280_ON
void GetBmeSensorData()
{ 
  StringData.temperatureString = String(bmeSensor.readTempC());
  #ifdef DEBUG 
    Serial.print(F("Temperature: "));  Serial.print(StringData.temperatureString);  Serial.println(F(" C"));
  #endif

  StringData.pressureString = String(bmeSensor.readFloatPressure()/133.3);
  #ifdef DEBUG
    Serial.print(F("Pressure: "));  Serial.print(StringData.pressureString);  Serial.println(F(" mm"));
  #endif

  StringData.humidityString = String(bmeSensor.readFloatHumidity());
  #ifdef DEBUG
    Serial.print(F("%RH: "));  Serial.print(StringData.humidityString);  Serial.println(F(" %"));
  #endif
/*
  #ifdef DEBUG
    Serial.print("Altitude: ");
    Serial.print(bmeSensor.readFloatAltitudeMeters(), 2);
    Serial.println("m");
  #endif
  float altitudeData = bmeSensor.readFloatAltitudeMeters();
  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  altitude, ConfDevice.mqtt_name);
  client.publish(topic_buff, floatToChar(altitudeData));
*/
}
#endif



#ifdef SHT21_ON
void GetSHT21SensorData(){

  myHTU21D.setResolution(HTU21D_RES_RH8_TEMP12);
  StringData.temperatureString = String(myHTU21D.readTemperature());
  StringData.humidityString = String(myHTU21D.readCompensatedHumidity());

  #ifdef DEBUG
    Serial.println(F(""));  Serial.println(F(""));  Serial.println(F("<<%RH: 8Bit, Temperature - 12Bit>>"));
    
    Serial.println(F(""));  Serial.print(F("Humidity: "));  Serial.println(myHTU21D.readHumidity());
    
    Serial.println(F(""));  Serial.print(F("Compensated Humidity: "));   Serial.println(StringData.humidityString);
    
    Serial.println(F(""));  Serial.print(F("Temperature: "));  Serial.print(StringData.temperatureString);  Serial.println(F(" C"));

    Serial.println(F(""));  Serial.println(F(""));  Serial.println(F("<<Battery Status>>"));
    
    if (myHTU21D.batteryStatus() == true)
    {
      Serial.println(F("Battery OK. Level > 2.25v"));
    }
    else
    {
      Serial.println(F("Battery LOW. Level < 2.25v"));
    }
  #endif
}
#endif



#ifdef DHT_ON
void DHT22Sensor()
{
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
  } else if (client.connect(ConfDevice.mqtt_name)) {
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  temperature, ConfDevice.mqtt_name);
    client.publish(topic_buff, floatToChar(temperatureData));
    
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  humidity, ConfDevice.mqtt_name);
    client.publish(topic_buff, floatToChar(humidityData));
  }
}
#endif



void MotionDetect()
{
  if (digitalRead(ConfDevice.motion_pin) == HIGH) {
    #ifdef DEBUG
      Serial.println(F("MotionSensor moove detected"));
    #endif
    bool motion = true;
    LightControl(motion);
    run = true;
    if (client.connect(ConfDevice.mqtt_name)) {
      sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  motionSensor, ConfDevice.mqtt_name);
      client.publish(topic_buff, "ON");
    }
  }
}



String GetUptimeData(){
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
StringData.uptimeString = String(value_buff);
#ifdef DEBUG
  Serial.print(F("Uptime: "));  Serial.print(value_buff);  Serial.print(F(":"));  Serial.print(Second/10);  Serial.println(Second%10);
#endif
return value_buff;
};



void RebootESP()
{
  if (millis() - rebootTimer >= ConfDevice.reboot_delay){
  ESP.restart();
  }
}



// handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  subscribeTimer = millis();
  rebootTimer = millis();

  int i = 0;

  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    value_buff[i] = payload[i];
  }
  value_buff[i] = '\0';

  #ifdef DEBUG
  Serial.print(F("callback: "));  Serial.print(topic);  Serial.print(F(" payload "));  Serial.println(value_buff);
  #endif


  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.commandPub_topic, lightType, ConfDevice.mqtt_name);
  if (strcmp (topic,topic_buff) == 0){
    #ifdef DEBUG
      Serial.print(F("topic: "));  Serial.print(topic);  Serial.print(F(" equals "));  Serial.println(topic_buff);
    #endif
    if (strncmp (value_buff,"1",1) == 0){
      #ifdef DEBUG
        Serial.print(F("value_buff: "));  Serial.print(value_buff);  Serial.println(F(" contains ON"));
      #endif
      StringData.lightState = String(F("ON"));
      //digitalWrite(ConfDevice.light_pin, HIGH);
    } else if (strncmp (value_buff,"0",1) == 0){
      #ifdef DEBUG
        Serial.print(F("value_buff: "));  Serial.print(value_buff);  Serial.println(F(" contains OFF"));
      #endif
      StringData.lightState = String(F("OFF"));
      //digitalWrite(ConfDevice.light_pin, LOW);
    } else if (strncmp (value_buff,"2",1) == 0){
      #ifdef DEBUG
        Serial.print(F("value_buff: "));  Serial.print(value_buff);  Serial.println(F(" contains Auto"));
      #endif
      StringData.lightState = String(F("AUTO"));
    }
    LightControl();
  }



  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.commandPub_topic, motionsensortimer, ConfDevice.mqtt_name);
    if (strcmp (topic,topic_buff) == 0){
      #ifdef DEBUG
        Serial.print(F("topic: "));  Serial.print(topic);  Serial.print(F(" equals "));  Serial.println(topic_buff);
      #endif
      StringData.lightOffTimerStting = value_buff;
    }


  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.subscribe_topic, version, ConfDevice.mqtt_name);
  if (strcmp (topic,topic_buff) == 0){
    if (strncmp (value_buff, ver, 4) == 0){
      ConfDevice.ver_send = true;
      ConfDevice.ip_send = true;
      ConfDevice.mac_send = true;
    } else {
      ConfDevice.ver_send = false;
      ConfDevice.ip_send = false;
      ConfDevice.mac_send = false;
    }
  }


  #ifdef DEBUG
    Serial.print(F("topic: "));  Serial.println(topic);  Serial.print(F("value_buff: "));  Serial.println(value_buff);
    Serial.print(F("topic_buff: "));  Serial.println(topic_buff);  Serial.println();
  #endif
}



void MqttPubLightState(){
  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  lightType, ConfDevice.mqtt_name);
  String lightStateNum;
  if (StringData.lightState == "ON"){
    lightStateNum = String(F("1"));
  } else if (StringData.lightState == "OFF"){
    lightStateNum = String(F("0"));
  } else {
    lightStateNum = String(F("2"));
  }
  client.publish(topic_buff, lightStateNum.c_str());
}



void MqttPubData()
{
  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  lux, ConfDevice.mqtt_name);
  client.publish(topic_buff, StringData.luxString.c_str());

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic, temperature, ConfDevice.mqtt_name);
  client.publish(topic_buff, StringData.temperatureString.c_str());

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic, pressure, ConfDevice.mqtt_name);
  client.publish(topic_buff, StringData.pressureString.c_str());

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  humidity, ConfDevice.mqtt_name);
  client.publish(topic_buff, StringData.humidityString.c_str());

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  freeMemory, ConfDevice.mqtt_name);
  client.publish(topic_buff, StringData.freeMemoryString.c_str());

  sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic, uptime, ConfDevice.mqtt_name);
  client.publish(topic_buff, StringData.uptimeString.c_str());


  if (ConfDevice.ver_send == false){
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  version, ConfDevice.mqtt_name);
    client.publish(topic_buff, ver);
  }
  
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  ip, ConfDevice.mqtt_name);
    client.publish(topic_buff, StringData.ipString.c_str());

  
  if (ConfDevice.mac_send == false){  
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  mac, ConfDevice.mqtt_name);
    client.publish(topic_buff, StringData.macString.c_str());
  }

 
  #ifdef DHT_ON
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.publish_topic,  errorsDHT, ConfDevice.mqtt_name);
    sprintf_P(value_buff, (const char *)F("%d"), errorDHTdata);  
    client.publish(topic_buff, value_buff);
  #endif
}



void MqttSubscribePrint(char *sub_buff)
{
    #ifdef DEBUG
      Serial.print(F("Try subscribe: "));  Serial.println(sub_buff);
    #endif

  if (client.subscribe(sub_buff)) {
    #ifdef DEBUG
      Serial.print(F("subscribe: "));  Serial.println(sub_buff);
    #endif
  } else {
    #ifdef DEBUG
      client.disconnect();
      Serial.print(F("ERROR subscribe: "));  Serial.println(sub_buff);
    #endif
  }
}



void MqttSubscribe()
{
  if (millis() - subscribeTimer >= ConfDevice.subscribe_delay) {
    subscribeTimer = millis();
    
    sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.commandPub_topic, motionsensortimer, ConfDevice.mqtt_name);
    MqttSubscribePrint(topic_buff);

    sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.commandPub_topic, lightType, ConfDevice.mqtt_name);
    MqttSubscribePrint(topic_buff);

    sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.subscribe_topic, uptime, ConfDevice.mqtt_name);
    MqttSubscribePrint(topic_buff);

    sprintf_P(topic_buff, (const char *)F("%s%s%s"), ConfDevice.subscribe_topic, version, ConfDevice.mqtt_name);
    MqttSubscribePrint(topic_buff);
  }
}



void TestMQTTPrint()
{
  int state = client.state();

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
}


static char* floatToChar(float charester)
{
 dtostrf(charester, 1, 0, value_buff);
 return value_buff;
}



void GetFreeMemory () {
  StringData.freeMemoryString = String(ESP.getFreeHeap());
}



String GetIpString (IPAddress ip) {
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  return ipStr;
}



void GetMacString () {
  uint8_t macData[6];
  WiFi.macAddress(macData);
  sprintf_P(value_buff, (const char *)F("%x:%x:%x:%x:%x:%x"), macData[0],  macData[1], macData[2], macData[3], macData[4], macData[5]);
  if (StringData.macString != String(value_buff)){  
    StringData.macString = String(value_buff);
    ConfDevice.mac_send = false;
  }
}



IPAddress stringToIp (String strIp) {

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
  return ip;
}



void TestSystemPrint()
{
  Serial.println(F("----------------"));

  TestMQTTPrint();

  Serial.println(__TIMESTAMP__);

  Serial.print(F("Version "));  Serial.println(ver);

  Serial.print(F("IP address: "));  Serial.println(StringData.ipString);

  Serial.print(F("Sketch size: "));  Serial.println(ESP.getSketchSize());

  Serial.print(F("Free size: "));  Serial.println(ESP.getFreeSketchSpace());

  Serial.print(F("Free memory: "));  Serial.println(StringData.freeMemoryString);

  Serial.print(F("WiFi status: "));  Serial.println(WiFi.status());

  Serial.print(F("Chip Id: "));  Serial.println(ESP.getChipId());

  Serial.print(F("Flash Chip Id: "));  Serial.println(ESP.getFlashChipId());

  Serial.print(F("Flash Chip Size: ")); Serial.println(ESP.getFlashChipSize());

  Serial.print(F("Flash Chip Speed: "));  Serial.println(ESP.getFlashChipSpeed());

  Serial.println(F("----------------"));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////   SPIFFS  Start   ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

bool saveConfig() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["sta_ssid"] = ConfDevice.sta_ssid;
  json["sta_pwd"] = ConfDevice.sta_pwd;
  json["staticIpMode"] = staticIpMode;
  json["staticIP"] = staticIpStr;
  json["staticGateway"] = staticGatewayStr;
  json["staticSubnet"] = staticSubnetStr;
  json["mqtt_server_ip_srting"] = mqttServerIpStr;
  json["mqtt_name"] = ConfDevice.mqtt_name;
  json["publish_topic"] = ConfDevice.publish_topic;
  json["subscribe_topic"] = ConfDevice.subscribe_topic;
  json["light_pin"] = ConfDevice.light_pin;
  json["motion_pin"] = ConfDevice.motion_pin;
  json["dht_pin"] = ConfDevice.dht_pin;
  json["get_data_delay"] = ConfDevice.get_data_delay;
  json["publish_delay"] = ConfDevice.publish_delay;
  json["subscribe_delay"] = ConfDevice.subscribe_delay;
  json["motion_read_delay"] = ConfDevice.motion_read_delay;
  json["reboot_delay"] = ConfDevice.reboot_delay;


  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    #ifdef DEBUG
    Serial.println(F("Failed to open config file for writing"));
    #endif
    return false;
  }

  json.printTo(configFile);
  return true;
}



bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    #ifdef DEBUG
    Serial.println(F("Failed to open config file"));
    #endif
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    #ifdef DEBUG
    Serial.println(F("Config file size is too large"));
    #endif
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  String conv;

  if (!json.success()) {
    #ifdef DEBUG
    Serial.println(F("Failed to parse config file"));
    #endif
    return false;
  }

  const char* sta_ssid_char = json["sta_ssid"];
  sprintf_P(ConfDevice.sta_ssid, ("%s"), sta_ssid_char);

  const char* sta_pwd_char = json["sta_pwd"];
  sprintf_P(ConfDevice.sta_pwd, ("%s"), sta_pwd_char);

  const char* staticIpMode_char = json["staticIpMode"];
  sprintf_P(staticIpStr, ("%s"), staticIpMode_char);

  const char* staticIP_char = json["staticIP"];
  sprintf_P(staticIpStr, ("%s"), staticIP_char);

  const char* staticGateway_char = json["staticGateway"];
  sprintf_P(staticGatewayStr, ("%s"), staticGateway_char);

  const char* staticSubnet_char = json["staticSubnet"];
  sprintf_P(staticSubnetStr, ("%s"), staticSubnet_char);

  const char* mqtt_server_ip_srting_char = json["mqtt_server_ip_srting"];
  sprintf_P(mqttServerIpStr, ("%s"), mqtt_server_ip_srting_char);

  const char* mqtt_name_char = json["mqtt_name"];
  sprintf_P(ConfDevice.mqtt_name, ("%s"), mqtt_name_char);

  const char* publish_topic_char = json["publish_topic"];
  sprintf_P(ConfDevice.publish_topic, ("%s"), publish_topic_char);

  const char* subscribe_topic_char = json["subscribe_topic"];
  sprintf_P(ConfDevice.subscribe_topic, ("%s"), subscribe_topic_char);

  if (json["staticIpMode"]){
    const char* staticIpMode_char = json["staticIpMode"];
    conv = String(staticIpMode_char);
    staticIpMode = atoi(conv.c_str());
  } else {
    saveConfig();
  }

  if (json["light_pin"]){
    const char* light_pin_char = json["light_pin"];
    conv = String(light_pin_char);
    ConfDevice.light_pin = atoi(conv.c_str());
  } else {
    saveConfig();
  }

  if (json["motion_pin"]){
    const char* motion_pin_char = json["motion_pin"];
    conv = String(motion_pin_char);
    ConfDevice.motion_pin = atoi(conv.c_str());
  } else {
    saveConfig();
  }

  if (json["dht_pin"]){
    const char* dht_pin_char = json["dht_pin"];
    conv = String(dht_pin_char);
    ConfDevice.dht_pin = atoi(conv.c_str());
  } else {
    saveConfig();
  }

  if (json["get_data_delay"]){
    const char* get_data_delay_char = json["get_data_delay"];
    conv = String(get_data_delay_char);
    ConfDevice.get_data_delay = atoi(conv.c_str());
  } else {
    saveConfig();
  }

  if (json["publish_delay"]){
    const char* publish_delay_char = json["publish_delay"];
    conv = String(publish_delay_char);
    ConfDevice.publish_delay = atoi(conv.c_str());
  } else {
    saveConfig();
  }

  if (json["subscribe_delay"]){
    const char* subscribe_delay_char = json["subscribe_delay"];
    conv = String(subscribe_delay_char);
    ConfDevice.subscribe_delay = atoi(conv.c_str());
  } else {
    saveConfig();
  }

  if (json["motion_read_delay"]){
    const char* motion_read_delay_char = json["motion_read_delay"];
    conv = String(motion_read_delay_char);
    ConfDevice.motion_read_delay = atoi(conv.c_str());
  } else {
    saveConfig();
  }

  if (json["reboot_delay"]){
    const char* reboot_delay_char = json["reboot_delay"];
    conv = String(reboot_delay_char);
    ConfDevice.reboot_delay = atoi(conv.c_str());
  } else {
    saveConfig();
  }

  // Real world application would store these values in some variables for
  // later use.

  return true;
}



String PinControlView() {

  String headerStart;           headerStart += FPSTR(headerStartP);
  String headerEnd;             headerEnd += FPSTR(headerEndP);
  String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
  String navbarStart;           navbarStart += FPSTR(navbarStartP);
  String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);
  String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
  String containerStart;        containerStart += FPSTR(containerStartP);
  String containerEnd;          containerEnd += FPSTR(containerEndP);
  String siteEnd;               siteEnd += FPSTR(siteEndP);

  String pinControlStart;       pinControlStart += FPSTR(pinControlStartP);
  String pinControlOn;          pinControlOn += FPSTR(pinControlOnP);
  String pinControlOff;         pinControlOff += FPSTR(pinControlOffP);
  String pinControlBody1;       pinControlBody1 += FPSTR(pinControlBody1P);

  String pinControlClassInfo;       pinControlClassInfo += FPSTR(pinControlClassInfoP);
  String pinControlClassDanger;     pinControlClassDanger += FPSTR(pinControlClassDangerP);
  String pinControlClassDefault;    pinControlClassDefault += FPSTR(pinControlClassDefaultP);
  String pinControlClassSuccess;    pinControlClassSuccess += FPSTR(pinControlClassSuccessP);

  String pinControlBody2;       pinControlBody2 += FPSTR(pinControlBody2P);

  String pinControlTxtOn;       pinControlTxtOn += FPSTR(pinControlTxtOnP);
  String pinControlTxtOff;      pinControlTxtOff += FPSTR(pinControlTxtOffP);

  String pinControlBody3;       pinControlBody3 += FPSTR(pinControlBody3P);
  String pinControlBody4;       pinControlBody4 += FPSTR(pinControlBody4P);
  String pinControlBody5;       pinControlBody5 += FPSTR(pinControlBody5P);

  String pinControlStatusOff;   pinControlStatusOff += FPSTR(pinControlStatusOffP);
  String pinControlStatusOn;    pinControlStatusOn += FPSTR(pinControlStatusOnP);
  String pinControlStatusAuto;  pinControlStatusAuto += FPSTR(pinControlStatusAutoP);

  String pinControlEnd;         pinControlEnd += FPSTR(pinControlEndP);

  unsigned long timeOff = 0;
  if (millis() - lightOffTimer < atoi(StringData.lightOffTimerStting.c_str())*60*1000){
    timeOff = atoi(StringData.lightOffTimerStting.c_str())*60*1000 - (millis() - lightOffTimer);
  } 



  String mode;
  if (StringData.lightState == "AUTO"){
    mode = pinControlClassSuccess + pinControlStatusAuto;
  } else if (StringData.lightState == "ON") {
    mode = pinControlClassInfo + pinControlStatusOn;
  } else {
    mode = pinControlClassDanger + pinControlStatusOff;
  }
  
  String pinControl = headerStart + headerEnd + bodyNonAjax + navbarStart + navbarNonActive + navbarEnd + containerStart + pinControlStart;

  if (digitalRead(ConfDevice.light_pin) == HIGH){
    if (StringData.lightState == "AUTO"){
      pinControl += pinControlOff + pinControlBody1 + pinControlClassDefault + pinControlBody2 + pinControlTxtOff + pinControlBody3 + pinControlClassDanger + pinControlBody4 + pinControlClassInfo + pinControlStatusOn + pinControlBody5;
    } else {
      pinControl += pinControlOff + pinControlBody1 + pinControlClassDanger + pinControlBody2 + pinControlTxtOff + pinControlBody3 + pinControlClassDefault + pinControlBody4 + pinControlClassInfo + pinControlStatusOn + pinControlBody5;
    }
  } else {
    if (StringData.lightState == "AUTO"){
      pinControl += pinControlOn + pinControlBody1 + pinControlClassDefault + pinControlBody2 + pinControlTxtOn + pinControlBody3 + pinControlClassDanger + pinControlBody4 + pinControlClassDanger + pinControlStatusOff + pinControlBody5;
    } else {
      pinControl += pinControlOn + pinControlBody1 + pinControlClassInfo + pinControlBody2 + pinControlTxtOn + pinControlBody3 + pinControlClassDefault + pinControlBody4 + pinControlClassDanger + pinControlStatusOff + pinControlBody5;
    }
  }

  pinControl += mode + pinControlEnd + containerEnd + String(timeOff) + siteEnd;

  return pinControl;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////   WEB PAGES  Start  //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

void rootWebPage(void) {
  server.on("/", []() {

    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");

    String headerStart;           headerStart += FPSTR(headerStartP);
    String headerRefreshStatus;   headerRefreshStatus += FPSTR(headerRefreshStatusP);
    String headerEnd;             headerEnd += FPSTR(headerEndP);
    String javaScript;            javaScript += FPSTR(javaScriptP);
    String javaScriptEnd;         javaScriptEnd += FPSTR(javaScriptEndP);
    String bodyAjax;              bodyAjax += FPSTR(bodyAjaxP);
    String navbarStart;           navbarStart += FPSTR(navbarStartP);
    String navbarActive;          navbarActive += FPSTR(navbarActiveP);
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
    String Humidity     = panelBodySymbol + String(F("tint"))          + panelBodyName + String(F("Humidity"))    + panelBodyValue + String(F(" id='humidityId'")) + closingAngleBracket      + panelBodyEnd;
    String Temperature  = panelBodySymbol + String(F("fire"))          + panelBodyName + String(F("Temperature")) + panelBodyValue + String(F(" id='temperatureId'")) + closingAngleBracket   + panelBodyEnd;
    String Pressure;
    #ifdef BME280_ON
      Pressure          = panelBodySymbol + String(F("cloud"))         + panelBodyName + String(F("Pressure"))    + panelBodyValue + String(F(" id='pressureId'")) + closingAngleBracket      + panelBodyEnd;
    #endif
    String Lux          = panelBodySymbol + String(F("asterisk"))      + panelBodyName + String(F("illuminance")) + panelBodyValue + String(F(" id='illuminanceId'")) + closingAngleBracket           + panelBodyEnd;
    
    String title2       = panelHeaderName + String(F("ESP Settings"))  + panelHeaderEnd;
    String ssid         = panelBodySymbol + String(F("signal"))        + panelBodyName + String(F("Wi-Fi SSID"))  + panelBodyValue + closingAngleBracket + ConfDevice.sta_ssid              + panelBodyEnd;
    String IPAddClient  = panelBodySymbol + String(F("globe"))         + panelBodyName + String(F("IP Address"))  + panelBodyValue + closingAngleBracket + StringData.ipString              + panelBodyEnd;
    String MacAddr      = panelBodySymbol + String(F("scale"))         + panelBodyName + String(F("MAC Address")) + panelBodyValue + closingAngleBracket + StringData.macString             + panelBodyEnd;
    String MqttPrefix   = panelBodySymbol + String(F("tag"))           + panelBodyName + String(F("MQTT Prefix")) + panelBodyValue + closingAngleBracket + ConfDevice.mqtt_name             + panelBodyEnd;
    String Uptime       = panelBodySymbol + String(F("time"))          + panelBodyName + String(F("Uptime"))      + panelBodyValue + String(F(" id='uptimeId'"))     + closingAngleBracket  + panelBodyEnd;
    String FreeMem      = panelBodySymbol + String(F("flash"))         + panelBodyName + String(F("Free Memory")) + panelBodyValue + String(F(" id='freeMemoryId'")) + closingAngleBracket  + panelBodyEnd;
    String Ver          = panelBodySymbol + String(F("flag"))          + panelBodyName + String(F("Version"))     + panelBodyValue + closingAngleBracket + ver                              + panelBodyEnd;
    
    server.send ( 200, "text/html", headerStart + headerEnd + javaScript + javaScriptEnd + bodyAjax + navbarStart + navbarActive + navbarEnd + containerStart + title1 + Temperature + Humidity + Pressure + Lux + panelEnd + title2 + ssid + IPAddClient + MacAddr + MqttPrefix + Uptime + FreeMem + Ver + panelEnd + panelEnd + containerEnd + siteEnd);
  });
}



void rebootWebPage(void) {
  server.on("/reboot", []() {

    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");

    String headerStart;           headerStart += FPSTR(headerStartP);
    String headerRefreshStatus;   headerRefreshStatus += FPSTR(headerRefreshStatusP);
    String headerEnd;             headerEnd += FPSTR(headerEndP);
    String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
    String navbarStart;           navbarStart += FPSTR(navbarStartP);
    String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);
    String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
    String containerStart;        containerStart += FPSTR(containerStartP);
    String containerEnd;          containerEnd += FPSTR(containerEndP);
    String siteEnd;               siteEnd += FPSTR(siteEndP);

    String data = "<div class='col-md-4'><div class='page-header'><h1>Reboot ESP</h1></div><div class='alert alert-info' role='alert'><a href='#' class='alert-link'>Rebooting...</a></div></div>";
    server.send ( 200, "text/html", headerStart + headerRefreshStatus + headerEnd + bodyNonAjax + navbarStart + navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);
    ESP.restart();

  });
}



void setupWebUpdate(void) {
  server.on("/update", HTTP_GET, []() {

    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");

    String headerStart;           headerStart += FPSTR(headerStartP);
    String headerEnd;             headerEnd += FPSTR(headerEndP);
    String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
    String navbarStart;           navbarStart += FPSTR(navbarStartP);
    String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);
    String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
    String containerStart;        containerStart += FPSTR(containerStartP);
    String containerEnd;          containerEnd += FPSTR(containerEndP);
    String siteEnd;               siteEnd += FPSTR(siteEndP);
    String sketchUploadForm;      sketchUploadForm += FPSTR(sketchUploadFormP);

    server.send(200, "text/html", headerStart + headerEnd + bodyNonAjax + navbarStart + navbarNonActive + navbarEnd + containerStart + sketchUploadForm + containerEnd + siteEnd);
  });
  server.onFileUpload([]() {
    if (server.uri() != "/upload_sketch") return;
    HTTPUpload& upload = server.upload();
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
  });
  server.on("/upload_sketch", HTTP_POST, []() {

    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");

    String headerStart;           headerStart += FPSTR(headerStartP);
    String headerRefreshStatus;   headerRefreshStatus += FPSTR(headerRefreshStatusP);
    String headerEnd;             headerEnd += FPSTR(headerEndP);
    String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
    String navbarStart;           navbarStart += FPSTR(navbarStartP);
    String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);
    String navbarEnd;             navbarEnd += FPSTR(navbarEndP);
    String containerStart;        containerStart += FPSTR(containerStartP);
    String containerEnd;          containerEnd += FPSTR(containerEndP);
    String siteEnd;               siteEnd += FPSTR(siteEndP);

    String varDataString = String(F("<div class='col-md-4'><div class='page-header'><h1>Update Frimware</h1></div><div class='alert alert-success'>")) + ((Update.hasError()) ? String(F("FAIL")) : String(F("Update Frimware: OK"))) + String(F("</div></div>"));


    server.send(200, "text/html", headerStart + headerRefreshStatus + headerEnd + bodyNonAjax + navbarStart + navbarNonActive + navbarEnd + containerStart + varDataString + containerEnd + siteEnd);
    ESP.restart();
  });
}



void web_espConf(void) {

  server.on("/espconf", []() {

    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");

    String headerStart;           headerStart += FPSTR(headerStartP);
    String headerEnd;             headerEnd += FPSTR(headerEndP);
    String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
    String navbarStart;           navbarStart += FPSTR(navbarStartP);
    String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);
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

    String title1 = panelHeaderName + String(F("ESP Configuration")) + panelHeaderEnd;
    String data = title1 + network_html + inputBodyStart;
    String d = "";


    String payload=server.arg("sta_ssid");
    if (payload.length() > 0 ) {
      payload.toCharArray(ConfDevice.sta_ssid, sizeof(ConfDevice.sta_ssid));
    }
    data += inputBodyName + String(F("STA SSID")) + inputBodyPOST + String(F("sta_ssid"))  + inputPlaceHolder + ConfDevice.sta_ssid + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("sta_pwd");
    if (payload.length() > 7 &&  payload != "********") {
      payload.toCharArray(ConfDevice.sta_pwd, sizeof(ConfDevice.sta_pwd));
    }
    data += inputBodyName + String(F("Password")) + String(F("</span><input type='password' name='")) + String(F("sta_pwd")) + inputPlaceHolder + String(F("********")) + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("staticIP");
    if (payload.length() > 6 ) {
      payload.toCharArray(staticIpStr, sizeof(staticIpStr));
      staticIpMode = 1;
    } else {
      d.toCharArray(staticIpStr, sizeof(staticIpStr));
      staticIpMode = 0;
    }
    data += inputBodyName + String(F("Static IP")) + inputBodyPOST + String(F("staticIP")) + inputPlaceHolder + staticIpStr + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("staticGateway");
    if (payload.length() > 6 ) {
      payload.toCharArray(staticGatewayStr, sizeof(staticGatewayStr));
      staticIpMode += 1;
    } else {
      d.toCharArray(staticGatewayStr, sizeof(staticGatewayStr));
      staticIpMode = 0;
    }
    data += inputBodyName + String(F("Static Gateway")) + inputBodyPOST + String(F("staticGateway")) + inputPlaceHolder + staticGatewayStr + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("staticSubnet");
    if (payload.length() > 6 ) {
      payload.toCharArray(staticSubnetStr, sizeof(staticSubnetStr));
      staticIpMode += 1;
    } else {
      d.toCharArray(staticSubnetStr, sizeof(staticSubnetStr));
      staticIpMode = 0;
    }
    data += inputBodyName + String(F("Static Subnet")) + inputBodyPOST + String(F("staticSubnet")) + inputPlaceHolder + staticSubnetStr + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("light_pin");
    if (payload.length() > 0 ) {
      ConfDevice.light_pin = atoi(payload.c_str());
    }
    data += inputBodyName + String(F("Light Pin")) + inputBodyPOST + String(F("light_pin")) + inputPlaceHolder + String(ConfDevice.light_pin) + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("motion_pin");
    if (payload.length() > 0 ) {
      ConfDevice.motion_pin = atoi(payload.c_str());
    }
    data += inputBodyName + String(F("Motion Pin")) + inputBodyPOST + String(F("motion_pin")) + inputPlaceHolder + String(ConfDevice.motion_pin) + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("dht_pin");
    if (payload.length() > 0 ) {
      ConfDevice.dht_pin = atoi(payload.c_str());
    }
    data += inputBodyName + String(F("DHT Pin")) + inputBodyPOST + String(F("dht_pin")) + inputPlaceHolder + String(ConfDevice.dht_pin) + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("get_data_delay");
    if (payload.length() > 0 ) {
      ConfDevice.get_data_delay = atoi(payload.c_str())*1000;
    }
    data += inputBodyName + String(F("Update Data Delay")) + inputBodyPOST + String(F("get_data_delay")) + inputPlaceHolder + String(ConfDevice.get_data_delay/1000) + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;

    payload=server.arg("motion_read_delay");
    if (payload.length() > 0 ) {
      ConfDevice.motion_read_delay = atoi(payload.c_str())*1000;
    }
    data += inputBodyName + String(F("Motion Read Delay")) + inputBodyPOST + String(F("motion_read_delay")) + inputPlaceHolder + String(ConfDevice.motion_read_delay/1000) + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;

    payload=server.arg("reboot_delay");
    if (payload.length() > 0 ) {
      ConfDevice.reboot_delay = atoi(payload.c_str())*1000;
    }
    data += inputBodyName + String(F("Reboot Delay")) + inputBodyPOST + String(F("reboot_delay")) + inputPlaceHolder + String(ConfDevice.reboot_delay/1000) + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;


    data += inputBodyEnd;
    saveConfig();

    server.send ( 200, "text/html", headerStart + headerEnd + bodyNonAjax + navbarStart + navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);
  });
}



void web_mqttConf(void) {

  server.on("/mqttconf", []() {

    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");

    String headerStart;           headerStart += FPSTR(headerStartP);
    String headerEnd;             headerEnd += FPSTR(headerEndP);
    String bodyNonAjax;           bodyNonAjax += FPSTR(bodyNonAjaxP);
    String navbarStart;           navbarStart += FPSTR(navbarStartP);
    String navbarNonActive;       navbarNonActive += FPSTR(navbarNonActiveP);
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

    String title1 = panelHeaderName + String(F("MQTT Configuration")) + panelHeaderEnd;
    String data = title1 + inputBodyStart;

    String payload=server.arg("mqtt_ip");
    if (payload.length() > 0 ) {
      payload.toCharArray(mqttServerIpStr, sizeof(mqttServerIpStr));
    }
    data += inputBodyName + String(F("Server MQTT")) + inputBodyPOST + String(F("mqtt_ip")) + inputPlaceHolder + mqttServerIpStr + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("mqtt_name");
    if (payload.length() > 0 ) {
      payload.toCharArray(ConfDevice.mqtt_name, sizeof(ConfDevice.mqtt_name));
    }
    data += inputBodyName + String(F("MQTT Prefix")) + inputBodyPOST + String(F("mqtt_name")) + inputPlaceHolder + ConfDevice.mqtt_name + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("publish_topic");
    if (payload.length() > 0 ) {
      payload.replace("%2F", String(F("/")));
      payload.toCharArray(ConfDevice.publish_topic, sizeof(ConfDevice.publish_topic));
    }
    data += inputBodyName + String(F("Publish Topic")) + inputBodyPOST + String(F("publish_topic")) + inputPlaceHolder + ConfDevice.publish_topic + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("subscribe_topic");
    if (payload.length() > 0 ) {
      payload.replace("%2F", String(F("/")));
      payload.toCharArray(ConfDevice.subscribe_topic, sizeof(ConfDevice.subscribe_topic));
    }
    data += inputBodyName + String(F("Subscribe Topic")) + inputBodyPOST + String(F("subscribe_topic")) + inputPlaceHolder + ConfDevice.subscribe_topic + inputBodyClose + inputBodyCloseDiv;

    payload=server.arg("publish_delay");
    if (payload.length() > 0 ) {
      ConfDevice.publish_delay = atoi(payload.c_str())*1000;
    }
    data += inputBodyName + String(F("Publish Delay")) + inputBodyPOST + String(F("publish_delay")) + inputPlaceHolder + String(ConfDevice.publish_delay/1000) + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;

    payload=server.arg("subscribe_delay");
    if (payload.length() > 0 ) {
      ConfDevice.subscribe_delay = atoi(payload.c_str())*1000;
    }
    data += inputBodyName + String(F("Subscribe Delay")) + inputBodyPOST + String(F("subscribe_delay")) + inputPlaceHolder + String(ConfDevice.subscribe_delay/1000) + inputBodyClose + inputBodyUnitStart + String(FPSTR(sec)) + inputBodyUnitEnd + inputBodyCloseDiv;


    data += inputBodyEnd;
    saveConfig();

    server.send ( 200, "text/html", headerStart + headerEnd + bodyNonAjax + navbarStart + navbarNonActive + navbarEnd + containerStart + data + containerEnd + siteEnd);
  });
}



void web_pinControl(void) {

  server.on("/pincontrol", []() {

    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");

    String data = PinControlView();

    server.send ( 200, "text/html", data);
  });


  server.on("/pincontrol/button1on", []() {

    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");

    StringData.lightState = "ON";
    LightControl();
    MqttPubLightState();
    String data = PinControlView();

    server.send ( 200, "text/html", data);
  });

  server.on("/pincontrol/button1off", []() {

    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");

    StringData.lightState = "OFF";
    LightControl();
    MqttPubLightState();
    String data = PinControlView();

    server.send ( 200, "text/html", data);
  });

  server.on("/pincontrol/button1auto", []() {

    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");

    StringData.lightState = "AUTO";
    LightControl();
    MqttPubLightState();
    String data = PinControlView();

    server.send ( 200, "text/html", data);
  });
}




///////////////////////////////////   WEB PAGES  End  //////////////////////////////////////////////



String XML;

void buildXML(){
XML=String(F("<?xml version='1.0'?>"));
XML+=String(F("<Donnees>")); 
XML+=String(F("<temperature>"));
XML+=StringData.temperatureString;
XML+=String(F(" °C"));
XML+=String(F("</temperature>"));
XML+=String(F("<humidity>"));
XML+=StringData.humidityString;
XML+=String(F(" %"));
XML+=String(F("</humidity>"));
XML+=String(F("<pressure>"));
XML+=StringData.pressureString;
XML+=String(F(" mm"));
XML+=String(F("</pressure>"));
XML+=String(F("<illuminance>"));
XML+=StringData.luxString;
XML+=String(F(" lux"));
XML+=String(F("</illuminance>"));
XML+=String(F("<uptime>"));
XML+=StringData.uptimeString;
XML+=String(F("</uptime>"));
XML+=String(F("<freeMemory>"));
XML+=StringData.freeMemoryString;
XML+=String(F("</freeMemory>"));
XML+=String(F("</Donnees>")); 
}


void handleXML(){
  buildXML();
  server.send(200,"text/xml",XML);
}




void setup() {

  #ifdef SHT21_ON
    myHTU21D.begin();
  #endif

  #ifdef DHT_ON
    dht.begin();
  #endif
  // Setup console
  #ifdef DEBUG
    Serial.begin(115200);
    delay(10);
    Serial.println();
  #endif

  if (!SPIFFS.begin()) {
    #ifdef DEBUG
    Serial.println(F("Failed to mount file system"));
    #endif
    return;
  }
/*
  if (!saveConfig()) {
    Serial.println("Failed to save config");
  } else {
    Serial.println("Config saved");
  }
*/
  if (!loadConfig()) {
    #ifdef DEBUG
    Serial.println(F("Failed to load config"));
    #endif
  } else {
    #ifdef DEBUG
    Serial.println(F("Config loaded"));
    #endif
  }

  pinMode(ConfDevice.light_pin, OUTPUT);
  pinMode(ConfDevice.motion_pin, INPUT);           // set pin to input


  client.setClient(espClient);
  IPAddress mqtt_ip = stringToIp(mqttServerIpStr);
  client.setServer(mqtt_ip, 1883);
  client.setCallback (callback);

  #ifdef BME280_ON
    bmeSensor.settings.commInterface = I2C_MODE;
    bmeSensor.settings.I2CAddress = 0x76;
    bmeSensor.settings.runMode = 3;
    bmeSensor.settings.tStandby = 0;
    bmeSensor.settings.filter = 4;
    bmeSensor.settings.tempOverSample = 5;
    bmeSensor.settings.pressOverSample = 5;
    bmeSensor.settings.humidOverSample = 5;
    bmeSensor.begin();
  #endif
  //Wire.begin(4,5); //SDA=4, SCL=5
  lightSensor.begin();
  Wire.setClock(100000);

  // scan Access Points
  scanWiFi();

  // start WiFi
  WiFi.mode(WIFI_AP_STA);
  if (staticIpMode == 3){
    IPAddress staticIP = stringToIp(staticIpStr);
    IPAddress staticGateway = stringToIp(staticGatewayStr);
    IPAddress staticSubnet = stringToIp(staticSubnetStr);
    WiFi.config(staticIP, staticGateway, staticSubnet);
  }
  WiFi.begin(ConfDevice.sta_ssid, ConfDevice.sta_pwd);

  waitConnected();
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.softAP(ConfDevice.module_id);
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ConfDevice.module_id);
  }


  setupWebUpdate();
  rebootWebPage();
  rootWebPage();
  web_espConf();
  web_mqttConf();
  web_pinControl();
  server.on("/xml",handleXML);

  // start Web Server
  server.begin();


  #ifdef DEBUG
  Serial.println();  Serial.println(F("Server started"));
  #endif
  // start mDNS responder
  if (!MDNS.begin(ConfDevice.module_id)) {
    #ifdef DEBUG
    Serial.println();  Serial.println(F("Error setting up MDNS responder!"));
    #endif
    while (1) {
      delay(1000);
      yield();
    }
  }
  #ifdef DEBUG
  Serial.println();  Serial.println(F("mDNS responder started"));
  #endif

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
}



void loop() {
  // handle web server
  server.handleClient();

  RebootESP();

  if (millis() - getDataTimer >= ConfDevice.get_data_delay){
    getDataTimer = millis();
    GetLightSensorData();
    #ifdef BME280_ON
      GetBmeSensorData();
    #endif

    #ifdef SHT21_ON
      GetSHT21SensorData();
    #endif

    GetUptimeData();
    GetFreeMemory();

    IPAddress espIP = WiFi.localIP();
    StringData.ipString = GetIpString(espIP);

    GetMacString();

    #ifdef DHT_ON
      DHT22Sensor();
    #endif

    #ifdef DEBUG
      TestSystemPrint();
    #endif
  }


  if (run == false){
    MotionDetect();
  }  

  if (millis() - motionTimer >= ConfDevice.motion_read_delay){
    motionTimer = millis();
    run = false;
    MotionDetect();
  }

  if (StringData.lightState == "AUTO"){
    LightControl();
  }


  if (WiFi.status() != WL_CONNECTED) {
    #ifdef DEBUG
    Serial.print(F("Connecting to "));
    Serial.print(ConfDevice.sta_ssid);
    Serial.println(F("..."));
    #endif
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ConfDevice.sta_ssid, ConfDevice.sta_pwd);
    delay(10);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    #ifdef DEBUG
    Serial.println(F("WiFi connected"));
    #endif
  }


  if (WiFi.status() == WL_CONNECTED) {

    if (!client.connected()) {
      client.connect(ConfDevice.mqtt_name);
    }

    if (client.connected())
      client.loop();
      MqttSubscribe();

    if (millis() - publishTimer >= ConfDevice.publish_delay){
      publishTimer = millis();
      MqttPubData();
    }
  }
}
