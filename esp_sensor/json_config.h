#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"


#define DEBUG_JSON_CONFIG

const char def_module_id                    [] = "ESP8266"            ;
const char def_sta_ssid                     [] = "HomeNET"            ;
const char def_sta_pwd                      [] = "Asdf1234"           ;
const char def_static_ip_mode               [] = "0"                  ;
const char def_static_ip                    [] = "192.168.1.220"      ;
const char def_static_gateway               [] = "192.168.1.1"        ;
const char def_static_subnet                [] = "255.255.255.0"      ;
const char def_ntp_server                   [] = "europe.pool.ntp.org";
const char def_time_zone                    [] = "+6"                 ;
const char def_mqtt_server                  [] = "192.168.1.200"      ;
const char def_mqtt_port                    [] = "1883"               ;
const char def_mqtt_user                    [] = "none"               ;
const char def_mqtt_pwd                     [] = "none"               ;
const char def_mqtt_name                    [] = "_BedM"              ;
const char def_publish_topic                [] = "/stateSub/"         ;
const char def_subscribe_topic              [] = "/statePub/"         ;
const char def_command_pub_topic            [] = "/commandPub/"       ;
const char def_light_pin                    [] = "13"                 ;
const char def_lightoff_delay               [] = "5"                  ;
const char def_light_pin2                   [] = "12"                 ;
const char def_light2off_delay              [] = "5"                  ;
const char def_motion_pin                   [] = "2"                  ;
const char def_dht_pin                      [] = "14"                 ;
const char def_get_data_delay               [] = "10"                 ;
const char def_publish_delay                [] = "10"                 ;
const char def_subscribe_delay              [] = "60"                 ;
const char def_motion_read_delay            [] = "10"                 ;
const char def_reboot_delay                 [] = "1800"               ;
const char def_uart_delay_analog_pin0       [] = "none"               ;
const char def_uart_delay_analog_pin1       [] = "none"               ;
const char def_uart_delay_analog_pin2       [] = "none"               ;
const char def_uart_delay_analog_pin3       [] = "none"               ;
const char def_uart_delay_analog_pin4       [] = "none"               ;
const char def_uart_delay_analog_pin5       [] = "none"               ;
const char def_green_light_on               [] = "8:00"               ;
const char def_green_light_off              [] = "22:00"              ;
const char def_green_light_pin              [] = "12"                 ;
const char def_green_humidity_threshold_up  [] = "800"                ;
const char def_green_humidity_threshold_down[] = "250"                ;
const char def_green_humidity_sensor_pin    [] = "20"                 ;
const char def_green_pump_pin               [] = "33"                 ;


char module_id                    [] = "ESP8266"            ;
char sta_ssid                     [] = "HomeNET"            ;
char sta_pwd                      [] = "Asdf1234"           ;
char static_ip_mode               [] = "0"                  ;
char static_ip                    [] = "192.168.1.220"      ;
char static_gateway               [] = "192.168.1.1"        ;
char static_subnet                [] = "255.255.255.0"      ;
char ntp_server                   [] = "europe.pool.ntp.org";
char time_zone                    [] = "+6"                 ;
char mqtt_server                  [] = "192.168.1.200"      ;
char mqtt_port                    [] = "1883"               ;
char mqtt_user                    [] = "none"               ;
char mqtt_pwd                     [] = "none"               ;
char mqtt_name                    [] = "_BedM"              ;
char publish_topic                [] = "/stateSub/"         ;
char subscribe_topic              [] = "/statePub/"         ;
char command_pub_topic            [] = "/commandPub/"       ;
char light_pin                    [] = "13"                 ;
char lightoff_delay               [] = "5"                  ;
char light_pin2                   [] = "12"                 ;
char light2off_delay              [] = "5"                  ;
char motion_pin                   [] = "2"                  ;
char dht_pin                      [] = "14"                 ;
char get_data_delay               [] = "10"                 ;
char publish_delay                [] = "10"                 ;
char subscribe_delay              [] = "60"                 ;
char motion_read_delay            [] = "10"                 ;
char reboot_delay                 [] = "1800"               ;
char uart_delay_analog_pin0       [] = "none"               ;
char uart_delay_analog_pin1       [] = "none"               ;
char uart_delay_analog_pin2       [] = "none"               ;
char uart_delay_analog_pin3       [] = "none"               ;
char uart_delay_analog_pin4       [] = "none"               ;
char uart_delay_analog_pin5       [] = "none"               ;
char green_light_on               [] = "8:00"               ;
char green_light_off              [] = "22:00"              ;
char green_light_pin              [] = "12"                 ;
char green_humidity_threshold_up  [] = "800"                ;
char green_humidity_threshold_down[] = "250"                ;
char green_humidity_sensor_pin    [] = "20"                 ;
char green_pump_pin               [] = "33"                 ;




class JsonConf
{
  public:
    bool saveConfig();
    bool loadConfig();
  private:
};

#endif