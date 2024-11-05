#include <avr/io.h>

// Initialize as master
// Code modified from ATMega162 data sheet
void spi_init(void){
	DDRB |= (1 << PB5)|(1 << PB7); // MOSI out, SCK out
	// Enable SPI as master (SPE, MSTR)
	// Set clock rate F_CPU/16 (SPR0)
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


// Send data as master
void spi_send(char cData){
	SPDR = cData;
	
	//Wait for transmission complete
	while(!(SPSR & 1<<SPIF)){;};
}

// Receive data as master
// Same as sending
// Moves ring buffer by 8 bits to receive 8 bits from slave
uint8_t spi_recv(void){
	spi_send(0);
	return SPDR;
}