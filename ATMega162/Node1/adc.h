#ifndef ADC_H
#define ADC_H


/**
 * Initialize the Analog-Digital-Converter (ADC)
 * 
 * Use hardware counter 3 (special function of pin PD4) to generate a PWM signal that acts as the ADC's clock
 */
void adc_init();


/**
 * Triggers a reading from the ADC
 *
 * @returns 8-bit value from requested ADC channel
 * @returns 0 if the channel number is invalid
 */
uint8_t adc_read(uint8_t channel);
	
#endif