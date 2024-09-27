#include <util/delay.h>
#include <inttypes.h> // Needed for uint16_t
#include <avr/io.h>

#define ADC_START_ADR 0x1400 // From 0x1400 to 0x17FF
#define ADC_CHANNELS 4
#define ADC_CONV_TIME 9*ADC_CHANNELS*2/F_CPU*1000000 // Conversion time [microseconds]


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
		// This triggers the chip select and write control strobe when external memory is being used
		volatile char* mem_adc = (char *) ADC_START_ADR;
		mem_adc[0] = 0;

		// Wait for conversion to finish
		uint16_t convTime = ADC_CONV_TIME;
		_delay_us(convTime);

		// Read requested channel
		if (channel <= ADC_CHANNELS){ // Returns 0 if channel number is invalid
			for (int i = 0; i <= channel; ++i){
				digitalVal = mem_adc[i];
			}
		}
		return digitalVal;
}