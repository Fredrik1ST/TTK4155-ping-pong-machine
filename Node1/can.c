#include "mcp2515.h"
#include "can.h"

/*
* Initialize CAN Bus controller via SPI, in normal mode
*/
void can_init(void){
	mcp2515_init();
}


/*
* Receive a CAN message via MCP2515
* @param	msg		Received CAN Message
*/
void can_recv(CanMsg* msg){
	msg->id |= (mcp2515_read(MCP_RXB0SIDH)<<3); // Read top 8 bits
	msg->id |= (mcp2515_read(MCP_RXB0SIDL)>>5); // Read bottom 3 bits
	msg->len = mcp2515_read(MCP_RXB0DLC);		// Read message length
	for (int i = 0; i<msg->len; i++){
		msg->data[i] = mcp2515_read(MCP_RXB0D0 + i);
	}
	mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0); // Reset receive buffer
}


/*
* Send a CAN message via MCP2515
* @param	msg		CAN message to send
*/
void can_send(CanMsg* msg){
	mcp2515_write(TXB0SIDH, msg->id>>3);	// Write top 8 bits of ID
	mcp2515_write(TXB0SIDL, msg->id<<5);	// Write bottom 3 bits of ID (to MSb of register)
	mcp2515_write(TXB0DLC, msg->len);		// Write length of message
	for(int i=0; i<msg->len; i++){
		mcp2515_write(TXB0D0 + i, msg->data[i]);
	}
	mcp2515_request_to_send(MCP_RTS_TX0); // Request to send via transmit buffer 0
}