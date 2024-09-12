#include <avr/io.h>
#include <util/delay.h>

/**
 * Generates a square wave signal on PORTB0.
 *
 * This function sets the PORTB0 pin as an output and continuously toggles it to
 * produce a square wave signal with a 500 ms delay between toggles (1Hz)
 */
void square_wave_test(void) {
    DDRB |= (1 << DDB0);  // Set PORTB0 as output
    while (1) {
        PORTB ^= (1 << PORTB0);  // Toggle PORTB0
        _delay_ms(500);          // Delay for square wave
    }
}
