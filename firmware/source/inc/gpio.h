/*
 * gpio.h
 *
 *  Created on: 25 нояб. 2022 г.
 *      Author: Andrej
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <avr/io.h>

//BOARD - версия 2 целиком на плате, SHIELD - версия 1 в виде шилда с преобразователем

#define BOARD
//#define SHIELD
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
	#define PGD_PIN		4
	#define PGD_PORT	PORTB
	#define PGD_DDR		DDRB
	#define PGD_IDR		PINB
	#define PGC_PIN		5
	#define PGC_PORT	PORTB
	#define PGC_DDR		DDRB
	#define PGM_PIN		3
	#define PGM_PORT	PORTB
	#define PGM_DDR		DDRB
	#define VDD_PIN		3
	#define VDD_PORT	PORTD
	#define VDD_DDR		DDRD
	#define VPP_PIN		4
	#define VPP_PORT	PORTD
	#define VPP_DDR		DDRD
	#define PULL_PIN	2
	#define PULL_PORT	PORTB
	#define PULL_DDR	DDRB
#endif

#define PGD_HIGH	PGD_PORT |= (1 << PGD_PIN);
#define PGC_HIGH	PGC_PORT |= (1 << PGC_PIN);
#define PGM_HIGH	PGM_PORT |= (1 << PGM_PIN);
#define VDD_HIGH	VDD_PORT |= (1 << VDD_PIN);
#define VPP_HIGH	VPP_PORT |= (1 << VPP_PIN);
#define PULL_HIGH	PULL_PORT |= (1 << PULL_PIN);
#define PGD_LOW		PGD_PORT &= ~(1 << PGD_PIN);
#define PGC_LOW		PGC_PORT &= ~(1 << PGC_PIN);
#define PGM_LOW		PGM_PORT &= ~(1 << PGM_PIN);
#define VDD_LOW		VDD_PORT &= ~(1 << VDD_PIN);
#define VPP_LOW		VPP_PORT &= ~(1 << VPP_PIN);
#define PULL_LOW	PULL_PORT &= ~(1 << PULL_PIN);

#define PINS_IDDLE		{PGD_DDR &= ~(1 << PGD_PIN); PGC_DDR &= ~(1 << PGC_PIN);}
#define PINS_ACTIVE		{PGD_DDR |= (1 << PGD_PIN); PGC_DDR |= (1 << PGC_PIN);}


void gpio_init(void);
void pgd_read(void);
void pgd_write(void);



#endif /* INC_GPIO_H_ */
