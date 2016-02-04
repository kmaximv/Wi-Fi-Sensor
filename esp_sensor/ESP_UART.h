#ifndef ESP_UART_h
#define ESP_UART_h

#include <Arduino.h>


#define DEBUG_ESP_UART
#define CRC_ENABLE

#define DIGITAL_PINS 14   //Кол-во цифровых входов/выходов
#define ANALOG_PINS 6     //Кол-во цифровых входов/выходов
#define PARSE_CELLS 4     //Кол-во ячеек в массиве принимаемых данных
#define DATA_LENGTH 10    //Максимальный размер пакета данных без маркеров и CRC


class Espuart
{
  public:
    bool crcCheck(String dataStr, uint8_t crcControl);
    bool serialEvent(void);
    uint8_t crcCalc(String dataStr);
    bool Send(String data);
    void SetAnalogReadCycle(int pin, int delay, String timeRank);
    String dataString;
    String startMarker;
    String stopMarker;
    char delimiter;
    unsigned int  * valueAnalogPin;
    unsigned long * timerAnalogPin;
    unsigned long * delayAnalogPin;
    String * parseArray;

  private:

};

extern Espuart Uart;
#endif