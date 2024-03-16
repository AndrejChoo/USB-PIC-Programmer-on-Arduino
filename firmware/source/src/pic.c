/*
 * pic.c
 *
 *  Created on: 25 íîÿá. 2022 ã.
 *      Author: Andrej
 */


#include "pic.h"

extern uint8_t main_buff[];
extern uint8_t exchange[];
extern uint32_t f_size;
extern uint16_t f_psize;
extern uint16_t ee_size;
extern uint8_t osc_start;
extern uint8_t osc_cnt;
extern uint8_t conf_cnt;
extern uint8_t device;
extern uint16_t wr_delay;
extern uint32_t main_counter;
extern volatile uint8_t read_bsy;

//Oscal
uint16_t oscal1, oscal2, oscal3;


void delay_ms(uint16_t del)
{
	for(int d = 0; d < del; d++) _delay_ms(1);
}

void delay_us(uint16_t del)
{
	for(int d = 0; d < del; d++) _delay_us(1);
}


void p16f7x_progMode(void)
{
	pgd_write();
	  PGM_LOW
	  PGC_LOW
	  PGD_LOW
	  _delay_us(50);
	  VDD_HIGH
	  _delay_us(50);
	  VPP_HIGH
	  _delay_ms(1);
}

void p16f628A_progMode(void)
{
	pgd_write();
	  PGM_LOW
	  PGC_LOW
	  PGD_LOW
	  _delay_us(50);
	  VPP_HIGH
	  _delay_us(50);
	  VDD_HIGH
	  _delay_ms(2);
}

void progMode(void)
{
	switch(device)
	{
		case 0x02: {p16f628A_progMode(); break;}
		case 0x06: {p16f628A_progMode(); break;}
#ifdef PIC18FXXK80
		case 0x05: {p16f7x_progMode(); p18fxxk80_password(0x4D43, 0x4850); break;}
#endif
		case 0x0D: {p18fxxjxx_progMode(); break;}
		default: {p16f7x_progMode(); break;}
	}
}

void reset(void)
{
	switch(device)
	{
		case 0x0D: {p18fxxjxx_reset(); break;}
		default: {p16f7x_reset(); break;}
	}
}

void p18fxxjxx_progMode(void)
{
	VPP_LOW
	pgd_write();
	 PGM_LOW
	 PGC_LOW
	 PGD_LOW
	 _delay_us(20);
	 VDD_HIGH
	 _delay_us(20);
	 PGM_HIGH
	 _delay_us(20);
	 PGM_LOW
	 _delay_us(20);
	 p18fxxk80_password(0x4D43, 0x4850);
	 _delay_us(20);
	 PGM_HIGH
}

void p16f7x_writeComm(uint8_t comm, uint16_t data, uint8_t shrt)
{
	pgd_write();
	PGD_LOW
	_delay_us(2);
	for(int i = 0; i < 6; i++)
	{
		_delay_us(2);
		if(comm & (1 << i)) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	_delay_us(1);
	PGD_LOW
	_delay_us(2);

	if(shrt) {_delay_us(10); return;}

	data = data << 1;

	for(int i = 0; i < 16; i++)
	{
		_delay_us(2);
		if(data & (1 << i)) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	_delay_us(7);
}

void p16f183xx_setAdd(uint8_t cmd, uint8_t h_add, uint8_t m_add, uint8_t l_add)
{
	pgd_write();
	PGD_LOW
	_delay_us(2);
	for(int i = 0; i < 6; i++)
	{
		_delay_us(2);
		if(cmd & (1 << i)) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	_delay_us(5);
	PGD_LOW
	_delay_us(2);
	PGC_HIGH
	_delay_us(2);
	PGC_LOW

	for(int i = 0; i < 8; i++)
	{
		_delay_us(2);
		if(l_add & (1 << i)) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	for(int i = 0; i < 8; i++)
	{
		_delay_us(2);
		if(m_add & (1 << i)) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	for(int i = 0; i < 7; i++)
	{
		_delay_us(2);
		if(h_add & (1 << i)) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	_delay_us(5);
}

void p16f184xx_writeComm(uint8_t cmd, uint8_t pl, uint8_t h_add, uint8_t m_add, uint8_t l_add)
{
	pgd_write();
	PGD_LOW

	for(int i = 0; i < 8; i++)
	{
		_delay_us(2);
		if(cmd & (1 << (7-i))) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	_delay_us(2);
	PGD_LOW
	_delay_us(7);

	if(pl) return;

	//Start bit
	PGC_HIGH
	_delay_us(2);
	PGC_LOW


	for(int i = 0; i < 6; i++)
	{
		_delay_us(2);
		if(h_add & (1 << (5-i))) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	for(int i = 0; i < 8; i++)
	{
		_delay_us(2);
		if(m_add & (1 << (7-i))) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	for(int i = 0; i < 8; i++)
	{
		_delay_us(2);
		if(l_add & (1 << (7-i))) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	//Stop bit
	_delay_us(2);
	PGD_LOW
	PGC_HIGH
	_delay_us(2);
	PGC_LOW
	_delay_us(8);
}

uint16_t p16f184xx_readComm(uint8_t cmd)
{
	uint16_t dat = 0;

	pgd_write();
	PGD_LOW

	for(int i = 0; i < 8; i++)
	{
		_delay_us(2);
		if(cmd & (1 << (7-i))) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}
	_delay_us(2);

	pgd_read();
	_delay_us(8);


	for(int i = 0; i < 7; i++)
	{
		_delay_us(5);
		PGC_HIGH
		_delay_us(5);
		PGC_LOW
	}
	for(int i = 0; i < 16; i++)
	{
		_delay_us(5);
		PGC_HIGH
		_delay_us(5);
		PGC_LOW
		if(PGD_IDR & (1 << PGD_PIN)) dat |= (1 << (15-i));
	}
	_delay_us(5);
	PGC_HIGH
	_delay_us(5);
	PGC_LOW
	_delay_us(7);
	pgd_write();

	return dat;
}

uint16_t p16f7x_readComm(uint8_t comm)
{
	uint16_t data = 0;
	pgd_write();
	_delay_us(5);
	for(int i = 0; i < 6; i++)
	{
		_delay_us(2);
		if(comm & (1 << i)) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
	}

	_delay_us(2);
	PGD_LOW
	_delay_us(2);
	PGC_HIGH
	_delay_us(2);
	PGC_LOW
	_delay_us(7);
	pgd_read();
	for(int i = 0; i < 14; i++)
	{
		_delay_us(2);
		PGC_HIGH
		_delay_us(2);
		PGC_LOW
		if(PGD_IDR & (1 << PGD_PIN)) data |= (1 << i);
	}
	_delay_us(2);
	pgd_write();
	PGD_LOW
	PGC_HIGH
	_delay_us(2);
	PGC_LOW

	_delay_us(7);
	return data;
}

void p18fx5xx_writeComm(uint8_t comm, uint16_t data)
{
	pgd_write();
	PGD_LOW
	_delay_us(10);
	for(int i = 0; i < 4; i++)
	{
		_delay_us(5);
		if(comm & (1 << i)) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(5);
		PGC_LOW
	}
	_delay_us(5);
	PGD_LOW
	_delay_us(5);

	for(int i = 0; i < 16; i++)
	{
		_delay_us(5);
		if(data & (1 << i)) PGD_HIGH
		else PGD_LOW
		PGC_HIGH
		_delay_us(5);
		PGC_LOW
	}
	PGD_LOW
	_delay_us(10);
}

void p18fx5xx_writeNop(void)
{
	pgd_write();
		PGC_LOW
		PGD_LOW
		_delay_us(5);
		for(int i = 0; i < 3; i++)
		{
			_delay_us(5);
			PGC_HIGH
			_delay_us(5);
			PGC_LOW
		}
		_delay_us(5);
		PGC_HIGH
		_delay_ms(2);
		PGC_LOW
		_delay_us(100);
}

void p18fxxk80_password(uint16_t psw0, uint16_t psw1)
{
	//uint16_t psw0 = 0x4D43;
	//uint16_t psw1 = 0x4850;

	uint16_t pass[] = {psw0, psw1};

	pgd_write();
	_delay_us(20);
	for(int k = 0; k < 2; k++)
	{
		for(int i = 0; i < 16; i++)
		{
			_delay_us(10);
			if(pass[k] & (1 << (15 - i))) PGD_HIGH
			else PGD_LOW
			PGC_HIGH
			_delay_us(10);
			PGC_LOW
		}
	}
	_delay_us(20);
}

uint8_t p18fx5xx_readComm(uint8_t comm, uint8_t data)
{
	uint8_t result = 0;

	pgd_write();
		PGD_LOW
		_delay_us(10);
		for(int i = 0; i < 4; i++)
		{
			_delay_us(5);
			if(comm & (1 << i)) PGD_HIGH
			else PGD_LOW
			PGC_HIGH
			_delay_us(5);
			PGC_LOW
		}
		_delay_us(5);
		PGD_LOW
		_delay_us(5);

		for(int i = 0; i < 8; i++)
		{
			_delay_us(5);
			if(data & (1 << i)) PGD_HIGH
			else PGD_LOW
			PGC_HIGH
			_delay_us(5);
			PGC_LOW
		}

		pgd_read();
		_delay_us(5);

		for(int i = 0; i < 8; i++)
		{
			_delay_us(5);
			PGC_HIGH
			_delay_us(5);
			PGC_LOW
			if(PGD_IDR & (1 << PGD_PIN)) result |= (1 << i);
		}
		pgd_write();
		PGD_LOW
		_delay_us(10);
		return result;
}


//Óñòàíîâèòü ñ÷¸ò÷èê êîììàíä íà àäðåñ
void p18fx5xx_setDP(uint8_t h_add, uint8_t m_add, uint8_t l_add)
{
	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | h_add));
	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EF8);
	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | m_add));
	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EF7);
	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | l_add));
	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EF6);
}


void p16f7x_reset(void)
{
	  VPP_LOW
	  VDD_LOW
	  PGM_LOW
	  PGC_LOW
	  PGD_LOW
	  _delay_us(200);
}

void p18fxxjxx_reset(void)
{
	  PGM_LOW
	  VDD_LOW
	  PGM_LOW
	  PGC_LOW
	  PGD_LOW
	  _delay_us(200);
}

void p16f7x_getId(void)
{
	uint16_t id;

	progMode();

	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
	for(int i = 0; i < 6; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	id = p16f7x_readComm(P16F7X_READ_DATA);
	reset();

	main_buff[0] = 0xFE;
	main_buff[1] = 0x02;
	main_buff[2] = 0xA0;
	main_buff[3] = (id >> 8);
	main_buff[4] = (id & 0xFF);
	send_frame(main_buff, 5);
}

void p16f184xx_getId(void)
{
	uint16_t id;

	progMode();
	if(device == 0x12) p16f184xx_writeComm(0x80, 0, 0x00, 0x80, 0x06); //P16F184xx
	if(device == 0x13) p16f184xx_writeComm(0x80, 0, 0x3F, 0xFF, 0xFE); //P18F2xQ10/4xQ10
	id = p16f184xx_readComm(0xFC);
	reset();

	main_buff[0] = 0xFE;
	main_buff[1] = 0x02;
	main_buff[2] = 0xA0;
	main_buff[3] = (id >> 8);
	main_buff[4] = (id & 0xFF);
	send_frame(main_buff, 5);
}

void p18fx5xx_getId(void)
{
	progMode();

	p18fx5xx_setDP(0x3F, 0xFF, 0xFE);
	main_buff[4] = p18fx5xx_readComm(P18FX5XX_TABLE_RDPI, 0x00);
	p18fx5xx_setDP(0x3F, 0xFF, 0xFF);
	main_buff[3] = p18fx5xx_readComm(P18FX5XX_TABLE_RDPI, 0x00);
	reset();

	_delay_ms(5);

	main_buff[0] = 0xFE;
	main_buff[1] = 0x02;
	main_buff[2] = 0xA0;
	send_frame(main_buff, 5);
}


void p16f7x_readCfg(void)
{
	uint16_t cfg[7];

	progMode();

	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
	for(int i = 0; i < 7; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);

	for(int k = 0; k < conf_cnt; k++)
	{
		cfg[k] = p16f7x_readComm(P16F7X_READ_DATA);
		p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	}
	reset();

	main_buff[0] = 0xFE;
	main_buff[1] = 0x0E;
	main_buff[2] = 0xA3;
	main_buff[3] = (cfg[0] >> 8);
	main_buff[4] = (cfg[0] & 0xFF);
	main_buff[5] = (cfg[1] >> 8);
	main_buff[6] = (cfg[1] & 0xFF);
	main_buff[7] = (cfg[2] >> 8);
	main_buff[8] = (cfg[2] & 0xFF);
	main_buff[9] = (cfg[3] >> 8);
	main_buff[10] = (cfg[3] & 0xFF);
	main_buff[11] = (cfg[4] >> 8);
	main_buff[12] = (cfg[4] & 0xFF);
	main_buff[13] = (cfg[5] >> 8);
	main_buff[14] = (cfg[5] & 0xFF);
	main_buff[15] = (cfg[6] >> 8);
	main_buff[16] = (cfg[6] & 0xFF);
	send_frame(main_buff, 17);
}

void p16f184xx_readCfg(void)
{
	uint16_t cfg[7];

	progMode();
	if(device == 0x12) p16f184xx_writeComm(0x80, 0, 0x00, 0x80, 0x07);
	if(device == 0x13) p16f184xx_writeComm(0x80, 0, 0x30, 0x00, 0x00);

	for(int k = 0; k < conf_cnt; k++)
		{
			cfg[k] = p16f184xx_readComm(0xFC);
			p16f184xx_writeComm(0xF8, 1, 0x00, 0x00, 0x00);
		}
	reset();

	main_buff[0] = 0xFE;
	main_buff[1] = 0x0E;
	main_buff[2] = 0xA3;
	main_buff[3] = (cfg[0] >> 8);
	main_buff[4] = (cfg[0] & 0xFF);
	main_buff[5] = (cfg[1] >> 8);
	main_buff[6] = (cfg[1] & 0xFF);
	main_buff[7] = (cfg[2] >> 8);
	main_buff[8] = (cfg[2] & 0xFF);
	main_buff[9] = (cfg[3] >> 8);
	main_buff[10] = (cfg[3] & 0xFF);
	main_buff[11] = (cfg[4] >> 8);
	main_buff[12] = (cfg[4] & 0xFF);
	main_buff[13] = (cfg[5] >> 8);
	main_buff[14] = (cfg[5] & 0xFF);
	main_buff[15] = (cfg[6] >> 8);
	main_buff[16] = (cfg[6] & 0xFF);
	send_frame(main_buff, 17);
}

#ifdef BASELINE
void p16f5x_readCfg(void)
{
	uint16_t cfg;

	progMode();
	cfg = p16f7x_readComm(P16F7X_READ_DATA);
	reset();

	main_buff[0] = 0xFE;
	main_buff[1] = 0x02;
	main_buff[2] = 0xA3;
	main_buff[3] = (cfg >> 8);
	main_buff[4] = (cfg & 0xFF);
	send_frame(main_buff, 5);
}
#endif

#ifdef PIC18FX5XX
void p18fx5xx_readCfg(void)
{
	main_buff[0] = 0xFE;
	main_buff[1] = 0x0E;
	main_buff[2] = 0xA3;
	for(int i = 0; i < 14; i+= 2)
	{
		progMode();

		p18fx5xx_setDP(0x30, 0x00, i);
		main_buff[i + 4] = p18fx5xx_readComm(P18FX5XX_TABLE_RD, 0x00);
		p18fx5xx_setDP(0x30, 0x00, i + 1);
		main_buff[i + 3] = p18fx5xx_readComm(P18FX5XX_TABLE_RD, 0x00);

		reset();
		_delay_ms(5);
	}

	send_frame(main_buff, 17);
}
#endif

void p16f7x_writeCfg(void)
{
	progMode();

	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
	for(int i = 0; i < 7; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	p16f7x_writeComm(P16F7X_LOAD_DATA, (exchange[4] | (exchange[5] <<8)), 0);
	if(device == 0x14) p16f7x_writeComm(0x18, 0x0000, 1);
	else p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
	_delay_ms(5);
	p16f7x_writeComm(P16F7X_END_PROG, 0x0000, 1);
	_delay_ms(5);
	reset();
}

void p16f184xx_writeCfg(void)
{
	progMode();
	if(device == 0x12) p16f184xx_writeComm(0x80, 0, 0x00, 0x80, 0x07);
	if(device == 0x13) p16f184xx_writeComm(0x80, 0, 0x30, 0x00, 0x00);

	for(int k = 0; k < conf_cnt*2; k+=2)
	{
		if(device == 0x12)
		{
			p16f184xx_writeComm(0x00, 0, 0x00, exchange[5 + k], exchange[4 + k]);
			p16f184xx_writeComm(0xE0, 1, 0x00, 0x00, 0x00);
		}
		if(device == 0x13)
		{
			p16f184xx_writeComm(0xC0, 0, 0x00, exchange[5 + k], exchange[4 + k]);
		}
		_delay_ms(30);
		p16f184xx_writeComm(0xF8, 1, 0x00, 0x00, 0x00);
	}
	reset();
}

#ifdef BASELINE
void p16f5x_writeCfg(void)
{
	progMode();
	p16f7x_writeComm(P16F7X_LOAD_DATA, (exchange[4] | (exchange[5] <<8)), 0);
	p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
	_delay_ms(5);
	p16f7x_writeComm(P16F7X_END_PROG, 0x0000, 1);
	_delay_us(100);
	reset();
}
#endif

void p12f1840_writeCfg(void)
{
	progMode();

	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
	for(int c = 0; c < 7; c++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);

	for(int k = 0; k < conf_cnt*2; k+=2)
	{
		p16f7x_writeComm(P16F7X_LOAD_DATA, (exchange[4 + k] | (exchange[5 + k] <<8)), 0);
		p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
		_delay_ms(5);
		p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	}
	reset();
}

void p16f84a_writeCfg(void)
{
	progMode();

	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
	for(int i = 0; i < 7; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	p16f7x_writeComm(P16F7X_LOAD_DATA, (exchange[4] | (exchange[5] <<8)), 0);
	p16f7x_writeComm(0x18, 0x0000, 1);
	_delay_ms(5);
	reset();
}

#ifdef PIC16F676
void p16f676_writeCfg(void)
{
	uint16_t cfg = exchange[4] | (exchange[5] << 8);
	p16f676_storeOsc();

	cfg &= 0x0FFF;
	cfg |= oscal2;

	progMode();
	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x2000, 0);
	for(int i = 0; i < 7; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	p16f7x_writeComm(P16F7X_LOAD_DATA, cfg, 0);
	p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
	_delay_ms(5);
	p16f7x_writeComm(0x0A, 0x0000, 1);
	reset();
}
#endif

#ifdef PIC18FX5XX
void p18fx5xx_writeCfg(void)
{
	for(int i = 0; i < 14; i++)
	{
		progMode();

		p18fx5xx_setDP(0x30, 0x00, (i));
		p18fx5xx_writeComm(P18FX5XX_START_PR, (exchange[i + 4] | (exchange[i + 4] << 8)));
		p18fx5xx_writeNop();
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
		_delay_ms(50);
		reset();
		_delay_ms(50);
	}
}
#endif

#ifdef PIC18FXXK80
void p18fxxk80_writeCfg(void)
{
	for(int i = 0; i < 14; i++)
	{
		progMode();

		//Access to code memory
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x8E7F);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9C7F);

		p18fx5xx_setDP(0x30, 0x00, (i));
		p18fx5xx_writeComm(P18FX5XX_START_PR, (exchange[i + 4] | (exchange[i + 4] << 8)));
		p18fx5xx_writeNop();
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
		_delay_ms(50);
		reset();
		_delay_ms(50);
	}
}
#endif

void p16f7x_read(void)
{
	uint16_t data;

	progMode();

	if(device == 0x09 || device == 0x0A || device == 0x0B || device == 0x0C) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);

	for(uint32_t b = 0; b < f_size; b+= 128)
	{
		for(int k = 0; k < 128; k+= 2)
		{
			data = p16f7x_readComm(P16F7X_READ_DATA);
			main_buff[k + 3] = (data >> 8);
			main_buff[k + 4] = (data & 0xFF);
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		}
		main_buff[0] = 0xFE;
		main_buff[1] = 0x80;
		main_buff[2] = 0xA1;
		send_frame(main_buff, 131);

		while(!read_bsy);
		read_bsy = 0;
	}
	reset();
}

void p16f183xx_read(void)
{
	uint16_t data;

	progMode();

	for(uint32_t y = 0; y < f_size; y+= 128)
	{
		for(int k = 0; k < 128; k+= 2)
		{
			data = p16f7x_readComm(0x24); //READ POST INCREMENT
			main_buff[k + 3] = (data >> 8);
			main_buff[k + 4] = (data & 0xFF);
		}
		main_buff[0] = 0xFE;
		main_buff[1] = 0x80;
		main_buff[2] = 0xA1;
		send_frame(main_buff, 131);

		while(!read_bsy);
		read_bsy = 0;
	}
	reset();

	progMode();

	p16f183xx_setAdd(0x1D, 0x00, 0xF0, 0x00); //Load ADD to 0xF000

	for(int i = 0; i < ee_size; i+= 64)
	{
		for(int k = 0; k < 64; k++)
		{
			main_buff[k + 3] = (p16f7x_readComm(0x24) & 0xFF);
		}
		main_buff[0] = 0xFE;
		main_buff[1] = 0x40;
		main_buff[2] = 0xA9;
		send_frame(main_buff, 67);

		while(!read_bsy);
		read_bsy = 0;
	}
	reset();
}

void p16f184xx_read(void)
{
	uint16_t data;

		progMode();

		for(uint32_t y = 0; y < f_size; y+= 128)
		{
			for(int k = 0; k < 128; k+= 2)
			{
				data = p16f184xx_readComm(0xFE); //READ POST INCREMENT
				main_buff[k + 3] = (data >> 8);
				main_buff[k + 4] = (data & 0xFF);
			}
			main_buff[0] = 0xFE;
			main_buff[1] = 0x80;
			main_buff[2] = 0xA1;
			send_frame(main_buff, 131);

			while(!read_bsy);
			read_bsy = 0;
		}
		reset();

		progMode();

		if(device == 0x12) p16f184xx_writeComm(0x80, 0, 0x00, 0xF0, 0x00); //Load ADD to 0xF000
		if(device == 0x13) p16f184xx_writeComm(0x80, 0, 0x31, 0x00, 0x00);

		for(int i = 0; i < ee_size; i+= 64)
		{
			for(int k = 0; k < 64; k++)
			{
				main_buff[k + 3] = (p16f184xx_readComm(0xFE) & 0xFF);
			}
			main_buff[0] = 0xFE;
			main_buff[1] = 0x40;
			main_buff[2] = 0xA9;
			send_frame(main_buff, 67);

			while(!read_bsy);
			read_bsy = 0;
		}
		reset();
}

void p16f628a_read(void)
{
	uint16_t data;

	progMode();

	for(uint32_t m = 0; m < f_size; m+= 128)
	{
		for(int k = 0; k < 128; k+= 2)
		{
			data = p16f7x_readComm(P16F628A_READ_CODE);
			main_buff[k + 3] = (data >> 8);
			main_buff[k + 4] = (data & 0xFF);
			p16f7x_writeComm(P16F628A_INC_ADD, 0x0000, 1);
		}
		main_buff[0] = 0xFE;
		main_buff[1] = 0x80;
		main_buff[2] = 0xA1;
		send_frame(main_buff, 131);

		while(!read_bsy);
		read_bsy = 0;
	}
	reset();

	_delay_ms(50);

	progMode();

	for(int i = 0; i < ee_size; i+= 64)
	{
		for(int k = 0; k < 64; k++)
		{
			main_buff[k + 3] = (p16f7x_readComm(P16F628A_READ_DATA) & 0xFF);
			p16f7x_writeComm(P16F628A_INC_ADD, 0x0000, 1);
		}
		main_buff[0] = 0xFE;
		main_buff[1] = 0x40;
		main_buff[2] = 0xA9;
		send_frame(main_buff, 67);

		while(!read_bsy);
		read_bsy = 0;
	}
	reset();
}


void p18fx5xx_read(void)
{
	progMode();

	p18fx5xx_setDP(0x00, 0x00, 0x00);

	for(uint32_t m = 0; m < f_size; m+= 128)
	{
		for(int k = 0; k < 128; k+= 2)
		{
			main_buff[k + 4] = p18fx5xx_readComm(P18FX5XX_TABLE_RDPI, 0x00);
			main_buff[k + 3] = p18fx5xx_readComm(P18FX5XX_TABLE_RDPI, 0x00);
		}
		main_buff[0] = 0xFE;
		main_buff[1] = 0x80;
		main_buff[2] = 0xA1;
		send_frame(main_buff, 131);

		while(!read_bsy);
		read_bsy = 0;
	}
	reset();

	if(ee_size > 0)
	{
		progMode();

		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9EA6);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9CA6);
		for(int i = 0; i < ee_size; i+= 64)
		{
			for(int k = 0; k < 64; k++)
			{
				uint16_t c_add = k + i;
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | (c_add & 0xFF)));
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EA9);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | (c_add >> 8)));
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EAA);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x80A6);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x50A8);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EF5);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
				p18fx5xx_readComm(P18FX5XX_SHIFT_TABL, 0x00);
				main_buff[k + 3] = p18fx5xx_readComm(P18FX5XX_SHIFT_TABL, 0x00);
			}
			main_buff[0] = 0xFE;
			main_buff[1] = 0x40;
			main_buff[2] = 0xA9;
			send_frame(main_buff, 67);

			while(!read_bsy);
			read_bsy = 0;
		}
		reset();
	}
}


#ifdef PIC18FXXK80
void p18fxxk80_read(void)
{
	progMode();

		p18fx5xx_setDP(0x00, 0x00, 0x00);

		for(uint32_t m = 0; m < f_size; m+= 128)
		{
			for(int k = 0; k < 128; k+= 2)
			{
				main_buff[k + 4] = p18fx5xx_readComm(P18FX5XX_TABLE_RDPI, 0x00);
				main_buff[k + 3] = p18fx5xx_readComm(P18FX5XX_TABLE_RDPI, 0x00);
			}
			main_buff[0] = 0xFE;
			main_buff[1] = 0x80;
			main_buff[2] = 0xA1;
			send_frame(main_buff, 131);

			while(!read_bsy);
			read_bsy = 0;
		}
		reset();

	_delay_ms(10);

	if(ee_size > 0)
	{
		for(int i = 0; i < ee_size; i+= 64)
		{
			for(int k = 0; k < 64; k++)
			{
				progMode();
				uint16_t c_add = k + i;

				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9E7F);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9C7F);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | (c_add & 0xFF)));
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6E74);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | (c_add >> 8)));
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6E75);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x807F);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x5073);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EF5);
				p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
				main_buff[k + 3] = p18fx5xx_readComm(P18FX5XX_SHIFT_TABL, 0x00);
				p18fx5xx_readComm(P18FX5XX_SHIFT_TABL, 0x00);
				reset();
			}
			main_buff[0] = 0xFE;
			main_buff[1] = 0x40;
			main_buff[2] = 0xA9;
			send_frame(main_buff, 67);

			while(!read_bsy);
			read_bsy = 0;
		}
	}
}
#endif

void p16f7x_erase(void)
{
	progMode();

	if(device == 0x0E) p16f183xx_setAdd(0x1D, 0x00, 0xE8, 0x00);

	p16f7x_writeComm(P16F7X_ERASE, 0x0000, 1);
	_delay_ms(200);
	reset();
}

void p16f184xx_erase(void)
{
	progMode();
	if(device == 0x12) p16f184xx_writeComm(0x80, 0, 0x00, 0x80, 0x00);
	if(device == 0x13) p16f184xx_writeComm(0x80, 0, 0x20, 0x00, 0x00);
	p16f184xx_writeComm(0x18, 1, 0x00, 0x00, 0x00);
	_delay_ms(50);
	reset();

	if(device == 0x13)
	{
		progMode();
		p16f184xx_writeComm(0x80, 0, 0x31, 0x00, 0x01);
		p16f184xx_writeComm(0x18, 1, 0x00, 0x00, 0x00);
		_delay_ms(50);
		reset();
	}
}

#ifdef BASELINE
void p16f5x_erase(void)
{
	uint16_t osc_start = 0;

	switch(device)
	{
		case 0x0A: {osc_start = 0x400; break;}
		case 0x0B: {osc_start = 0x100; break;}
		case 0x0C: {osc_start = 0x200; break;}
	}

	progMode();

	if(device == 0x0A || device == 0x0B || device == 0x0C)
	{
		for(int a = 0; a < osc_start; a++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		oscal2 = p16f7x_readComm(P16F7X_READ_DATA);
		for(int a = 0; a < 5; a++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		oscal3 = p16f7x_readComm(P16F7X_READ_DATA);
	}
	reset();

	//ERASE
	progMode();
	p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	p16f7x_writeComm(P16F7X_ERASE, 0x0000, 1);
	_delay_ms(200);
	reset();

	_delay_ms(5);

	//Restore OSCAL
	if(device == 0x0A || device == 0x0B || device == 0x0C)
	{
		progMode();
		for(int a = 0; a < osc_start; a++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		p16f7x_writeComm(P16F7X_LOAD_DATA, oscal2, 0);
		p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
		_delay_ms(5);
		p16f7x_writeComm(P16F7X_END_PROG, 0x0000, 1);
		_delay_us(100);
		for(int a = 0; a < 5; a++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		p16f7x_writeComm(P16F7X_LOAD_DATA, oscal3, 0);
		p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
		_delay_ms(5);
		p16f7x_writeComm(P16F7X_END_PROG, 0x0000, 1);
		_delay_us(100);
		reset();
	}
}
#endif

void p16f628a_erase(void)
{
	progMode();
	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
	_delay_ms(50);
	p16f7x_writeComm(P16F628A_LOAD_CODE, 0x3FFF, 0);
	_delay_ms(50);
	p16f7x_writeComm(P16F628A_ERASE_CODE, 0x0000, 1);
	_delay_ms(200);
	reset();
	_delay_ms(200);

	progMode();
	_delay_ms(50);
	p16f7x_writeComm(P16F628A_ERASE_DATA, 0x0000, 1);
	_delay_ms(200);
	reset();
}

void p16f81x_erase(void)
{
	progMode();
	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
	p16f7x_writeComm(0x1F, 0x0000, 1);
	//p16f7x_writeComm(0x08, 0x0000, 1);
	_delay_ms(200);
	reset();
}

#ifdef PIC16F676
void p16f676_erase(void)
{
	p16f676_storeOsc();

	progMode();
	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
	_delay_ms(50);
	p16f7x_writeComm(P16F628A_ERASE_CODE, 0x0000, 1);
	_delay_ms(200);
	reset();
	_delay_ms(200);

	progMode();
	_delay_ms(50);
	p16f7x_writeComm(P16F628A_ERASE_DATA, 0x0000, 1);
	_delay_ms(200);
	reset();

	p16f676_restoreOsc();
}
#endif

void p16f84a_erase(void)
{
	progMode();
	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x3FFF, 0);
	for(int i = 0; i < 7; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	p16f7x_writeComm(0x01, 0x0000, 1);
	p16f7x_writeComm(0x07, 0x0000, 1);
	p16f7x_writeComm(0x08, 0x0000, 1);
	_delay_ms(12);
	p16f7x_writeComm(0x01, 0x0000, 1);
	p16f7x_writeComm(0x07, 0x0000, 1);
	reset();
	_delay_ms(50);
	/*
	progMode();
	p16f7x_writeComm(P16F628A_LOAD_CODE, 0x3FFF, 0);
	p16f7x_writeComm(P16F628A_ERASE_CODE, 0x0000, 1);
	p16f7x_writeComm(0x18, 0x0000, 1);
	_delay_ms(50);
	reset();

	progMode();
	p16f7x_writeComm(P16F628A_LOAD_DATA, 0x3FFF, 0);
	p16f7x_writeComm(P16F628A_ERASE_DATA, 0x0000, 1);
	p16f7x_writeComm(0x18, 0x0000, 1);
	_delay_ms(50);
	reset();
	*/
}

void p16f87x_erase(void)
{
	for(int i = 0; i < 3; i++)
	{
		progMode();
		if(i == 0) p16f7x_writeComm(P16F628A_LOAD_CODE, 0x3FFF, 0);
		else if(i == 1) p16f7x_writeComm(P16F628A_LOAD_DATA, 0x3FFF, 0);
		else
		{
			p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
			for(int a = 0; a < 7; a++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		}

		p16f7x_writeComm(0x01, 0x0000, 1);
		p16f7x_writeComm(0x07, 0x0000, 1);
		p16f7x_writeComm(0x08, 0x0000, 1);
		_delay_ms(10);
		p16f7x_writeComm(0x01, 0x0000, 1);
		p16f7x_writeComm(0x07, 0x0000, 1);
		_delay_ms(3);
		reset();
	}
}

void p18fx5xx_erase(void)
{
	uint16_t wr1 = 0, wr2 = 0;

	switch(device)
	{
		case 0x03: {wr1 = 0x3F3F, wr2 = 0x8F8F; break;}
		case 0x0D: {wr1 = 0x0101, wr2 = 0x8080; break;}
		case 0x10: {wr1 = 0x0F0F, wr2 = 0x8F8F; break;}
	}

	progMode();

	p18fx5xx_setDP(0x3C, 0x00, 0x05);
	p18fx5xx_writeComm(P18FX5XX_TABLE_WR, wr1);
	p18fx5xx_setDP(0x3C, 0x00, 0x04);
	p18fx5xx_writeComm(P18FX5XX_TABLE_WR, wr2);
	p18fx5xx_writeNop();
	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
	_delay_ms(50);

	reset();
}

#ifdef PIC18FXXK80
void p18fxxk80_erase(void)
{
	uint16_t er_04[] = {0x0404, 0x0404, 0x0404, 0x0404, 0x0505, 0x0202, 0x0404};
	uint16_t er_05[] = {0x0101, 0x0202, 0x0404, 0x0808, 0x0000, 0x0000, 0x0000};


	for(int g = 0; g < 7; g++)
	{
		progMode();
		p18fx5xx_setDP(0x3C, 0x00, 0x04);
		p18fx5xx_writeComm(P18FX5XX_TABLE_WR, er_04[g]);
		p18fx5xx_setDP(0x3C, 0x00, 0x05);
		p18fx5xx_writeComm(P18FX5XX_TABLE_WR, er_05[g]);
		p18fx5xx_setDP(0x3C, 0x00, 0x06);
		p18fx5xx_writeComm(P18FX5XX_TABLE_WR, 0x8080);
		p18fx5xx_writeNop();
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
		_delay_ms(50);
		reset();
		_delay_ms(5);
	}
}
#endif

void p16f7x_progCode(void)
{
	uint16_t zer;
	if(device == 0x09 || device == 0x0A || device == 0x0B || device == 0x0C) zer = 0x0FFF;
	else zer = 0x3FFF;

	for(int i = 0; i < 128; i+= 2)
	{
		if((exchange[i + 4] | (exchange[i + 5] << 8)) == zer)
		{
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
			continue;
		}
		else
		{
			p16f7x_writeComm(P16F7X_LOAD_DATA, (exchange[i + 4] | (exchange[i + 5] <<8)), 0);
			if(device == 0x14) p16f7x_writeComm(0x18, 0x0000, 1);
			else p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
			delay_ms(wr_delay);
			p16f7x_writeComm(P16F7X_END_PROG, 0x0000, 1);
			_delay_us(100);
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		}
	}
}


void p16f628a_progCode(void)
{
	for(int i = 0; i < 128; i+= 2)
	{
		if((exchange[i + 4] | (exchange[i + 5] << 8)) == 0x3FFF)
		{
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
			continue;
		}
		else
		{
			p16f7x_writeComm(P16F628A_LOAD_CODE, (exchange[i + 4] | (exchange[i + 5] <<8)), 0);
			p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
			delay_ms(wr_delay);
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		}
	}
}

void p16f81x_progCode(void)
{
	for(int i = 0; i < 128; i+= 2)
	{
		if((exchange[i + 4] | (exchange[i + 5] << 8)) == 0x3FFF)
		{
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
			continue;
		}
		else
		{
			p16f7x_writeComm(P16F628A_LOAD_CODE, (exchange[i + 4] | (exchange[i + 5] <<8)), 0);
			p16f7x_writeComm(0x18, 0x0000, 1);
			delay_ms(wr_delay);
			p16f7x_writeComm(0x17, 0x0000, 1);
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		}
	}
}

#ifdef PIC16F676
void p16f676_progCode(void)
{
	for(int i = 0; i < 128; i+= 2)
	{
		if((exchange[i + 4] | (exchange[i + 5] << 8)) == 0x3FFF)
		{
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
			continue;
		}
		else if(main_counter == 0x3FF)
		{
			p16f7x_writeComm(P16F628A_LOAD_CODE, oscal1, 0);
			p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
			delay_ms(wr_delay);
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		}
		else
		{
			p16f7x_writeComm(P16F628A_LOAD_CODE, (exchange[i + 4] | (exchange[i + 5] <<8)), 0);
			p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
			delay_ms(wr_delay);
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		}
		main_counter++;
	}
}
#endif

void p16f84a_progCode(void)
{
	for(int i = 0; i < 128; i+= 2)
	{
		if((exchange[i + 4] | (exchange[i + 5] << 8)) == 0x3FFF)
		{
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
			_delay_us(20);
			continue;
		}
		else
		{
			p16f7x_writeComm(P16F628A_LOAD_CODE, (exchange[i + 4] | (exchange[i + 5] <<8)), 0);
			_delay_us(20);
			p16f7x_writeComm(0x08, 0x0000, 1);
			delay_ms(wr_delay);
			p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		}
	}
}

void p16f184xx_progCode(void)
{
	int ff_val;
	uint16_t tmp_a;
	uint8_t l_a, h_a, m_a;


	for(int i = 0; i < 128; i+= f_psize)
	{

		ff_val = 0;
		for(int k = 0; k < f_psize; k+= 2)
		{
			if(exchange[i + k + 4] == 0xFF && exchange[i + k + 5] == 0x3F) ff_val += 2;
		}
		if(ff_val == f_psize) {main_counter += f_psize/2; continue;}

		tmp_a = main_counter >> 8;
		h_a = tmp_a >> 8;
		m_a = (main_counter >> 8) & 0xFF;
		l_a = main_counter & 0xFF;

		p16f184xx_writeComm(0x80, 0, h_a, m_a, l_a);

		for(int k = 0; k < f_psize; k+= 2)
			{
				p16f184xx_writeComm(0x00, 0, 0x00, exchange[i + k + 5], exchange[i + k + 4]);
				if(k < (f_psize - 2)) p16f184xx_writeComm(0xF8, 1, 0x00, 0x00, 0x00);
			}
		p16f184xx_writeComm(0xE0, 1, 0x00, 0x00, 0x00);
		delay_ms(wr_delay);

		main_counter += f_psize/2;
	}
}

void p18fxxq10_progCode(void)
{
	for(int i = 0; i < 128; i+= 2)
	{
		if((exchange[i + 4] | (exchange[i + 5] << 8)) == 0xFFFF)
		{
			p16f184xx_writeComm(0xF8, 1, 0x00, 0x00, 0x00); //Inc ADD
			continue;
		}
		else
		{
			p16f184xx_writeComm(0xE0, 0, 0x00, exchange[5 + i], exchange[4 + i]);
			delay_ms(wr_delay);
		}
	}
}

#ifdef PIC18FX5XX
void p18fx5xx_progCode(void)
{
	uint8_t h_a, m_a, l_a;
	uint16_t  tmp_a;
	int ff_val;

	for(int i = 0; i < 128; i+= f_psize)
	{
		ff_val = 0;
		for(int k = 0; k < f_psize; k+= 2)
		{
			if(exchange[i + k + 4] == 0xFF && exchange[i + k + 5] == 0xFF) ff_val += 2;
		}
		if(ff_val == f_psize) {main_counter += f_psize; continue;}

		progMode();

		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x8EA6);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9CA6);
		if(device == 0x10) p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x84A6);

		tmp_a = main_counter >> 8;
		h_a = tmp_a >> 8;
		m_a = (main_counter >> 8) & 0xFF;
		l_a = main_counter & 0xFF;


		p18fx5xx_setDP(h_a, m_a, l_a);
		for(int k = 0; k < f_psize; k+= 2)
		{
			if(k >= (f_psize - 2)) p18fx5xx_writeComm(P18FX5XX_START_PR, exchange[i + k + 4] | (exchange[i + k + 5] << 8));
			else p18fx5xx_writeComm(P18FX5XX_TABLE_WRPI2, exchange[i + k + 4] | (exchange[i + k + 5] << 8));
		}

		p18fx5xx_writeNop();
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);

		main_counter += f_psize;
		reset();

		_delay_ms(2);
	}
}
#endif

#ifdef PIC18FXXK80
void p18fxxk80_progCode(void)
{
	uint8_t h_a, m_a, l_a;
	uint16_t  tmp_a;
	int ff_val;

	for(int i = 0; i < 128; i+= f_psize)
	{
		ff_val = 0;
		for(int k = 0; k < f_psize; k+= 2)
		{
			if(exchange[i + k + 4] == 0xFF && exchange[i + k + 5] == 0xFF) ff_val += 2;
		}
		if(ff_val == f_psize) {main_counter += f_psize; continue;}

		progMode();

		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x8E7F);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9C7F);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x847F);

		tmp_a = main_counter >> 8;
		h_a = tmp_a >> 8;
		m_a = (main_counter >> 8) & 0xFF;
		l_a = main_counter & 0xFF;


		p18fx5xx_setDP(h_a, m_a, l_a);
		for(int k = 0; k < f_psize; k+= 2)
		{
			if(k >= (f_psize - 2)) p18fx5xx_writeComm(P18FX5XX_START_PR, exchange[i + k + 4] | (exchange[i + k + 5] << 8));
			else p18fx5xx_writeComm(P18FX5XX_TABLE_WRPI2, exchange[i + k + 4] | (exchange[i + k + 5] << 8));
		}

		p18fx5xx_writeNop();
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);

		main_counter += f_psize;
		reset();

		_delay_ms(2);
	}
}
#endif

void p16f628a_progData(void)
{
	for(int i = 0; i < 64; i++)
	{
			if(exchange[i + 4] == 0xFF)
			{
				p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
				continue;
			}
			else
			{
				p16f7x_writeComm(P16F628A_LOAD_DATA, exchange[i + 4], 0);
				p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
				delay_ms(wr_delay + 2);
				p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
			}

	}
}

void p16f81x_progData(void)
{
	for(int i = 0; i < 64; i++)
	{
			if(exchange[i + 4] == 0xFF)
			{
				p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
				continue;
			}
			else
			{
				p16f7x_writeComm(P16F628A_LOAD_DATA, exchange[i + 4], 0);
				p16f7x_writeComm(18, 0x0000, 1);
				delay_ms(wr_delay + 5);
				p16f7x_writeComm(17, 0x0000, 1);
				p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
			}

	}
}


void p16f183xx_progData(void)
{
	for(int i = 0; i < 64; i++)
	{
			if(exchange[i + 4] == 0xFF)
			{
				p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
				continue;
			}
			else
			{
				p16f7x_writeComm(0x02, exchange[i + 4], 0);
				p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
				delay_ms(wr_delay + 2);
				p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
			}

	}
}

void p16f184xx_progData(void)
{
	uint8_t tdat;
	for(int i = 0; i < 64; i++)
	{
		tdat = (p16f184xx_readComm(0xFC) & 0xFF);
		if(exchange[i + 4] == tdat)
		{
			p16f184xx_writeComm(0xF8, 1, 0x00, 0x00, 0x00);
			continue;
		}
		else
		{
			if(device == 0x12)
			{
				p16f184xx_writeComm(0x00, 0, 0x00, 0x00, exchange[i + 4]);
				p16f184xx_writeComm(0xC0, 1, 0x00, 0x00, 0x00);
				delay_ms(wr_delay + 2);
				p16f184xx_writeComm(0xF8, 1, 0x00, 0x00, 0x00);
			}
			if(device == 0x13)
			{
				p16f184xx_writeComm(0xC0, 0, 0x00, 0x00, exchange[i + 4]);
				delay_ms(wr_delay + 3);
				p16f184xx_writeComm(0xF8, 1, 0x00, 0x00, 0x00);
			}
		}
	}
}

#ifdef PIC18FX5XX
void p18fx5xx_progData(void)
{
	uint8_t a_h, a_l;

	progMode();

	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9EA6);
	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9CA6);

	for(int i = 0; i < 64; i ++)
	{
		if(exchange[i + 4] == 0xFF) continue;

		a_h = (main_counter + i) >> 8;
		a_l = (main_counter + i) & 0xFF;
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | a_l));
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EA9);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | a_h));
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EAA);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | exchange[i + 4]));
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EA8);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x84A6);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x82A6);
		do
		{
			p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x50A6);
			p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EF5);
			p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
			a_l = p18fx5xx_readComm(P18FX5XX_SHIFT_TABL, 0x00);
			a_h = p18fx5xx_readComm(P18FX5XX_SHIFT_TABL, 0x00);
		}
		while(!(a_h | a_l));
		PGC_LOW
		_delay_ms(5);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x94A6);
	}
	main_counter += 64;
	reset();
}
#endif

#ifdef PIC18FXXK80
void p18fxxk80_progData(void)
{
	uint8_t a_h, a_l;

	progMode();

	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9E7F);
	p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x9C7F);

	for(int i = 0; i < 64; i ++)
	{
		if(exchange[i + 4] == 0xFF) continue;

		a_h = (main_counter + i) >> 8;
		a_l = (main_counter + i) & 0xFF;
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | a_l));
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6E74);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | a_h));
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6E75);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, (0x0E00 | exchange[i + 4]));
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6E73);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x847F);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x827F);
		do
		{
			p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x507F);
			p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x6EF5);
			p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x0000);
			a_l = p18fx5xx_readComm(P18FX5XX_SHIFT_TABL, 0x00);
			a_h = p18fx5xx_readComm(P18FX5XX_SHIFT_TABL, 0x00);
		}
		while(!(a_h | a_l));
		PGC_LOW
		_delay_ms(5);
		p18fx5xx_writeComm(P18FX5XX_CORE_INSTR, 0x947F);
	}
	main_counter += 64;
	reset();
}
#endif

#ifdef PIC16F676
void p16f676_readOscall(void)
{
	uint16_t osc;
	progMode();

	for(int i = 0; i < 0x3FF; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	osc = p16f7x_readComm(P16F7X_READ_DATA);
	reset();

	main_buff[0] = 0xFE;
	main_buff[1] = 0x02;
	main_buff[2] = 0xAB;
	main_buff[3] = (osc >> 8);
	main_buff[4] = (osc & 0xFF);
	send_frame(main_buff, 5);
}
#endif

void p12f1840_readOscall(void)
{
	progMode();

	if(device == 0x0E) p16f183xx_setAdd(0x1D, 0x00, 0xE0, 0x00);
	else
	{
	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
	for(int i = 0; i < osc_start; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	}
	oscal1 = p16f7x_readComm(P16F7X_READ_DATA);
	p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	oscal2 = p16f7x_readComm(P16F7X_READ_DATA);
	p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	oscal3 = p16f7x_readComm(P16F7X_READ_DATA);
	reset();

	main_buff[0] = 0xFE;
	main_buff[1] = 0x06;
	main_buff[2] = 0xAB;
	main_buff[3] = (oscal1 >> 8);
	main_buff[4] = (oscal1 & 0xFF);
	main_buff[5] = (oscal2 >> 8);
	main_buff[6] = (oscal2 & 0xFF);
	main_buff[7] = (oscal3 >> 8);
	main_buff[8] = (oscal3 & 0xFF);
	send_frame(main_buff, 9);
}


#ifdef BASELINE
void p16f5x_readOscall(void)
{
	uint16_t osc_start = 0;
	switch(device)
	{
		case 0x0A: {osc_start = 0x400; break;}
		case 0x0B: {osc_start = 0x100; break;}
		case 0x0C: {osc_start = 0x200; break;}
	}

	progMode();
	for(int i = 0; i < osc_start; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	oscal1 = p16f7x_readComm(P16F7X_READ_DATA);
	reset();

	main_buff[0] = 0xFE;
	main_buff[1] = 0x02;
	main_buff[2] = 0xAB;
	main_buff[3] = (oscal1 >> 8);
	main_buff[4] = (oscal1 & 0xFF);
	send_frame(main_buff, 5);
}

void p12f5x_writeOscall(void)
{
	uint16_t osc_start = 0;
	oscal1 = exchange[4] | (exchange[5] << 8);

		switch(device)
		{
			case 0x0A: {osc_start = 0x400; break;}
			case 0x0B: {osc_start = 0x100; break;}
			case 0x0C: {osc_start = 0x200; break;}
		}

		progMode();
		for(int a = 0; a < osc_start; a++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
		p16f7x_writeComm(P16F7X_LOAD_DATA, oscal1, 0);
		p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
		_delay_ms(5);
		p16f7x_writeComm(P16F7X_END_PROG, 0x0000, 1);
		_delay_us(100);
		reset();
}
#endif

#ifdef PIC16F676
void p16f676_writeOscall(void)
{
	oscal1 = exchange[4] | (exchange[5] << 8);

	progMode();
	for(int i = 0; i < 0x3FF; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	p16f7x_writeComm(P16F7X_LOAD_DATA, oscal1, 0);
	p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
	_delay_ms(5);
	p16f7x_writeComm(0x0A, 0x0000, 1);
	reset();
}
#endif

void p12f1840_writeOscall(void)
{
	progMode();

	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x0000, 0);
	for(int i = 0; i < osc_start; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);

	for(int k = 0; k < osc_cnt; k++)
	{
		p16f7x_writeComm(P16F7X_LOAD_DATA, (exchange[k*2 + 4] | (exchange[k*2 + 5] <<8)), 0);
		p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
		_delay_ms(5);
		p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	}
	reset();
}

#ifdef PIC16F676
void p16f676_storeOsc(void)
{
	progMode();
	for(int i = 0; i < 0x3FF; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	oscal1 = p16f7x_readComm(P16F7X_READ_DATA);
	reset();
	_delay_ms(5);

	p16f628A_progMode();
	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x2000, 0);
	for(int i = 0; i < 7; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	oscal2 = p16f7x_readComm(P16F7X_READ_DATA);
	reset();

	oscal2 &= 0x3000;
	_delay_ms(5);
}

void p16f676_restoreOsc(void)
{
	progMode();
	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x2000, 0);
	for(int i = 0; i < 7; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	oscal3 = p16f7x_readComm(P16F7X_READ_DATA);
	reset();

	oscal3 &= 0x0FFF;
	oscal3 |= oscal2;

	progMode();
	p16f7x_writeComm(P16F7X_LOAD_CONF, 0x2000, 0);
	for(int i = 0; i < 7; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	p16f7x_writeComm(P16F7X_LOAD_DATA, oscal3, 0);
	p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
	_delay_ms(5);
	p16f7x_writeComm(0x0A, 0x0000, 1);
	reset();

	progMode();
	for(int i = 0; i < 0x3FF; i++) p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1);
	p16f7x_writeComm(P16F7X_LOAD_DATA, oscal1, 0);
	p16f7x_writeComm(P16F7X_BEGIN_PROG, 0x0000, 1);
	_delay_ms(5);
	p16f7x_writeComm(0x0A, 0x0000, 1);
	reset();
}
#endif

