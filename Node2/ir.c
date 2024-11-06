#include "ir.h"
#include <sam.h>

#define PIN_CH A
#define PIN_NR 10

void IR_init(void){
	REG_PMC_PCER0 |= PMC_PCER0_PID11; // Enable PWM clock for PIOA
	REG_PIOA_PER |= (1 << PIN_NR); // Enable pin
	REG_PIOA_ODR |= (1 << PIN_NR); // Disable output for pin PA10
	REG_PIOA_PUDR |= (1 << PIN_NR); // Disable pullup
	
	REG_PIOA_SCDR |= (0x8); // PIO Slow clock divider debounce
	REG_PIOA_DIFSR |= (1 << PIN_NR); // Enable debouncing filter
}

uint8_t getIR(){
	return (REG_PIOA_PDSR & (1 << PIN_NR)) >> PIN_NR;
}