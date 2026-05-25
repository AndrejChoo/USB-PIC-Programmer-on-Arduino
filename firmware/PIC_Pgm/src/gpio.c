/*
 * gpio.c
 *
 *  Created on: 25 но€б. 2022 г.
 *      Author: Andrej
 */

#include "gpio.h"

void gpio_init(void)
{
	PGD_DDR |= (1 << PGD_PIN);
	PGC_DDR |= (1 << PGC_PIN);
	PGM_DDR |= (1 << PGM_PIN);
	VDD_DDR |= (1 << VDD_PIN);
	VPP_DDR |= (1 << VPP_PIN);
	PULL_DDR |= (1 << PULL_PIN);

	VDD_HIGH
}

void pgd_read(void)
{
	PGD_DDR &= ~(1 << PGD_PIN);
}

void pgd_write(void)
{
	PGD_DDR |= (1 << PGD_PIN);
}

