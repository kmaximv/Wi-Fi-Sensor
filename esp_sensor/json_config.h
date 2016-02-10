#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"


#define DEBUG_JSON_CONFIG

#define NUM_COMMON_KEYS 28
#define NUM_UART_KEYS 6
#define NUM_GREEN_KEYS 7

#define JSON_KEY 0
#define JSON_VALUE 1


#define MODULE_ID          JConf.jconfig_common_value[0]
#define STA_SSID           JConf.jconfig_common_value[1]
#define STA_PWD            JConf.jconfig_common_value[2]
#define STATIC_IP_MODE     JConf.jconfig_common_value[3]
#define STATIC_IP          JConf.jconfig_common_value[4]
#define STATIC_GATEWAY     JConf.jconfig_common_value[5]
#define STATIC_SUBNET      JConf.jconfig_common_value[6]
#define NTP_SERVER         JConf.jconfig_common_value[7]
#define TIME_ZONE          JConf.jconfig_common_value[8]
#define MQTT_SERVER        JConf.jconfig_common_value[9]
#define MQTT_PORT          JConf.jconfig_common_value[10]
#define MQTT_USER          JConf.jconfig_common_value[11]
#define MQTT_PWD           JConf.jconfig_common_value[12]
#define MQTT_NAME          JConf.jconfig_common_value[13]
#define PUBLISH_TOPIC      JConf.jconfig_common_value[14]
#define SUBSCRIBE_TOPIC    JConf.jconfig_common_value[15]
#define COMMAND_PUB_TOPIC  JConf.jconfig_common_value[16]
#define LIGHT_PIN          JConf.jconfig_common_value[17]
#define LIGHTOFF_DELAY     JConf.jconfig_common_value[18]
#define LIGHT_PIN2         JConf.jconfig_common_value[19]
#define LIGHT2OFF_DELAY    JConf.jconfig_common_value[20]
#define MOTION_PIN         JConf.jconfig_common_value[21]
#define DHT_PIN            JConf.jconfig_common_value[22]
#define GET_DATA_DELAY     JConf.jconfig_common_value[23]
#define PUBLISH_DELAY      JConf.jconfig_common_value[24]
#define SUBSCRIBE_DELAY    JConf.jconfig_common_value[25]
#define MOTION_READ_DELAY  JConf.jconfig_common_value[26]
#define REBOOT_DELAY       JConf.jconfig_common_value[27]

#define UART_DELAY_ANALOG_PIN0  JConf.jconfig_uart[0][JSON_KEY]
#define UART_DELAY_ANALOG_PIN1  JConf.jconfig_uart[1][JSON_KEY]
#define UART_DELAY_ANALOG_PIN2  JConf.jconfig_uart[2][JSON_KEY]
#define UART_DELAY_ANALOG_PIN3  JConf.jconfig_uart[3][JSON_KEY]
#define UART_DELAY_ANALOG_PIN4  JConf.jconfig_uart[4][JSON_KEY]
#define UART_DELAY_ANALOG_PIN5  JConf.jconfig_uart[5][JSON_KEY]

#define GREEN_LIGHT_ON                 JConf.jconfig_green[0][JSON_KEY]
#define GREEN_LIGHT_OFF                JConf.jconfig_green[1][JSON_KEY]
#define GREEN_LIGHT_PIN                JConf.jconfig_green[2][JSON_KEY]
#define GREEN_HUMIDITY_THRESHOLD_UP    JConf.jconfig_green[3][JSON_KEY]
#define GREEN_HUMIDITY_THRESHOLD_DOWN  JConf.jconfig_green[4][JSON_KEY]
#define GREEN_HUMIDITY_SENSOR_PIN      JConf.jconfig_green[5][JSON_KEY]
#define GREEN_PUMP_PIN                 JConf.jconfig_green[6][JSON_KEY]



class JsonConf
{
  public:
    bool saveConfig();
    bool saveConfig(String conf_key, String conf_value);
    bool loadConfig();

    String jconfig_common_key[NUM_COMMON_KEYS] = {
        "module_id",
        "sta_ssid",
        "sta_pwd",
        "static_ip_mode",
        "static_ip",
        "static_gateway",
        "static_subnet",
        "ntp_server",
        "time_zone",
        "mqtt_server",
        "mqtt_port",
        "mqtt_user",
        "mqtt_pwd",
        "mqtt_name",
        "publish_topic",
        "subscribe_topic",
        "command_pub_topic",
        "light_pin",
        "lightOff_delay",
        "light_pin2",
        "light2Off_delay",
        "motion_pin",
        "dht_pin",
        "get_data_delay",
        "publish_delay",
        "subscribe_delay",
        "motion_read_delay",
        "reboot_delay"
    };

    String jconfig_common_value[NUM_COMMON_KEYS] = {
        "ESP8266",
        "HomeNET",
        "Asdf1234",
        "0",
        "192.168.1.220",
        "192.168.1.1",
        "255.255.255.0",
        "europe.pool.ntp.org",
        "+6",
        "192.168.1.200",
        "1883",
        "none",
        "none",
        "_BedM",
        "/stateSub/",
        "/statePub/",
        "/commandPub/",
        "13",
        "5",
        "12",
        "5",
        "2",
        "14",
        "10",
        "10",
        "60",
        "10",
        "180"
    };


    String jconfig_uart[NUM_UART_KEYS][2] = {
        {"uart_delay_analog_pin0", "none"},
        {"uart_delay_analog_pin1", "none"},
        {"uart_delay_analog_pin2", "none"},
        {"uart_delay_analog_pin3", "none"},
        {"uart_delay_analog_pin4", "none"},
        {"uart_delay_analog_pin5", "none"}
    };


    String jconfig_green[NUM_GREEN_KEYS][2] = {
        {"green_light_on", "8:00"},
        {"green_light_off", "22:00"},
        {"green_light_pin", "12"},
        {"green_humidity_threshold_up", "800"},
        {"green_humidity_threshold_down", "250"},
        {"green_humidity_sensor_pin", "20"},
        {"green_pump_pin", "33"}
    };


  private:

};

#endif