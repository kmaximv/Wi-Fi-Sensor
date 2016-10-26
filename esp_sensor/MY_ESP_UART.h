/* Команды управления

<beg>6љout&03<end>
<beg>4Sin&3<end>
<beg>5Son&03<end>
<beg>6Eoff&03<end>
<beg>49rd&3<end>

<beg>8дp&03&100<end>

<beg>4щra&3<end>

<beg>4!a&03&10&s<end>
*/

#ifndef MY_ESP_UART_H
#define MY_ESP_UART_H

#include "json_config.h"


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
    String parseArray[PARSE_CELLS];   //Распарсенный массив принимаемых данных

  private:
    void printByte(uint8_t *data);
    uint8_t crc8(uint8_t crc, uint8_t data, uint8_t polynomial);
    uint8_t crc8_ccitt(uint8_t crc, uint8_t data);
    void Reset();
    void Read();
    bool ParseCommand();
    String startMarker = "<beg>";     // Переменная, содержащая маркер начала пакета
    String stopMarker  = "<end>";     // Переменная, содержащая маркер конца пакета
    String dataString;                // Здесь будут храниться принимаемые данные
    uint8_t startMarkerStatus;        // Флаг состояния маркера начала пакета
    uint8_t stopMarkerStatus;         // Флаг состояния маркера конца пакета
    uint8_t dataLength;               // Флаг состояния принимаемых данных
    boolean packetAvailable;          // Флаг завершения приема пакета
    uint8_t crc_byte;                 // Принятый байт контрольной суммы CRC
    String a = "a";                   // Тип команды
    String av = "av";                 // Тип команды
};

#endif