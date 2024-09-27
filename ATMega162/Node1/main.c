#define F_CPU 4915200 // Set clock frequency to 4,9MHz
#define BAUD 9600
#define MY_UBBR F_CPU/16/BAUD - 1 // UART baud rate from datasheet
#define ADC_CONV_TIME 1000000*9*4*2/F_CPU // ADC Conversion time [microseconds]

#include <util/delay.h>
#include <inttypes.h> // Needed for uint8_t, uint16_t, etc.
#include <avr/io.h>
#include "uart.h"
#include "sram.h"
#include "adc.h"

volatile char *mem_sram = (char *) 0x1800; // Start address for the SRAM
volatile char *mem_adc = (char *) 0x1400; // Start address for the ADC
volatile char *mem_oled_dat = (char *) 0x1200; // Start address for the OLED Data
volatile char *mem_oled_cmd = (char *) 0x1000; // Start address for the OLED commands


int main(void) {
	UART_init(MY_UBBR);
	SRAM_init();
	adc_init();
	
	while(1){
		
		mem_adc[0] = 0; // Write anything to trigger the write strobe + chip select to the ADC.
		  
		_delay_ms(1); // Give ADC time to convert

		volatile uint8_t adc_ch0 = mem_adc[0]; // Read first channel from ADC
		
		_delay_ms(1);

		volatile uint8_t adc_ch1 = mem_adc[2]; // Read second channel from ADC

		printf("ADC0: %02X    -    ADC1: %02X\r\n\r\n", adc_ch0, adc_ch1);
		  
		_delay_ms(100);

	}
	return 0;
}