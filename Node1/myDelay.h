/**
* @file delay.h
* @brief delay_us and delay_ms
*/

#ifndef MYDELAY_H
#define MYDELAY_H

#include <inttypes.h> // Needed for uint8_t, uint16_t, etc.

/**
 * @brief Microsecond delay. F_CPU must be defined!
 */
void delay_us(uint32_t);


/**
 * @brief Millisecond delay. F_CPU must be defined!
 */
void delay_ms(uint32_t);


#endif /* MYDELAY_H */