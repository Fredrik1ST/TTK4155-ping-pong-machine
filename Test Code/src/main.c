#include "uart.h"
#include "mcu_init.h"
#include "square_wave_test.c"

int main(void) {
    UART_init();
    init_printf();

    square_wave_test();

    return 0;
}
