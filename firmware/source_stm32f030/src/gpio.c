/*
 * gpio.c
 *
 *  Created on: 25 но€б. 2022 г.
 *      Author: Andrej
 */

#include "gpio.h"

void gpio_init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitTypeDef pin = {0};

    pin.GPIO_Mode = GPIO_Mode_OUT;
    pin.GPIO_OType = GPIO_OType_PP;
    pin.GPIO_Speed = GPIO_Speed_50MHz;
    pin.GPIO_Pin = PGD_PIN;
    GPIO_Init(PGD_PORT, &pin);

    pin.GPIO_Pin = PGC_PIN;
    GPIO_Init(PGC_PORT, &pin);

    pin.GPIO_Pin = VDD_PIN;
    GPIO_Init(VDD_PORT, &pin);

    pin.GPIO_Pin = PGM_PIN;
    GPIO_Init(PGM_PORT, &pin);

    pin.GPIO_Pin = VPP_PIN;
    GPIO_Init(VPP_PORT, &pin);

    pin.GPIO_Pin = PULL_PIN;
    GPIO_Init(PULL_PORT, &pin);

	VDD_HIGH
}

void pgd_read(void)
{
	GPIOA->MODER &= ~(GPIO_MODER_MODER6);
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR6);
}

void pgd_write(void)
{
	GPIOA->MODER &= ~(GPIO_MODER_MODER6);
	GPIOA->MODER |= GPIO_MODER_MODER6_0;
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_6);
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6);
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_1;
}

