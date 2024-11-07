#include <sam.h>
#include "encoder.h"
#include <stdio.h>

void encoder_init(){
	//REG_PMC_PCER0 |= PMC_PCER0_PID27;
	//REG_PMC_PCER0 |= PMC_PCER0_PID28;
	
	//REG_PMC_PCER0 |= PMC_PCER0_PID29;	// Enable PWM clock for TC2 in Power Management Controller
	REG_PMC_PCER1 |= PMC_PCER1_PID33; // TC6
	
	REG_TC2_WPMR |= ('T'<<24)|('I'<<16)|('M'<8); // Writes "TIM" as ASCII... why God, why.
	REG_TC2_WPMR &= ~(0x1); // Disable first bit to allow writing
	
	REG_PIOC_PDR |= PIO_PDR_P25;	// Disable PIO controller for pin
	REG_PIOC_PDR |= PIO_PDR_P26;	// Disable PIO controller for pin
	REG_PIOC_ABSR |= PIO_ABSR_P25;	// Enable peripheral B control of pin
	REG_PIOC_ABSR |= PIO_ABSR_P26;	// Enable peripheral B control of pin
	
	// TC_BMR_POSEN  enable position mode
	// TC_BMR_QDEN   enable quadrature decoder
	REG_TC2_BMR |= TC_BMR_POSEN | TC_BMR_QDEN;
	
	// TC_CMR_ETRGEDG_RISING  detection on rising edge
	// TC_CMR_ABETRG		  Velg input A som trigger
	// TC_CMR_TCCLKS_XC0	  Velg klokke 0
	REG_TC2_CMR0 |= TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG | TC_CMR_TCCLKS_XC0;
	
	REG_TC2_CCR0 |= 0x5; // Reset & start counter
}


uint32_t encoder_read(){
	
	uint32_t registerPos = REG_TC2_CV0;
	uint32_t registerDir = (REG_TC2_QISR & (1 << 8));
	
	if (registerPos > 0xFFFFF){
		encoder_reset();
	}

	return registerPos;
}


void encoder_reset(){
	REG_TC2_CCR0 |= 0x5;
}