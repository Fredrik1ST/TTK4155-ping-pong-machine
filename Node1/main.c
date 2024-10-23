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

int main(void) {
	UART_init(DEF_UBBR);
	SRAM_init();
	adc_init();
	gamepad_init();
	oled_init();
	oled_reset();
	menu_init();
	//mcp2515_init();
	mcp2515_init_loopback(); // TODO - Delete and replace with mcp2515_init(); after CAN controller test
	
	uint8_t sendVal = 0; // TODO: Delete after CAN controller test
	uint8_t sendVal2 = 200; // TODO: Delete after CAN controller test
	
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
		// Test MCP2515 CAN Controller in loopback mode
		_delay_ms(500);
		sendVal += 1;
		mcp2515_write(0xAA, sendVal);
		_delay_ms(500);
		uint8_t recVal = mcp2515_read(0xAA);
		printf("Sent: %02X    -    Received: %02X \r\n\r\n", sendVal, recVal);
		
	}
	return 0;
}