#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"


#define DEBUG_JSON_CONFIG

#define MODULE_ID          0
#define STA_SSID           1
#define STA_PWD            2
#define STATIC_IP_MODE     3
#define STATIC_IP          4
#define STATIC_GATEWAY     5
#define STATIC_SUBNET      6
#define NTP_SERVER         7
#define TIME_ZONE          8
#define MQTT_SERVER        9
#define MQTT_PORT          10
#define MQTT_USER          11
#define MQTT_PWD           12
#define MQTT_NAME          13
#define PUBLISH_TOPIC      14
#define SUBSCRIBE_TOPIC    15
#define COMMAND_PUB_TOPIC  16
#define LIGHT_PIN          17
#define LIGHTOFF_DELAY     18
#define LIGHT_PIN2         19
#define LIGHT2OFF_DELAY    20
#define MOTION_PIN         21
#define DHT_PIN            22
#define GET_DATA_DELAY     23
#define PUBLISH_DELAY      24
#define SUBSCRIBE_DELAY    25
#define MOTION_READ_DELAY  26
#define REBOOT_DELAY       27

#define UART_DELAY_ANALOG_PIN0  28
#define UART_DELAY_ANALOG_PIN1  29
#define UART_DELAY_ANALOG_PIN2  30
#define UART_DELAY_ANALOG_PIN3  31
#define UART_DELAY_ANALOG_PIN4  32
#define UART_DELAY_ANALOG_PIN5  33

#define GREEN_LIGHT_ON                 34
#define GREEN_LIGHT_OFF                35
#define GREEN_LIGHT_PIN                36
#define GREEN_HUMIDITY_THRESHOLD_UP    37
#define GREEN_HUMIDITY_THRESHOLD_DOWN  38
#define GREEN_HUMIDITY_SENSOR_PIN      39
#define GREEN_PUMP_PIN                 40




class JsonConf
{
  public:
    void begin();
    void set(int size, char *str);
    void setStr(int size, String str);
    bool saveConfig();
    bool loadConfig();
    char **conf = 0;
  private:
    char **AddPtr (char **conf, int size, char *str);
    int conf_size = 0;
};

#endif