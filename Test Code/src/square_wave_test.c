#include <avr/io.h>
#include <util/delay.h>

void square_wave_test(void) {
    DDRB |= (1 << DDB0);  // Set PORTB0 as output
    while (1) {
        PORTB ^= (1 << PORTB0);  // Toggle PORTB0
        _delay_ms(500);          // Delay for square wave
    }
}
