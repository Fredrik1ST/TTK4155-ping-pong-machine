#include "oled.h"
#include "fonts.h"
#include "def.h"

/*
 * Useful(?) commands:
    0xA5    -   Entire display on (read data pins, ignore RAM)
    0xAE    -   Turn display off (sleep mode)
    0xAF    -   Turn display on (normal mode)
    0x20    -   Set horizontal addressing mode
*/

void oled_write_data(uint8_t data){
    volatile char* adr = (char*) DEF_ADR_OLED_DATA;
    adr[0] = data;
}


void oled_write_cmd(uint8_t cmd){
    volatile char* adr = (char*) DEF_ADR_OLED_CMD;
    adr[0] = cmd;
}