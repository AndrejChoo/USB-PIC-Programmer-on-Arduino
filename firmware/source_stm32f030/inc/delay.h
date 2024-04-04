/*
 * delay.h
 *
 *  Created on: 25 сент. 2019 г.
 *      Author: andre
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f0xx.h"

void delay_init(void);
void _delay_ms(uint32_t);
void _delay_us(uint32_t);

#endif /* DELAY_H_ */


