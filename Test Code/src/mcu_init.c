#include <stdio.h>
#include "uart.h"
#include "mcu_init.h"

/**
 * Configures UART as the output stream for printf functionality.
 *
 * This sets up a FILE structure for UART output using the UART_putchar function
 * as the write function, wich will allow the use of printf to send data via UART.
 */
FILE uart_output = FDEV_SETUP_STREAM(UART_putchar, NULL, _FDEV_SETUP_WRITE);

/**
 * Initializes the printf functionality to use UART.
 *
 * This function assigns the UART output stream to the stdout file stream
 * allowing printf statements to send output via UART.
 */
void init_printf(void) {
    stdout = &uart_output;
}

