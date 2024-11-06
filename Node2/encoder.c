#include <sam.h>
#include "encoder.h"

void encoder_init(){
	REG_PMC_PCER0 |= PMC_PCER0_PID29;	// Enable PWM clock for TC2 in Power Management Controller
	
	REG_PIOC_PDR |= PIO_PDR_P25;	// Disable PIO controller for pin
	REG_PIOC_ABSR |= PIO_ABSR_P25;	// Enable peripheral B control of pin
	REG_PIOC_PDR |= PIO_PDR_P26;	// Disable PIO controller for pin
	REG_PIOC_ABSR |= PIO_ABSR_P26;	// Enable peripheral B control of pin
	
	
	// TC_BMR_POSEN  enable position mode
	// TC_BMR_QDEN   enable quadrature decoder
	REG_TC2_BMR |= TC_BMR_POSEN | TC_BMR_QDEN;
	
	// TC_CMR_ETRGEDG_RISING  detection on rising edge
	// TC_CMR_ABETRG		  Velg input A som trigger
	// TC_CMR_TCCLKS_XC0	  Velg klokke 0
	REG_TC2_CMR0 |= TC_CMR_ETRGEDG_RISING | TC_CMR_ABETRG | TC_CMR_TCCLKS_XC0;
}


int32_t encoder_read(Encoder e){
	
	uint32_t registerPos = REG_TC2_CV0;
	uint32_t registerDir = (REG_TC2_QISR & (1 << 8));
	
	//e.pos = e.pos + registerPos * (2 * registerDir - 1);
	e.pos = registerPos;
	
	REG_TC2_CCR0 = 1; // Reset counter
	return e.pos;
}


void encoder_reset(Encoder e){
	e.pos =  0;
}