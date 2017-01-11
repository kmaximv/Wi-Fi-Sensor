/**************************************************************************/
/* 
This is an example for Si7021 Digital Humidity & Temperature Sensor

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
    Serial.println("Si7021 sensor is not present...");
    delay(5000);
  }
  
  Serial.println("Si7021 sensor is present");
}

void loop()
{
  Serial.println("...");
  Serial.println("<< DEMO: %RH - 12Bit, Temperature - 14Bit (default settings) >>"); 
  Serial.print("Compensated Humidity: ");
  Serial.print(myHTU21D.readHumidity());
  Serial.println(" +-2%RH");
  Serial.print(F("Temperature: "));
  Serial.print(myHTU21D.readTemperature(HTU21D_TEMP_READ_AFTER_RH_MEASURMENT));
  Serial.println(" +-0.5deg.C");
  
  Serial.println("...");
  Serial.println("<< DEMO: %RH - 11Bit, Temperature - 11Bit >>");
  myHTU21D.setResolution(HTU21D_RES_RH11_TEMP11);
  Serial.print("Compensated Humidity: ");
  Serial.print(myHTU21D.readHumidity());
  Serial.println(" +-2%RH");
  Serial.print("Temperature: ");
  Serial.print(myHTU21D.readTemperature(HTU21D_TEMP_READ_AFTER_RH_MEASURMENT));
  Serial.println(" +-0.5deg.C");
    
  Serial.println("...");
  Serial.println("<< DEMO: Battery Status >>");
  if (myHTU21D.batteryStatus() == true)
  {
    Serial.println("Battery OK. Level > 1.9v");
  }
  else
  {
    Serial.println("Battery LOW. Level < 1.9v");
  }

  Serial.println("...");
  Serial.println("<< DEMO: Fimware version >>");
  Serial.print("FW version: ");
  Serial.println(myHTU21D.readFirmwareVersion());

  Serial.println("...");
  Serial.println("<< DEMO: Device ID >>");
  Serial.print("Sensor's name: ");
  Serial.println(myHTU21D.readDeviceID());

  Serial.println("...");
  Serial.println("<< DEMO: Temperature - 14Bit >>");
  myHTU21D.setResolution(HTU21D_RES_RH12_TEMP14);
  Serial.print("Temperature: ");
  Serial.print(myHTU21D.readTemperature());
  Serial.println(" +-0.5deg.C");

  Serial.println("");
  Serial.print("DEMO: Starts over again in 8 sec.");
  delay(8000);
}
