#include <sam.h>

#define PIN_CH A
#define PIN_NR 11

void solenoid_init(){
	REG_PMC_PCER0 |= PMC_PCER0_PID11; // Enable PWM clock for PIOA
	REG_PIOA_PER |= (1 << PIN_NR); // Enable pin
	REG_PIOA_OER |= (1 << PIN_NR); // Enable output for pin PA10
	REG_PIOA_ODSR &= ~(1 << PIN_NR);
}

void solenoid_kick(){
	REG_PIOA_SODR |= (1 << PIN_NR);
	//REG_PIOA_ODSR |= (1 << PIN_NR);
}

void solenoid_retract(){
	REG_PIOA_CODR |= (1 << PIN_NR);
	//REG_PIOA_ODSR &= ~(1 << PIN_NR);
}