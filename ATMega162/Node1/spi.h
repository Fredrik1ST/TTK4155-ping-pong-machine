/**
 * @file spi.h
 * @brief Serial Peripheral Interface (SPI) for ATMega162
 */

#ifndef SPI_H
#define SPI_H

/*
* Initialize the SPI interface as master. Uses PB4 as inverted chip select pin.
*/
void spi_init(void);


/*
* Send 8 bits over SPI as master
* @param	cData	Byte to be transmitted
*/
void spi_send(char cData);

/*
* Receive 8 bits over SPI as master
*/
uint8_t spi_recv(void);

#endif /* SPI_H */