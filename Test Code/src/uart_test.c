#include <avr/io.h>
#include "uart.h"

#define F_CPU 4915200UL
#define BAUD 9600
#define BAUD_PRESCALE ((F_CPU / (16UL * BAUD)) - 1)

void UART_init(void) {
    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

void UART_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

char UART_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

int UART_putchar(char c, FILE *stream) {
    if (c == '\n') {
        UART_transmit('\r');
    }
    UART_transmit(c);
    return 0;
}
