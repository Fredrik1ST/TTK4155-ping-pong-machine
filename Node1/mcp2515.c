#include <inttypes.h> // Needed for uint8_t, uint16_t, etc.
#include <avr/io.h>
#include <stdio.h>
#include <avr/delay.h>

#include "mcp2515.h"
#include "spi.h"
#include "def.h"

// Stuff related to bit timing (see datasheet)
#define MCP2515_F_CPU 16000000	// 16MHz clock connected to controller
#define BRP 0x3					// Baud Rate Prescaling (3+1 => TQ = 500ns)
#define TQ 0x2 * (BRP + 0x1) / MCP2515_F_CPU	// Time Quanta = Time between each bit
#define PROPSEG 0x01			// PROPSEG*TQ = Time compensation for delay between CAN nodes (see fig. 5-2)
#define PS1 0x06				// Propagation segment 1 = (PS1+1)*TQ = Positive time compensation for edge phase errors 
#define PS2 0x05				// Propagation segment 2 = (PS2+1*TQ) = Negative time compensation for edge phase errors


void mcp2515_init(){
	spi_init();
	// Set chip select pin as output, then keep it inactive (pin low)
	DEF_CAN_CS_ENABLE;
	DEF_CAN_CS_OFF;
	
	mcp2515_reset();
	uint8_t mode = mcp2515_read(MCP_CANSTAT);
	if ((mode & MODE_MASK) != MODE_CONFIG){
		printf ("ERROR %02X: MCP2515 is NOT in config mode after reset!\r\n", mode);
	}else{
	printf("MCP2515 entered config mode\r\n");
	}
	
	//mcp2515_write(MCP_CNF1, BRP); // Config CNF1, SJW = 1, BRP (Baud Rate Prescaling) = 3+1
	
	//mcp2515_write(MCP_CNF2, ((PS1 << 3) | PROPSEG)); //Config CNF2, PS1 = 7+1, PropSeg = 1+1
	
	//mcp2515_write(MCP_CNF3, (PS2 | BTLMODE)); // Write to CNF3: PS2 = 6+1, BTLMode ON (muy importante)

	// Configure bit timing
	mcp2515_write(MCP_CNF1, 0x03);	// BRP (Baud Rate Prescaling) = 3+1
	mcp2515_write(MCP_CNF2, 0xB1);	// Propagation Segment PS = 1+1, PS1 = 7+1
	mcp2515_write(MCP_CNF3, 0x85);	// PS2 = 5+1, BTLMode ON (muy importante)

	mcp2515_bit_modify(MCP_CANINTE, 0b00000001, 0); // Enable interrupt bit when Receive Buffer 0 has new data
	mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0); // Reset interrupt bit in buffer 0 (must be done to receive next 8 bits)
	

	mcp2515_write(MCP_CANCTRL, MODE_NORMAL); // Enter normal mode
	mode = mcp2515_read(MCP_CANSTAT);
	if ((mode & MODE_MASK ) != MODE_NORMAL) {
		printf("ERROR: MCP2515 is NOT in normal mode!\r\n");
	}else{
		printf("MCP2515 entered normal mode\r\n");
	}
}


void mcp2515_init_loopback(){ // Init MCP in loopback mode (for testing)
	spi_init();
	DEF_CAN_CS_ENABLE;
	DEF_CAN_CS_OFF;
	
	// Reset and verify config mode
	mcp2515_reset();
	uint8_t mode = mcp2515_read(MCP_CANSTAT);
	if ((mode & MODE_MASK) != MODE_CONFIG){
		printf (" ERROR %02X: MCP2515 is NOT in config mode after reset!\r\n", mode);
	}else{
		printf(" MCP2515 is working my dudes! :) \r\n");
	}
	
	// Set to loopback mode and verify
	mcp2515_write(0xF, MODE_LOOPBACK);
	mode = mcp2515_read(MCP_CANSTAT);
	if ((mode & MODE_MASK) != MODE_LOOPBACK){
		printf ("ERROR: MCP2515 did not enter loopback mode\r\n");
	}else{
		printf("MCP2515 Entered loopback mode B-)\r\n");
	}
}


uint8_t mcp2515_read(uint8_t adr){
	DEF_CAN_CS_ON;
	spi_send(MCP_READ);
	spi_send(adr);
	uint8_t result = spi_recv();
	DEF_CAN_CS_OFF;
	return result;
}


void mcp2515_write(uint8_t adr, uint8_t data){
	DEF_CAN_CS_ON;
	spi_send(MCP_WRITE);
	spi_send(adr);
	spi_send(data);
	DEF_CAN_CS_OFF;
}


void mcp2515_request_to_send(uint8_t buffer){
	DEF_CAN_CS_ON;
	spi_send(buffer);
	DEF_CAN_CS_OFF;
}


void mcp2515_bit_modify(uint8_t adr, uint8_t mask, uint8_t data){
	DEF_CAN_CS_ON;
	spi_send(MCP_BITMOD); 
	spi_send(adr);
	spi_send(mask);
	spi_send(data);
	DEF_CAN_CS_OFF;
}


void mcp2515_reset(){
	DEF_CAN_CS_ON;
	spi_send(MCP_RESET);
	DEF_CAN_CS_OFF;
	_delay_ms(10);
}


uint8_t mcp2515_read_status()
{
	DEF_CAN_CS_ON;
	spi_send(MCP_READ_STATUS);
	uint8_t status = spi_recv();
	DEF_CAN_CS_OFF;
	return status;
}


uint8_t mcp2515_rx_status(){
	DEF_CAN_CS_ON;
	spi_send(MCP_RX_STATUS);
	uint8_t status = spi_recv();
	DEF_CAN_CS_OFF;
	return status;
}


uint8_t mcp_read_rx0_buffer(){
	uint8_t result;
	DEF_CAN_CS_ON;
	spi_send(MCP_READ_RX0);
	result = spi_recv();
	DEF_CAN_CS_OFF;
	return result;
}


void mcp_write_tx0_buffer(uint8_t data){
	DEF_CAN_CS_ON;
	spi_send(MCP_LOAD_TX0);
	spi_send(data);
	DEF_CAN_CS_OFF;
}