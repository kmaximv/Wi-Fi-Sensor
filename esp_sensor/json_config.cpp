#include "json_config.h"


const char* JsonConf::GetDataCommon(const size_t i) {

  if (i < NUM_COMMON_KEYS){
    const char** data_set = jconfig_common[i];
    const char* val = data_set[JSON_VALUE];
    return val;
  } 
}


const char* JsonConf::GetDataCommon(const size_t i, const size_t j) {

  const char** data_set = jconfig_common[i];

  if (j == JSON_KEY){
    const char* key = data_set[JSON_KEY];
    return key;
  } else if (j == JSON_VALUE){
    const char* val = data_set[JSON_VALUE];
    return val;
  } 
}


bool JsonConf::SetDataCommon(const size_t i, String str) {
  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("SetDataCommon(String)"));  Serial.println();
    Serial.print(F("String : "));  Serial.println(str);
  #endif
  
  if (i < NUM_COMMON_KEYS){
    //char** data_set = jconfig_common[i];
    char val[str.length() + 1];
    str.toCharArray(val, str.length() + 1);
    jconfig_common[i][JSON_VALUE] = val;
    return true;
  } else {
    return false;
  }
}

bool JsonConf::SetDataCommonChar(const size_t i, const char* str) {

  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("SetDataCommon(char*)"));  Serial.println();
    Serial.print(F("char* : "));  Serial.println(str);
  #endif

  if (i < NUM_COMMON_KEYS){
    const char** data_set = jconfig_common[i];
    jconfig_common[i][JSON_VALUE] = (char*)str;
    return true;
  } else {
    return false;
  }
}


bool JsonConf::SetDataGreen(const size_t i, String str) {
  
  if (i < NUM_GREEN_KEYS){
    const char** data_set = jconfig_green[i];
    char val[sizeof(str)];
    str.toCharArray(val, sizeof(val));
    data_set[JSON_VALUE] = val;
    return true;
  } else {
    return false;
  }
}



bool JsonConf::saveConfig() {
  StaticJsonBuffer<1024> jsonBuffer;
  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("saveConfig()"));  Serial.println();
  #endif

  JsonObject& json = jsonBuffer.createObject();

  for (size_t i = 0; i < sizeof(jconfig_common) / sizeof(jconfig_common[0]); i++)
  {
    const char** data_set = jconfig_common[i];
    const char* key = data_set[JSON_KEY];
    const char* val = data_set[JSON_VALUE];
    json.set(key, val);
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

  json.printTo(Serial);
  json.printTo(configFile);
  configFile.close();
  return true;
}


bool JsonConf::saveConfig(size_t i) {
  StaticJsonBuffer<1024> jsonBuffer;
  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("saveConfig(i)"));  Serial.println();
  #endif

  JsonObject& json = jsonBuffer.createObject();


  const char** data_set = jconfig_common[i];

  const char* key = data_set[JSON_KEY];
  const char* val = data_set[JSON_VALUE];
  json.set(key, val);
  //json[key] = val;
  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("Json Save Key: ")); Serial.print(key); 
    Serial.print(F(" = Value: ")); Serial.println(val);
  #endif

  File configFile = SPIFFS.open("/config.json", "w+");
  if (!configFile) {
    #ifdef DEBUG_JSON_CONFIG
      Serial.println(F("Failed to open config file for writing"));
    #endif
    return false;
  }

  json.printTo(Serial);
  json.printTo(configFile);
  delay(100);
  configFile.close();
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

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    #ifdef DEBUG_JSON_CONFIG
      Serial.println(F("Failed to parse config file"));
    #endif
    return false;
  }


  for (size_t i = 0; i < sizeof(jconfig_common) / sizeof(jconfig_common[0]); i++)
  {
    const char** data_set = jconfig_common[i];
    const char* key = data_set[JSON_KEY];
    if (json.get(key)){
      const char* val = json.get(key);
      SetDataCommonChar(i, val);
      #ifdef DEBUG_JSON_CONFIG
        Serial.print(F("Json Load Key: ")); Serial.println(key); 
        Serial.print(F("Json Load Value: ")); Serial.println(val); 
      #endif
    } else {
      configFile.close();
      saveConfig();
      return false;
    }
  }
  configFile.close();
  return true;
}


bool JsonConf::PrintConfigFile(){
  #ifdef DEBUG_JSON_CONFIG
    Serial.print(F("PrintConfigFile()"));  Serial.println();
  #endif

  File configFile = SPIFFS.open("/config.json", "r+");
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

  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    #ifdef DEBUG_JSON_CONFIG
      Serial.println(F("Failed to parse config file"));
      if (SPIFFS.format()){
        Serial.println(F("!!!!!!!!!Format SPIFFS"));
      }
      SPIFFS.remove("/config.json");
    #endif
    return false;
  }


  for (size_t i = 0; i < sizeof(jconfig_common) / sizeof(jconfig_common[0]); i++)
  {
    const char** data_set = jconfig_common[i];
    const char* key = data_set[JSON_KEY];
    #ifdef DEBUG_JSON_CONFIG
      if (json.get(key)){
        const char* val = json.get(key);
        Serial.print(F("Json Key: ")); Serial.println(key); 
        Serial.print(F("Json Value: ")); Serial.println(val); 
      } else {
        //saveConfig(i);
      }
    #endif
  }
  configFile.close();
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