#include<avr/io.h>
#include<avr/delay.h>
#include"NRF24L01.h"
#include <avr/wdt.h>
#include "../Debug.h"
#include "../types.h"
#include "../MD_Protocol.h"
// SPI(nRF24L01) commands
#define READ_REG_NRF24L01    	0x00 				// Define read command to register
#define WRITE_REG_NRF24L01   	0x20 				// Define write command to register
#define RD_RX_PLOAD 			0x61 				// Define RX payload register address
#define WR_TX_PLOAD 			0xA0 				// Define TX payload register address
#define FLUSH_TX    			0xE1 				// Define flush TX register command
#define FLUSH_RX    			0xE2 				// Define flush RX register command
#define REUSE_TX_PL 			0xE3 				// Define reuse TX payload register command
#define NOP         			0xFF 				// Define No Operation, might be used to read status register
//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG      			0x00				// 'Config' register address
#define EN_AA       			0x01                		// 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   			0x02                		// 'Enabled RX addresses' register address
#define SETUP_AW    			0x03                		// 'Setup address width' register address
#define SETUP_RETR  			0x04                		// 'Setup Auto. Retrans' register address
#define RF_CH       			0x05                		// 'RF channel' register address
#define RF_SETUP    			0x06 				// 'RF setup' register address
#define STATUS      			0x07 				// 'Status' register address
#define OBSERVE_TX  			0x08 				// 'Observe TX' register address
#define CD          			0x09 				//'Carrier Detect' register address
#define RX_ADDR_P0  			0x0A				// 'RX address pipe0' register address
#define RX_ADDR_P1  			0x0B 				// 'RX address pipe1' register address
#define RX_ADDR_P2  			0x0C 				// 'RX address pipe2' register address
#define RX_ADDR_P3  			0x0D 				// 'RX address pipe3' register address
#define RX_ADDR_P4  			0x0E 				// 'RX address pipe4' register address
#define RX_ADDR_P5  			0x0F				// 'RX address pipe5' register address
#define TX_ADDR     			0x10 				// 'TX address' register address
#define RX_PW_P0    			0x11 				// 'RX payload width, pipe0' register address
#define RX_PW_P1    			0x12 				// 'RX payload width, pipe1' register address
#define RX_PW_P2    			0x13 				// 'RX payload width, pipe2' register address
#define RX_PW_P3    			0x14 				// 'RX payload width, pipe3' register address
#define RX_PW_P4    			0x15 				// 'RX payload width, pipe4' register address
#define RX_PW_P5    			0x16 				// 'RX payload width, pipe5' register address
#define FIFO_STATUS 			0x17 			    	// 'FIFO Status Register' register address
#define TX_ADR_WIDTH   	5  // (5 byte-wide Transmit / Receive Address)
#define TX_PLOAD_WIDTH 	32  // (Data path width of valid data)
// CE Pin & CSN Pin & IRQ Pin
/*
 IRQ(PD2)   CSN(PD7)   CE(PD6)
 */
extern uint8 g_DimmingLevel[4];
extern uint8 g_TestFlag;
#define IRQ		  (PIND & (1<<2))

uint8_t RF_CHANNEL;

#define CE_0      PORTD &= ~(1<<6);
#define CE_1      PORTD |= (1<<6);

#define CSN_0     PORTD &= ~(1<<7);
#define CSN_1     PORTD |= (1<<7);
#define	RX_DR			0x40
#define	TX_DS			0x20
#define	MAX_RT			0x10
uint8_t rotary__GetValue() {
	//id
	uint8 id = 0;
	if ((PINC & _BV(5)) == 0) {
		id |= _BV(3);
	}
	if ((PINC & _BV(4)) == 0) {
		id |= _BV(2);
	}
	if ((PINC & _BV(3)) == 0) {
		id |= _BV(1);
	}
	if ((PINC & _BV(2)) == 0) {
		id |= _BV(0);
	}

	if (id == 0) {
		RF_CHANNEL = 94;
	} else {
		RF_CHANNEL = 94 + (2 * id);

	}
	return id;
}
uint8 flag = 1;
uint8 channelId = 0;
uint8 dimmingLevel = 0;
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = { 0xb2, 0xb2, 0xb3, 0xb4, 0x01 }; //  (Define a static send address)

uint8_t RX_BUF[TX_PLOAD_WIDTH];

uint8_t TX_BUF[TX_PLOAD_WIDTH];

uint8_t SPI_RW(uint8_t Data) {
	SPDR = Data;

	while (!(SPSR & 0x80)) {
	};
	return SPDR;
}

/****Reg to register a byte written while returning state bytes****/
uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value) {
	uint8_t status;

	CSN_0;
	status = SPI_RW(reg); //select register  and write value to it
	SPI_RW(value);
	CSN_1;
	return (status);
}
/****To register reg read a byte, and return to state bytes*****/
uint8_t SPI_Read_Reg(uint8_t reg) {
	uint8_t status;
	CSN_0;
	SPI_RW(reg);
	status = SPI_RW(0);

	CSN_1;
	return (status);
}

/********Read the bytes bytes of data****************/

uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes) {
	uint8_t status, byte_ctr;

	CSN_0;
	status = SPI_RW(reg);
	_delay_us(10);
	for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++) {
		pBuf[byte_ctr] = SPI_RW(0);
	}
	CSN_1;
	return (status);
}

/****************Write bytes bytes of data*************/
uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes) {
	uint8_t status, byte_ctr;
	CSN_0;
	status = SPI_RW(reg);
	_delay_us(10);
	for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
		SPI_RW(*pBuf++);
	CSN_1;
	return (status);
}

/*Receive function, and returns 1 said data received, or no data to receive*/
uint8_t nRF24L01_RxPacket(uint8_t* rx_buf) {
	uint8_t status, revale = 0;
	uint8_t checkSum = 0;
	flag = 1;
	CE_0;
	_delay_us(10);
	status = SPI_RW(STATUS); //(Check the state judge read data receiving condition)

//	printf("STATUS接受状态：0x%2x\r\n", status);

	if (status & RX_DR) //(Judge whether to receive data)
	{
		SPI_Read_Buf(RD_RX_PLOAD, rx_buf, TX_PLOAD_WIDTH); // read receive payload from RX_FIFO buffer

		revale = 1;
	}
	SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, status); //
	CE_1;

	return revale;
}

/****************发送函数***************************/
void nRF24L01_TxPacket(unsigned char * tx_buf) {
	CE_0; //StandBy I模式
	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); //(Loading the receiver address)
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); // (Load data)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e); // (IRQ transceiver complete interrupt response, 16 CRC, the Lord sent)
	CE_1; //(Buy high CE, stimulate the data sent)
	_delay_us(10);
}

void RX_Mode(void) {
	CE_0;
	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // (Receiving device to receive and transmit channels 0 to use the device to send the same address)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RX_PW_P0, TX_PLOAD_WIDTH); //  (Receive Channel 0 select and send the same channel width of valid data)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f); // (Enable automatic answering receive channel 0)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f); // (Enable Receive Channel 0)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, RF_CHANNEL); // (Select the RF channel 0x40)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP, 0x07); // (Data transfer rate 1Mbps, transmitting power 0dBm, low-noise amplifier gain)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0f); // (CRC is enabled, the 16-bit CRC, the power, the receive mode)
	CE_1;
	_delay_us(10);
}

void TX_Mode(uint8_t * tx_buf) {
	CE_0;
	SPI_Write_Buf(WRITE_REG_NRF24L01 + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); //(Written to the transmit address)
	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // (In order to answer the receiving device, the receiving channel 0 address and send the address the same)
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); // (Loading data)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f); // (Enable automatic answering receive channel 0)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f); //(Enable Receive Channel 0)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_RETR, 0x0a); // (Automatic retransmission delay to wait 250us +86 us, automatic repeat 10 times)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, RF_CHANNEL); // 0(Select the RF channel 0x40)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP, 0x07); //(Data transfer rate 1Mbps, transmitting power 0dBm, low-noise amplifier gain)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RX_PW_P0, TX_PLOAD_WIDTH); /// / Receive channel 0 select and send the same channel width of valid data)
	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e); // (CRC is enabled, the 16-bit CRC, power)
	CE_1;
	_delay_us(10);
}

void nRF24L01_Initial(void) {
	/* IRQ(PB4)   CSN(PD6)   CE(PD7) */
	/* MISO(PB3)  MOSI(PB2)  SCK(PB1) SS(PB0) */
//
//		DDRB |= (1<<1) | (1<<2) | (1<<0) ;        // ->output
//		DDRB &= ~((1<<4)|(1<<3));
//		PORTB |=  (1<<3) |(1<<4) |(1<<2) |(1<<1);
//		PORTB &= ~(1<<0);
//
//		DDRD |= (1<<6) | (1<<7);
//		PORTD |=  (1<<6) |(1<<7);
	DDRB |= (1 << 2) | (1 << 3) | (1 << 5);
	DDRB &= ~(1 << 4);
	PORTB |= (1 << 3) | (1 << 4) | (1 << 5);

	DDRD |= (1 << 6) | (1 << 7);
	DDRD &= ~(1 << 2);
	PORTD |= (1 << 2) | (1 << 6) | (1 << 7);
	//    while(1);
//	SPCR = 0X5E;
//    SPSR=0x00;
	SPCR = 0x50; //
	SPSR = 0x00; //setup SPI

	CSN_1;
	CE_0;
}

void nRF24L01_Config(void) {

	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e); // Set PWR_UP bit, enable CRC(2 bytes) &Prim:RX. RX_DR enabled..
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f);
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f); // Enable Pipe0
//	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_AW, 0x02); // Setup address width=5 bytes
//	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, RF_CHANNEL);
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP, 0x07); // TX_PWR:0dBm, Datarate:2Mbps,
}

void NRF24L01_Send(void) {
	uint8_t status = 0x00;

	CE_0;
	TX_Mode(RX_BUF);
//	while(IRQ);
	_delay_us(10);
	status = SPI_Read_Reg(STATUS); // (Read the status register to determine its data reception status)
//	printf("STATUS=0x%2x\r\n", status);
	if (status & TX_DS) /*tx_ds == 0x20*/
	{
//		printf("STATUS=0x%x\r\n", status);
//		printf("XXXXXX!!\r\n");
		SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x20); // ；(Clear TX, let IRQ low;)
	} else if (status & MAX_RT) /* status = 0x10*/
	{
//		printf("Send to achieve maximum number of sending!!\r\n");
		SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x10); //；	(Clear TX, let IRQ low;)
	}
	CE_1;
}

char seqcode = 0;
void NRF24L01_Receive(void) {
	uint8_t status = 0x01;
	uint8_t checkSum = 0;
	flag = 1;
	CE_0;
	_delay_us(10);
	status = SPI_Read_Reg(STATUS); // (Read the status register to determine its data reception status)
	if (status & 0x40) //(Receive Interrupt Flag bit)
			{
		SPI_Read_Buf(RD_RX_PLOAD, RX_BUF, TX_PLOAD_WIDTH); // read receive payload from RX_FIFO buffer
		SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x40); // (Clear TX, let IRQ low)
//		printf("Receiving data: channel : 0x%02x        data : 0x%02x\r\n",
//				RX_BUF[13], RX_BUF[14]);
		DEBUG("DATA : 0x%02X\n\r", RX_BUF[5]);
		DEBUG("BUFF = ");
		for (int i = 0; i < 24; i++) {
			DEBUG("%02X ", RX_BUF[i]);
		}
		DEBUG("\n\r");

////		channelId = RX_BUF[13];
////		dimmingLevel = RX_BUF[14];
//		DEBUG("seqcode = 0x%02X\n\r", seqcode);
//		DEBUG("RX_BUF  = 0x%02X\n\r", RX_BUF[5]);

//		if (seqcode < RX_BUF[5]) {
		if (RX_BUF[2] == 0x80) {
			if (RX_BUF[3] == 0x11) {
				for (int i = 2; i < (2 + 20); i++) {
					checkSum += (RX_BUF[i] & 0xFF);
				}
				if (RX_BUF[22] == (uint8) checkSum) {

					if (RX_BUF[21] == rotary__GetValue()) {

						if (RX_BUF[13] == 0x00) {
							if (g_DimmingLevel[0] != PWM_change(RX_BUF[14])) {
								flag = 0;
							}
//						g_DimmingLevel[0] = RX_BUF[14];
							g_DimmingLevel[0] = PWM_change(RX_BUF[14]);
//							DEBUG("channel 0 : 0x%02X\n\r", RX_BUF[14]);
						}
						if (RX_BUF[15] == 0x01) {
							if (g_DimmingLevel[1] != PWM_change(RX_BUF[16])) {
								flag = 0;
							}
//						g_DimmingLevel[1] = RX_BUF[16];
							g_DimmingLevel[1] = PWM_change(RX_BUF[16]);
//							DEBUG("channel 1 : 0x%02X\n\r", RX_BUF[16]);
						}
						if (RX_BUF[17] == 0x02) {
							if (g_DimmingLevel[2] != PWM_change(RX_BUF[18])) {
								flag = 0;
							}
//						g_DimmingLevel[2] = RX_BUF[18];
							g_DimmingLevel[2] = PWM_change(RX_BUF[18]);
//							DEBUG("channel 2 : 0x%02X\n\r", RX_BUF[18]);
						}
						if (RX_BUF[19] == 0x03) {
							if (g_DimmingLevel[3] != PWM_change(RX_BUF[20])) {
								flag = 0;
							}
//						g_DimmingLevel[3] = RX_BUF[20];
							g_DimmingLevel[3] = PWM_change(RX_BUF[20]);
//							DEBUG("channel 3 : 0x%02X\n\r", RX_BUF[20]);
						}
					}
					g_TestFlag = 1;
				}
			}

		}
	}
	CE_1;
//	_delay_ms(1);
//	if (flag == 0) {
//		NRF24L01_Send();
//		_delay_ms(1);
//		RX_Mode();
//	}
}

