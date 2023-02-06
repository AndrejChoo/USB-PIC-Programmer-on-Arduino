/*
 * progger.h
 *
 *  Created on: 19 ���. 2020 �.
 *      Author: andre
 */

#ifndef PROGGER_H_
#define PROGGER_H_

#include "usart.h"
#include "gpio.h"
#include "pic.h"
#include <util/delay.h>


void deassemble_frame(void);//������� ������� �������
void send_errorFrame(void); //������� ��������� ��������� "������ ������"
void send_successConn(void); //������� �������� ������ "������������ ������������"
void send_frame(uint8_t *buff, uint8_t count); //������� �������� ������ �����
void send_arr(uint8_t marker, uint8_t *buff, uint16_t count, uint16_t offset);

#endif /* PROGGER_H_ */
