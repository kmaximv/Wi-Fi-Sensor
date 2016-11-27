
void CFG_Default() {

}



/*********************************************************************************************\
 * Wi-Fi
\*********************************************************************************************/

void scanWiFi(void) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: scanWiFi Start");

  int founds = WiFi.scanNetworks();

  if (founds == 0) {
    addLog_P(LOG_LEVEL_ERROR, "scanWiFi: No networks found");
  } else if (LOG_LEVEL_INFO <= atoi(JConf.serial_log_level)){
    snprintf_P(log, sizeof(log), PSTR("scanWiFi: %d networks found"), founds);
    addLog(LOG_LEVEL_INFO, log);

    for (size_t i = 0; i < founds; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);  Serial.print(F(": "));  Serial.print(WiFi.SSID(i));  Serial.print(F(" ("));  Serial.print(WiFi.RSSI(i));  Serial.print(F(")"));
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? F(" ") : F("*"));
      delay(10);
    }
  }

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

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: scanWiFi load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
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
      addLog_P(LOG_LEVEL_ERROR, "Wifi: Error Wifi AP_STA!");
      delay(1000);
  }
}



bool wifiTryConnect(){
  byte i=0;
  while (WiFi.status() != WL_CONNECTED && i<40) {  //try to connect
    switch(WiFi.status()) {
    case 1:
      addLog_P(LOG_LEVEL_ERROR, "Wifi: No SSID found!");
      break;
    case 4:
      addLog_P(LOG_LEVEL_ERROR, "Wifi: No Connection!");
      break;
    default:
      addLog_P(LOG_LEVEL_INFO, "Wifi: Connecting...");
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



bool wifiSTA() {
  WiFi.mode(WIFI_STA);                            //setup station mode
  WiFi.begin(JConf.sta_ssid, JConf.sta_pwd);
  delay(500);
  
  wifi_set_phy_mode((phy_mode_t)PHY_MODE_11N);    //setup PHY_MODE

  if (!wifiTryConnect()) {
    return false;
  }
  WiFi.hostname(JConf.module_id);
  return true; 
}



bool wifiAP_STA() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(JConf.sta_ssid, JConf.sta_pwd);

  if (!wifiTryConnect()) {
    return false;
  }
  wifiAPSettings();
  return true;
}



void wifiReconnect() {
  if (WiFi.status() != WL_CONNECTED && atoi(JConf.wifi_mode) != AP && wifiSafeMode == false) {
    addLog_P(LOG_LEVEL_INFO, "Wifi: Reconnecting...");
    WiFiSetup();
  }
}



bool WiFiSetup()
{
  wifi_set_sleep_type ((sleep_type_t)NONE_SLEEP_T);   // NONE_SLEEP_T,LIGHT_SLEEP_T,MODEM_SLEEP_T
  WiFi.disconnect();

  if (atoi(JConf.wifi_mode) == AP) {
    wifiAP();
  } else if (atoi(JConf.wifi_mode) == STA && !wifiSTA()) {
    return false;
  } else if (atoi(JConf.wifi_mode) == AP_STA && !wifiAP_STA()) {
    return false;
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
  addLog_P(LOG_LEVEL_INFO, "Wifi: Safe mode started");
  wifiSafeMode = true;
}



void wifiSafeModeReconnect() {
  if (wifiSafeMode == true && WiFiSetup()) {
    wifiSafeMode = false; 
  }
}
/*********************************************************************************************\
 *                                                                                      Wi-fi*
\*********************************************************************************************/



void GetFreeMemory () {
  
  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: GetFreeMemory Start");

  freeMemoryString = String(ESP.getFreeHeap());

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: GetFreeMemory load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



String GetIpString (IPAddress ip) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: GetIpString Start");

  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: GetIpString load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  return ipStr;
}



void GetMacString () {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: GetMacString Start");

  uint8_t macData[6];
  WiFi.macAddress(macData);
  sprintf_P(value_buff, (const char *)F("%x:%x:%x:%x:%x:%x"), macData[0], macData[1], macData[2], macData[3], macData[4], macData[5]);
  
  macString = String(value_buff);

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: GetMacString load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



IPAddress stringToIp (String strIp) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: stringToIp Start");

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

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: stringToIp load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  return ip;
}



bool isIPValid(const char * IP) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: isIPValid Start");

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

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: isIPValid load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);

  return true;
}



void PWMChange(int pin, int bright) {

  char log[LOGSZ];
  unsigned long start_time = millis();
  addLog_P(LOG_LEVEL_DEBUG_MORE, "Func: PWMChange Start");

  fading[pin].cycleEnd = bright;

  if ( ( atoi(JConf.light_smooth) == 0 && fading[pin].pin == atoi(JConf.light_pin) )   ||   ( atoi(JConf.light2_smooth) == 0 && fading[pin].pin == atoi(JConf.light2_pin) ) ){
    if (fading[pin].cycleNow < fading[pin].cycleEnd){
      fading[pin].cycleNow = 1022;
    } else if (fading[pin].cycleNow > fading[pin].cycleEnd){
      fading[pin].cycleNow = 1;
    }
  }

  unsigned long load_time = millis() - start_time;
  snprintf_P(log, sizeof(log), PSTR("Func: PWMChange load time: %d"), load_time);
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}



void FadeSwitchDelay(int pin){

  char log[LOGSZ];

  if (millis() - fading[pin].timerFade >= fading[pin].delayFade && fading[pin].cycleNow != fading[pin].cycleEnd){
    fading[pin].timerFade = millis();
    if (fading[pin].cycleNow < fading[pin].cycleEnd){
      fading[pin].cycleNow = constrain(fading[pin].cycleNow + 10, 0, 1023);
    } else if (fading[pin].cycleNow > fading[pin].cycleEnd){
      fading[pin].cycleNow = constrain(fading[pin].cycleNow - 10, 0, 1023);
    }
    analogWrite(fading[pin].pin, fading[pin].cycleNow);

    snprintf_P(log, sizeof(log), PSTR("FadeSwitchDelay: PWM pin: %d, PWM Value: %d"), pin, fading[pin].cycleNow);
    addLog(LOG_LEVEL_DEBUG_MORE, log);
  }
}


/*
void FadeSwitchLoop(){
  for ( size_t i = 0; i < FADE_PINS; i++ ){
    FadeSwitchDelay(i);
  }
}
*/

void FadeSwitchLoop(){
  if (fading[0].cycleEnd != fading[0].cycleNow) {
    FadeSwitchDelay(0);
    fading[1].timerFade = millis();
  } else {
    FadeSwitchDelay(1);
  }
}




void restartESP() {
  addLog_P(LOG_LEVEL_INFO, "restartESP: Restart ESP!");
  delay(100);
  ESP.restart();
}



void deleteConfigFile() {
  pinMode(atoi(JConf.reset_pin), INPUT); 
  if (digitalRead(atoi(JConf.reset_pin)) == LOW) {
    delay(3000);
    if (digitalRead(atoi(JConf.reset_pin)) == LOW) {
      addLog_P(LOG_LEVEL_INFO, "deleteConfigFile: Reset pin pressed. Delete config file!!!");
      JConf.deleteConfig();
    }
  } 
}


/*********************************************************************************************\
 * Syslog
\*********************************************************************************************/

void syslog(const char *message)
{
  char mess[MESSZ], str[TOPSZ+MESSZ];

  portUDP.beginPacket(JConf.sys_log_host, atoi(JConf.sys_log_port));
  strlcpy(mess, message, sizeof(mess));
  mess[sizeof(mess)-1] = 0;
  snprintf_P(str, sizeof(str), PSTR("%s => %s"), JConf.module_id, mess);
  portUDP.write(str);
  portUDP.endPacket();
}



void addLog(byte loglevel, const char *line)
{
  //char mxtime[9];
  
//  snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d:%02d"), rtcTime.Hour, rtcTime.Minute, rtcTime.Second);
 
#ifdef DEBUG_ESP_PORT
  if (atoi(JConf.ntp_enable) == 1) {
    DEBUG_ESP_PORT.printf("%s %s\n", ntpTimeString, line);  
  } else{
    DEBUG_ESP_PORT.printf("%s\n", line);  
  }
#endif  // DEBUG_ESP_PORT

  if (atoi(JConf.ntp_enable) == 1) {
    if (loglevel <= atoi(JConf.serial_log_level)) Serial.printf("%s %s\n", ntpTimeString.c_str(), line);
  } else {
    if (loglevel <= atoi(JConf.serial_log_level)) Serial.printf("%s\n", line);
  }

#ifdef USE_WEBSERVER
  if (loglevel <= atoi(JConf.web_log_level)) {
    if (atoi(JConf.ntp_enable) == 1) {
      Log[logidx] = ntpTimeString + " " + String(line);
    } else {
      Log[logidx] = String(line);
    }
    logidx++;
    if (logidx > MAX_LOG_LINES -1) logidx = 0;
  }
#endif  // USE_WEBSERVER
  if ((WiFi.status() == WL_CONNECTED) && (loglevel <= atoi(JConf.sys_log_level))) {
    syslog(line);
  }
}



void addLog_P(byte loglevel, const char *formatP)
{
  char mess[MESSZ];
  
  snprintf_P(mess, sizeof(mess), formatP);
  addLog(loglevel, mess);
}

/*********************************************************************************************\
 *                                                                                    Syslog *
\*********************************************************************************************/

