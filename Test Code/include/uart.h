#ifndef UART_H
#define UART_H

void UART_init(void);
void UART_transmit(char data);
char UART_receive(void);
int UART_putchar(char c, FILE *stream);

#endif