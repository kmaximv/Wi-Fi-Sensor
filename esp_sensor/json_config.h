#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"


#define DEBUG_JSON_CONFIG

#define NUM_COMMON_KEYS 28
#define NUM_UART_KEYS    6
#define NUM_GREEN_KEYS   7


#define JSON_KEY   0
#define JSON_VALUE 1


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

#define UART_DELAY_ANALOG_PIN0  JConf.jconfig_uart[0][JSON_KEY]
#define UART_DELAY_ANALOG_PIN1  JConf.jconfig_uart[1][JSON_KEY]
#define UART_DELAY_ANALOG_PIN2  JConf.jconfig_uart[2][JSON_KEY]
#define UART_DELAY_ANALOG_PIN3  JConf.jconfig_uart[3][JSON_KEY]
#define UART_DELAY_ANALOG_PIN4  JConf.jconfig_uart[4][JSON_KEY]
#define UART_DELAY_ANALOG_PIN5  JConf.jconfig_uart[5][JSON_KEY]

#define GREEN_LIGHT_ON                 0
#define GREEN_LIGHT_OFF                1
#define GREEN_LIGHT_PIN                2
#define GREEN_HUMIDITY_THRESHOLD_UP    3
#define GREEN_HUMIDITY_THRESHOLD_DOWN  4
#define GREEN_HUMIDITY_SENSOR_PIN      5
#define GREEN_PUMP_PIN                 6



class JsonConf
{
  public:
    bool saveConfig();
    bool saveConfig(size_t i);
    bool loadConfig();
    const char* GetDataCommon(const size_t i);
    const char* GetDataCommon(const size_t i, const size_t j);
    bool SetDataCommon(const size_t i, String str);
    bool SetDataCommon(const size_t i, char* str);
    bool SetDataGreen(const size_t i, String str);



    char* jconfig_common[NUM_COMMON_KEYS][2] =
    {
        { "module_id",         "ESP8266"             },
        { "sta_ssid",          "HomeNET"             },
        { "sta_pwd",           "Asdf1234"            },
        { "static_ip_mode",    "0"                   },
        { "static_ip",         "192.168.1.220"       },
        { "static_gateway",    "192.168.1.1"         },
        { "static_subnet",     "255.255.255.0"       },
        { "ntp_server",        "europe.pool.ntp.org" },
        { "time_zone",         "+6"                  },
        { "mqtt_server",       "192.168.1.200"       },
        { "mqtt_port",         "1883"                },
        { "mqtt_user",         "none"                },
        { "mqtt_pwd",          "none"                },
        { "mqtt_name",         "_BedM"               },
        { "publish_topic",     "/stateSub/"          },
        { "subscribe_topic",   "/statePub/"          },
        { "command_pub_topic", "/commandPub/"        },
        { "light_pin",         "13"                  },
        { "lightOff_delay",    "5"                   },
        { "light_pin2",        "12"                  },
        { "light2Off_delay",   "5"                   },
        { "motion_pin",        "2"                   },
        { "dht_pin",           "14"                  },
        { "get_data_delay",    "10"                  },
        { "publish_delay",     "10"                  },
        { "subscribe_delay",   "60"                  },
        { "motion_read_delay", "10"                  },
        { "reboot_delay",      "1800"                }
    };


    char* jconfig_green[NUM_GREEN_KEYS][2] =
    {
        { "green_light_on",                "8:00"  },
        { "green_light_off",               "22:00" },
        { "green_light_pin",               "12"    },
        { "green_humidity_threshold_up",   "800"   },
        { "green_humidity_threshold_down", "250"   },
        { "green_humidity_sensor_pin",     "20"    },
        { "green_pump_pin",                "33"    }
    };




    String jconfig_uart[NUM_UART_KEYS][2] = 
    {
        {"uart_delay_analog_pin0", "none"},
        {"uart_delay_analog_pin1", "none"},
        {"uart_delay_analog_pin2", "none"},
        {"uart_delay_analog_pin3", "none"},
        {"uart_delay_analog_pin4", "none"},
        {"uart_delay_analog_pin5", "none"}
    };

  private:
};

#endif