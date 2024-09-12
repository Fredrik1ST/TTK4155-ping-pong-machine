#include <stdio.h>
#include "uart.h"
#include "mcu_init.h"

FILE uart_output = FDEV_SETUP_STREAM(UART_putchar, NULL, _FDEV_SETUP_WRITE);

void init_printf(void) {
    stdout = &uart_output;
}
