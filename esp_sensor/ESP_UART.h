#ifndef ESP_UART_h
#define ESP_UART_h

#include <Arduino.h>



class Espuart
{
  public:
    bool crcCheck(uint8_t *data, size_t length, uint8_t crcControl);
    bool serialEvent(void);
    uint8_t crcCalc(uint8_t *data, size_t length);
    String dataString;
    String startMarker;
    String stopMarker;
    char delimiter;
  private:

};

#endif