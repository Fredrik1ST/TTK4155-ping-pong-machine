// Warning: If using Microchip studio, the compiler may cause wacky behavior due to optimization
// Go to Project > Properties > Toolchain > Optimization and turn that shit off

#include "def.h" // System-specific definitions (e.g. CPU clock frequency)
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "gamepad.h"
#include "myDelay.h" // Homemade delays
#include "oled.h"

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
	
	while(1){
		oled_reset();
		oled_pos(0x0, 0x0);
		for (char i = 0; i<128; i++){
			if (i%4 == 0){
				oled_write_data(0x1);
			}else{
				oled_pos{0x0, i};
			}
	}
	return 0;
}