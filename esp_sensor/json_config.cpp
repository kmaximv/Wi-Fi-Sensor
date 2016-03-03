#include "json_config.h"


bool JsonConf::saveConfig() {
  StaticJsonBuffer<1024> jsonBuffer;
#ifdef DEBUG_JSON_CONFIG
  Serial.print(F("saveConfig()"));  Serial.println();
#endif

  JsonObject& json = jsonBuffer.createObject();


  json["module_id"]                     = module_id                    ;                           
  json["wifi_mode"]                     = wifi_mode                    ;                           
  json["sta_ssid"]                      = sta_ssid                     ;                         
  json["sta_pwd"]                       = sta_pwd                      ;                       
  json["static_ip"]                     = static_ip                    ;                           
  json["static_gateway"]                = static_gateway               ;                                     
  json["static_subnet"]                 = static_subnet                ;                                   
  json["ntp_server"]                    = ntp_server                   ;                             
  json["my_time_zone"]                  = my_time_zone                 ;                           
  json["mqtt_server"]                   = mqtt_server                  ;                               
  json["mqtt_port"]                     = mqtt_port                    ;                           
  json["mqtt_user"]                     = mqtt_user                    ;                           
  json["mqtt_pwd"]                      = mqtt_pwd                     ;                         
  json["mqtt_name"]                     = mqtt_name                    ;                           
  json["publish_topic"]                 = publish_topic                ;                                   
  json["subscribe_topic"]               = subscribe_topic              ;                                       
  json["command_pub_topic"]             = command_pub_topic            ;                                           
  json["light_pin"]                     = light_pin                    ;                           
  json["lightoff_delay"]                = lightoff_delay               ;                                     
  json["light_pin2"]                    = light_pin2                   ;                             
  json["light2off_delay"]               = light2off_delay              ;                                       
  json["motion_pin"]                    = motion_pin                   ;                             
  json["dht_pin"]                       = dht_pin                      ;                       
  json["get_data_delay"]                = get_data_delay               ;                                     
  json["publish_delay"]                 = publish_delay                ;                                   
  json["subscribe_delay"]               = subscribe_delay              ;                                       
  json["motion_read_delay"]             = motion_read_delay            ;                                           
  json["reboot_delay"]                  = reboot_delay                 ;                                 
  json["uart_delay_analog_pin0"]        = uart_delay_analog_pin0       ;                                                     
  json["uart_delay_analog_pin1"]        = uart_delay_analog_pin1       ;                                                     
  json["uart_delay_analog_pin2"]        = uart_delay_analog_pin2       ;                                                     
  json["uart_delay_analog_pin3"]        = uart_delay_analog_pin3       ;                                                     
  json["uart_delay_analog_pin4"]        = uart_delay_analog_pin4       ;                                                     
  json["uart_delay_analog_pin5"]        = uart_delay_analog_pin5       ;                                                     
  json["green_light_on"]                = green_light_on               ;                                     
  json["green_light_off"]               = green_light_off              ;                                       
  json["green_light_pin"]               = green_light_pin              ;                                       
  json["green_humidity_threshold_up"]   = green_humidity_threshold_up  ;                                                               
  json["green_humidity_threshold_down"] = green_humidity_threshold_down;                                                                   
  json["green_humidity_sensor_pin"]     = green_humidity_sensor_pin    ;                                                           
  json["green_pump_pin"]                = green_pump_pin               ;                                     

  json["static_ip_enable"]              = static_ip_enable             ;                                     
  json["ntp_enable"]                    = ntp_enable                   ;                               
  json["mqtt_enable"]                   = mqtt_enable                  ;                               
  json["bme280_enable"]                 = bme280_enable                ;                               
  json["sht21_enable"]                  = sht21_enable                 ;                               
  json["bh1750_enable"]                 = bh1750_enable                ;                               
  json["motion_sensor_enable"]          = motion_sensor_enable         ;                               




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
    saveConfig();
#endif
    return false;
  }

  size_t size = configFile.size();
  if (size > 2048) {
#ifdef DEBUG_JSON_CONFIG
    Serial.println(F("Config file size is too large"));
    SPIFFS.remove("/config.json");
    saveConfig();
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
    SPIFFS.remove("/config.json");
    saveConfig();
#endif
    return false;
  }


  if (json.containsKey("module_id"                     )) {  const char* module_id_char                     = json["module_id"                    ];    sprintf_P(module_id,                     ("%s"), module_id_char                    ); }
  if (json.containsKey("wifi_mode"                     )) {  const char* wifi_mode_char                     = json["wifi_mode"                    ];    sprintf_P(wifi_mode,                     ("%s"), wifi_mode_char                    ); }
  if (json.containsKey("sta_ssid"                      )) {  const char* sta_ssid_char                      = json["sta_ssid"                     ];    sprintf_P(sta_ssid,                      ("%s"), sta_ssid_char                     ); }
  if (json.containsKey("sta_pwd"                       )) {  const char* sta_pwd_char                       = json["sta_pwd"                      ];    sprintf_P(sta_pwd,                       ("%s"), sta_pwd_char                      ); }
  if (json.containsKey("static_ip"                     )) {  const char* static_ip_char                     = json["static_ip"                    ];    sprintf_P(static_ip,                     ("%s"), static_ip_char                    ); }
  if (json.containsKey("static_gateway"                )) {  const char* static_gateway_char                = json["static_gateway"               ];    sprintf_P(static_gateway,                ("%s"), static_gateway_char               ); }
  if (json.containsKey("static_subnet"                 )) {  const char* static_subnet_char                 = json["static_subnet"                ];    sprintf_P(static_subnet,                 ("%s"), static_subnet_char                ); }
  if (json.containsKey("ntp_server"                    )) {  const char* ntp_server_char                    = json["ntp_server"                   ];    sprintf_P(ntp_server,                    ("%s"), ntp_server_char                   ); }
  if (json.containsKey("my_time_zone"                  )) {  const char* my_time_zone_char                  = json["my_time_zone"                 ];    sprintf_P(my_time_zone,                  ("%s"), my_time_zone_char                 ); }
  if (json.containsKey("mqtt_server"                   )) {  const char* mqtt_server_char                   = json["mqtt_server"                  ];    sprintf_P(mqtt_server,                   ("%s"), mqtt_server_char                  ); }
  if (json.containsKey("mqtt_port"                     )) {  const char* mqtt_port_char                     = json["mqtt_port"                    ];    sprintf_P(mqtt_port,                     ("%s"), mqtt_port_char                    ); }
  if (json.containsKey("mqtt_user"                     )) {  const char* mqtt_user_char                     = json["mqtt_user"                    ];    sprintf_P(mqtt_user,                     ("%s"), mqtt_user_char                    ); }
  if (json.containsKey("mqtt_pwd"                      )) {  const char* mqtt_pwd_char                      = json["mqtt_pwd"                     ];    sprintf_P(mqtt_pwd,                      ("%s"), mqtt_pwd_char                     ); }
  if (json.containsKey("mqtt_name"                     )) {  const char* mqtt_name_char                     = json["mqtt_name"                    ];    sprintf_P(mqtt_name,                     ("%s"), mqtt_name_char                    ); }
  if (json.containsKey("publish_topic"                 )) {  const char* publish_topic_char                 = json["publish_topic"                ];    sprintf_P(publish_topic,                 ("%s"), publish_topic_char                ); }
  if (json.containsKey("subscribe_topic"               )) {  const char* subscribe_topic_char               = json["subscribe_topic"              ];    sprintf_P(subscribe_topic,               ("%s"), subscribe_topic_char              ); }
  if (json.containsKey("command_pub_topic"             )) {  const char* command_pub_topic_char             = json["command_pub_topic"            ];    sprintf_P(command_pub_topic,             ("%s"), command_pub_topic_char            ); }
  if (json.containsKey("light_pin"                     )) {  const char* light_pin_char                     = json["light_pin"                    ];    sprintf_P(light_pin,                     ("%s"), light_pin_char                    ); }
  if (json.containsKey("lightoff_delay"                )) {  const char* lightoff_delay_char                = json["lightoff_delay"               ];    sprintf_P(lightoff_delay,                ("%s"), lightoff_delay_char               ); }
  if (json.containsKey("light_pin2"                    )) {  const char* light_pin2_char                    = json["light_pin2"                   ];    sprintf_P(light_pin2,                    ("%s"), light_pin2_char                   ); }
  if (json.containsKey("light2off_delay"               )) {  const char* light2off_delay_char               = json["light2off_delay"              ];    sprintf_P(light2off_delay,               ("%s"), light2off_delay_char              ); }
  if (json.containsKey("motion_pin"                    )) {  const char* motion_pin_char                    = json["motion_pin"                   ];    sprintf_P(motion_pin,                    ("%s"), motion_pin_char                   ); }
  if (json.containsKey("dht_pin"                       )) {  const char* dht_pin_char                       = json["dht_pin"                      ];    sprintf_P(dht_pin,                       ("%s"), dht_pin_char                      ); }
  if (json.containsKey("get_data_delay"                )) {  const char* get_data_delay_char                = json["get_data_delay"               ];    sprintf_P(get_data_delay,                ("%s"), get_data_delay_char               ); }
  if (json.containsKey("publish_delay"                 )) {  const char* publish_delay_char                 = json["publish_delay"                ];    sprintf_P(publish_delay,                 ("%s"), publish_delay_char                ); }
  if (json.containsKey("subscribe_delay"               )) {  const char* subscribe_delay_char               = json["subscribe_delay"              ];    sprintf_P(subscribe_delay,               ("%s"), subscribe_delay_char              ); }
  if (json.containsKey("motion_read_delay"             )) {  const char* motion_read_delay_char             = json["motion_read_delay"            ];    sprintf_P(motion_read_delay,             ("%s"), motion_read_delay_char            ); }
  if (json.containsKey("reboot_delay"                  )) {  const char* reboot_delay_char                  = json["reboot_delay"                 ];    sprintf_P(reboot_delay,                  ("%s"), reboot_delay_char                 ); }
  if (json.containsKey("uart_delay_analog_pin0"        )) {  const char* uart_delay_analog_pin0_char        = json["uart_delay_analog_pin0"       ];    sprintf_P(uart_delay_analog_pin0,        ("%s"), uart_delay_analog_pin0_char       ); }
  if (json.containsKey("uart_delay_analog_pin1"        )) {  const char* uart_delay_analog_pin1_char        = json["uart_delay_analog_pin1"       ];    sprintf_P(uart_delay_analog_pin1,        ("%s"), uart_delay_analog_pin1_char       ); }
  if (json.containsKey("uart_delay_analog_pin2"        )) {  const char* uart_delay_analog_pin2_char        = json["uart_delay_analog_pin2"       ];    sprintf_P(uart_delay_analog_pin2,        ("%s"), uart_delay_analog_pin2_char       ); }
  if (json.containsKey("uart_delay_analog_pin3"        )) {  const char* uart_delay_analog_pin3_char        = json["uart_delay_analog_pin3"       ];    sprintf_P(uart_delay_analog_pin3,        ("%s"), uart_delay_analog_pin3_char       ); }
  if (json.containsKey("uart_delay_analog_pin4"        )) {  const char* uart_delay_analog_pin4_char        = json["uart_delay_analog_pin4"       ];    sprintf_P(uart_delay_analog_pin4,        ("%s"), uart_delay_analog_pin4_char       ); }
  if (json.containsKey("uart_delay_analog_pin5"        )) {  const char* uart_delay_analog_pin5_char        = json["uart_delay_analog_pin5"       ];    sprintf_P(uart_delay_analog_pin5,        ("%s"), uart_delay_analog_pin5_char       ); }
  if (json.containsKey("green_light_on"                )) {  const char* green_light_on_char                = json["green_light_on"               ];    sprintf_P(green_light_on,                ("%s"), green_light_on_char               ); }
  if (json.containsKey("green_light_off"               )) {  const char* green_light_off_char               = json["green_light_off"              ];    sprintf_P(green_light_off,               ("%s"), green_light_off_char              ); }
  if (json.containsKey("green_light_pin"               )) {  const char* green_light_pin_char               = json["green_light_pin"              ];    sprintf_P(green_light_pin,               ("%s"), green_light_pin_char              ); }
  if (json.containsKey("green_humidity_threshold_up"   )) {  const char* green_humidity_threshold_up_char   = json["green_humidity_threshold_up"  ];    sprintf_P(green_humidity_threshold_up,   ("%s"), green_humidity_threshold_up_char  ); }
  if (json.containsKey("green_humidity_threshold_down" )) {  const char* green_humidity_threshold_down_char = json["green_humidity_threshold_down"];    sprintf_P(green_humidity_threshold_down, ("%s"), green_humidity_threshold_down_char); }
  if (json.containsKey("green_humidity_sensor_pin"     )) {  const char* green_humidity_sensor_pin_char     = json["green_humidity_sensor_pin"    ];    sprintf_P(green_humidity_sensor_pin,     ("%s"), green_humidity_sensor_pin_char    ); }
  if (json.containsKey("green_pump_pin"                )) {  const char* green_pump_pin_char                = json["green_pump_pin"               ];    sprintf_P(green_pump_pin,                ("%s"), green_pump_pin_char               ); }

  if (json.containsKey("static_ip_enable"              )) {  const char* static_ip_enable_char              = json["static_ip_enable"             ];    sprintf_P(static_ip_enable,              ("%s"), static_ip_enable_char             ); }
  if (json.containsKey("ntp_enable"                    )) {  const char* ntp_enable_char                    = json["ntp_enable"                   ];    sprintf_P(ntp_enable,                    ("%s"), ntp_enable_char                   ); }
  if (json.containsKey("mqtt_enable"                   )) {  const char* mqtt_enable_char                   = json["mqtt_enable"                  ];    sprintf_P(mqtt_enable,                   ("%s"), mqtt_enable_char                  ); }
  if (json.containsKey("bme280_enable"                 )) {  const char* bme280_enable_char                 = json["bme280_enable"                ];    sprintf_P(bme280_enable,                 ("%s"), bme280_enable_char                ); }
  if (json.containsKey("sht21_enable"                  )) {  const char* sht21_enable_char                  = json["sht21_enable"                 ];    sprintf_P(sht21_enable,                  ("%s"), sht21_enable_char                 ); }
  if (json.containsKey("bh1750_enable"                 )) {  const char* bh1750_enable_char                 = json["bh1750_enable"                ];    sprintf_P(bh1750_enable,                 ("%s"), bh1750_enable_char                ); }
  if (json.containsKey("motion_sensor_enable"          )) {  const char* motion_sensor_enable_char          = json["motion_sensor_enable"         ];    sprintf_P(motion_sensor_enable,          ("%s"), motion_sensor_enable_char         ); }






  configFile.close();
  return true;
}


bool JsonConf::printConfig() {

  Serial.print(F("module_id                    : "));   Serial.println(module_id                    );
  Serial.print(F("wifi_mode                    : "));   Serial.println(wifi_mode                    );
  Serial.print(F("sta_ssid                     : "));   Serial.println(sta_ssid                     );
  Serial.print(F("sta_pwd                      : "));   Serial.println(sta_pwd                      );
  Serial.print(F("static_ip                    : "));   Serial.println(static_ip                    );
  Serial.print(F("static_gateway               : "));   Serial.println(static_gateway               );
  Serial.print(F("static_subnet                : "));   Serial.println(static_subnet                );
  Serial.print(F("ntp_server                   : "));   Serial.println(ntp_server                   );
  Serial.print(F("my_time_zone                 : "));   Serial.println(my_time_zone                 );
  Serial.print(F("mqtt_server                  : "));   Serial.println(mqtt_server                  );
  Serial.print(F("mqtt_port                    : "));   Serial.println(mqtt_port                    );
  Serial.print(F("mqtt_user                    : "));   Serial.println(mqtt_user                    );
  Serial.print(F("mqtt_pwd                     : "));   Serial.println(mqtt_pwd                     );
  Serial.print(F("mqtt_name                    : "));   Serial.println(mqtt_name                    );
  Serial.print(F("publish_topic                : "));   Serial.println(publish_topic                );
  Serial.print(F("subscribe_topic              : "));   Serial.println(subscribe_topic              );
  Serial.print(F("command_pub_topic            : "));   Serial.println(command_pub_topic            );
  Serial.print(F("light_pin                    : "));   Serial.println(light_pin                    );
  Serial.print(F("lightoff_delay               : "));   Serial.println(lightoff_delay               );
  Serial.print(F("light_pin2                   : "));   Serial.println(light_pin2                   );
  Serial.print(F("light2off_delay              : "));   Serial.println(light2off_delay              );
  Serial.print(F("motion_pin                   : "));   Serial.println(motion_pin                   );
  Serial.print(F("dht_pin                      : "));   Serial.println(dht_pin                      );
  Serial.print(F("get_data_delay               : "));   Serial.println(get_data_delay               );
  Serial.print(F("publish_delay                : "));   Serial.println(publish_delay                );
  Serial.print(F("subscribe_delay              : "));   Serial.println(subscribe_delay              );
  Serial.print(F("motion_read_delay            : "));   Serial.println(motion_read_delay            );
  Serial.print(F("reboot_delay                 : "));   Serial.println(reboot_delay                 );
  Serial.print(F("uart_delay_analog_pin0       : "));   Serial.println(uart_delay_analog_pin0       );
  Serial.print(F("uart_delay_analog_pin1       : "));   Serial.println(uart_delay_analog_pin1       );
  Serial.print(F("uart_delay_analog_pin2       : "));   Serial.println(uart_delay_analog_pin2       );
  Serial.print(F("uart_delay_analog_pin3       : "));   Serial.println(uart_delay_analog_pin3       );
  Serial.print(F("uart_delay_analog_pin4       : "));   Serial.println(uart_delay_analog_pin4       );
  Serial.print(F("uart_delay_analog_pin5       : "));   Serial.println(uart_delay_analog_pin5       );
  Serial.print(F("green_light_on               : "));   Serial.println(green_light_on               );
  Serial.print(F("green_light_off              : "));   Serial.println(green_light_off              );
  Serial.print(F("green_light_pin              : "));   Serial.println(green_light_pin              );
  Serial.print(F("green_humidity_threshold_up  : "));   Serial.println(green_humidity_threshold_up  );
  Serial.print(F("green_humidity_threshold_down: "));   Serial.println(green_humidity_threshold_down);
  Serial.print(F("green_humidity_sensor_pin    : "));   Serial.println(green_humidity_sensor_pin    );
  Serial.print(F("green_pump_pin               : "));   Serial.println(green_pump_pin               );

  Serial.print(F("static_ip_enable             : "));   Serial.println(static_ip_enable             );
  Serial.print(F("ntp_enable                   : "));   Serial.println(ntp_enable                   );
  Serial.print(F("mqtt_enable                  : "));   Serial.println(mqtt_enable                  );
  Serial.print(F("bme280_enable                : "));   Serial.println(bme280_enable                );
  Serial.print(F("sht21_enable                 : "));   Serial.println(sht21_enable                 );
  Serial.print(F("bh1750_enable                : "));   Serial.println(bh1750_enable                );
  Serial.print(F("motion_sensor_enable         : "));   Serial.println(motion_sensor_enable         );
}