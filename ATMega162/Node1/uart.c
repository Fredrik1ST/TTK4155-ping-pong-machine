#include <stdio.h>
#include <avr/io.h>


/**
 * Receives a single byte of data via UART.
 *
 * This function waits until data is available in the UART data register, and
 * then returns the received byte.
 *
 * @return The received byte of data.
 */
int UART_receive(FILE * file) {
    while (!(UCSR0A & (1 << RXC0)));  // Wait until data is available to read
    return UDR0;  // Return the received data
}


/**
 * Transmits a single byte of data via UART.
 *
 * This function waits until the UART data register is empty, and then transmits
 * the given data byte through the UART.
 *
 * @param data The byte of data to be transmitted.
 */
int UART_transmit(char data, FILE * file) {
    while (!(UCSR0A & (1 << UDRE0)));  // Wait for the data register to be empty
    UDR0 = data;  // Load the data into the register for transmission
	
	return 0;
}


/**
 * Initializes the UART communication interface.
 *
 * This function sets the baud rate for UART transmission and configures the UART
 * control registers to enable the transmitter and receiver. The data frame size
 * is configured to 8 bits with 1 stop bit and no parity.
 */
void UART_init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	// Make the transmit & receive functions write to a stream / file
	fdevopen(UART_transmit, UART_receive);
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
/*
int UART_putchar(char c) {
    if (c == '\n') {
        UART_transmit('\r');
    }
    UART_transmit(c);
    return 0;
}
*/


/*
* Flushes the receive buffer
*/
/*
void USART_Flush(void)
{
	unsigned char dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}
*/