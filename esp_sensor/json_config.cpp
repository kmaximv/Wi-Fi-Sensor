#include "json_config.h"


bool JsonConf::saveConfig() {
  StaticJsonBuffer<1000> jsonBuffer;
  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("saveConfig()"));  Serial.println();
  #endif

  JsonObject& json = jsonBuffer.createObject();

  for (int i = 0; i < NUM_COMMON_KEYS; i++){
    json[jconfig_common_key[i]] = jconfig_common_value[i];
    #ifdef DEBUG_JSON_CONFIG
      Serial.print(F("Json Save Key: ")); Serial.print(jconfig_common_key[i]); 
      Serial.print(F(" = Value: ")); Serial.println(jconfig_common_value[i]);
    #endif
  }

#ifdef UART_ON
  for (int i = 0; i < NUM_UART_KEYS; i++){
    json[jconfig_uart_key[i]] = jconfig_uart_value[i];
    #ifdef DEBUG_JSON_CONFIG
      Serial.print(F("Json Save Key: ")); Serial.print(jconfig_uart_key[i]); 
      Serial.print(F(" = Value: ")); Serial.println(jconfig_uart_value[i]);
    #endif
  }
#endif


  for (int i = 0; i < NUM_GREEN_KEYS; i++){
    json[jconfig_green[i][JSON_KEY]] = jconfig_green[i][JSON_VALUE];
    #ifdef DEBUG_JSON_CONFIG
      Serial.print(F("Json Save Key: ")); Serial.print(jconfig_green[i][JSON_KEY]); 
      Serial.print(F(" = Value: ")); Serial.println(jconfig_green[i][JSON_VALUE]);
    #endif
  }


  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    #ifdef DEBUG_JSON_CONFIG
      Serial.println(F("Failed to open config file for writing"));
    #endif
    return false;
  }

  json.printTo(configFile);
  return true;
}



bool JsonConf::loadConfig() {
  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("loadConfig()"));  Serial.println();
  #endif

  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    #ifdef DEBUG_JSON_CONFIG
      Serial.println(F("Failed to open config file"));
    #endif
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    #ifdef DEBUG_JSON_CONFIG
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

  if (!json.success()) {
    #ifdef DEBUG_JSON_CONFIG
      Serial.println(F("Failed to parse config file"));
    #endif
    return false;
  }

  for (int i = 0; i < NUM_COMMON_KEYS; i++){
    const char* val = json[jconfig_common_key[i]];
    jconfig_common_value[i] = String(val);
    #ifdef DEBUG_JSON_CONFIG
      Serial.print(F("Json Load Key: ")); Serial.print(jconfig_common_key[i]); 
      Serial.print(F(" = Value: ")); Serial.println(jconfig_common_value[i]);
    #endif
  }

#ifdef UART_ON
  for (int i = 0; i < NUM_UART_KEYS; i++){
    const char* val = json[jconfig_uart_key[i]];
    jconfig_uart_value[i] = String(val);
    #ifdef DEBUG_JSON_CONFIG
      Serial.print(F("Json Load Key: ")); Serial.print(jconfig_uart_key[i]); 
      Serial.print(F(" = Value: ")); Serial.println(jconfig_uart_value[i]);
    #endif
  }
#endif

  return true;
}