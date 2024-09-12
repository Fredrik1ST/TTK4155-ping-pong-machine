#ifndef UART_H
#define UART_H

#include <stdio.h>

void UART_init(unsigned int ubbr);
int UART_transmit(char data, FILE * file);
int UART_receive(FILE * file);
#endif