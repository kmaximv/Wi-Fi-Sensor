#include "ESP_UART.h"

//#define PARSE_CELLS 4     //Кол-во ячеек в массиве принимаемых данных
#define DATA_LENGTH 10    //Максимальный размер пакета данных без маркеров и CRC


String startMarker = "<beg>";           // Переменная, содержащая маркер начала пакета
String stopMarker  = "<end>";            // Переменная, содержащая маркер конца пакета
String dataString;            // Здесь будут храниться принимаемые данные
//uint8_t sp_data[DATA_LENGTH];             
int startMarkerStatus;        // Флаг состояния маркера начала пакета
int stopMarkerStatus;         // Флаг состояния маркера конца пакета
uint8_t dataLength;               // Флаг состояния принимаемых данных
boolean packetAvailable;      // Флаг завершения приема пакета
uint8_t crc_byte;

//String parseArray[PARSE_CELLS];            //Распарсенный массив принимаемых данных

char delimiter = '&';             // Разделительный символ в пакете данных



uint8_t crc8(uint8_t crc, uint8_t data, uint8_t polynomial){
  crc ^= data;
  for (size_t i = 0; i < 8; ++i){
    crc = (crc << 1) ^ ((crc & 0x80) ? polynomial : 0);
  }
  return crc;
}


uint8_t crc8_ccitt(uint8_t crc, uint8_t data){
  return crc8(crc, data, 0x07);
}


/* Вспомогательная функция вычисления CRC для строки */
uint8_t Espuart::crcCalc(String dataStr){

  uint8_t data[DATA_LENGTH];
  size_t length = dataStr.length();
  dataStr.getBytes(data, length + 1); // + 1 для дополнительного символа окончания строки

  #ifdef DEBUG
  printByte(data);
  #endif

  uint8_t crc = 0;
  for (size_t i = 0; i < length; ++i) {
    crc = crc8_ccitt(crc, data[i]);
    #ifdef DEBUG
    Serial.print(crc, DEC); Serial.print(F(" "));
    #endif
  }
  return crc;
}


bool Espuart::crcCheck(String dataStr, uint8_t crcControl) {
  uint8_t crc = crcCalc(dataStr);
  #ifdef DEBUG
  Serial.print(F("CRC:        "));    Serial.println(crc);    Serial.print(F("crcControl: "));    Serial.println(crcControl);
  #endif
  if (crc == crcControl) {
    #ifdef DEBUG
    Serial.println(F("CRC OK!"));
    #endif
    return true;
  } else {
    #ifdef DEBUG
    Serial.println(F("CRC Error!"));
    #endif
    return false;
  }
}



void sp_Reset(){
  dataString = "";           // Обнуляем буфер приема данных
  startMarkerStatus = 0;     // Сброс флага маркера начала пакета
  stopMarkerStatus = 0;      // Сброс флага маркера конца пакета
  dataLength = 0;            // Сброс флага принимаемых данных
  packetAvailable = false;   // Сброс флага завершения приема пакета
  crc_byte = 0;
}


void sp_Read()
{
  while(Serial.available() && !packetAvailable) {                   // Пока в буфере есть что читать и пакет не является принятым
    uint8_t bufferChar = Serial.read();                               // Читаем очередной байт из буфера
    if(startMarkerStatus < startMarker.length()) {              // Если стартовый маркер не сформирован (его длинна меньше той, которая должна быть) 
      if(startMarker[startMarkerStatus] == bufferChar) {        // Если очередной байт из буфера совпадает с очередным байтом в маркере
       startMarkerStatus++;                                        // Увеличиваем счетчик совпавших байт маркера
      } else {
       sp_Reset();                                                 // Если байты не совпали, то это не маркер. Нас нае****, расходимся. 
      }
    } else {
     // Стартовый маркер прочитан полностью
      if(dataLength <= 0) {                                        // Если длинна пакета не установлена
        dataLength = (int)bufferChar - 48;                          // Значит этот байт содержит длину пакета данных
        #ifdef DEBUG
        Serial.println();   Serial.println();
        Serial.print(F("dataLength: "));  Serial.println(dataLength);
        #endif
      } else if (crc_byte <= 0) { 
        crc_byte = bufferChar;                                        // Значит этот байт содержит контрольную сумму пакета данных
        #ifdef DEBUG
        Serial.print(F("crc_byte: "));  Serial.println(crc_byte);
        #endif
      } else {                                                        // Если прочитанная из буфера длинна пакета больше нуля
        if(dataLength > dataString.length()) {                  // Если длинна пакета данных меньше той, которая должна быть
          dataString += (char)bufferChar;                          // прибавляем полученный байт к строке пакета
        } else {                                                      // Если с длинной пакета данных все нормально
          if(stopMarkerStatus < stopMarker.length()) {          // Если принятая длинна маркера конца пакета меньше фактической
            if(stopMarker[stopMarkerStatus] == bufferChar) {    // Если очередной байт из буфера совпадает с очередным байтом маркера
              stopMarkerStatus++;                                  // Увеличиваем счетчик удачно найденных байт маркера
              if(stopMarkerStatus == stopMarker.length()) {
                #ifdef DEBUG
                Serial.println(F("Packet recieve!"));
                Serial.println(dataString);
                #endif
                packetAvailable = true;                            // и устанавливаем флаг готовности пакета
              }
            } else {
              sp_Reset();                                          // Иначе это не маркер, а х.з. что. Полный ресет.
            }
          }
        }
      } 
    }    
  }
}


bool Espuart::Send(String data){
  String packet = startMarker;      // Отправляем маркер начала пакета
  packet += String(data.length());  // Отправляем длину передаваемых данных
  packet += String(crcCalc(data));  // Отправляем контрольную сумму данных
  packet += data;                   // Отправляем сами данные
  packet += stopMarker;             // Отправляем маркер конца пакета
  Serial.print(packet);

  #ifdef DEBUG
    Serial.print(F("Send Uart:"));  Serial.println(data);
  #endif

  if (serialEvent() &&  dataString == data){
    return true;
  }

  #ifdef DEBUG
    Serial.print(F("Receive Uart:"));  Serial.println(dataString);
  #endif

  return false;
}





bool Espuart::serialEvent(){
  sp_Read();                         // Вызов «читалки» принятых данных
  if(packetAvailable){             // Если после вызова «читалки» пакет полностью принят
    //ParseCommand();                   // Обрабатываем принятую информацию
    sp_Reset();                    // Полный сброс протокола.
  	return true;
  }
  return false;
}