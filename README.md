# Ping-Pong Machine
Group 33's semester project for TTK4155 (Embedded and Industrial Computer Systems) at NTNU, Autumn 2024.

The purpose of this project is to assemble and program a machine for a mechanical game of single-player pong. It consists of two main nodes:

**Node 1**: A breadboard with an [Atmel AVR ATmega162](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2513-8-bit-AVR-Microntroller-ATmega162_Datasheet.pdf). Manages the user interface, e.g. reading joysticks inputs via ADC and displaying graphics on an OLED.

**Node 2**: An [Arduino Due](https://docs.arduino.cc/hardware/due) (based on [Atmel ATSAM3X8E](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf)) to control actuators & IO. Communicates with node 1 using CAN bus.

This document is meant as an informal progress report where we jot down anything of note.

[comment]: <![Welcome to byggern.](https://i.imgur.com/ccaTs94.mp4)> 
<p style="font-family: Papyrus, Charcoal, fantasy; color:red">🗲 Byggern Begins 🗲</p>



## Lab 1 - Initial assembly of Atmega162 and RS-232

Quick rundown:
- V<sub>a</sub> = 9V. Reduced to 5V by an [LMN7805](https://www.sparkfun.com/datasheets/Components/LM7805.pdf) voltage regulator.
- Button with [pull-up resistor](https://learn.sparkfun.com/tutorials/pull-up-resistors/all) connected to Atmega162 reset pin.
- 4.9152MHz crystal oscillator connected to XTAL pins of Atmega162.
- JTAG Interface for [Atmel-ICE](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-ICE_UserGuide.pdf) connected and tested using Microchip Studio. Used for programming and debugging the ATmega162.

At the start of main.c, the clock frequency, baud rate and other important parameters are defined.

```c
#define F_CPU 4915200 // Set clock frequency to 4,9MHz
#define BAUD 9600
#define MY_UBBR F_CPU/16/BAUD - 1 // USART Baud Rate Register for asynchronous normal mode
```

A [Max233 RS-232 line driver](https://en.wikipedia.org/wiki/MAX232) helps the ATmega162 maintain the correct signal levels while sending/receiving data over UART.

To be able to transmit data on the serial line using the standard printf function, the fdevopen function is called in the UART driver. This makes the controller open a file-like stream that it writes to. We can then read the output using 

The code below is modified from page 174 in the ATmega162 datasheet. Note that the channel number had to be added (e.g. UBRR**0**H instead of UBBRH).

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



## Lab 2 - Address decoding and external RAM

The ATmega162 has a built-in interface for external SRAM that can be activated by setting the SRE bit in the MCUCR register:

```c
void SRAM_init() {
	MCUCR = (1 << SRE); // Enable external SRAM
	SFIOR = (1 << XMM2); // Masks PC4-PC7 to make sure JTAG remains enabled
}
```

Enabling this makes the ATmega162 do some cool tricks in the background whenever we read or write to addresses between 0x0500 and 0xFFFF in the code.

The following pins get new functionality:

| Pins		| Function 	|
| ---		| ---		|
| PA0-PA7	| Alternates between sending the first half of the 16-bit memory address, then eight data bits |
| PC0-PC7 	| Sends the rest of the memory address |
| PD6		| WR (Write control strobe) - Falling edge signals to other equipment that new data has been output |
| PD7		| RD (Read control strobe) - Falling edge signals to other equipment that they should write something |
| PE1		| ALE (Address latch enable) - Rising edge when writing address, falling edge when done |

PE1 is connected to a [74ALS573](https://www.ti.com/lit/ds/symlink/sn74as573a.pdf?ts=1727201799450) latch circuit. This circuit "saves" the address bits from the ATmega162 while data is being read/written to the SRAM. This is needed because we don't have enough available pins to control everything at once from the ATmega162.

The addresses are reserved as such:

| Reserved for			| Address (Hex)	| Address (Binary)		|
| ---					| ---			| ---					| 
| OLED Commands Start	| 0x1000		| 0001 000 000000000	|
| OLED Commands End		| 0x11FF		| 0001 000 111111111	|
| OLED Data Start		| 0x1200		| 0001 001 000000000	|
| OLED Data End			| 0x13FF		| 0001 001 111111111	|
| ADC Start				| 0x1400		| 0001 010 000000000	|
| ADC End				| 0x17FF		| 0001 011 111111111	|
| SRAM Start			| 0x1800		| 0001 100 000000000	|
| SRAM End				| 0x1FFF		| 0001 111 111111111	|

Notice the three "standalone" bits in the binary addresses. Since they form a unique pattern for each component, we can connect the address pins on the ATmega162 to NAND gates to create an address decoder.

The address decoder sends a signal to the components' "chip select" pins whenever we read or write to their addresses. This makes the component only listen to the Write / Read control strobe when the signal is meant for them.



## Lab 3 - A/D converting and joystick inputs
Our controller has a joystick consisting of two variable resistors (potmeter). To read their position a [MAX156 analog-to-digital-converter (ADC)](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX155-MAX156.pdf) is used. 

The MAX156 converts input voltages into 8-bit numbers every time the write strobe signal is triggered from the ATmega162. Every time the read strobe is triggered after that, it outputs the next channel's converted voltage on its pins.

NB! It takes some microseconds to convert each channel (9 x N x 2) / f<sub>CLK</sub>, so beware of that before reading.

The filter on the USB multifunction board has a Capacitance of 100nF and a resistance of 2kΩ, giving us a cutoff frequency of 795.7747 Hz

The relationship between the joystick angle (θ) and voltage (V) is V(θ) = 2.5 + (1/12 * θ)
