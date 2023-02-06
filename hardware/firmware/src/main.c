/*
 * main.c
 *
 *  Created on: 25 но€б. 2022 г.
 *      Author: Andrej
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "gpio.h"
#include "progger.h"

uint8_t exchange[150];
uint8_t rx_counter = 0;
uint8_t full_buff = 0; //‘лаг, указывающий, что приЄмный буффер заполнен
uint8_t packet_size = 0;
uint8_t main_buff[256];
uint8_t read_bsy = 0;


ISR(USART_RX_vect)
{
	if(!(UCSR0A & ((1 << FE0) | (1 << DOR0) | (1 << UPE0))))
	{
		exchange[rx_counter] = UDR0;
		rx_counter++;
		if(rx_counter == 3) {packet_size = exchange[1];}
		if((packet_size > 0) && (rx_counter == packet_size + 3))
		{
			rx_counter = 0;
			packet_size = 0;
			if(exchange[2] == 0x3D) read_bsy = 1;
			else full_buff = 1;
		}
	}
}


int main(void)
{

	usart_init();
	gpio_init();

	PGD_LOW
	PGC_LOW
	PGM_LOW
	VDD_LOW
	VPP_LOW

	sei();


	while(1)
	{
		if(full_buff)
		{
			deassemble_frame();
			full_buff = 0;
		}
		asm(" ");
	}
}


