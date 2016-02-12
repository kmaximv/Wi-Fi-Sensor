#include "json_config.h"


void JsonConf::begin() {
conf_size = 0;
conf = AddPtr( conf, MODULE_ID        ,  "ESP8266"             );
conf = AddPtr( conf, STA_SSID         ,  "HomeNET"             );
conf = AddPtr( conf, STA_PWD          ,  "Asdf1234"            );
conf = AddPtr( conf, STATIC_IP_MODE   ,  "0"                   );
conf = AddPtr( conf, STATIC_IP        ,  "192.168.1.220"       );
conf = AddPtr( conf, STATIC_GATEWAY   ,  "192.168.1.1"         );
conf = AddPtr( conf, STATIC_SUBNET    ,  "255.255.255.0"       );
conf = AddPtr( conf, NTP_SERVER       ,  "europe.pool.ntp.org" );
conf = AddPtr( conf, TIME_ZONE        ,  "+6"                  );
conf = AddPtr( conf, MQTT_SERVER      ,  "192.168.1.200"       );
conf = AddPtr( conf, MQTT_PORT        ,  "1883"                );
conf = AddPtr( conf, MQTT_USER        ,  "none"                );
conf = AddPtr( conf, MQTT_PWD         ,  "none"                );
conf = AddPtr( conf, MQTT_NAME        ,  "_BedM"               );
conf = AddPtr( conf, PUBLISH_TOPIC    ,  "/stateSub/"          );
conf = AddPtr( conf, SUBSCRIBE_TOPIC  ,  "/statePub/"          );
conf = AddPtr( conf, COMMAND_PUB_TOPIC,  "/commandPub/"        );
conf = AddPtr( conf, LIGHT_PIN        ,  "13"                  );
conf = AddPtr( conf, LIGHTOFF_DELAY   ,  "5"                   );
conf = AddPtr( conf, LIGHT_PIN2       ,  "12"                  );
conf = AddPtr( conf, LIGHT2OFF_DELAY  ,  "5"                   );
conf = AddPtr( conf, MOTION_PIN       ,  "2"                   );
conf = AddPtr( conf, DHT_PIN          ,  "14"                  );
conf = AddPtr( conf, GET_DATA_DELAY   ,  "10"                  );
conf = AddPtr( conf, PUBLISH_DELAY    ,  "10"                  );
conf = AddPtr( conf, SUBSCRIBE_DELAY  ,  "60"                  );
conf = AddPtr( conf, MOTION_READ_DELAY,  "10"                  );
conf = AddPtr( conf, REBOOT_DELAY     ,  "1800"                );
conf = AddPtr( conf, UART_DELAY_ANALOG_PIN0       ,  "none"    );    
conf = AddPtr( conf, UART_DELAY_ANALOG_PIN1       ,  "none"    );    
conf = AddPtr( conf, UART_DELAY_ANALOG_PIN2       ,  "none"    );    
conf = AddPtr( conf, UART_DELAY_ANALOG_PIN3       ,  "none"    );    
conf = AddPtr( conf, UART_DELAY_ANALOG_PIN4       ,  "none"    );    
conf = AddPtr( conf, UART_DELAY_ANALOG_PIN5       ,  "none"    );    
conf = AddPtr( conf, GREEN_LIGHT_ON               ,  "8:00"    );            
conf = AddPtr( conf, GREEN_LIGHT_OFF              ,  "22:00"   );            
conf = AddPtr( conf, GREEN_LIGHT_PIN              ,  "12"      );            
conf = AddPtr( conf, GREEN_HUMIDITY_THRESHOLD_UP  ,  "800"     );            
conf = AddPtr( conf, GREEN_HUMIDITY_THRESHOLD_DOWN,  "250"     );            
conf = AddPtr( conf, GREEN_HUMIDITY_SENSOR_PIN    ,  "20"      );            
conf = AddPtr( conf, GREEN_PUMP_PIN               ,  "33"      );            
}


void JsonConf::setStr(int size, String str){
  if (size <= conf_size){
    char charBufVar[50];
    str.toCharArray(charBufVar, 50);

    strcpy(conf[size], charBufVar);
  }
}

void JsonConf::set(int size, char *str){
  if (size <= conf_size)
    strcpy(conf[size], str);
}


char **JsonConf::AddPtr (char **conf, int size, char *str)
{
    if(size == 0){
        conf = new char *[size+1];
    }
    else{                      
        char **copy = new char* [size+1];
        for(int i = 0; i < size; i++)
        {
            copy[i] = conf[i];
        }  
 
        delete [] conf;      
        conf = copy;     
    }
    conf[size] = new char [strlen(str) + 1];
    strcpy(conf[size], str);
    conf_size++;
    return conf;
}


bool JsonConf::saveConfig() {
  StaticJsonBuffer<1024> jsonBuffer;
#ifdef DEBUG_JSON_CONFIG
  Serial.print(F("saveConfig()"));  Serial.println();
#endif

  JsonObject& json = jsonBuffer.createObject();

  for (size_t i = 0; i < conf_size; i++)
  {
    const char* key = (const char*)i;
    json.set(key, conf[i]);
#ifdef DEBUG_JSON_CONFIG
    Serial.print(F("Json Save Key: ")); Serial.print(i); 
    Serial.print(F(" = Value: ")); Serial.println(conf[i]);
#endif
  }

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
#ifdef DEBUG_JSON_CONFIG
    Serial.println(F("Failed to open config file for writing"));
#endif
    return false;
  }
#ifdef DEBUG_JSON_CONFIG
  Serial.println();
  json.printTo(Serial);
  Serial.println();
#endif
  json.printTo(configFile);
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


  for (size_t i = 0; i < conf_size; i++)
  {
    const char* key = (const char*)i;
    if (json.get(key)){
      const char* val = json.get(key);
      char* newVal = strdup(val);
      set(i, newVal);
#ifdef DEBUG_JSON_CONFIG
      Serial.print(F("Json Load Key: ")); Serial.println(i); 
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