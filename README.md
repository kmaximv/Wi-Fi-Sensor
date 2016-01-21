# Wi-Fi Sensor

Датчик температуры, влажности, давления, движения и освещенности на ESP-12F

Для управления нагрузкой есть два варианта платы. Первый вариант для низковольтной нагрузки, в моем случае это светодиодные ленты. Второй вариант для высоковольтной нагрузки (220 вольт), используемое твёрдотельное реле выдерживает ток до 8 ампер.

Все датчики и Wi-Fi модуль работают от питания 3,3V.

Плату заказывал по инструкции автора Vipeg [Ссылка](http://mysku.ru/blog/others/36157.html) , за что ему отдельное спасибо.


### Возможности беспроводного датчика:

- Передача данных по протоколу MQTT
- Управление нагрузкой по протоколу MQTT и через веб-интерфейс
- Настройка через веб-интерфейс
- Мониторинг данных через веб-интерфейс
- Обновление прошивки через веб-интерфейс


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


### Необходимое ПО:

- IDE Arduino 1.6.5
- Arduino core for ESP8266 Staging version  [Ссылка](https://github.com/esp8266/Arduino)
- Eagle 7.5.0


### Библиотеки Arduino:

- ArduinoJson   [Ссылка](https://github.com/bblanchon/ArduinoJson)
- Pubsubclient  [Ссылка](https://github.com/knolleary/pubsubclient)
- BH1750        [Ссылка](https://github.com/claws/BH1750)
- BME280        [Ссылка](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library)
- HTU21D        [Ссылка](https://github.com/enjoyneering/HTU21D)


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
