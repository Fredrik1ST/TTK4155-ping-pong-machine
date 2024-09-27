#ifndef UART_H
#define UART_H

#include <stdio.h>


/**
 * Initializes the UART communication interface.
 *
 * This function sets the baud rate for UART transmission and configures the UART
 * control registers to enable the transmitter and receiver. The data frame size
 * is configured to 8 bits with 2 stop bits and no parity.
 */
void UART_init(unsigned int ubbr);


/**
 * Receives a single byte of data via UART.
 *
 * This function waits until data is available in the UART data register, and
 * then returns the received byte.
 *
 * @returns The received byte of data.
 */
int UART_receive(FILE * file);


/**
 * Transmits a single byte of data via UART.
 *
 * This function waits until the UART data register is empty, and then transmits
 * the given data byte through the UART.
 *
 * @param	data	The byte of data to be transmitted.
 * @param	file	
 */
int UART_transmit(char data, FILE * file);

#endif