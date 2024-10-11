#include "def.h"

#include <util/delay.h>
#include <inttypes.h> // Needed for uint16_t
#include <avr/io.h>


void adc_init(){
	// Set pin as output
	DDRD |= (1<<DDD4);
	
	// Toggle output pin on compare match with counter
	TCCR3A = (1<<COM3A0);
	
	// No clock prescaling on counter (i.e. update every clock cycle)
	// Set counter mode to CTC (clear on match)
	TCCR3B = (1<<WGM32)|(1<<CS30);
	
	// Value to be matched (0 = toggle output each cycle)
	OCR3A = 0;
}


volatile uint8_t adc_read(uint8_t channel){
		// Set up return data
		uint8_t digitalVal = 0;
		
		// Write anything to the ADC to trigger a conversion
		// Triggers the chip select and write control strobe when external memory interface is active
		volatile char* mem_adc = (char *) DEF_ADR_ADC;
		mem_adc[0] = 0;

		// Wait for conversion to finish
		uint16_t convTime = DEF_ADC_CONV_TIME;
		_delay_us(convTime);
		//_delay_ms(convTime);

		// Read requested channel
		if (channel <= DEF_ADC_CHANNELS){ // Returns 0 if channel number is invalid
			for (int i = 0; i <= channel; ++i){
				digitalVal = mem_adc[i];
			}
		}
		return digitalVal;
}