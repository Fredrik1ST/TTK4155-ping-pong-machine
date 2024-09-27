#include <stdio.h>
#include <avr/io.h>


int UART_receive(FILE * file) {
    while (!(UCSR0A & (1 << RXC0)));  // Wait until data is available to read
    return UDR0;  // Return the received data
}


int UART_transmit(char data, FILE * file) {
    while (!(UCSR0A & (1 << UDRE0)));  // Wait for the data register to be empty
    UDR0 = data;  // Load the data into the register for transmission
	
	return 0;
}


void UART_init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit, synchronous mode */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	// Make the transmit & receive functions write to a stream / file
	fdevopen(UART_transmit, UART_receive);
}