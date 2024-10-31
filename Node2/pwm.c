#include <sam.h>

void pwm_init(void){
	REG_PMC_PCER1 |= PMC_PCER1_PID36;	// Enable PWM clock in Power Management Controller
	REG_PWM_CLK |= PWM_CLK_PREB(0);		// Use clock MCK as input for PWM controller (MCK = F_CPU/2 = 42MHz)
	REG_PWM_CLK |= PWM_CLK_DIVB(42);		// Divide MCK by 42 (=> 1MHz)
	
	// PWMH5 -> pin PC19 (peripheral B) according to table 38-2
	REG_PIOC_PDR |= PIO_PDR_P19;	// Disable PIO controller for pin
	REG_PIOC_ABSR |= PIO_ABSR_P19;	// Enable peripheral control of pin
	
	//REG_PWM_CMR5 = PWM_CMR_CALG | PWM_CMR_CPOL | PWM_CMR_CPRE_CLKA; // Left alignment, default level low, MCK without prescaler
	REG_PWM_CPRD5 = 20000;		// Waveform period = (1*CPRD / PWM_CLK) = 20000/1MHz = 20ms
	REG_PWM_CDTY5 = 1500;		// Default waveform duty cycle = 1.5ms (middle)
	REG_PWM_ENA = PWM_ENA_CHID5;
}

void(pwm_setDutyCycle)(uint32_t dutyCycle_us){
	uint32_t minVal = 900;		//0.9ms
	uint32_t maxVal = 2100;		//2.1ms
	uint32_t output = dutyCycle_us;
	if (output > maxVal){
		output = maxVal;
	}
	if (output < minVal){
		output = minVal;
	}
	REG_PWM_CDTY5 = output;
}