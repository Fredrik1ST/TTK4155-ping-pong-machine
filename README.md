# Ping-Pong Machine
Group 33's semester project for TTK4155 (Embedded and Industrial Computer Systems) at NTNU, Autumn 2024

The project is divided in two main parts:

**Node 1**: A breadboard with an Atmel AVR Atmega162. Manages the user interface, e.g. reading joysticks inputs via ADC and displaying graphics on an OLED.
**Node 2**: 


## Lab 1 - Initial assembly of Atmega162 and RS-232
- V<sub>a</sub> = 9V. Reduced to 5V by an LMN7805 voltage regulator.
- Button with pull-up resistor connected to Atmega162 reset pin.
- 4.9152MHz crystal oscillator connected to XTAL pins of Atmega162.
- JTAG Interface for Atmel-ICE connected and tested using Microchip Studio.

At the start of main.c, the clock frequency, baud rate and other important parameters are defined.
```c
#define F_CPU 4915200 // Set clock frequency to 4,9MHz
#define BAUD 9600
#define MY_UBBR F_CPU/16/BAUD - 1 // USART Band Rate Register
```

A Max233 RS-232 ICU is used to establish 2-way UART communication to/from Atmega162. 

To be able to transmit data on the serial line using the printf function, the fdevopen function is run in the UART driver.
```c
void UART_init(unsigned int ubrr)
{
	/* Set baud rate ( */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	// Make the transmit & receive functions write to a stream / file
	// This makes the printf function write to the serial line
	fdevopen(UART_transmit, UART_receive);
}
```
