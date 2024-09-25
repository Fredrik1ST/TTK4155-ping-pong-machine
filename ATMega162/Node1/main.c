/*
 * Node1.c
 *
 * Created: 12/09/2024 16:37:04
 * Author : User
 */

#define F_CPU 4915200 // Set clock frequency to 4,9MHz
#define BAUD 9600
#define MY_UBBR F_CPU/16/BAUD - 1
#define ADC_CONV_TIME 1000000*9*4*2/F_CPU // ADC Conversion time [microseconds]

#include <inttypes.h> // Needed for uint8_t, uint16_t, etc.
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "sram.h"

volatile char *mem_sram = (char *) 0x1800; // Start address for the SRAM
volatile char *mem_adc = (char *) 0x1400; // Start address for the ADC
volatile char *mem_oled = (char *) 0x1000; // Start address for the OLED

int main(void) {
	UART_init(MY_UBBR);
	SRAM_init();
	
	while(1){
		
		  mem_adc[0] = 0; // Write anything to trigger the write strobe + chip select to the ADC.
		  
		  _delay_us(ADC_CONV_TIME); // Give the ADC time to convert

		  uint8_t adc_ch1 = mem_adc[0]; // Read first channel from ADC
		  
		  _delay_us(ADC_CONV_TIME)

		  uint8_t adc_ch2 = mem_adc[8] // Read second channel from ADC

		  printf("ADC1: %02X    -    ADC2: %02X\n\n", adc_ch1, adc_ch2);
	}
	return 0;
}