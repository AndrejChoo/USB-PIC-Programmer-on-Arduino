/*
 * progger.h
 *
 *  Created on: 19 棹�. 2020 �.
 *      Author: andre
 */

#ifndef PROGGER_H_
#define PROGGER_H_

#include "usart.h"
#include "gpio.h"
#include "pic.h"
#include "delay.h"

#define PIC18FX5XX
#define PIC18FXXK80
//#define PIC16F676
#define BASELINE


void deassemble_frame(void);//泽黻鲨� 疣玑铕� 镟赍蝾�
void send_errorFrame(void); //泽黻鲨� 铗囡疣怅� 耦钺龛� "硒栳赅 镟赍蜞"
void send_successConn(void); //泽黻鲨� 铗镳噔觇 镟赍蜞 "橡钽疣祆囹铕 镳桉篁耱怏弪"
void send_frame(uint8_t *buff, uint8_t count); //泽黻鲨� 铗镳噔觇 钿眍泐 赅漯�
void send_arr(uint8_t marker, uint8_t *buff, uint16_t count, uint16_t offset);

#endif /* PROGGER_H_ */
