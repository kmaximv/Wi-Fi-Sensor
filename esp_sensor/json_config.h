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


//#define REBOOT_ON

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
    bool printConfig();

    char module_id                    [32] = "ESP8266"            ;
    char sta_ssid                     [32] = "IoT"                ;
    char sta_pwd                      [32] = "Poiu0987"           ;
    char static_ip_mode               [32] = "0"                  ;
    char static_ip                    [32] = "192.168.2.220"      ;
    char static_gateway               [32] = "192.168.2.1"        ;
    char static_subnet                [32] = "255.255.255.0"      ;
    char ntp_server                   [32] = "europe.pool.ntp.org";
    char my_time_zone                 [32] = "+6"                 ;
    char mqtt_enable                  [32] = "1"                  ;
    char mqtt_server                  [32] = "192.168.2.205"      ;
    char mqtt_port                    [32] = "1883"               ;
    char mqtt_user                    [32] = "none"               ;
    char mqtt_pwd                     [32] = "none"               ;
    char mqtt_name                    [32] = "_BedM"              ;
    char publish_topic                [32] = "/stateSub/"         ;
    char subscribe_topic              [32] = "/statePub/"         ;
    char command_pub_topic            [32] = "/commandPub/"       ;
    char light_pin                    [32] = "13"                 ;
    char lightoff_delay               [32] = "5"                  ;
    char light_pin2                   [32] = "12"                 ;
    char light2off_delay              [32] = "5"                  ;
    char motion_pin                   [32] = "2"                  ;
    char dht_pin                      [32] = "14"                 ;
    char get_data_delay               [32] = "10"                 ;
    char publish_delay                [32] = "10"                 ;
    char subscribe_delay              [32] = "60"                 ;
    char motion_read_delay            [32] = "10"                 ;
    char reboot_delay                 [32] = "1800"               ;
    char uart_delay_analog_pin0       [32] = "none"               ;
    char uart_delay_analog_pin1       [32] = "none"               ;
    char uart_delay_analog_pin2       [32] = "none"               ;
    char uart_delay_analog_pin3       [32] = "none"               ;
    char uart_delay_analog_pin4       [32] = "none"               ;
    char uart_delay_analog_pin5       [32] = "none"               ;
    char green_light_on               [32] = "8:00"               ;
    char green_light_off              [32] = "22:00"              ;
    char green_light_pin              [32] = "12"                 ;
    char green_humidity_threshold_up  [32] = "800"                ;
    char green_humidity_threshold_down[32] = "250"                ;
    char green_humidity_sensor_pin    [32] = "20"                 ;
    char green_pump_pin               [32] = "33"                 ;

  private:
};

#endif