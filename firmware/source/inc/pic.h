/*
 * pic.h
 *
 *  Created on: 25 но€б. 2022 г.
 *      Author: Andrej
 */

#ifndef INC_PIC_H_
#define INC_PIC_H_

#include <util/delay.h>
#include "gpio.h"
#include "progger.h"

//PIC16F7X Commands
#define P16F7X_LOAD_CONF		0x00
#define P16F7X_LOAD_DATA		0x02
#define P16F7X_READ_DATA		0x04
#define P16F7X_INC_ADD			0x06
#define P16F7X_BEGIN_PROG		0x08
#define P16F7X_ERASE			0x09
#define P16F7X_END_PROG			0x0E
//PIC16F628A
#define P16F628A_LOAD_CODE		0x02
#define P16F628A_LOAD_DATA		0x03
#define P16F628A_READ_CODE		0x04
#define P16F628A_READ_DATA		0x05
#define P16F628A_INC_ADD		0x06
#define P16F628A_ERASE_CODE		0x09
#define P16F628A_ERASE_DATA		0x0B
//PIC18F2550 and like it
#define P18FX5XX_CORE_INSTR		0x00
#define P18FX5XX_SHIFT_TABL		0x02
#define P18FX5XX_TABLE_RD		0x08
#define P18FX5XX_TABLE_RDPI		0x09
#define P18FX5XX_TABLE_RDPD		0x0A
#define P18FX5XX_TABLE_RDPRI	0x0B
#define P18FX5XX_TABLE_WR		0x0C
#define P18FX5XX_TABLE_WRPI2	0x0D
#define P18FX5XX_START_PRPI2	0x0E
#define P18FX5XX_START_PR		0x0F


//Common
void delay_ms(uint16_t);
void delay_us(uint16_t);

void progMode(void);
void reset(void);

//PIC16F7X
void p16f7x_progMode(void);
void p16f7x_writeComm(uint8_t comm, uint16_t data, uint8_t shrt);
uint16_t p16f7x_readComm(uint8_t comm);
void p16f7x_reset(void);
void p16f7x_getId(void);
void p16f7x_read(void);
void p16f7x_erase(void);
void p16f7x_readCfg(void);
void p16f7x_writeCfg(void);
void p16f7x_progCode(void);

//PIC16F5X
void p16f5x_writeCfg(void);
void p16f5x_readCfg(void);
void p16f5x_erase(void);
void p16f5x_readOscall(void);
void p12f5x_writeOscall(void);

//PIC16F628A
void p16f628A_progMode(void);
void p16f628a_read(void);
void p16f628a_erase(void);
void p16f628a_progCode(void);
void p16f628a_progData(void);

//PIC16F676/630
void p16f676_readOscall(void);
void p16f676_writeOscall(void);
void p16f676_storeOsc(void);
void p16f676_restoreOsc(void);
void p16f676_erase(void);
void p16f676_writeCfg(void);
void p16f676_progCode(void);

//PIC16F84A
void p16f84a_erase(void);
void p16f84a_writeCfg(void);
void p16f84a_progCode(void);

//PIC12F1840
void p12f1840_readOscall(void);
void p12f1840_writeOscall(void);
void p12f1840_writeCfg(void);

//PIC16F183XX
void p16f183xx_setAdd(uint8_t cmd, uint8_t h_add, uint8_t m_add, uint8_t l_add);
void p16f183xx_read(void);
void p16f183xx_progData(void);

//PIC16F184xx
void p16f184xx_writeComm(uint8_t cmd, uint8_t pl, uint8_t h_add, uint8_t m_add, uint8_t l_add);
uint16_t p16f184xx_readComm(uint8_t cmd);
void p16f184xx_getId(void);
void p16f184xx_read(void);
void p16f184xx_erase(void);
void p16f184xx_readCfg(void);
void p16f184xx_writeCfg(void);
void p16f184xx_progCode(void);
void p16f184xx_progData(void);

//PIC16F81x
void p16f81x_erase(void);
void p16f81x_progCode(void);
void p16f81x_progData(void);

//PIC16F87X
void p16f87x_erase(void);

//PIC18F2550
void p18fx5xx_writeComm(uint8_t comm, uint16_t data);
uint8_t p18fx5xx_readComm(uint8_t comm, uint8_t data);
void p18fx5xx_writeNop(void);
void p18fx5xx_setDP(uint8_t h_add, uint8_t m_add, uint8_t l_add);
void p18fx5xx_getId(void);
void p18fx5xx_read(void);
void p18fx5xx_readCfg(void);
void p18fx5xx_writeCfg(void);
void p18fx5xx_erase(void);
void p18fx5xx_progCode(void);
void p18fx5xx_progData(void);


//PIC18FXXK80
void p18fxxk80_password(uint16_t psw0, uint16_t psw1);
void p18fxxk80_read(void);
void p18fxxk80_erase(void);
void p18fxxk80_writeCfg(void);
void p18fxxk80_progCode(void);
void p18fxxk80_progData(void);

//PIC18FXXQ10
void p18fxxq10_progCode(void);

//PIC18FXXJXX
void p18fxxjxx_progMode(void);
void p18fxxjxx_reset(void);

#endif /* INC_PIC_H_ */









