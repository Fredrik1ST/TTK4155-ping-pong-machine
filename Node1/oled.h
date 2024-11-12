/**
* @file oled.h
* @brief Handles writing to a monochrome 128x64 OLED display
*/
#ifndef OLED_H
#define OLED_H
#endif

#include <inttypes.h>

/**
* @brief Write data to the OLED controller's data memory
*/
void oled_write_data(uint8_t data);


/**
* @brief Writes data to the OLED controller's command memory
*/
void oled_write_cmd(uint8_t cmd);


/**
* @brief Initializes the OLED display
*/
void oled_init();


/**
* @brief Moves cursor to page (row) on the OLED display
* @param	page	The page (row) to move to (0-7)
*/
void oled_goto_page(uint8_t page);


/**
* @brief Moves the cursor to a column on the OLED display
* @param	col		The column to move to (0-127)
*/
void oled_goto_col(uint8_t col);


/**
* @brief Moves the cursor to a specific position (page + column)
* @param	page	The page to move to (0-7)
* @param	col		The column to move to (0-127)
*/
void oled_pos(uint8_t page, uint8_t col);

/**
* @brief Writes pixels across a selected page / row
* @param	page	The page to print to
* @param	col		The column to move to (0-127)
*/
void oled_write_line(uint8_t page);


/**
* @brief Clears pixels from the selected page
* @param	page	The page to clear
*/
void oled_clear_line(uint8_t page);


/**
* @brief Clears all pixels on the OLED display
*/
void oled_reset();

/**
* @brief Print a character on the OLED screen
* @param	c		The char to be printed
* @param	font	Font number to use
*/
void oled_printChar(char c, char font);


/**
* @brief Print a string on the OLED screen
* @param	data	The string to be printed
* @param	font	Font number to use
*/
void oled_print(const char* data, char font);