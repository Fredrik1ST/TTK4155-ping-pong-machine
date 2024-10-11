
#include <inttypes.h>
#include "def.h"

#ifndef F_CPU 
#define F_CPU 4915200
#endif

void delay_us(uint32_t us){
	uint8_t loopCycleTime = 26;
	volatile int32_t cycles = us * (F_CPU / 1000000) / loopCycleTime; // calculate the number of cycles for 1 us
	
	for (int i = 0; i < cycles; i++){
		
	}
}

void delay_ms(uint32_t us){
	uint8_t loopCycleTime = 26;
	volatile int32_t cycles = us * 1000* (F_CPU / 1000000) / loopCycleTime; // calculate the number of cycles for 1 us
	
	for (int i = 0; i < cycles; i++){
		
	}
}


