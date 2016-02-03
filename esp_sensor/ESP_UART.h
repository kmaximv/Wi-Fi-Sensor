#ifndef ESP_UART_h
#define ESP_UART_h

#include <Arduino.h>


class Espuart
{
  public:
    bool crcCheck(String dataStr, uint8_t crcControl);
    bool serialEvent(void);
    uint8_t crcCalc(String dataStr);
    bool Send(String data);
    String dataString;
    String startMarker;
    String stopMarker;
    char delimiter;
    unsigned int * stateAnalogPin;
    unsigned long * timerAnalogPin;

  private:

};

extern Espuart Uart;
#endif