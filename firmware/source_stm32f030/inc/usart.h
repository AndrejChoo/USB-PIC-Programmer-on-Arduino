/*
 * usart.h
 *
 *  Created on: 19 θών. 2020 γ.
 *      Author: andre
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f0xx.h"


void usart1_init(void);
void USARTSend_string(const unsigned char*);
void USARTSend_char(unsigned char);

#endif /* USART_H_ */
