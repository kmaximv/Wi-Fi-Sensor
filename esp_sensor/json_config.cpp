#include "json_config.h"


const char* JsonConf::GetDataCommon(const size_t i) {

  if (i < NUM_COMMON_KEYS){
    char** data_set = jconfig_common[i];
    const char* val = data_set[JSON_VALUE];
    return val;
  } else {
    const char* err = "none";
    return err;
  }
}


const char* JsonConf::GetDataCommon(const size_t i, const size_t j) {

  char** data_set = jconfig_common[i];

  if (j == JSON_KEY){
    const char* key = data_set[JSON_KEY];
    return key;
  } else if (j == JSON_VALUE){
    const char* val = data_set[JSON_VALUE];
    return val;
  } else {
    const char* err = "none";
    return err;
  }
}


bool JsonConf::SetDataCommon(const size_t i, String str) {
  
  if (i < NUM_COMMON_KEYS){
    char** data_set = jconfig_common[i];
    char val[sizeof(str)];
    str.toCharArray(val, sizeof(val));
    data_set[JSON_VALUE] = val;
    return true;
  } else {
    return false;
  }
}

bool JsonConf::SetDataCommon(const size_t i, char* str) {
  
  if (i < NUM_COMMON_KEYS){
    char** data_set = jconfig_common[i];
    data_set[JSON_VALUE] = str;
    return true;
  } else {
    return false;
  }
}


bool JsonConf::SetDataGreen(const size_t i, String str) {
  
  if (i < NUM_GREEN_KEYS){
    char** data_set = jconfig_green[i];
    char val[sizeof(str)];
    str.toCharArray(val, sizeof(val));
    data_set[JSON_VALUE] = val;
    return true;
  } else {
    return false;
  }
}



bool JsonConf::saveConfig() {
  StaticJsonBuffer<200> jsonBuffer;
  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("saveConfig()"));  Serial.println();
  #endif

  JsonObject& json = jsonBuffer.createObject();

  for (size_t i = 0; i < sizeof(jconfig_common) / sizeof(jconfig_common[0]); i++)
  {
    char** data_set = jconfig_common[i];
    const char* key = data_set[JSON_KEY];
    const char* val = data_set[JSON_VALUE];
    json[key] = val;
    #ifdef DEBUG_JSON_CONFIG
      Serial.print(F("Json Save Key: ")); Serial.print(key); 
      Serial.print(F(" = Value: ")); Serial.println(val);
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


bool JsonConf::saveConfig(size_t i) {
  StaticJsonBuffer<200> jsonBuffer;
  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("saveConfig()"));  Serial.println();
  #endif

  JsonObject& json = jsonBuffer.createObject();


  char** data_set = jconfig_common[i];

  const char* key = data_set[JSON_KEY];
  const char* val = data_set[JSON_VALUE];

  json[key] = val;
  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("Json Save Key: ")); Serial.print(key); 
    Serial.print(F(" = Value: ")); Serial.println(val);
  #endif

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

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    #ifdef DEBUG_JSON_CONFIG
      Serial.println(F("Failed to parse config file"));
    #endif
    return false;
  }


  for (size_t i = 0; i < sizeof(jconfig_common) / sizeof(jconfig_common[0]); i++)
  {
    char** data_set = jconfig_common[i];
    char* key = data_set[JSON_KEY];
    SetDataCommon(i, json[key]);
    #ifdef DEBUG_JSON_CONFIG
      Serial.print(F("Json Load Key: ")); Serial.println(key); 
    #endif
  }

  return true;
}


/*
for (size_t i = 0; i < sizeof(jconfig_common) / sizeof(jconfig_common[0]); i++)
{
    const char** data_set = jconfig_common[i];
    printf("data_set[%u]\n", i);
    for (size_t j = 0; data_set[j]; j++)
    {
        printf("  [%s]\n", data_set[j]);
    }
}
*/