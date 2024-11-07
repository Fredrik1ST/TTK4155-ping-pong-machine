/*
* @file oled.h
* @brief Handles writing to a monochrome 128x64 OLED display
*/
#ifndef OLED_H
#define OLED_H
#endif

#include <inttypes.h>

/*
* Writes data to the OLED controller's data memory
*/
void oled_write_data(uint8_t data);


/*
* Writes data to the OLED controller's command memory
*/
void oled_write_cmd(uint8_t cmd);


/*
* Initializes the OLED display
*/
void oled_init();


/*
* Moves cursor to page (row) on the OLED display
* @param	page	The page (row) to move to (0-7)
*/
void oled_goto_page(uint8_t page);


/*
* Moves the cursor to a column on the OLED display
* @param	col		The column to move to (0-127)
*/
void oled_goto_col(uint8_t col);


/*
* Moves the cursor to a specific position (page + column)
* @param	page	The page to move to (0-7)
* @param	col		The column to move to (0-127)
*/
void oled_pos(uint8_t page, uint8_t col);

/*
* Writes pixels across a selected page / row
* @param	page	The page to print to
* @param	col		The column to move to (0-127)
*/
void oled_write_line(uint8_t page);


/*
* Clears pixels from the selected page
* @param	page	The page to clear
*/
void oled_clear_line(uint8_t page);


/*
* Clears all pixels on the OLED display
*/
void oled_reset();

/*
* Print a character on the OLED screen
* @param	c		The char to be printed
* @param	font	Font number to use
*/
void oled_printChar(char c, char font);


/*
* Print a string on the OLED screen
* @param	data	The string to be printed
* @param	font	Font number to use
*/
void oled_print(const char* data, char font);

/**
 * @brief Stores Data in the buffer at a given position
 * 
 * @param page  Page of the display in the buffer to write on
 * @param col   Column of the display in the buffer to write on
 * @param data  Data to be stored in the buffer
 */
void oled_update_buffer(uint8_t page, uint8_t col, uint8_t data);

/**
 * @brief Writes the contents of the buffer on the display
 * 
 */
void oled_push_buffer(void);

/**
 * @brief Writes the buffer full of zeroes
 * 
 */
void oled_clear_buffer(void);

/**
 * @brief Writes a character into the buffer
 * 
 * @param c     The Character to be written
 * @param font  The font the character should be in
 * @param page  The page on the buffer
 * @param col   The column of the buffer
 */
void oled_printCharToBuffer(char c, char font, uint8_t page, uint8_t col);

/**
 * @brief Writes a string to the Buffer. Automatically moves cursor.
 * 
 * @param data  The string to be written to the buffer
 * @param font  The font to be used
 * @param page  The starting page
 * @param col   The starting column
 */
void oled_printToBuffer(const char* data, char font, uint8_t page, uint8_t col);