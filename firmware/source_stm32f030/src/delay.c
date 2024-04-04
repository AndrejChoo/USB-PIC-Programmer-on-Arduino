/*
 * delay.c
 *
 *  Created on: 16 дек. 2019 г.
 *      Author: andre
 */

#include "delay.h"

void delay_init()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   //подать тактирование на TIM2
    TIM2->PSC     = 47;               //настроить делитель для формирования миллисекунд
    TIM2->CR1     = TIM_CR1_OPM;          //режим одного импульса
}


void _delay_us(uint32_t value)
{
	TIM2->PSC     = 47;               //настроить делитель для формирования миллисекунд
	TIM2->ARR = value;                  //загрузить значение задержки
	TIM2->CNT = 0;
	TIM2->CR1 = TIM_CR1_CEN;	          //запустить таймер
	while((TIM2->SR & TIM_SR_UIF)==0){} //дождаться конца задержки
	TIM2->SR &= ~TIM_SR_UIF;	          //сбросить флаг
}

void _delay_ms(uint32_t value)
{
	TIM2->PSC     = 47000;               //настроить делитель для формирования миллисекунд
	TIM2->ARR = value;                  //загрузить значение задержки
	TIM2->CNT = 0;
	TIM2->CR1 = TIM_CR1_CEN;	          //запустить таймер
	while((TIM2->SR & TIM_SR_UIF)==0){} //дождаться конца задержки
	TIM2->SR &= ~TIM_SR_UIF;	          //сбросить флаг
}

