# USB_PIC_Programmer_on_Arduino
Open USB Programmer project for PIC MCU based on Arduino (ATMEGA328)

Программатор для микроконтроллеров PIC, основанный на atmega328 (arduino nano/uno) с минимальной внешней обвязкой. 

![screenshot](https://github.com/AndrejChoo/USB-PIC-Programmer-on-Arduino/blob/main/software/soft.png)

# 06.02.2023
На данный момент поддерживаются следующие модели PIC:

-PIC12: PIC12F1840,PIC12F629/675,PIC12F635,PIC12F683

-PIC16: PIC16F1512,PIC16F1513,PIC16F1516,PIC16F1517,PIC16F1518,PIC16F1519,PIC16F1526,
PIC16F1527,PIC16F1847,PIC16F1933,PIC16F1934,PIC16F1936,PIC16F1937,PIC16F1938,PIC16F1939,
PIC16F1946,PIC16F1947,PIC16F627A,PIC16F628A,PIC16F648A,PIC16F676/630,PIC16F631,
PIC16F636,PIC16F639,PIC16F677,PIC16F684,PIC16F685,PIC16F687,PIC16F688,PIC16F689,
PIC16F690,PIC16F73,PIC16F74,PIC16F76,PIC16F77,PIC16F84A

-PIC18: PIC18F2510,PIC18F2525,PIC18F2550,PIC18F2585,PIC18F25K80,PIC18F2610,PIC18F2620,
PIC18F2680,PIC18F26K80,PIC18F4515,PIC18F4525,PIC18F4550,PIC18F45K80,PIC18F4585,PIC18F4610,
PIC18F4620,PIC18F4680,PIC18F46K80

Тестирую обычно 1-2 модели PIC из линейки однотипно программируемых, остальные просто 
добавляю. Так что могут быть ошибки непротестированных моделей. Надо пробовать.

# 10.02.2023
Изменён алгоритм записи со стороны хоста, теперь не нужно предварительно стирать чип перед записью. Добавлены некоторые оптимизации.
Добавлена новые модели PIC:
PIC10: PIC10F200, PIC10F202, PIC10F204, PIC10F206
PIC12: PIC12F510
PIC16F: PIC16F505, PIC16F506, PIC16F54, PIC16F57, PIC16F59

# 17.02.2023
Исправлен крупный баг софта при чтении микроконтроллеров с объёмом FLASH 16KW и более.
Добавлена и протестирована поддержка новых линеек PIC:
PIC16F183xx
Добавлены новые модели без тестирования:
PIC10F32x, PIC16F150x, PIC16F88x

# 29.03.2023
На этот раз были добавлены семейства:
PIC16F170X, PIC16F18XX, PIC16F183XX, PIC16F184XX, PIC18FXXK22, PIC18FXXQ10 (у последней серии некоррекно читается или записывается EEPROM, проверить мне не чем).

# 23.02.2024
Переписал софт на Qt. Теперь программа стала кроссплатформенной. Работа проверена не Windows x64, Linux Fedora 39 x64, Ubuntu 22.04 aarch64. Внешний вид программы и функционал остались прежними.

![screenshot1](https://github.com/AndrejChoo/USB-PIC-Programmer-on-Arduino/blob/main/software/soft_Qt.png)

Список поддерживаемых моделей расположен всё в том же файле "pic.xml". Для обеих версий программы файлы идентичны.
Для работы на Linux потребуется установить QSetialPort (libqt6-serialport-dev) и qt6-base-dev, возможно ещё какие-либо пакеты. Проверить можно при запуске из терминала.
Если не появится или не будет доступа к /dev/ttyUSB0, то необходимо его предоставить: "sudo chmod a+rw /dev/ttyUSB0". На Ubuntu могут конфликтовать драйверы ch340 и brltty, поэтому последний можно удалить "sudo apt autoremove brltty".
На Windows, если не установлен qt-creator, возможно потребуются какие-либо Qt-шные dll-ки. Проверить не могу, так как у меня на всех ОС уже установлено всё необходимое.
