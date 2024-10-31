// Warning: Compiler optimization in Microchip Studio may cause wacky behavior
// Go to Project > Properties > Toolchain > Optimization if having trouble with output or timing on address pins

#include <util/delay.h>
#include <inttypes.h> // Needed for uint8_t, uint16_t, etc.
#include <avr/io.h>

#include "def.h" // System-specific definitions (e.g. CPU clock frequency, SRAM addresses)
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "gamepad.h"
#include "oled.h"
#include "menu.h"
#include "spi.h"
#include "mcp2515.h"
#include "can.h"

int main(void) {
	UART_init(DEF_UBBR);
	SRAM_init();
	adc_init();
	gamepad_init();
	oled_init();
	oled_reset();
	menu_init();

	can_init();
	
	Gamepad gp;
	Dir new_gp_dir;
	Dir prev_gp_dir;
	
	while(1){
		// =================================================
		// Read gamepad inputs
		
		gp = read_gamepad();
		gp = calibrate_gamepad(gp);
		new_gp_dir = getJoystickDir(gp);
		
		
		// =================================================
		// Display menu on OLED
		
		oled_reset();
		if (new_gp_dir == UP && prev_gp_dir != UP ){
			menu_moveCursor(-1);
		}
		else if (new_gp_dir == DOWN && prev_gp_dir != DOWN ){
			menu_moveCursor(1);
		}
		if(gp.btn){
			menu_selectPage();
		}
		prev_gp_dir = new_gp_dir;

		
		// =================================================
		// Test CAN communication with node 2
		CanMsg* msgOut;
		msgOut->id = 0xA5;
		msgOut->len = 2;
		msgOut->data[0] = gp.pos_x;
		msgOut->data[1] = gp.pos_y;
		can_send(msgOut);
		printf("Sent: ID: %02X    -    Len: %02X    -    Dat: %02X %02X \r\n\r\n", msgOut->id, msgOut->len, msgOut->data[0], msgOut->data[1]);
		
		/*
		CanMsg* msgIn;
		can_recv(msgIn);
		uint8_t recId = msgIn->id;
		uint8_t recVal = msgIn->data[0];
		uint8_t recSize = msgIn->len;
		printf("Recv: ID: %02X    -    Len: %02X    -    Dat: %d %d \r\n\r\n", msgIn->id, msgIn->len, msgIn->data[0], msgIn->data[1]);
		*/

	}
	return 0;
}