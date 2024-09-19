/*
 * Node1.c
 *
 * Created: 12/09/2024 16:37:04
 * Author : User
 */

#define F_CPU 4915200 // Set clock frequency to 4,9MHz
#define BAUD 9600
#define MY_UBBR F_CPU/16/BAUD - 1

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "sram.h"

volatile char *mem_xram = (char *) 0x1800; // Start address for the SRAM
volatile char *mem_adc = (char *) 0x1400; // Start address for the ADC
volatile char *mem_oled = (char *) 0x1000; // Start address for the OLED

int main(void) {
	UART_init(MY_UBBR);
	SRAM_init();
	
	while(1){
		SRAM_test();
		_delay_ms(2000);
	}
	return 0;
}