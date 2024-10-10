/*
* @file delay.h
* @brief delay_us and delay_ms
*/

#ifndef DELAY_H_
#define DELAY_H_

#include <inttypes.h> // Needed for uint8_t, uint16_t, etc.

/**
 * Microsecond delay. F_CPU must be defined!
 */
void delay_us(uint32_t);


/**
 * Millisecond delay. F_CPU must be defined!
 */
void delay_ms(uint32_t);


#endif /* DELAY_H_ */