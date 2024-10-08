// Warning: If using Microchip studio, the compiler may cause wacky behavior due to optimization
// Go to Project > Properties > Toolchain > Optimization and turn that shit off

#include "def.h" // System-specific definitions (e.g. CPU clock frequency)
#include "uart.h"
#include "sram.h"
#include "adc.h"

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
	
	while(1){

		adc_read(1);
		_delay_ms(500);
		adc_read(2);
		_delay_ms(500);
		
		mem_adc[0] = 0;
		
		_delay_ms(1); // Give ADC time to convert
		
		uint8_t adc_ch0 = mem_adc[0]; // Read first channel from ADC
		
		uint8_t adc_ch1 = mem_adc[0]; // Read second channel from ADC
		
		uint8_t adc_ch2 = mem_adc[0];
		
		uint8_t adc_ch3 = mem_adc[0];
		
		printf("ADC0: %02X    -    ADC1: %02X\r\n\r\n", adc_ch0, adc_ch1);
		
		_delay_ms(500);

	}
	return 0;
}