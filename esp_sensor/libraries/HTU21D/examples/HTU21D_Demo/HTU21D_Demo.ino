/**************************************************************************/
/* 
This is an Arduino example for SHT21, HTU21D Digital Humidity & Temperature
Sensor

Written by enjoyneering79

These sensor uses I2C to communicate. Two pins are required to interface

Connect HTU21D to pins:  SDA  SCL
Uno, Mini, Pro:          A4      A5
Mega2560, Due:           20      21
Leonardo:                2       3
Atiny85:                 0/PWM   2/A1   (TinyWireM)
NodeMCU 1.0:             D1/ANY  D2/ANY (D1 & D2 by default)
ESP8266 ESP-01:          ANY     ANY

BSD license, all text above must be included in any redistribution
*/
/**************************************************************************/

#if defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny26__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny45__) || (__AVR_ATtiny84__) || defined(__AVR_ATtiny85__)
 #include <TinyWireM.h>
 #define Wire TinyWireM
#else defined
 #include <Wire.h>
#endif

#include <HTU21D.h>

/*
HTU21D(resolution)

resolution:
HTU21D_RES_RH12_TEMP14 - RH: 12Bit. Temperature: 14Bit.
HTU21D_RES_RH8_TEMP12  - RH: 8Bit.  Temperature: 12Bit.
HTU21D_RES_RH10_TEMP13 - RH: 10Bit. Temperature: 13Bit.
HTU21D_RES_RH11_TEMP11 - RH: 11Bit. Temperature: 11Bit.


DEFAULT
HTU21D(HTU21D_RES_RH12_TEMP14)
*/

HTU21D myHTU21D;

void setup()
{
  Serial.begin(115200);
  Serial.println(F(""));
  
 #if defined(ARDUINO_ARCH_ESP8266) || (ESP8266_NODEMCU)
  while (myHTU21D.begin(D1, D2) != true)
 #else
  while (myHTU21D.begin() != true)
 #endif
  {
    Serial.println(F("HTU21D, SHT21 or Si70xx sensor is not present..."));
    delay(5000);
  }
  
  Serial.println(F("HTU21D, SHT21 or Si70xx sensor is present"));
}

void loop()
{
  Serial.println(F("..."));
  Serial.println(F("<< DEMO: %RH - 12Bit, Temperature - 14Bit (default settings) >>")); 
  Serial.print(F("Humidity: "));
  Serial.print(myHTU21D.readHumidity());
  Serial.println(F(" +-2%RH"));
  Serial.print(F("Compensated Humidity: "));
  Serial.print(myHTU21D.readCompensatedHumidity());
  Serial.println(F(" +-2%RH"));
  Serial.print(F("Temperature: "));
  Serial.print(myHTU21D.readTemperature());
  Serial.println(F(" +-0.5deg.C"));
  
  Serial.println(F("..."));
  Serial.println(F("<< DEMO: %RH - 11Bit, Temperature - 11Bit >>"));
  myHTU21D.setResolution(HTU21D_RES_RH11_TEMP11);
  Serial.print(F("Humidity: "));
  Serial.print(myHTU21D.readHumidity());
  Serial.println(F(" +-2%RH"));
  Serial.print(F("Compensated Humidity: "));
  Serial.print(myHTU21D.readCompensatedHumidity());
  Serial.println(F(" +-2%RH"));
  Serial.print(F("Temperature: "));
  Serial.print(myHTU21D.readTemperature());
  Serial.println(F(" +-0.5deg.C"));
    
  Serial.println(F("..."));
  Serial.println(F("<< DEMO: Battery Status >>"));
  if (myHTU21D.batteryStatus() == true)
  {
    Serial.println(F("Battery OK. Level > 2.25v"));
  }
  else
  {
    Serial.println(F("Battery LOW. Level < 2.25v"));
  }

  Serial.println(F("..."));
  Serial.println(F("<< DEMO: Fimware version >>"));
  Serial.print(F("FW version: "));
  Serial.println(myHTU21D.readFirmwareVersion());

  Serial.println(F("..."));
  Serial.println(F("<< DEMO: Device ID >>"));
  Serial.print(F("Sensor's name: "));
  Serial.println(myHTU21D.readDeviceID());


/* UNCOMENT FOR OF SENSOR'S HEATER DIAGNOSTIC.
  Serial.println(F("!!!"));
  Serial.println(F("<< DEMO: built-in Heater test >>"));
  Serial.println(F(""));
  Serial.println(F("Built-in Heater - ON (+ 2sec. delay to warm up the sensor)"));
  myHTU21D.setHeater(HTU21D_ON);
  delay(2000);
  Serial.print(F("Compensated Humidity: "));
  Serial.print(myHTU21D.readCompensatedHumidity());
  Serial.println(F(" +-2%RH"));
  Serial.print(F("Temperature: "));
  Serial.print(myHTU21D.readTemperature());
  Serial.println(F(" +-0.5deg.C"));

  Serial.println(F("!!!"));
  Serial.println(F("Built-in Heater - OFF (+ 8sec. delay to cool down the sensor)"));
  myHTU21D.setHeater(HTU21D_OFF);
  delay(8000);
  Serial.println(F(""));
  Serial.print(F("Compensated Humidity: "));
  Serial.print(myHTU21D.readCompensatedHumidity());
  Serial.println(F(" +-2%RH"));
  
  Serial.println(F(""));
  Serial.print(F("Temperature: "));
  Serial.print(myHTU21D.readTemperature());
  Serial.println(F(" -+0.5deg.C"));
*/

  Serial.println(F(""));
  Serial.print(F("DEMO: Starts over again in 8 sec."));
  delay(8000);
}
