/*
 * delay.c
 *
 *  Created on: 16 ���. 2019 �.
 *      Author: andre
 */

#include "delay.h"

void delay_init()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   //������ ������������ �� TIM2
    TIM2->PSC     = 47;               //��������� �������� ��� ������������ �����������
    TIM2->CR1     = TIM_CR1_OPM;          //����� ������ ��������
}


void _delay_us(uint32_t value)
{
	TIM2->PSC     = 47;               //��������� �������� ��� ������������ �����������
	TIM2->ARR = value;                  //��������� �������� ��������
	TIM2->CNT = 0;
	TIM2->CR1 = TIM_CR1_CEN;	          //��������� ������
	while((TIM2->SR & TIM_SR_UIF)==0){} //��������� ����� ��������
	TIM2->SR &= ~TIM_SR_UIF;	          //�������� ����
}

void _delay_ms(uint32_t value)
{
	TIM2->PSC     = 47000;               //��������� �������� ��� ������������ �����������
	TIM2->ARR = value;                  //��������� �������� ��������
	TIM2->CNT = 0;
	TIM2->CR1 = TIM_CR1_CEN;	          //��������� ������
	while((TIM2->SR & TIM_SR_UIF)==0){} //��������� ����� ��������
	TIM2->SR &= ~TIM_SR_UIF;	          //�������� ����
}

