/*
 * progger.c
 *
 *  Created on: 19 θών. 2020 γ.
 *      Author: andre
 */

#include "progger.h"

extern uint8_t exchange[];
extern uint8_t main_buff[];

uint32_t f_size;
uint16_t f_psize;
uint16_t ee_size;
uint8_t device;
uint8_t osc_start;
uint8_t osc_cnt;
uint8_t conf_cnt;
uint16_t wr_delay;
uint32_t main_counter;


void send_successConn(void)
{
	exchange[0] = 0xFE;
	exchange[1] = 0x01;
	exchange[2] = 0xCC;
	exchange[3] = 0x00;
	send_frame(exchange, 4);
}

void send_frame(uint8_t *buff, uint8_t count)
{
	for(int n = 0; n < count; n++) USARTSend_char(buff[n]);
}

//
void send_arr(uint8_t marker, uint8_t *buff, uint16_t count, uint16_t offset)
{
	USARTSend_char(0xFE); //Start
	USARTSend_char(count);
	USARTSend_char(marker);
	for(int u = 0; u < count; u++) USARTSend_char(buff[u + offset]);
}

void deassemble_frame(void)
{
	switch(exchange[2])
	{
		case 0xCC:
		{
			send_successConn();
			break;
		}
		case 0xA0: //Get ID
				{
					device = exchange[3];
					switch(device)
						{
							case 0x01: { p16f7x_getId(); break; }
							case 0x02: { p16f7x_getId(); break; }
							case 0x03: { p18fx5xx_getId(); break; }
							case 0x04: { p16f7x_getId(); break; }
							case 0x05: { p18fx5xx_getId(); break; }
							case 0x06: { p16f7x_getId(); break; }
							case 0x07: { p16f7x_getId(); break; }
							default: break;
						}
					break;
				}
				case 0xA1: //Read
				{
					device = exchange[3];
					f_size = (exchange[4] << 8);
					f_size = (f_size << 8);
					f_size |= (exchange[5] << 8) | exchange[6];
					ee_size = (exchange[7] << 8) | exchange[8];

					osc_cnt = exchange[9];
					osc_start = exchange[10];

					switch(device)
						{
							case 0x01: { p16f7x_read(); break; }
							case 0x02: { p16f628a_read(); break; }
							case 0x03: { p18fx5xx_read(); break; }
							case 0x04: { p16f628a_read(); break; }
							case 0x05: { p18fxxk80_read(); break; }
							case 0x06: { p16f628a_read(); p16f676_readOscall(); break; }
							case 0x07: { p16f628a_read(); p12f1840_readOscall(); break; }
							default: break;
						}
					break;
				}
				case 0xA2: //Erase
						{
							device = exchange[3];
							switch(device)
								{
									case 0x01: { p16f7x_erase(); break; }
									case 0x02: { p16f628a_erase(); break; }
									case 0x03: { p18fx5xx_erase(); break; }
									case 0x04: { p16f84a_erase(); break; }
									case 0x05: { p18fxxk80_erase(); break; }
									case 0x06: { p16f676_erase(); break; }
									case 0x07: { p16f628a_erase(); break; }
									default: break;
								}

							main_buff[0] = 0xFE;
							main_buff[1] = 0x01;
							main_buff[2] = 0xA2;
							main_buff[3] = 0x00;
							send_frame(main_buff, 4);
							break;
						}
				case 0xA3: //Read config
						{
							device = exchange[3];
							conf_cnt = exchange[4];

							switch(device)
								{
									case 0x01: { p16f7x_readCfg(); break; }
									case 0x02: { p16f7x_readCfg(); break; }
									case 0x03: { p18fx5xx_readCfg(); break; }
									case 0x04: { p16f7x_readCfg(); break; }
									case 0x05: { p18fx5xx_readCfg(); break; }
									case 0x06: { p16f7x_readCfg(); break; }
									case 0x07: { p16f7x_readCfg(); break; }
									default: break;
								}
							break;
						}
				case 0xA4: //Write config
						{
							device = exchange[3];
							conf_cnt = exchange[18];

							switch(device)
								{
									case 0x01: { p16f7x_writeCfg(); break; }
									case 0x02: { p16f7x_writeCfg(); break; }
									case 0x03: { p18fx5xx_writeCfg(); break; }
									case 0x04: { p16f84a_writeCfg(); break; }
									case 0x05: { p18fxxk80_writeCfg(); break; }
									case 0x06: { p16f676_writeCfg(); break; }
									case 0x07: { p12f1840_writeCfg(); break; }
									default: break;
								}

							main_buff[0] = 0xFE;
							main_buff[1] = 0x01;
							main_buff[2] = 0xA4;
							main_buff[3] = 0x00;
							send_frame(main_buff, 4);
							break;
						}
				case 0xA5: //Prepare to write code
				{
					device = exchange[3];
					f_size = (exchange[4] << 8);
					f_size = f_size << 8;
					f_size |= exchange[6] | (exchange[5] << 8);
					ee_size = exchange[8] | (exchange[7] << 8);
					wr_delay = exchange[10] | (exchange[9] << 8);
					f_psize = exchange[11];
					switch(device)
						{
							case 0x01: { p16f7x_progMode(); break; }
							case 0x02: { p16f628A_progMode(); break; }
							case 0x03: { main_counter = 0; break; }
							case 0x04: { p16f7x_progMode(); break; }
							case 0x05: { main_counter = 0; break; }
							case 0x06: { p16f676_storeOsc(); p16f628A_progMode(); main_counter = 0; break; }
							case 0x07: { p16f7x_progMode(); break; }
							default: break;
						}
					main_buff[0] = 0xFE;
					main_buff[1] = 0x01;
					main_buff[2] = 0xA6;
					main_buff[3] = 0x00;
					send_frame(main_buff, 4);
					break;
				}
				case 0xA6: //Write code
				{
					device = exchange[3];
					switch(device)
						{
							case 0x01: { p16f7x_progCode(); break; }
							case 0x02: { p16f628a_progCode(); break; }
							case 0x03: { p18fx5xx_progCode(); break; }
							case 0x04: { p16f84a_progCode(); break; }
							case 0x05: { p18fxxk80_progCode(); break; }
							case 0x06: { p16f676_progCode(); break; }
							case 0x07: { p16f676_progCode(); break; }
							default: break;
						}
					main_buff[0] = 0xFE;
					main_buff[1] = 0x01;
					main_buff[2] = 0xA6;
					main_buff[3] = 0x00;
					send_frame(main_buff, 4);
					break;
				}
				case 0xA7: //Prepare to write eeprom
				{
					device = exchange[3];
					switch(device)
						{
							case 0x02: { p16f628A_progMode(); break; }
							case 0x03: { main_counter = 0; break; }
							case 0x04: { p16f628A_progMode(); break; }
							case 0x05: { main_counter = 0; break; }
							case 0x06: { p16f628A_progMode(); break; }
							case 0x07: { p16f7x_progMode(); break; }
							default: break;
						}

					main_buff[0] = 0xFE;
					main_buff[1] = 0x01;
					main_buff[2] = 0xA8;
					main_buff[3] = 0x00;
					send_frame(main_buff, 4);

					break;
				}
				case 0xA8: //Write eeprom
				{
					device = exchange[3];
					switch(device)
						{
							case 0x02: { p16f628a_progData(); break; }
							case 0x03: { p18fx5xx_progData(); break; }
							case 0x04: { p16f628a_progData(); break; }
							case 0x05: { p18fxxk80_progData(); break; }
							case 0x06: { p16f628a_progData(); break; }
							case 0x07: { p16f628a_progData(); break; }
							default: break;
						}
					main_buff[0] = 0xFE;
					main_buff[1] = 0x01;
					main_buff[2] = 0xA8;
					main_buff[3] = 0x00;
					send_frame(main_buff, 4);
					break;
				}
				//Read Oscal
				case 0xAB:
				{
					device = exchange[3];
					osc_start = exchange[4];
					switch(device)
						{
							case 0x06: { p16f676_readOscall(); break; }
							case 0x07: { p12f1840_readOscall(); break; }
							default: break;
						}
					break;
				}
				//Write Oscal
				case 0xAC:
				{
					device = exchange[3];
					osc_start = exchange[10];
					osc_cnt = exchange[11];

					switch(device)
						{
							case 0x06: { p16f676_writeOscall(); break; }
							case 0x07: { p12f1840_writeOscall(); break; }
							default: break;
						}

					main_buff[0] = 0xFE;
					main_buff[1] = 0x01;
					main_buff[2] = 0xAC;
					main_buff[3] = 0x00;
					send_frame(main_buff, 4);
					break;
				}
				case 0xAF:
				{
					device = exchange[3];
					switch(device)
						{
							case 0x01: { p16f7x_reset(); break; }
							case 0x02: { p16f7x_reset(); break; }
							case 0x03: { break; }
							case 0x04: { p16f7x_reset(); break; }
							case 0x07: { p16f7x_reset(); break; }
							default: break;
						}
					break;
				}
		default:
		{
			break;
		}
	}
}


