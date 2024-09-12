#include <avr/io.h>
#include "uart.h"

#define F_CPU 4915200UL
#define BAUD 9600
#define BAUD_PRESCALE ((F_CPU / (16UL * BAUD)) - 1)

/**
 * Initializes the UART communication interface.
 *
 * This function sets the baud rate for UART transmission and configures the UART
 * control registers to enable the transmitter and receiver. The data frame size
 * is configured to 8 bits with 1 stop bit and no parity.
 */
void UART_init(void) {
    UBRR0H = (BAUD_PRESCALE >> 8);  // Set high byte of baud rate
    UBRR0L = BAUD_PRESCALE;         // Set low byte of baud rate
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // Enable transmitter and receiver
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // Set data frame size to 8 bits
}

/**
 * Transmits a single byte of data via UART.
 *
 * This function waits until the UART data register is empty, and then transmits
 * the given data byte through the UART.
 *
 * @param data The byte of data to be transmitted.
 */
void UART_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));  // Wait for the data register to be empty
    UDR0 = data;  // Load the data into the register for transmission
}

/**
 * Receives a single byte of data via UART.
 *
 * This function waits until data is available in the UART data register, and
 * then returns the received byte.
 *
 * @return The received byte of data.
 */
char UART_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));  // Wait until data is available to read
    return UDR0;  // Return the received data
}

/**
 * Transmits a character via UART and handles newline characters.
 *
 * This function transmits a character through UART. If the character is a newline
 * ('\n'), it also transmits a carriage return ('\r') before the newline to ensure
 * proper formatting for systems that require it.
 *
 * @param c The character to be transmitted.
 * @param stream The output stream (unused in this implementation).
 * @return Always returns 0.
 */
int UART_putchar(char c, FILE *stream) {
    if (c == '\n') {
        UART_transmit('\r');
    }
    UART_transmit(c);
    return 0;
}