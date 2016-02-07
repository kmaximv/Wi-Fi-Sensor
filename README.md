# Wi-Fi Sensor

Датчик температуры, влажности, давления, движения и освещенности на ESP-12F

Для управления нагрузкой есть два варианта платы. Первый вариант для низковольтной нагрузки, в моем случае это светодиодные ленты. Второй вариант для высоковольтной нагрузки (220 вольт), используемое твёрдотельное реле выдерживает ток до 8 ампер.

Все датчики и Wi-Fi модуль работают от питания 3,3V.

Плату заказывал по инструкции автора Vipeg [Ссылка](http://mysku.ru/blog/others/36157.html) , за что ему отдельное спасибо.


## Содержание
- [Возможности беспроводного датчика](#Возможности-беспроводного-датчика)
- [Варианты прошивки модуля ESP](#Варианты-прошивки-модуля-esp)
   - [Прошиваем с помощью утилиты Esptool](#1-Прошиваем-с-помощью-утилиты-esptool)
   - [Прошиваем с помощью IDE Arduino](#2-Прошиваем-с-помощью-ide-arduino)
- [Библиотеки Arduino](#Библиотеки-arduino)
- [Первоначальная настройка модуля ESP](#Первоначальная-настройка-модуля-esp)
- [Железо, которое использовалось в этом проекте](#Железо-которое-использовалось-в-этом-проекте)
- [Датчик движения HC-SR501](#Датчик-движения-hc-sr501)  
- [Схема](#Схема) 
- [Печатная плата](#Печатная-плата) 
- [Web-интерфес](#web-интерфес) 
- [OpenHab](#openhab)


### Возможности беспроводного датчика:

- Передача данных по протоколу MQTT
- Управление нагрузкой по протоколу MQTT и через веб-интерфейс
- Настройка через веб-интерфейс
- Мониторинг данных через веб-интерфейс
- Обновление прошивки через веб-интерфейс


### Варианты прошивки модуля ESP

####1. Прошиваем с помощью утилиты Esptool
  - Скачиваем Esptool для своей ОС [ссылка](https://github.com/igrr/esptool-ck/releases)
  - Скачиваем прошивку по [ссылке](https://github.com/kmaximv/Wi-Fi-Sensor/releases)
  - Переводим модуль в режим прошивки 
  - В коммандной строке выполняем комманду:

```bash
Windows
C:\Soft/esptool.exe -vv -cd nodemcu -cb 115200 -cp COM6 -ca 0x00000 -cf C:\Soft/esp_sensor.ino.nodemcu.bin

Linux
./esptool -vv -cd nodemcu -cb 115200 -cp /dev/ttyUSB0 -ca 0x00000 -cf esp_sensor.ino.bin
```

  Пути и номер порта ставим свои.

####2. Прошиваем с помощью IDE Arduino
   - Устанавливаем IDE Arduino 1.6.5
   - Устанавливаем Arduino core for ESP8266 Staging version  [Ссылка](https://github.com/esp8266/Arduino)
   - Устанавливаем библиотеки Arduino, перечисленные ниже
   - Скачиваем файлы по [ссылке](https://github.com/kmaximv/Wi-Fi-Sensor/tree/master/esp_sensor)
   - Открываем файл с расширением *.ino в программе IDE Arduino
   - Выбираем наш модуль в настройках 
  [![arduino-ide](/screenshots/thumbs/arduino-ide.png)](/screenshots/arduino-ide.png)
   - Переводим модуль в режим прошивки 
   - Прошиваем, нажав кнопку "Загрузка"


### Библиотеки Arduino:

- ArduinoJson   [Ссылка](https://github.com/bblanchon/ArduinoJson)
- Pubsubclient  [Ссылка](https://github.com/knolleary/pubsubclient)
- BH1750        [Ссылка](https://github.com/claws/BH1750)
- BME280        [Ссылка](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library)
- HTU21D        [Ссылка](https://github.com/enjoyneering/HTU21D)


### Первоначальная настройка модуля ESP

- Прошиваем модуль одним из выше описанных вариантов. 
- Подключаемся к нему по Wi-Fi
- Через браузер заходим на адрес http://192.168.4.1
- Прописываем свои настройки, перезагружаемся.

По умолчанию модуль пытается подключиться к Wi-Fi точке доступа со следующими параметрами:
```bash
SSID:       HomeNET
Password:   Asdf1234
```
Параметр Reboot Delay перезагружает устройство если не получает никаких данных по MQTT за указанный промежуток времени. Если Вы не используете MQTT, установите этот параметр на максимальное значение 4294966 секунд (49 дней).


### Железо, которое использовалось в этом проекте:

- BME280 - Датчик температуры, влажности, давления   [Покупал здесь](http://www.ebay.com/itm/361366521095?_trksid=p2060353.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT) 
- SHT21 - Датчик температуры, влажности   [Покупал здесь](http://www.ebay.com/itm/201371694878?ssPageName=STRK:MESINDXX:IT&_trksid=p3984.m1436.l2649)
- BH1750 - Датчик освещенности   [Покупал здесь](http://www.ebay.com/itm/281774009375?_trksid=p2060353.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT)
- HC-SR501 - Датчик движения
- ESP8266-12F - Wi-Fi модуль   [Покупал здесь](http://www.aliexpress.com/snapshot/7056558451.html?orderId=70501426978354)
- IR4427 - Mosfet драйвер
- MP1584EN - DC-DC преобразователь   [Покупал здесь](http://www.aliexpress.com/snapshot/7056558454.html?orderId=70501426988354)
- IRLU024N - Mosfet транзистор
- Sharp S202s02 - Твёрдотельное реле   [Покупал здесь](http://www.aliexpress.com/snapshot/7220777148.html?orderId=71990495728354)


### Датчик движения HC-SR501
В доработке нуждается только датчик движения HC-SR501, который рассчитан на работу от 5V. Нужно отпаять регулятор напряжения и поставить перемычку.

   [![Фото доработки HC-SR501](/screenshots/HC-SR501.jpg)](/screenshots/HC-SR501.jpg)


### Схема

   [![Schema_LED](/screenshots/Schema_LED.png)](/screenshots/Schema_LED.png)

   [![Schema_SSR](/screenshots/Schema_SSR.png)](/screenshots/Schema_SSR.png)


### Печатная плата

   [![Top](/screenshots/thumbs/PCB_top_th.png)](/screenshots/PCB_top.png)
   [![Bottom](/screenshots/thumbs/PCB_bottom_th.png)](/screenshots/PCB_bottom.png)


### Web-интерфес

   [![Status](/screenshots/Status.png)](/screenshots/Status.png)

   [![Control](/screenshots/Control.png)](/screenshots/Control.png)

   [![ConfigESP](/screenshots/ConfigESP.png)](/screenshots/ConfigESP.png)

   [![ConfigMQTT](/screenshots/ConfigMQTT.png)](/screenshots/ConfigMQTT.png)

   [![Update](/screenshots/Update.png)](/screenshots/Update.png)


### OpenHab

   [![Status](/screenshots/OpenHab.png)](/screenshots/OpenHab.png)

Управление освещением имеет 3 режима: Включен, Выключен, Авто.
В Авто режиме свет включается по датчику движения. Также есть настройка задержки отключения.


   [![Status](/screenshots/OpenHab_graph.png)](/screenshots/OpenHab_graph.png)

   [![Extand](/screenshots/OpenHab_ext.png)](/screenshots/OpenHab_ext.png)
