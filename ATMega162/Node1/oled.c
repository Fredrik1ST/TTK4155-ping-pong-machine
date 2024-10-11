#include <stdio.h>

#include "fonts.h"
#include "def.h"


void oled_write_data(uint8_t data){
    volatile char* adr = (char*) DEF_ADR_OLED_DATA;
    adr[0] = data;
}


void oled_write_cmd(uint8_t cmd){
    volatile char* adr = (char*) DEF_ADR_OLED_CMD;
    adr[0] = cmd;
}


void oled_init()
{
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


// Go to page - a row or line on screen
void oled_goto_page(uint8_t page)
{
	// Mask of 0x07 to avoid writing beyond first 3 bits
	oled_write_cmd(0xB0 | (page & 0x07));
}


void oled_goto_col(uint8_t col)
{
	// Must be written twice (4 bits each writing for whatever reason)
	
	// Lower 4 bits (mask first 4 bits to avoid writing above)
	uint8_t lower_adr = col & 0x0F;
	oled_write_cmd(lower_adr);

	// Upper 4 bits (mask last 4 bits)
	//uint8_t upper_adr = 0x1F & ((col & 0xF0)>>4);
	uint8_t upper_adr = 0x10 | ((col & 0xF0)>>4);
	oled_write_cmd(upper_adr);
}


void oled_pos(uint8_t page, uint8_t col)
{
	oled_goto_page(page);
	oled_goto_col(col);
}

void oled_home(){
	oled_pos(0x0,0x0);
}

void oled_write_line(uint8_t page){
	oled_goto_page(page);
	for (uint8_t i = 0; i < 128; i++){
		oled_write_data(0x01);
	}
}


void oled_clear_line(uint8_t page){
	oled_goto_page(page);
	for (uint8_t i = 0; i < 128; i++){
		oled_write_data(0x00);
	}
}


void oled_reset(){
	for (uint8_t i=0; i < 8; i++){
		oled_clear_line(i);
	}
	oled_home();
}


void oled_printChar(char c, char font){
	//printf("%c = %d\r\n", c, c);
	if (c >= 0x20 && c <= 0x7F){ // Do not attempt to write if ASCII character does not exist
		switch (font){
			case 4: // 4x8 SMALL
				for (uint8_t i = 0; i < 4; i++)
				{
					oled_write_data(pgm_read_byte(&font4[c-32][i])); // Function & font from fonts.h
				}
				break;
			
			case 8: // 8x8 LARGE
				for (uint8_t i = 0; i < 8; i++)
				{
					oled_write_data(pgm_read_byte(&font8[c-32][i]));
				}
				break;
		
			default: // 5x8 NORMAL
				for (uint8_t i = 0; i < 5; i++)
				{
					oled_write_data(pgm_read_byte(&font5[c-32][i]));
				}
				break;
		}
	}
}


// Print strings to OLED
void oled_print(const char* data, char font){
	// Select font (from fonts.h)
	char f = 5; // Default font (5x8 NORMAL)
	if (f == 4 || f == 8){ // Selected fonts 4x8 SMALL or 8x8 LARGE
		f = font;
	}
	
	int i = 0;
	while(data[i] != '\0')
	{
		oled_printChar(data[i], f);
		++i;
	}
}