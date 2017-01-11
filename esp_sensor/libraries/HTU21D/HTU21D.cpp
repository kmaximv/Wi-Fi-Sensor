/**************************************************************************/
/*
This is an Arduino library for SHT21, HTU21D & Si70xx
Digital Humidity & Temperature Sensor

  written by enjoyneering79

  These sensor uses I2C to communicate. Two pins are required to  
  interface

  Connect HTU21D to pins:  SDA     SCL
  Uno, Mini, Pro:          A4      A5
  Mega2560, Due:           20      21
  Leonardo:                2       3
  Atiny85:                 0/PWM   2/A1  (TinyWireM)
  NodeMCU 1.0:             4/ANY   5/ANY (4 & 5 by default)
  ESP8266 ESP-01:          ANY     ANY

  BSD license, all text above must be included in any redistribution
*/
 /**************************************************************************/

#include "HTU21D.h"


/**************************************************************************/
/*
    Constructor
*/
/**************************************************************************/
HTU21D::HTU21D(HTU21D_Resolution sensorResolution)
{
  _HTU21Dinitialisation = false;
  _HTU21D_Resolution = sensorResolution;
}

/**************************************************************************/
/*
    Initializes I2C and configures the sensor (call this function before
    doing anything else)

    Wire.endTransmission():
    0 - success
    1 - data too long to fit in transmit data16
    2 - received NACK on transmit of address
    3 - received NACK on transmit of data
    4 - other error
*/
/**************************************************************************/
#if defined(ARDUINO_ARCH_ESP8266)
bool HTU21D::begin(uint8_t sda, uint8_t scl)
{
  _sda = sda;
  _scl = scl;

  Wire.begin(_sda, _scl);
#else
bool HTU21D::begin(void) 
{
  Wire.begin();
#endif

  /* Make sure we're actually connected */
  Wire.beginTransmission(HTU21D_ADDRESS);
  if (Wire.endTransmission() != 0)
  {
    return false;
  }

  _HTU21Dinitialisation = true;

  setResolution(_HTU21D_Resolution);
  setHeater(HTU21D_OFF);

  return true;
}

/**************************************************************************/
/*
    Sets sensor's resolution
*/
/**************************************************************************/
void HTU21D::setResolution(HTU21D_Resolution sensorResolution)
{
  uint8_t userRegisterData;

  if (_HTU21Dinitialisation != true)
  {
    #if defined(ARDUINO_ARCH_ESP8266) || (ESP8266_NODEMCU)
     begin(_sda, _scl);
    #else
     begin();
    #endif
  }

  /* Get the current register state */
  userRegisterData = read8(HTU21D_USER_REGISTER_READ);

  /* Replace current resolution bits with "0" */
  userRegisterData &= 0x7E;
  /* Add new resolution bits to userRegisterData */
  userRegisterData |= sensorResolution;

  /* Write updeted userRegisterData to sensor */
  write8(HTU21D_USER_REGISTER_WRITE, userRegisterData);

  /* Update value placeholder */
  _HTU21D_Resolution = sensorResolution;
}

/**************************************************************************/
/*
    Soft reset.
 
    Switch sensor OFF & ON again. Takes about 15ms

    NOTE: All registers set to default exept heater bit.
*/
/**************************************************************************/
void HTU21D::softReset(void)
{
  if (_HTU21Dinitialisation != true)
  {
    #if defined(ARDUINO_ARCH_ESP8266) || (ESP8266_NODEMCU)
     begin(_sda, _scl);
    #else
     begin();
    #endif
  }

  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
    Wire.write(HTU21D_SOFT_RESET);
  #else
    Wire.send(HTU21D_SOFT_RESET);
  #endif
  Wire.endTransmission();
  delay(15);
}

/**************************************************************************/
/*
    Checks Battery Status

    for SHT21, HTU21D
    if VDD > 2.25v -+0.1v return TRUE
    if VDD < 2.25v -+0.1v return FALSE

    for Si7021
    if VDD > 1.9v -+0.1v return TRUE
    if VDD < 1.9v -+0.1v return FALSE
*/
/**************************************************************************/
bool HTU21D::batteryStatus(void)
{
  uint8_t userRegisterData;
  
  if (_HTU21Dinitialisation != true)
  {
    #if defined(ARDUINO_ARCH_ESP8266) || (ESP8266_NODEMCU)
     begin(_sda, _scl);
    #else
     begin();
    #endif
  }

  userRegisterData = read8(HTU21D_USER_REGISTER_READ);

  userRegisterData &= 0x40;

  if (userRegisterData == 0x00)
  {
    return true;
  }

  return false;
}

/**************************************************************************/
/*
    Turn ON/OFF build-in Heater

    The heater consumtion is 3.09mA - 94.20mA @ 3.3v.
    
    NOTE: Used to raise the temperature of the sensor. This element can be
          used to test the sensor, to drive off condensation, or to implement
          dew-point measurement when the Si7021 is used in conjunction with a
          separate temperature sensor (the heater will raise the temperature of
          the internal temperature sensor).
*/
/**************************************************************************/
void HTU21D::setHeater(HTU21D_toggleHeaterSwitch heaterSwitch)
{
  uint8_t userRegisterData;
  
  if (_HTU21Dinitialisation != true)
  {
    #if defined(ARDUINO_ARCH_ESP8266) || (ESP8266_NODEMCU)
     begin(_sda, _scl);
    #else
     begin();
    #endif
  }

  userRegisterData = read8(HTU21D_USER_REGISTER_READ);

  switch(heaterSwitch)
  {
    case HTU21D_ON:
      userRegisterData |= heaterSwitch;
      _heaterON = true;
      break;
    case HTU21D_OFF:
      userRegisterData &= heaterSwitch;
      _heaterON = false;
      break;
  }

  write8(HTU21D_USER_REGISTER_WRITE, userRegisterData);
}

/**************************************************************************/
/*
    Reads Humidity, %RH

    Max. measurement time about 16ms.
    Accuracy +-2%RH in range 20%RH - 80%RH at 25deg.C only

    NOTE: "operationMode" could be set up as:
    - "TRIGGER_TEMP_MEASURE_NOHOLD" mode, allows communication with another
      slave devices on I2C bus while sensor is measuring.
      WARNING!!! Could create collision if more than one slave devices are
      connected to the same bus.
    - "TRIGGER_HUMD_MEASURE_HOLD" mode, blocks communication on I2C bus while
      sensor is measuring.
    - suggested time between measurements is 17 sec. - 18 sec. (sensor could
      faster but it's pointless)
*/
/**************************************************************************/
float HTU21D::readHumidity(HTU21D_humdOperationMode sensorOperationMode)
{
  uint8_t   checksum;
  uint8_t   pollCounter;
  uint16_t  rawHumidity;
  float     humidity;

  /* request a humidity measurement/reading */
  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
    Wire.write(sensorOperationMode);
  #else
    Wire.send(sensorOperationMode);
  #endif

  if (Wire.endTransmission() != 0)
  {
    return(0.00);
  }

  /* humidity measurement delay */
  switch(_HTU21D_Resolution)
  {
    case HTU21D_RES_RH12_TEMP14:
      delay(16);                 //Si7021 - 17..23mS, SHT21 - 22..29mS
      break;
    case HTU21D_RES_RH8_TEMP12:
      delay(3);                  //Si7021 - 5..7mS,   SHT21 - 3..4mS
      break;
    case HTU21D_RES_RH10_TEMP13:
      delay(5);                  //Si7021 - 8..11mS,  SHT21 - 7..9mS
      break;
    case HTU21D_RES_RH11_TEMP11:
      delay(8);                  //HTU21 - 8..10mS,   SHT21 - 12..15mS
      break;
  }

  /* poll to check the end of the measurement, bacause Si7021 & SHT21 are slower than HTU21D */
  Wire.requestFrom(HTU21D_ADDRESS, 3);
  while (Wire.available() < 3)
  {
    pollCounter++;
    if (pollCounter > 8)
    {
      return(0.00);
    }
    delay(8);
  }

  /* reads MSB byte, LSB byte & Checksum */
  #if ARDUINO >= 100
    rawHumidity  = Wire.read() << 8; /* Reads MSB byte & shift it to the right */
    rawHumidity |= Wire.read();      /* reads LSB byte & sum. with MSB byte    */
    checksum     = Wire.read();
  #else
    rawHumidity  = Wire.receive() << 8;
    rawHumidity |= Wire.receive();
    checksum     = Wire.receive();
  #endif

  if (checkCRC8(rawHumidity) != checksum)
  {
    return(0.00);
  }

  /* clear last two status bits (humidity measurement always returns XXXXXX10 in the LSB field) */
  rawHumidity ^= 0x02;

  humidity = 0.001907 * (float)rawHumidity - 6;
  
  if (humidity > 100)
  {
   return humidity = 100.00;
  }
  if (humidity < 0)
  {
   return humidity = 0.01;
  }

  return humidity;
}

/**************************************************************************/
/*
    Reads Temperature, deg.C

    Max. measurement time about 50ms.
    Accuracy +-0.3deg.C in range 0deg.C - 60deg.C

    NOTE: "operationMode" could be set up as:
    - "TRIGGER_TEMP_MEASURE_NOHOLD" mode, allows communication with another
      slave devices on I2C bus while sensor is measuring.
      WARNING!!! Could create collision if more than one slave devices are
      connected to the same bus.
    - "TRIGGER_TEMP_MEASURE_HOLD" mode, blocks communication on I2C bus while
      sensor is measuring.
    - "TEMP_READ_AFTER_RH_MEASURMENT" mode, allows to retrive temperature
      measurement, which was made at previouse RH measurement.
      Used only by Si7021 sensor  to compensate the temperature effect on RH.
      For HTU21D & SHT21 you have to manualy call "readCompensatedHumidity(void)"
*/
/**************************************************************************/
float HTU21D::readTemperature(HTU21D_tempOperationMode sensorOperationMode)
{
  uint8_t  checksum;
  uint8_t  pollCounter;
  uint16_t rawTemperature;
  float    temperature;

  /* request a temperature measurement/reading */
  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
    Wire.write(sensorOperationMode); 
  #else
    Wire.send(sensorOperationMode);
  #endif

  if (Wire.endTransmission() != 0)
  {
    return(0.00);
  }

  if (sensorOperationMode == HTU21D_TEMP_READ_AFTER_RH_MEASURMENT)
  {
    goto skipMeasurementDelay;
  }

  /* temperature measurement delay */
  switch(_HTU21D_Resolution)
  {
    case HTU21D_RES_RH12_TEMP14:
      delay(11);                 //HTU21D - 44..50mS, SHT21 - 66..85mS
      break;
    case HTU21D_RES_RH8_TEMP12:
      delay(4);                  //HTU21D - 11..13mS, SHT21 - 17..22mS
      break;
    case HTU21D_RES_RH10_TEMP13:
      delay(7);                  //Si7021 - 22..25mS, SHT21 - 33..43mS
      break;
    case HTU21D_RES_RH11_TEMP11:
      delay(3);                  //Si7021 - 6..7mS,   SHT21 - 9..11mS
      break;
  }

  /* poll to check the end of the measurement, bacause HTU21D & SHT21 are slower than Si7021 */
  skipMeasurementDelay: Wire.requestFrom(HTU21D_ADDRESS, 3);
  while (Wire.available() < 3)
  {
    pollCounter++;
    if (pollCounter > 8)
    {
      return(0.00);
    }
    delay(16);
  }

  /* reads MSB byte, LSB byte & Checksum */
   #if ARDUINO >= 100
    rawTemperature  = Wire.read() << 8; /* reads MSB byte & shift it to the right */
    rawTemperature |= Wire.read();      /* reads LSB byte and sum. with MSB byte  */
    checksum        = Wire.read();
  #else
    rawTemperature  = Wire.receive() << 8;
    rawTemperature |= Wire.receive();
    checksum        = Wire.receive();
  #endif

  if (checkCRC8(rawTemperature) != checksum)
  {
    return(0.00);
  }

  temperature = 0.002681 * (float)rawTemperature - 46.85;

  /* temperature measurement will always return XXXXXX00 in the LSB field */
  return temperature;  
}

/**************************************************************************/
/*
    Calculates Compensated Humidity, %RH

    Compensate the temperature effect on RH measurement.

    Max. measurement time about 115ms.
    Accuracy +-2%RH in range 0%RH - 100%RH at tmp. range 0deg.C - 80deg.C

    NOTE: Only for HTU21D & SHT21. Si7021 automatically compensates
          temperature effect on RH every Humidity measurement.

*/
/**************************************************************************/
float HTU21D::readCompensatedHumidity(void)
{
  float humidity;
  float temperature;
  float compensatedHumidity;

  humidity    = readHumidity();
  temperature = readTemperature();

  if (humidity == 0.00 && temperature == 0.00)
  {
    return (0.00);
  }

  if (temperature > 0 && temperature < 80)
  {
    return compensatedHumidity = humidity + (25 - temperature) * HTU21D_TEMP_COEFFICIENT;
  }

  return humidity;
}


/***************************************************************************/
/*
    Reads Device ID

    SerialNumber = {SNA3, SNA2, SNA1, SNA0, SNB3**, SNB2, SNB1, SNB0}
    
    **chip ID
    0x0D - Si7013
    0x14 - Si7020
    0x15 - Si7021 
    0x32 - HTU21D  
*/
/**************************************************************************/
uint16_t HTU21D::readDeviceID(void)
{
  uint16_t deviceID;
  uint8_t  checksum;

  /* Serial_2 requests SNB3**, SNB2, SNB1, SNB0 */
  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
    Wire.write(HTU21D_SERIAL2_READ1);
    Wire.write(HTU21D_SERIAL2_READ2);
  #else
    Wire.send(HTU21D_SERIAL2_READ1);
    Wire.send(HTU21D_SERIAL2_READ2);
  #endif
  Wire.endTransmission();

  /* Serial_2 reads SNB3**, SNB2 & CRC */
  Wire.requestFrom(HTU21D_ADDRESS, 3);
  #if ARDUINO >= 100
    deviceID  = Wire.read() << 8;
    deviceID |= Wire.read();
    checksum  = Wire.read();
    if (checkCRC8(deviceID) == checksum)
    {
      deviceID = deviceID >> 8;
    }
  #else
    deviceID  = Wire.receive() << 8;
    deviceID |= Wire.receive();
    checksum  = Wire.receive();
    if (checkCRC8(deviceID) == checksum)
    {
      deviceID = deviceID >> 8;;
    }
  #endif

  switch(deviceID)
  {
    case HTU21D_CHIPID:
      deviceID = 21;
      break;
    case SI7013_CHIPID:
      deviceID = 7013;
      break;
    case SI7020_CHIPID:
      deviceID = 7020;
      break;
    case SI7021_CHIPID:
      deviceID = 7021;
      break;
    default:
      deviceID = 0;
      break;
  }

  return deviceID;
}

/***************************************************************************/
/*
    Reads Firware Version  
*/
/**************************************************************************/
uint8_t HTU21D::readFirmwareVersion(void)
{
  uint8_t  firmwareVersion;

  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
    Wire.write(HTU21D_FIRMWARE_READ1);
    Wire.write(HTU21D_FIRMWARE_READ2);
  #else
    Wire.send(HTU21D_FIRMWARE_READ1);
    Wire.send(HTU21D_FIRMWARE_READ2);
  #endif
  Wire.endTransmission();

  Wire.requestFrom(HTU21D_ADDRESS, 1);
  #if ARDUINO >= 100
      firmwareVersion = Wire.read();
  #else
      firmwareVersion = Wire.read();
  #endif

  switch(firmwareVersion)
  {
    case HTU21D_FIRMWARE_V1:
      firmwareVersion = 1;
      break;
    case HTU21D_FIRMWARE_V2:
      firmwareVersion = 2;
      break;
    default:
      firmwareVersion = 0;
    break;
  }

  return firmwareVersion;
}

/**************************************************************************/
/*
    Writes 8 bit value to the sensor register over I2C
*/
/**************************************************************************/
void HTU21D::write8 (uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
    Wire.write(reg);
    Wire.write(value);
  #else
    Wire.send(reg);
    Wire.send(value);
  #endif
  Wire.endTransmission();
}

/**************************************************************************/
/*
    Reads 8 bit value from the sensor, over I2C
*/
/**************************************************************************/
uint8_t HTU21D::read8(uint8_t reg)
{
  Wire.beginTransmission(HTU21D_ADDRESS);
  #if ARDUINO >= 100
    Wire.write(reg);
  #else
    Wire.send(reg);
  #endif
  Wire.endTransmission();

  Wire.requestFrom(HTU21D_ADDRESS, 1);
  #if ARDUINO >= 100
    return Wire.read();
  #else
    return Wire.receive();
  #endif
}

/**************************************************************************/
/*
    Calculates CRC8 for 16 bit received Data

    NOTE:
    - For more info about Cyclic Redundancy Check (CRC) see:
      http://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks
*/
/**************************************************************************/
uint8_t HTU21D::checkCRC8(uint16_t data)
{
  for (uint8_t bit = 0; bit < 16; bit++)
  {
    if (data & 0x8000)
    {
      data =  (data << 1) ^ HTU21D_CRC8_POLYNOMINAL;
    } 
    else
    {
      data <<= 1;
    }
  }

  return data >>= 8;
}
