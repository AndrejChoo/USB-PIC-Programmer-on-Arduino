/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "usart.h"
#include "gpio.h"
#include "delay.h"
#include "progger.h"


uint8_t exchange[150];
uint8_t rx_counter = 0;
uint8_t full_buff = 0; //Слаг, указывающий, что при™мный буффер заполнен
uint8_t packet_size = 0;
uint8_t main_buff[256];
volatile uint8_t read_bsy = 0;


void USART1_IRQHandler (void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        exchange[rx_counter++] = USART1->RDR;
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
	usart1_init();
	gpio_init();
	delay_init();

	PGD_LOW
	PGC_LOW
	PGM_LOW
	VDD_LOW
	VPP_LOW

	while(1)
	{
        if(full_buff)
        {
            deassemble_frame();
            full_buff = 0;
        }
        asm("nop");
	}
}
