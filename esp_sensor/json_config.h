#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"


//#define DHT_ON
#define BME280_ON
//#define SHT21_ON

#define DEBUG
#define DEBUG_JSON_CONFIG


//UART Settings    ------------------------------------------------------------
//#define UART_ON

#define DEBUG_ESP_UART
#define CRC_ENABLE

#define DIGITAL_PINS 14   //Кол-во цифровых входов/выходов
#define ANALOG_PINS 6     //Кол-во цифровых входов/выходов
#define PARSE_CELLS 4     //Кол-во ячеек в массиве принимаемых данных
#define DATA_LENGTH 10    //Максимальный размер пакета данных без маркеров и CRC
//------------------------------------------------------------------------------

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