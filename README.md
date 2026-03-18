# USB_PIC_Programmer_on_Arduino
Open USB Programmer project for PIC MCU based on Arduino (ATMEGA328)

Программатор для микроконтроллеров PIC, основанный на atmega328 (arduino nano/uno) с минимальной внешней обвязкой. 

![screenshot](https://github.com/AndrejChoo/USB-PIC-Programmer-on-Arduino/blob/main/software/soft.png)

Есть возможность добавления новых моделей самостоятельно, для этого необходимо редактировать файл "pic.xml". Он состоит из однотипных структур типа:

```
  <name model="PIC16F1938">
	<id>0x23A0</id>
	<type>7</type>
	<f_size>32768</f_size>
	<f_psize>1</f_psize>
	<ee_size>256</ee_size>
	<f_start>0</f_start>
	<ee_start>00F000</ee_start>
	<ee_type>2</ee_type>
	<conf1>008007</conf1>
	<conf2>008008</conf2>
	<conf3>none</conf3>
	<conf4>none</conf4>
	<conf5>none</conf5>
	<conf6>none</conf6>
	<conf7>none</conf7>
	<oscal1>008009</oscal1>
	<oscal2>00800A</oscal2>
	<oscal3>none</oscal3>
	<delay>3</delay>
	<word>3FFF</word>
  </name>
```
Для добавления нового чипа - копируем одну структуру от "name" до "/name" и исправляем значения тегов. Где:

>id - devid + revid
>
>type - семейство однотипно программируемых моделей pic, номера взяты абсолютно произвольно, просто находим по даташиту похожую по алгоритму программирования модель из уже поддерживаемых
>
>f_size - размер code сегмента в байтах!
>
>f_psize - размер страницы флеша
>
>ee_size - размер eeprom в байтах
>
>f_start - адрес начала code сегмента (обычно всегда равно нулю)
>
>ee_start - адрес начала data (eeprom) сегмента памяти
>
>ee_type - тип расположения данных eeprom в hex файле (1 - подряд, 2 - через пустой байт)
>
>conf1-conf7 - адреса смещения данных conf битов в hex файле (несуществующие conf должны иметь значение "none")
>
>oscal1-oscal3 - адреса смещения байтов калибровки генератора, аналогично conf
>
>delay - задержка программирования слова/страницы
>
>word - маска слова флеш памяти: 16 бит - FFFF, 14 бит - 3FFF и так далее.

Обратная связь приветствуется. Если у Вас есть модели pic, не поддерживаемые на данный момент, пишите мне: andrejchukov@yandex.ru. Буду пробовать добавлять, а Вы - тестировать.

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

PIC10: PIC10F200, PIC10F202, PIC10F204, PIC10F206
PIC12: PIC12F510
PIC16F: PIC16F505, PIC16F506, PIC16F54, PIC16F57, PIC16F59

# 17.02.2023

PIC16F183xx
Добавлены новые модели без тестирования:
PIC10F32x, PIC16F150x, PIC16F88x

# 29.03.2023

PIC16F170X, PIC16F18XX, PIC16F183XX, PIC16F184XX, PIC18FXXK22, PIC18FXXQ10 (у последней серии некоррекно читается или записывается EEPROM, проверить мне не чем).

# 12.03.2024

 PIC16F871/872/873/874/876/877 без проверки работоспособности. Тестируйте.

# 14.03.2024

PIC16F873A/874A/876A/877A

# 19.03.2024

PIC16F627/628(без А)

# 21.02.2026

PIC18F66K22
