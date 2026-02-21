/*
 * progger.c
 *
 *  Created on: 19 ���. 2020 �.
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
#ifdef PIC18FX5XX
							case 0x03: { p18fx5xx_getId(); break; }
#endif
							case 0x04: { p16f7x_getId(); break; }
#ifdef PIC18FXXK80
							case 0x05: { p18fx5xx_getId(); break; }
#endif

#ifdef PIC16F676
							case 0x06: { p16f7x_getId(); break; }
#endif
							case 0x07: { p16f7x_getId(); break; }
							case 0x0D: { p18fx5xx_getId(); break; }
							case 0x0E: { p16f7x_getId(); break; }
							case 0x0F: { p16f7x_getId(); break; }
							case 0x10: { p18fx5xx_getId(); break; }
							case 0x11: { p16f7x_getId(); break; }
							case 0x12: { p16f184xx_getId(); break; }
							case 0x13: { p16f184xx_getId(); break; }
							case 0x14: { p16f7x_getId(); break; }
							case 0x15: { p16f7x_getId(); break; }
							case 0x16: { p18fx5xx_getId(); break; }
							default: break;
						}
					break;
				}
				case 0xA1: //Read
				{

					/*
					f_size = (uint32_t)(exchange[4] << 8);
					f_size = (f_size << 8);
					f_size |= (uint32_t)((exchange[5] << 8) | exchange[6]);
					*/

					switch(exchange[4])
					{
						case 0x01: {f_size = 128; break;}
						case 0x02: {f_size = 256; break;}
						case 0x03: {f_size = 512; break;}
						case 0x04: {f_size = 1024; break;}
						case 0x05: {f_size = 2048; break;}
						case 0x06: {f_size = 4096; break;}
						case 0x07: {f_size = 8192; break;}
						case 0x08: {f_size = 16384; break;}
						case 0x09: {f_size = 32768; break;}
						case 0x0A: {f_size = 65536; break;}
						case 0x0B: {f_size = 131072; break;}
						case 0x0C: {f_size = 262144; break;}
						case 0x0D: {f_size = 49152; break;}
						default:  {f_size = 128; break;}
					}

					ee_size = (uint32_t)((exchange[7] << 8) | exchange[8]);

					device = exchange[3];

					osc_cnt = exchange[9];
					osc_start = exchange[10];

					switch(device)
						{
							case 0x01: { p16f7x_read(); break; }
							case 0x02: { p16f628a_read(); break; }
#ifdef PIC18FX5XX
							case 0x03: { p18fx5xx_read(); break; }
#endif
							case 0x04: { p16f628a_read(); break; }
#ifdef PIC18FXXK80
							case 0x05: { p18fxxk80_read(); break; }
#endif

#ifdef PIC16F676
							case 0x06: { p16f628a_read(); p16f676_readOscall(); break; }
#endif
							case 0x07: { p16f628a_read(); p12f1840_readOscall(); break; }
#ifdef BASELINE
							case 0x09: { p16f7x_read(); break; }
							case 0x0A: { p16f7x_read(); break; }
							case 0x0B: { p16f7x_read(); break; }
							case 0x0C: { p16f7x_read(); break; }
#endif
							case 0x0D: { p18fx5xx_read(); break; }
							case 0x0E: { p16f183xx_read(); break; }
							case 0x0F: { p16f628a_read(); break; }
							case 0x10: { p18fx5xx_read(); break; }
							case 0x11: { p16f628a_read(); break; }
							case 0x12: { p16f184xx_read(); break; }
							case 0x13: { p16f184xx_read(); break; }
							case 0x14: { p16f7x_read(); break; }
							case 0x15: { p16f7x_read(); break; }
							case 0x16: { p18fxxk80_read(); break; }
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
#ifdef PIC18FX5XX
									case 0x03: { p18fx5xx_erase(); break; }
#endif
									case 0x04: { p16f84a_erase(); break; }
#ifdef PIC18FXXK80
									case 0x05: { p18fxxk80_erase(); break; }
#endif

#ifdef PIC16F676
									case 0x06: { p16f676_erase(); break; }
#endif
									case 0x07: { p16f628a_erase(); break; }
#ifdef BASELINE
									case 0x09: { p16f5x_erase(); break; }
									case 0x0A: { p16f5x_erase(); break; }
									case 0x0B: { p16f5x_erase(); break; }
									case 0x0C: { p16f5x_erase(); break; }
#endif
									case 0x0D: { p18fx5xx_erase(); break; }
									case 0x0E: { p16f7x_erase(); break; }
									case 0x0F: { p16f81x_erase(); break; }
									case 0x10: { p18fx5xx_erase(); break; }
									case 0x11: { p16f87x_erase(); break; }
									case 0x12: { p16f184xx_erase(); break; }
									case 0x13: { p16f184xx_erase(); break; }
									case 0x14: { p16f7x_erase(); break; }
									case 0x15: { p16f7x_erase(); break; }
									case 0x16: { p18fxxk80_erase(); break; }
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
#ifdef PIC18FX5XX
									case 0x03: { p18fx5xx_readCfg(); break; }
#endif
									case 0x04: { p16f7x_readCfg(); break; }
#ifdef PIC18FXXK80
									case 0x05: { p18fx5xx_readCfg(); break; }
#endif

#ifdef PIC16F676
									case 0x06: { p16f7x_readCfg(); break; }
#endif
									case 0x07: { p16f7x_readCfg(); break; }
#ifdef BASELINE
									case 0x09: { p16f5x_readCfg(); break; }
									case 0x0A: { p16f5x_readCfg(); break; }
									case 0x0B: { p16f5x_readCfg(); break; }
									case 0x0C: { p16f5x_readCfg(); break; }
#endif
									case 0x0E: { p16f7x_readCfg(); break; }
									case 0x0F: { p16f7x_readCfg(); break; }
									case 0x10: { p18fx5xx_readCfg(); break; }
									case 0x11: { p16f7x_readCfg(); break; }
									case 0x12: { p16f184xx_readCfg(); break; }
									case 0x13: { p16f184xx_readCfg(); break; }
									case 0x14: { p16f7x_readCfg(); break; }
									case 0x15: { p16f7x_readCfg(); break; }
									case 0x16: { p18fx5xx_readCfg(); break; }
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
#ifdef PIC18FX5XX
									case 0x03: { p18fx5xx_writeCfg(); break; }
#endif
									case 0x04: { p16f84a_writeCfg(); break; }
#ifdef PIC18FXXK80
									case 0x05: { p18fxxk80_writeCfg(); break; }
#endif

#ifdef PIC16F676
									case 0x06: { p16f676_writeCfg(); break; }
#endif
									case 0x07: { p12f1840_writeCfg(); break; }
#ifdef BASELINE
									case 0x09: { p16f5x_writeCfg(); break; }
									case 0x0A: { p16f5x_writeCfg(); break; }
									case 0x0B: { p16f5x_writeCfg(); break; }
									case 0x0C: { p16f5x_writeCfg(); break; }
#endif
									case 0x0E: { p12f1840_writeCfg(); break; }
									case 0x0F: { p12f1840_writeCfg(); break; }
									case 0x10: { p18fx5xx_writeCfg(); break; }
									case 0x11: { p12f1840_writeCfg(); break; }
									case 0x12: { p16f184xx_writeCfg(); break; }
									case 0x13: { p16f184xx_writeCfg(); break; }
									case 0x14: { p16f7x_writeCfg(); break; }
									case 0x15: { p16f7x_writeCfg(); break; }
									case 0x16: { p18fxxk80_writeCfg(); break; }
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
#ifdef PIC18FX5XX
							case 0x03: { main_counter = 0; break; }
#endif
							case 0x04: { p16f7x_progMode(); break; }
#ifdef PIC18FXXK80
							case 0x05: { main_counter = 0; break; }
#endif

#ifdef PIC16F676
							case 0x06: { p16f676_storeOsc(); p16f628A_progMode(); main_counter = 0; break; }
#endif
							case 0x07: { p16f7x_progMode(); break; }
#ifdef BASELINE
							case 0x09: { p16f7x_progMode(); p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1); break; }
							case 0x0A: { p16f7x_progMode(); p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1); break; }
							case 0x0B: { p16f7x_progMode(); p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1); break; }
							case 0x0C: { p16f7x_progMode(); p16f7x_writeComm(P16F7X_INC_ADD, 0x0000, 1); break; }
#endif
							case 0x0E: { p16f7x_progMode(); break; }
							case 0x0F: { p16f7x_progMode(); break; }
							case 0x10: { main_counter = 0; break; }
							case 0x11: { p16f7x_progMode(); break; }
							case 0x12: { p16f7x_progMode(); main_counter = 0; break; }
							case 0x13: { p16f7x_progMode(); break; }
							case 0x14: { p16f7x_progMode(); break; }
							case 0x15: { p16f7x_progMode(); break; }
							case 0x16: { main_counter = 0; break; }
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
#ifdef PIC18FX5XX
							case 0x03: { p18fx5xx_progCode(); break; }
#endif
							case 0x04: { p16f84a_progCode(); break; }
#ifdef PIC18FXXK80
							case 0x05: { p18fxxk80_progCode(); break; }
#endif

#ifdef PIC16F676
							case 0x06: { p16f676_progCode(); break; }
#endif
							case 0x07: { p16f628a_progCode(); break; }
#ifdef BASELINE
							case 0x09: { p16f7x_progCode(); break; }
							case 0x0A: { p16f7x_progCode(); break; }
							case 0x0B: { p16f7x_progCode(); break; }
							case 0x0C: { p16f7x_progCode(); break; }
#endif
							case 0x0E: { p16f628a_progCode(); break; }
							case 0x0F: { p16f81x_progCode(); break; }
							case 0x10: { p18fx5xx_progCode(); break; }
							case 0x11: { p16f628a_progCode(); break; }
							case 0x12: { p16f184xx_progCode(); break; }
							case 0x13: { p18fxxq10_progCode(); break; }
							case 0x14: { p16f7x_progCode(); break; }
							case 0x15: { p16f7x_progCode(); break; }
							case 0x16: { p18fxxk80_progCode(); break; }
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
#ifdef PIC18FX5XX
							case 0x03: { main_counter = 0; break; }
#endif
							case 0x04: { p16f628A_progMode(); break; }
#ifdef PIC18FXXK80
							case 0x05: { main_counter = 0; break; }
#endif

#ifdef PIC16F676
							case 0x06: { p16f628A_progMode(); break; }
#endif
							case 0x07: { p16f7x_progMode(); break; }
							case 0x0E:
							{
								p16f7x_progMode();
								p16f183xx_setAdd(0x1D, 0x00, 0xF0, 0x00); //Load ADD to 0xF000
								break;
							}
							case 0x0F: { p16f7x_progMode(); break; }
							case 0x10: { main_counter = 0; break; }
							case 0x11: { p16f7x_progMode(); break; }
							case 0x12:
							{
								p16f7x_progMode();
								p16f184xx_writeComm(0x80, 0, 0x00, 0xF0, 0x00);
								break;
							}
							case 0x13:
							{
								p16f7x_progMode();
								p16f184xx_writeComm(0x80, 0, 0x31, 0x00, 0x00);
								break;
							}
							case 0x16: { main_counter = 0; break; }
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
#ifdef PIC18FX5XX
							case 0x03: { p18fx5xx_progData(); break; }
#endif
							case 0x04: { p16f628a_progData(); break; }
#ifdef PIC18FXXK80
							case 0x05: { p18fxxk80_progData(); break; }
#endif

#ifdef PIC16F676
							case 0x06: { p16f628a_progData(); break; }
#endif
							case 0x07: { p16f628a_progData(); break; }
							case 0x0E: { p16f183xx_progData(); break; }
							case 0x0F: { p16f81x_progData(); break; }
							case 0x10: { p18fx5xx_progData(); break; }
							case 0x11: { p16f628a_progData(); break; }
							case 0x12: { p16f184xx_progData(); break; }
							case 0x13: { p16f184xx_progData(); break; }
							case 0x16: { p18fxxk80_progData(); break; }
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
#ifdef PIC16F676
							case 0x06: { p16f676_readOscall(); break; }
#endif
							case 0x07: { p12f1840_readOscall(); break; }
#ifdef BASELINE
							case 0x0A: { p16f5x_readOscall(); break; }
							case 0x0B: { p16f5x_readOscall(); break; }
							case 0x0C: { p16f5x_readOscall(); break; }
#endif
							case 0x0E: { p12f1840_readOscall(); break; }
							case 0x15: { p12f6xx_readOscall(); break; }
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
#ifdef PIC16F676
							case 0x06: { p16f676_writeOscall(); break; }
#endif
							case 0x07: { p12f1840_writeOscall(); break; }
#ifdef BASELINE
							case 0x0A: { p12f5x_writeOscall(); break; }
							case 0x0B: { p12f5x_writeOscall(); break; }
							case 0x0C: { p12f5x_writeOscall(); break; }
#endif
							case 0x15: { p12f6xx_writeOscall(); break; }
							default: break;
						}

					main_buff[0] = 0xFE;
					main_buff[1] = 0x01;
					main_buff[2] = 0xAC;
					main_buff[3] = 0x00;
					send_frame(main_buff, 4);
					break;
				}
				//Reset
				case 0xAF:
				{
					device = exchange[3];
					reset();
					break;
				}
		default:
		{
			break;
		}
	}
}


