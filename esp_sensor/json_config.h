#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"


#define DEBUG_JSON_CONFIG

#define NUM_COMMON_KEYS 24
#define NUM_UART_KEYS 6
#define NUM_GREEN_KEYS 7

#define JSON_KEY 0
#define JSON_VALUE 1


#define STA_SSID           JConf.jconfig_common_value[0]
#define STA_PWD            JConf.jconfig_common_value[1]
#define STATIC_IP_MODE     JConf.jconfig_common_value[2]
#define STATIC_IP          JConf.jconfig_common_value[3]
#define STATIC_GATEWAY     JConf.jconfig_common_value[4]
#define STATIC_SUBNET      JConf.jconfig_common_value[5]
#define MQTT_SERVER        JConf.jconfig_common_value[6]
#define MQTT_PORT          JConf.jconfig_common_value[7]
#define MQTT_USER          JConf.jconfig_common_value[8]
#define MQTT_PWD           JConf.jconfig_common_value[9]
#define MQTT_NAME          JConf.jconfig_common_value[10]
#define PUBLISH_TOPIC      JConf.jconfig_common_value[11]
#define SUBSCRIBE_TOPIC    JConf.jconfig_common_value[12]
#define LIGHT_PIN          JConf.jconfig_common_value[13]
#define LIGHTOFF_DELAY     JConf.jconfig_common_value[14]
#define LIGHT_PIN2         JConf.jconfig_common_value[15]
#define LIGHT2OFF_DELAY    JConf.jconfig_common_value[16]
#define MOTION_PIN         JConf.jconfig_common_value[17]
#define DHT_PIN            JConf.jconfig_common_value[18]
#define GET_DATA_DELAY     JConf.jconfig_common_value[19]
#define PUBLISH_DELAY      JConf.jconfig_common_value[20]
#define SUBSCRIBE_DELAY    JConf.jconfig_common_value[21]
#define MOTION_READ_DELAY  JConf.jconfig_common_value[22]
#define REBOOT_DELAY       JConf.jconfig_common_value[23]




class JsonConf
{
  public:
    bool saveConfig();
    bool loadConfig();

	String jconfig_common_key[NUM_COMMON_KEYS] = {
	"sta_ssid",
	"sta_pwd",
	"static_ip_mode",
	"static_ip",
	"static_gateway",
	"static_subnet",
	"mqtt_server",
	"mqtt_port",
	"mqtt_user",
	"mqtt_pwd",
	"mqtt_name",
	"publish_topic",
	"subscribe_topic",
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
	"HomeNET",
	"Asdf1234",
	"0",
	"192.168.1.220",
	"192.168.1.1",
	"255.255.255.0",
	"192.168.1.200",
	"1883",
	"none",
	"none",
	"_BedM",
	"/stateSub/",
	"/statePub/",
	"13",
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


	#ifdef UART_ON
	String jconfig_uart_key[NUM_UART_KEYS] = {
	"delayAnalogPin0",
	"delayAnalogPin1",
	"delayAnalogPin2",
	"delayAnalogPin3",
	"delayAnalogPin4",
	"delayAnalogPin5"
	};

	String jconfig_uart_value[NUM_UART_KEYS] = {
	"none",
	"none",
	"none",
	"none",
	"none",
	"none"
	};
	#endif

	String jconfig_green[NUM_GREEN_KEYS][2] = {
	{"greenLightOn", "none"},
	{"greenLightOff", "none"},
	{"greenLightPin", "none"},
	{"greenHumidityThresholdUp", "none"},
	{"greenHumidityThresholdDown", "none"},
	{"greenHumiditySensorPin", "none"},
	{"greenPumpPin", "none"}
	};


  private:

};

#endif