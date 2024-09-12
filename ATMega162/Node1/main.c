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


int main(void) {
	UART_init(MY_UBBR);
	while(1){
		printf("Ting funker kanskje");
		_delay_ms(1000);
	}
	return 0;
}