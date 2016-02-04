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
	char delimiter = '&';             // Разделительный символ в пакете данных
	unsigned int  valueAnalogPin[ANALOG_PINS];
	unsigned long timerAnalogPin[ANALOG_PINS];
    unsigned long delayAnalogPin[ANALOG_PINS];
	String parseArray[PARSE_CELLS];   //Распарсенный массив принимаемых данных

  private:

};

extern Espuart Uart;
#endif