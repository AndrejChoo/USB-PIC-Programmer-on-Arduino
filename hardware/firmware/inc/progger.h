/*
 * progger.h
 *
 *  Created on: 19 июн. 2020 г.
 *      Author: andre
 */

#ifndef PROGGER_H_
#define PROGGER_H_

#include "usart.h"
#include "gpio.h"
#include "pic.h"
#include <util/delay.h>


void deassemble_frame(void);//Функция разбора пакетов
void send_errorFrame(void); //Функция отаправки сообщения "Ошибка пакета"
void send_successConn(void); //Функция отправки пакета "Программатор присутствует"
void send_frame(uint8_t *buff, uint8_t count); //Функция отправки одного кадра
void send_arr(uint8_t marker, uint8_t *buff, uint16_t count, uint16_t offset);

#endif /* PROGGER_H_ */
