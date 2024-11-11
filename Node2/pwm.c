#include <sam.h>
#include <stdio.h>

#define PIN_MOTOR_PWM 12
#define PIN_MOTOR_DIR 23

#define PIN_SERVO_PWM 13

void pwm_init(void){
	REG_PMC_PCER1 |= PMC_PCER1_PID36;	// Enable PWM clock in Power Management Controller
	REG_PWM_CLK |= PWM_CLK_PREB(0);		// Use clock MCK as input for PWM controller (MCK = F_CPU/2 = 42MHz)
	REG_PWM_CLK |= PWM_CLK_DIVB(42);	// Divide MCK by 42 (=> 1MHz)
	
	// PWMH0 -> pin PB12 (peripheral B) - Table 38-2 in datasheet
	REG_PIOB_PDR |= PIO_PDR_P12;	// Disable PIO controller for pin
	REG_PIOB_ABSR |= PIO_ABSR_P12;	// Enable peripheral control of pin
	
	
	// ===================================================================
	// Traversal motor
	REG_PWM_CMR0 = PWM_CMR_CALG | PWM_CMR_CPOL | PWM_CMR_CPRE_CLKB;
	REG_PWM_CPRD0 = 100; // 100/1Mhz = 100us ts = 10kHz
	REG_PWM_CDTY0 = 0; // Default duty cycle
	REG_PWM_ENA = PWM_ENA_CHID0;
	
	REG_PMC_PCER0 |= PMC_PCER0_PID11; // Enable PWM clock for PIOA
	REG_PIOC_PER |= (1 << PIN_MOTOR_DIR); // Enable direction pin
	REG_PIOC_OER |= (1 << PIN_MOTOR_DIR); // Enable output for direction pin
	REG_PIOC_ODSR &= ~(1 << PIN_MOTOR_DIR);
	
	
	// ===================================================================
	// Servo
	// PWMH5 -> pin PB13 (peripheral B)
	REG_PIOB_PDR |= PIO_PDR_P13;	
	REG_PIOB_ABSR |= PIO_ABSR_P13;	
	
	REG_PWM_CMR1 = PWM_CMR_CALG | PWM_CMR_CPOL | PWM_CMR_CPRE_CLKB; // Left alignment, default level low, MCK without prescaler
	REG_PWM_CPRD1 = 20000;		// Waveform period = (1*CPRD / PWM_CLK) = 20000/1MHz = 20ms
	REG_PWM_CDTY1 = 1500;		// Default waveform duty cycle = 1.5ms (middle)
	REG_PWM_ENA = PWM_ENA_CHID1;
	
}

void pwm_setDutyCycle_servo(uint32_t dutyCycle_us){
	uint32_t minVal = 900;		//0.9ms
	uint32_t maxVal = 2100;		//2.1ms
	uint32_t output = dutyCycle_us;
	if (output > maxVal){
		output = maxVal;
	}
	if (output < minVal){
		output = minVal;
	}
	REG_PWM_CDTY1 = output;
}

void pwm_setSpeed_motor(int8_t pct){
	
	REG_PWM_CDTY0 = (uint32_t) abs(pct);
	
	if (pct > 0){
		REG_PIOC_SODR |= (1 << PIN_MOTOR_DIR);
	}else{
		REG_PIOC_CODR |= (1 << PIN_MOTOR_DIR);
	}
}