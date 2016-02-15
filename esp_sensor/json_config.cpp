#include "json_config.h"


bool JsonConf::saveConfig() {
  StaticJsonBuffer<1024> jsonBuffer;
#ifdef DEBUG_JSON_CONFIG
  Serial.print(F("saveConfig()"));  Serial.println();
#endif

  JsonObject& json = jsonBuffer.createObject();


  json["module_id"]                     = module_id                    ;                           
  json["sta_ssid"]                      = sta_ssid                     ;                         
  json["sta_pwd"]                       = sta_pwd                      ;                       
  json["static_ip_mode"]                = static_ip_mode               ;                                     
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


  const char* module_id_char                     = json["module_id"];                        sprintf_P(module_id,                     ("%s"), module_id_char                    );
  const char* sta_ssid_char                      = json["sta_ssid"];                         sprintf_P(sta_ssid,                      ("%s"), sta_ssid_char                     );
  const char* sta_pwd_char                       = json["sta_pwd"];                          sprintf_P(sta_pwd,                       ("%s"), sta_pwd_char                      );
  const char* static_ip_mode_char                = json["static_ip_mode"];                   sprintf_P(static_ip_mode,                ("%s"), static_ip_mode_char               );
  const char* static_ip_char                     = json["static_ip"];                        sprintf_P(static_ip,                     ("%s"), static_ip_char                    );
  const char* static_gateway_char                = json["static_gateway"];                   sprintf_P(static_gateway,                ("%s"), static_gateway_char               );
  const char* static_subnet_char                 = json["static_subnet"];                    sprintf_P(static_subnet,                 ("%s"), static_subnet_char                );
  const char* ntp_server_char                    = json["ntp_server"];                       sprintf_P(ntp_server,                    ("%s"), ntp_server_char                   );
  const char* my_time_zone_char                  = json["my_time_zone"];                     sprintf_P(my_time_zone,                  ("%s"), my_time_zone_char                 );
  const char* mqtt_server_char                   = json["mqtt_server"];                      sprintf_P(mqtt_server,                   ("%s"), mqtt_server_char                  );
  const char* mqtt_port_char                     = json["mqtt_port"];                        sprintf_P(mqtt_port,                     ("%s"), mqtt_port_char                    );
  const char* mqtt_user_char                     = json["mqtt_user"];                        sprintf_P(mqtt_user,                     ("%s"), mqtt_user_char                    );
  const char* mqtt_pwd_char                      = json["mqtt_pwd"];                         sprintf_P(mqtt_pwd,                      ("%s"), mqtt_pwd_char                     );
  const char* mqtt_name_char                     = json["mqtt_name"];                        sprintf_P(mqtt_name,                     ("%s"), mqtt_name_char                    );
  const char* publish_topic_char                 = json["publish_topic"];                    sprintf_P(publish_topic,                 ("%s"), publish_topic_char                );
  const char* subscribe_topic_char               = json["subscribe_topic"];                  sprintf_P(subscribe_topic,               ("%s"), subscribe_topic_char              );
  const char* command_pub_topic_char             = json["command_pub_topic"];                sprintf_P(command_pub_topic,             ("%s"), command_pub_topic_char            );
  const char* light_pin_char                     = json["light_pin"];                        sprintf_P(light_pin,                     ("%s"), light_pin_char                    );
  const char* lightoff_delay_char                = json["lightoff_delay"];                   sprintf_P(lightoff_delay,                ("%s"), lightoff_delay_char               );
  const char* light_pin2_char                    = json["light_pin2"];                       sprintf_P(light_pin2,                    ("%s"), light_pin2_char                   );
  const char* light2off_delay_char               = json["light2off_delay"];                  sprintf_P(light2off_delay,               ("%s"), light2off_delay_char              );
  const char* motion_pin_char                    = json["motion_pin"];                       sprintf_P(motion_pin,                    ("%s"), motion_pin_char                   );
  const char* dht_pin_char                       = json["dht_pin"];                          sprintf_P(dht_pin,                       ("%s"), dht_pin_char                      );
  const char* get_data_delay_char                = json["get_data_delay"];                   sprintf_P(get_data_delay,                ("%s"), get_data_delay_char               );
  const char* publish_delay_char                 = json["publish_delay"];                    sprintf_P(publish_delay,                 ("%s"), publish_delay_char                );
  const char* subscribe_delay_char               = json["subscribe_delay"];                  sprintf_P(subscribe_delay,               ("%s"), subscribe_delay_char              );
  const char* motion_read_delay_char             = json["motion_read_delay"];                sprintf_P(motion_read_delay,             ("%s"), motion_read_delay_char            );
  const char* reboot_delay_char                  = json["reboot_delay"];                     sprintf_P(reboot_delay,                  ("%s"), reboot_delay_char                 );
  const char* uart_delay_analog_pin0_char        = json["uart_delay_analog_pin0"];           sprintf_P(uart_delay_analog_pin0,        ("%s"), uart_delay_analog_pin0_char       );
  const char* uart_delay_analog_pin1_char        = json["uart_delay_analog_pin1"];           sprintf_P(uart_delay_analog_pin1,        ("%s"), uart_delay_analog_pin1_char       );
  const char* uart_delay_analog_pin2_char        = json["uart_delay_analog_pin2"];           sprintf_P(uart_delay_analog_pin2,        ("%s"), uart_delay_analog_pin2_char       );
  const char* uart_delay_analog_pin3_char        = json["uart_delay_analog_pin3"];           sprintf_P(uart_delay_analog_pin3,        ("%s"), uart_delay_analog_pin3_char       );
  const char* uart_delay_analog_pin4_char        = json["uart_delay_analog_pin4"];           sprintf_P(uart_delay_analog_pin4,        ("%s"), uart_delay_analog_pin4_char       );
  const char* uart_delay_analog_pin5_char        = json["uart_delay_analog_pin5"];           sprintf_P(uart_delay_analog_pin5,        ("%s"), uart_delay_analog_pin5_char       );
  const char* green_light_on_char                = json["green_light_on"];                   sprintf_P(green_light_on,                ("%s"), green_light_on_char               );
  const char* green_light_off_char               = json["green_light_off"];                  sprintf_P(green_light_off,               ("%s"), green_light_off_char              );
  const char* green_light_pin_char               = json["green_light_pin"];                  sprintf_P(green_light_pin,               ("%s"), green_light_pin_char              );
  const char* green_humidity_threshold_up_char   = json["green_humidity_threshold_up"];      sprintf_P(green_humidity_threshold_up,   ("%s"), green_humidity_threshold_up_char  );
  const char* green_humidity_threshold_down_char = json["green_humidity_threshold_down"];    sprintf_P(green_humidity_threshold_down, ("%s"), green_humidity_threshold_down_char);
  const char* green_humidity_sensor_pin_char     = json["green_humidity_sensor_pin"];        sprintf_P(green_humidity_sensor_pin,     ("%s"), green_humidity_sensor_pin_char    );
  const char* green_pump_pin_char                = json["green_pump_pin"];                   sprintf_P(green_pump_pin,                ("%s"), green_pump_pin_char               );


  configFile.close();
  return true;
}