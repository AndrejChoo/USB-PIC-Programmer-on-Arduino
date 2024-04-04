/*
 * gpio.h
 *
 *  Created on: 25 нояб. 2022 г.
 *      Author: Andrej
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "stm32f0xx.h"

//BOARD - версия 2 целиком на плате, SHIELD - версия 1 в виде шилда с преобразователем

//#define BOARD
#define SHIELD
/*
 * Shield connectiond
 * PGD - D12(PB4), PGC - D13(PB5), PGM - D11(PB3), VDD - D3(PD3), VPP - D4(PB4)
 * Board connections
 * PGD - PB1, PGC - PB2, PGM - PC0, VDD - PD7, VPP - PD6
 */
#ifdef BOARD
	#define PGD_PIN		1
	#define PGD_PORT	PORTB
	#define PGD_DDR		DDRB
	#define PGD_IDR		PINB
	#define PGC_PIN		2
	#define PGC_PORT	PORTB
	#define PGC_DDR		DDRB
	#define PGM_PIN		0
	#define PGM_PORT	PORTC
	#define PGM_DDR		DDRC
	#define VDD_PIN		7
	#define VDD_PORT	PORTD
	#define VDD_DDR		DDRD
	#define VPP_PIN		6
	#define VPP_PORT	PORTD
	#define VPP_DDR		DDRD
	#define PULL_PIN	4
	#define PULL_PORT	PORTC
	#define PULL_DDR	DDRC
#endif

#ifdef SHIELD
	#define PGD_PIN		GPIO_Pin_6
	#define PGD_PORT	GPIOA
	#define PGD_IDR		(PGD_PORT->IDR & PGD_PIN)

	#define PGC_PIN		GPIO_Pin_5
	#define PGC_PORT	GPIOA

	#define PGM_PIN		GPIO_Pin_7
	#define PGM_PORT	GPIOA

	#define VDD_PIN		GPIO_Pin_2
	#define VDD_PORT	GPIOA

	#define VPP_PIN		GPIO_Pin_3
	#define VPP_PORT	GPIOA

	#define PULL_PIN	GPIO_Pin_4
	#define PULL_PORT	GPIOA
#endif

#define PGD_HIGH	GPIO_SetBits(PGD_PORT, PGD_PIN);
#define PGC_HIGH	GPIO_SetBits(PGC_PORT, PGC_PIN);
#define PGM_HIGH	GPIO_SetBits(PGM_PORT, PGM_PIN);
#define VDD_HIGH	GPIO_SetBits(VDD_PORT, VDD_PIN);
#define VPP_HIGH	GPIO_SetBits(VPP_PORT, VPP_PIN);
#define PULL_HIGH	GPIO_SetBits(PULL_PORT, PULL_PIN);
#define PGD_LOW		GPIO_ResetBits(PGD_PORT, PGD_PIN);
#define PGC_LOW		GPIO_ResetBits(PGC_PORT, PGC_PIN);
#define PGM_LOW		GPIO_ResetBits(PGM_PORT, PGM_PIN);
#define VDD_LOW		GPIO_ResetBits(VDD_PORT, VDD_PIN);
#define VPP_LOW		GPIO_ResetBits(VPP_PORT, VPP_PIN);
#define PULL_LOW	GPIO_ResetBits(PULL_PORT, PULL_PIN);

#define PINS_IDDLE		{GPIOA->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6); \
							GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6); }

#define PINS_ACTIVE		{	GPIOA->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6); \
							GPIOA->MODER |= GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0; \
							GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_6); \
							GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6); \
							GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1 | GPIO_OSPEEDER_OSPEEDR6_1; }


void gpio_init(void);
void pgd_read(void);
void pgd_write(void);



#endif /* INC_GPIO_H_ */
