// Warning: If using Microchip studio, the compiler may cause wacky behavior due to optimization
// Go to Project > Properties > Toolchain > Optimization and turn that shit off

#include "def.h" // System-specific definitions (e.g. CPU clock frequency)
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "gamepad.h"
#include "oled.h"
#include "menu.h"

#include <util/delay.h>
#include <inttypes.h> // Needed for uint8_t, uint16_t, etc.
#include <avr/io.h>

volatile char *mem_sram = (char *) 0x1800; // Start address for the SRAM
volatile char *mem_adc = (char *) 0x1400; // Start address for the ADC
volatile char *mem_oled_dat = (char *) 0x1200; // Start address for the OLED Data
volatile char *mem_oled_cmd = (char *) 0x1000; // Start address for the OLED commands

int main(void) {
	UART_init(DEF_UBBR);
	SRAM_init();
	adc_init();
	gamepad_init();
	oled_init();
	oled_reset();
	menu_init();
	
	Gamepad gp;
	Dir new_gp_dir;
	Dir prev_gp_dir;
	
	while(1){
		//oled_print("Test :) ", 5);
		menu_print();
		_delay_ms(100);
		oled_reset();
		
		// get gamepad position and update menu
		gp = read_gamepad();
		gp = calibrate_gamepad(gp);
		new_gp_dir = getJoystickDir(gp);
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

		//printf("Value: %i\n", gp.btn);

	}
	return 0;
}