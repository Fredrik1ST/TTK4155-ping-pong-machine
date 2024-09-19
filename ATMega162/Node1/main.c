/*
 * Node1.c
 *
 * Created: 12/09/2024 16:37:04
 * Author : User
 */

#define F_CPU 4915200 // Set clock frequency to 4,9MHz
#define BAUD 9600
#define MY_UBBR F_CPU/16/BAUD - 1

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
		  mem_adc[0] = 0x04; // Skriver 0x04 til ADC, altså vi sier til ADC at vi skal hente ut info fra CH1
		  
		  _delay_us(20);

		  uint8_t value = mem_adc[0]; // Read 8-bit value f
		  printf("ADC: %02X\n\n", value);

		  _delay_ms(500);
	}
	return 0;
}