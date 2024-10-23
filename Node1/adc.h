/**
 * @file adc.h
 * @brief Analog-to-digital converter
 * 
 * Reading from channel n is done by triggering the write control strobe, then pulsing the read control strobe n times.
 * The control strobes are automatically triggered when reading/writing by the ATmega162 when the external memory interface (XMEM) is activated.
 */

#ifndef ADC_H
#define ADC_H

#include "def.h"

/**
 * Initialize the Analog-Digital-Converter (ADC)
 * 
 * Use hardware counter 3 (special function of pin PD4) to generate a PWM signal that acts as the ADC's clock
 */
void adc_init(void);


/**
 * Triggers a reading from the ADC
 *
 * @returns 8-bit value from requested ADC channel
 * @returns 0 if the channel number is invalid
 */
uint8_t adc_read(uint8_t channel);
	
#endif