#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"


#define DEBUG_JSON_CONFIG

const char def_module_id                    [] PROGMEM = "ESP8266"            ;
const char def_sta_ssid                     [] PROGMEM = "HomeNET"            ;
const char def_sta_pwd                      [] PROGMEM = "Asdf1234"           ;
const char def_static_ip_mode               [] PROGMEM = "0"                  ;
const char def_static_ip                    [] PROGMEM = "192.168.1.220"      ;
const char def_static_gateway               [] PROGMEM = "192.168.1.1"        ;
const char def_static_subnet                [] PROGMEM = "255.255.255.0"      ;
const char def_ntp_server                   [] PROGMEM = "europe.pool.ntp.org";
const char def_my_time_zone                 [] PROGMEM = "+6"                 ;
const char def_mqtt_server                  [] PROGMEM = "192.168.1.200"      ;
const char def_mqtt_port                    [] PROGMEM = "1883"               ;
const char def_mqtt_user                    [] PROGMEM = "none"               ;
const char def_mqtt_pwd                     [] PROGMEM = "none"               ;
const char def_mqtt_name                    [] PROGMEM = "_BedM"              ;
const char def_publish_topic                [] PROGMEM = "/stateSub/"         ;
const char def_subscribe_topic              [] PROGMEM = "/statePub/"         ;
const char def_command_pub_topic            [] PROGMEM = "/commandPub/"       ;
const char def_light_pin                    [] PROGMEM = "13"                 ;
const char def_lightoff_delay               [] PROGMEM = "5"                  ;
const char def_light_pin2                   [] PROGMEM = "12"                 ;
const char def_light2off_delay              [] PROGMEM = "5"                  ;
const char def_motion_pin                   [] PROGMEM = "2"                  ;
const char def_dht_pin                      [] PROGMEM = "14"                 ;
const char def_get_data_delay               [] PROGMEM = "10"                 ;
const char def_publish_delay                [] PROGMEM = "10"                 ;
const char def_subscribe_delay              [] PROGMEM = "60"                 ;
const char def_motion_read_delay            [] PROGMEM = "10"                 ;
const char def_reboot_delay                 [] PROGMEM = "1800"               ;
const char def_uart_delay_analog_pin0       [] PROGMEM = "none"               ;
const char def_uart_delay_analog_pin1       [] PROGMEM = "none"               ;
const char def_uart_delay_analog_pin2       [] PROGMEM = "none"               ;
const char def_uart_delay_analog_pin3       [] PROGMEM = "none"               ;
const char def_uart_delay_analog_pin4       [] PROGMEM = "none"               ;
const char def_uart_delay_analog_pin5       [] PROGMEM = "none"               ;
const char def_green_light_on               [] PROGMEM = "8:00"               ;
const char def_green_light_off              [] PROGMEM = "22:00"              ;
const char def_green_light_pin              [] PROGMEM = "12"                 ;
const char def_green_humidity_threshold_up  [] PROGMEM = "800"                ;
const char def_green_humidity_threshold_down[] PROGMEM = "250"                ;
const char def_green_humidity_sensor_pin    [] PROGMEM = "20"                 ;
const char def_green_pump_pin               [] PROGMEM = "33"                 ;




class JsonConf
{
  public:
    bool saveConfig();
    bool loadConfig();

    char* module_id                     = "ESP8266"            ;
    char* sta_ssid                      = "HomeNET"            ;
    char* sta_pwd                       = "Asdf1234"           ;
    char* static_ip_mode                = "0"                  ;
    char* static_ip                     = "192.168.1.220"      ;
    char* static_gateway                = "192.168.1.1"        ;
    char* static_subnet                 = "255.255.255.0"      ;
    char* ntp_server                    = "europe.pool.ntp.org";
    char* my_time_zone                  = "+6"                 ;
    char* mqtt_server                   = "192.168.1.200"      ;
    char* mqtt_port                     = "1883"               ;
    char* mqtt_user                     = "none"               ;
    char* mqtt_pwd                      = "none"               ;
    char* mqtt_name                     = "_BedM"              ;
    char* publish_topic                 = "/stateSub/"         ;
    char* subscribe_topic               = "/statePub/"         ;
    char* command_pub_topic             = "/commandPub/"       ;
    char* light_pin                     = "13"                 ;
    char* lightoff_delay                = "5"                  ;
    char* light_pin2                    = "12"                 ;
    char* light2off_delay               = "5"                  ;
    char* motion_pin                    = "2"                  ;
    char* dht_pin                       = "14"                 ;
    char* get_data_delay                = "10"                 ;
    char* publish_delay                 = "10"                 ;
    char* subscribe_delay               = "60"                 ;
    char* motion_read_delay             = "10"                 ;
    char* reboot_delay                  = "1800"               ;
    char* uart_delay_analog_pin0        = "none"               ;
    char* uart_delay_analog_pin1        = "none"               ;
    char* uart_delay_analog_pin2        = "none"               ;
    char* uart_delay_analog_pin3        = "none"               ;
    char* uart_delay_analog_pin4        = "none"               ;
    char* uart_delay_analog_pin5        = "none"               ;
    char* green_light_on                = "8:00"               ;
    char* green_light_off               = "22:00"              ;
    char* green_light_pin               = "12"                 ;
    char* green_humidity_threshold_up   = "800"                ;
    char* green_humidity_threshold_down = "250"                ;
    char* green_humidity_sensor_pin     = "20"                 ;
    char* green_pump_pin                = "33"                 ;

  private:
};

#endif