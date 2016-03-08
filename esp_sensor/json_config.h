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

enum WIFI_MODE_ENUM {AP, STA, AP_STA};
enum WIFI_PHY_MODE_ENUM {B, G, N};
enum WIFI_AUTH_ENUM {OPEN, WPA_PSK, WPA2_PSK, WPA_WPA2_PSK};


class JsonConf
{
  public:
    bool saveConfig();
    bool loadConfig();
    bool printConfig();

    char module_id                    [16] = "ESP8266"            ;
    char wifi_mode                    [2]  = "0"                  ;
    char wifi_phy_mode                [2]  = "0"                  ;
    char wifi_channel                 [3]  = "10"                 ;
    char wifi_auth                    [2]  = "0"                  ;
    char sta_ssid                     [16] = "IoT"                ;
    char sta_pwd                      [32] = "Poiu0987"           ;
    char ap_pwd                       [32] = "Poiu0987"           ;
    char static_ip                    [16] = "192.168.2.220"      ;
    char static_gateway               [16] = "192.168.2.1"        ;
    char static_subnet                [16] = "255.255.255.0"      ;
    char ntp_server                   [32] = "europe.pool.ntp.org";
    char my_time_zone                 [4]  = "+6"                 ;
    char mqtt_server                  [32] = "192.168.2.205"      ;
    char mqtt_port                    [6]  = "1883"               ;
    char mqtt_user                    [32] = "none"               ;
    char mqtt_pwd                     [32] = "none"               ;
    char mqtt_name                    [32] = "_BedM"              ;
    char publish_topic                [32] = "/stateSub/"         ;
    char subscribe_topic              [32] = "/statePub/"         ;
    char command_pub_topic            [32] = "/commandPub/"       ;
    char light_pin                    [3]  = "13"                 ;
    char lightoff_delay               [32] = "5"                  ;
    char light_pin2                   [3]  = "12"                 ;
    char light2off_delay              [32] = "5"                  ;
    char motion_pin                   [3]  = "14"                 ;
    char dht_pin                      [3] =  "2"                  ;
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
    char green_light_on               [6]  = "8:00"               ;
    char green_light_off              [6]  = "22:00"              ;
    char green_light_pin              [3]  = "12"                 ;
    char green_humidity_threshold_up  [5]  = "800"                ;
    char green_humidity_threshold_down[5]  = "250"                ;
    char green_humidity_sensor_pin    [3]  = "20"                 ;
    char green_pump_pin               [3]  = "33"                 ;

    char static_ip_enable             [2]  = "0"                  ;
    char ntp_enable                   [2]  = "0"                  ;
    char mqtt_enable                  [2]  = "0"                  ;
    char bme280_enable                [2]  = "0"                  ;
    char sht21_enable                 [2]  = "0"                  ;
    char bh1750_enable                [2]  = "0"                  ;
    char motion_sensor_enable         [2]  = "0"                  ;


  private:
};

#endif