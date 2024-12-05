# Ping-Pong Machine
Group 33's semester project for TTK4155 (Embedded and Industrial Computer Systems) at NTNU, Autumn 2024.

The purpose of this project is to assemble and program a machine for a mechanical game of single-player pong. It consists of two main nodes:

**Node 1**: A breadboard with an [Atmel AVR ATmega162](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2513-8-bit-AVR-Microntroller-ATmega162_Datasheet.pdf), as well as a USB multifunction with joysticks, buttons and an OLED display for user interface.

**Node 2**: An [Arduino Due](https://docs.arduino.cc/hardware/due) (based on [Atmel ATSAM3X8E](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf)) to control actuators & IO. Communicates with node 1 using CAN bus.

This document is meant as an informal progress report where we jot down anything of note.

[comment]: <![Welcome to byggern.](https://i.imgur.com/ccaTs94.mp4)> 
<p style="font-family: Papyrus, Charcoal, fantasy; color:red">🗲 Byggern Begins 🗲</p> 

<br />

# Table of contents
1. [Initial assembly of Atmega162 and RS-232](#lab1)
2. [Address decoding and external RAM](#lab2)
3. [A/D converting and joystick inputs](#lab3)
4. [OLED display and user interface](#lab4)
5. [SPI & CAN Controller](#lab5)
6. [Assembly of node 2 with CAN Bus communication](#lab6)


<br />

## Lab 1 - Initial assembly of Atmega162 and RS-232 <a name="lab1"></a>

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


<br />

## Lab 2 - Address decoding and external RAM <a name="lab2"></a>

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

<img src="img/NAND%20GATES.png" width="600">

The address decoder sends a signal to the components' "chip select" pins whenever we read or write to their addresses. This makes the component only listen to the Write / Read control strobe when the signal is meant for them.


<br />

## Lab 3 - A/D converting and joystick inputs <a name="lab3"></a>
Our gamepad has a joystick consisting of two variable resistors (potmeter), two sliders and some buttons. To read their position a [MAX156 analog-to-digital-converter (ADC)](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX155-MAX156.pdf) is used. 

The MAX156 converts input voltages into 8-bit numbers every time the write strobe signal is triggered from the ATmega162. Every time the read strobe is triggered after that, it outputs the next channel's converted voltage on its pins.

NB! It takes some microseconds to convert each channel (9 x N x 2) / f<sub>CLK</sub>, so beware of that before reading.

Notes:
- The filter on the USB multifunction board has a Capacitance of 100nF and a resistance of 2kΩ, giving us a cutoff frequency of 795.7747 Hz
- The relationship between the joystick angle (θ) and voltage (V) is V(θ) = 2.5 + (1/12 * θ)
- **Weird behavior from address pin PC02 as long as compiler optimization was enabled in Microchip Studio**


<br />

## Lab 4 - OLED display and user interface <a name="lab4"></a>
#### Hardware notes
- Make sure the EXTSEL jumper is connected. Without the EXTSEL jumper, the monitor will not be controllable from outside the USB multifunction board.
- Pin 1-3 are the Chip select, Data/Command and Write pins. They are connected to the Nand gate output, the 3rd adress bit on the white adress bus, and the write pin of the ATMega162.
- The next 8 pins are the data pins, they are connected to our purple data bus. Last pin is ground (not used).
- Reading from the display is not possible (or desirable), due to the way the USB multifunction board has been constructed.

#### Software notes
- Use the initialization code from the data sheet. 
	- This will set up writing so that it is written horizontally in "pages". 
	- Each page consists of 128 columns that are 8 pixels tall, written from top to bottom.
	- After writing 8 bits / pixels, writing will automatically continue to the next column.
```c
void oled_init(){
	// From LY190-128064 data sheet
	oled_write_cmd(0xAE); // Display off
	oled_write_cmd(0xA1); // Segment remap
	oled_write_cmd(0xDA); // Common pads hardware: alternative
	oled_write_cmd(0x12);
	oled_write_cmd(0xC8); // Common output scan direction:com63~com0
	oled_write_cmd(0xA8); // Multiplex ration mode:63
	oled_write_cmd(0x3F);
	oled_write_cmd(0xD5); // Display divide ratio/osc. freq. mode
	oled_write_cmd(0x80);
	oled_write_cmd(0x81); // Contrast control
	oled_write_cmd(0x50);
	oled_write_cmd(0xD9); // Set pre-charge period
	oled_write_cmd(0x21);
	oled_write_cmd(0x20); // Set Memory Addressing Mode (mode is selected in next line)
	oled_write_cmd(0x02); // Page addressing mode
	oled_write_cmd(0xDB); // VCOM deselect level mode
	oled_write_cmd(0x30);
	oled_write_cmd(0xAD); // Master configuration
	oled_write_cmd(0x00);
	oled_write_cmd(0xA4); // Out follows RAM content
	oled_write_cmd(0xA6); // Set normal display
	oled_write_cmd(0xAF); // Display on
}
```
  
```c
// Each character in fonts.h is in an array of 8-pixel columns.
// Characters are stored in the ASCII code order, starting at 32 (space)
// Some examples from the small font (4 columns wide, 6 pixels tall)
const unsigned char PROGMEM font4[95][4] = {
		{0b00000000,0b00000000,0b00000000,0b00000000}, // Space (ASCII 32)
		{0b00000000,0b01011100,0b00000000,0b00000000}, // ! (ASCII 33)
		{0b00001100,0b00000000,0b00001100,0b00000000}, // " (ASCII 34)
		// ... and so on
```
- Use a for loop to print the characters. Since the fonts are stored in flash memory (PROGMEM) instead of RAM, *pgm_read_byte()* must be used to read the arrays.
```c
#include <avr/pgmspace.h>
// Write a character "char c" from font4 in fonts.h
// Loop writes one 8-pixel column per iteration
for (uint8_t i = 0; i < 4; i++){
	oled_write_data(pgm_read_byte(&font4[c-32][i]));
}
```


<br />

## Lab 5 - SPI & CAN Controller <a name="lab5"></a>
#### Hardware notes
- Connect SPI lines from ATMega162 to the MCP2515 CAN Bus controller (the latter is the slave)
- MCP2515 should have its own 16MHz oscillator

#### SPI Software notes
- Set ATMega162 as SPI master as described in the data sheet.
```c
void spi_init(void){
	DDRB |= (1 << PB5)|(1 << PB7); // MOSI out, SCK out
	// Enable SPI as master (SPE, MSTR)
	// Set clock rate F_CPU/16 (SPR0)
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
```
- Reading and sending works the same way. If you don't send valid data, you still receive 8 bits since the bus works like a ring buffer.
- Use the commands and addresses from the provided "mcp2515.h"
- Manually control the chip select pin to the MCP2515. We used PB4.
	- NB! CS is active low. Also, don't set/reset chip select every 8 bits when writing. Wait until command + address has been sent before releasing.

#### CAN Software Notes
- Read / write from CAN nodes via RX/TX buffers. Addresses for these are not already included in the header file, so we had to add them.
- Beware of bit timing (section 5 in data sheet). May need to be adjusted if nodes are out of sync (e.g. when interfacing with the Arduino Due in the next task).


<br />

## Lab 6 - Assembly of node 2 with CAN Bus communication <a name="lab6"></a>
#### Hardware notes
- Contains an Arduino Due ([ATSAM3X8E](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf)) with a motor control shield consisting of
	- An [Allegro A3949](https://www.allegromicro.com/-/media/files/datasheets/a3959-datasheet.pdf) motor driver, controlled by PWM
 	- An [MCP2562 CAN transceiver](https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/20005167C.pdf) for communicating with node 1


#### Software notes
- In Microchip Studio, files for ATSAM3X8E need to be downloaded from Tools > Device Pack Manager
	- Even after this, we lacked some necessary files for compiling in "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain" that we ended up copying from the lab PCs.
- For CAN bit timing: the important thing is to make the CAN nominal bit time match on both nodes. See the spreadsheet for an example.
	- Set time segments (SJW, PropSeg, PS1, PS2) equal on all nodes. SJW can be left alone (defaults to 1).
	- NB! Most registers are offset by n+1, so if you want e.g. BRP = 4, write 3 to the register.


<br />

## Lab 7 - Controlling servo and IR <a name="lab7"></a>
#### Hardware notes
- Use the PWM functionality of the ATSAM3X8E / Arduino Due to output PWM for controlling the servo.
	- Pulses between 0.9ms to 2.1ms control the setpoint.
- Create a sensor circuit with an infrared photodiode for detecting the ball. 
	- NB! Convert the diode's voltage 
