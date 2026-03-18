/*
 * usart.h
 *
 * Created: 20.04.2021 16:40:39
 *  Author: andre
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>

#define CPU_CLOCK		16000000UL
#define BAUDE			57600UL

void usart_init(void);
void USARTSend_char(uint8_t);
void usart_sendString(uint8_t*);


#endif /* USART_H_ */
