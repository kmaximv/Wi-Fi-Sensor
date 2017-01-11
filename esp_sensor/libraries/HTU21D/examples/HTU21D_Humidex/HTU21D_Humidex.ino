/**************************************************************************/
/* 

This is an Arduino code for calculation Humidex and Dew Point by using
DSHT21, HTU21D or Si70xx Digital Humidity & Temperature Sensor

These sensor uses I2C to communicate. Two pins are required to interface

Connect HTU21D to pins:  SDA  SCL
Uno, Mini, Pro:          A4      A5
Mega2560, Due:           20      21
Leonardo:                2       3
Atiny85:                 0/PWM   2/A1   (TinyWireM)
NodeMCU 1.0:             D1/ANY  D2/ANY (D1 & D2 by default)
ESP8266 ESP-01:          ANY     ANY

The Humidex is an index number used to describe how hot the weather feels
to the average person, by combining the effect of heat and humidity.
The humidex is a unit-less number based on the dew point, but it is
equivalent to dry temperature in degrees Celsius. For example, if the
temperature is 30 deg.C (86 F), and the calculated humidex is 40,
then it indicates the humid heat feels approximately like a dry 
temperature of 40 deg.C (104 F).
 
According to the Meteorological Service of Canada, a humidex of at 
least 30 deg.C causes "some discomfort", at least 40 deg.C causes
"great discomfort" and above 45 deg.C is "dangerous".
 
For more details visit the link: http://en.wikipedia.org/wiki/Humidex
 
Humidex is calculated as:

Humidex = air temperature + h
h = (0.5555)*(e - 10.0);
e = 6.11 * exp(5417.7530 * ((1/273.16) - (1/dewpoint)))


Written by enjoyneering79

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

float a = 17.271;
float b = 237.7;

float temperature;
float humidity;
float dewpoint;
float humidex;
 
HTU21D myHTU21D;

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  
 #if defined(ARDUINO_ARCH_ESP8266) || (ESP8266_NODEMCU)
  while (myHTU21D.begin(D1, D2) != true)
 #else
  while (myHTU21D.begin() != true)
 #endif
  {
    Serial.println("HTU21D, SHT21 or Si70xx sensor is not present...");
    delay(5000);
  }
  
  Serial.println("HTU21D, SHT21 or Si70xx sensor is present");
}


void loop()
{
  temperature = myHTU21D.readTemperature();
  printSerial("Temperature:", temperature, "deg.C");
 
  humidity = myHTU21D.readCompensatedHumidity();
  printSerial("Humidity:", humidity, "%RH");
 
  dewpoint = calculateDewPoint(temperature, humidity);
  printSerial("Dew Point:", dewpoint, "deg.C");
 
  humidex = calculateHumidex(temperature, dewpoint);
  printSerial("Humidex/Feels like:", humidex, "deg.C");

  calculateHumidexDiscomfortLevel(humidex);
 
  delay(8000);
}
 

float calculateDewPoint(float temperature, float humidity)
{
  float gammavalue = calculateDewPointGamma(temperature, humidity);
 
  float dewpoint = (b * gammavalue) / (a - gammavalue);

  return dewpoint;
 
}


float calculateDewPointGamma(float temperature, float humidity)
{
  float gamma = ((a * temperature) / (b + temperature)) + log(humidity / 100);

  return gamma;
}


float calculateHumidex(float temperature, float dewpoint)
{
  float humidex = temperature + 0.5555 * ( 6.11 * exp(calculateHumidexE(dewpoint)) - 10);

  return humidex;
}


float calculateHumidexE(float dewpoint)
{
  float e = 5417.7530 * ((1/273.16) - (1 / (273.16 + dewpoint)));

  return e;
}
 
void printSerial(String key_1, float value, String key_2)
{
  Serial.print(key_1 + " ");
  Serial.print(value);
  Serial.println(" " + key_2);
}


void calculateHumidexDiscomfortLevel(float humidex)
{
  if ((humidex >= 21 ) && (humidex < 27))
  {
    printSerial("No discomfort at", humidex, "deg.C");
  }
  else if ((humidex >= 27) && (humidex < 35))
  {
    printSerial("Some discomfort at", humidex, "deg.C");
  }
  else if ((humidex >= 35) && (humidex < 40))
  {
    printSerial("Great discomfort at", humidex, "deg.C");
  }
  else if ((humidex >= 40) && (humidex < 46))
  {
    printSerial("Intense discomfort, avoid exertion at", humidex, "deg.C");
  }
  else if ((humidex >= 46) && (humidex < 54))
  {
    printSerial("Dangerous discomfort at", humidex, "deg.C");
  }
  else if (humidex >= 54)
  {
    printSerial("Heat stroke probable at", humidex, "deg.C");
  }
}
