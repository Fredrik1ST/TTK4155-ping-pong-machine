/*
* @file sram.h
* @brief Handling of external SRAM / memory interface (XMEM) for ATmega162
*/

#ifndef SRAM_H
#define SRAM_H

/**
 * Initialize the external SRAM features of the AVR MCU.
 *
 * Also masks the PC4-PC7 bits so that they can still be used for JTAG communications.
 */
void SRAM_init();


/**
 * Tests the external SRAM by reading and writing to preset registries.
 *
 * Remember to run the function SRAM_init() before this.
 */
void SRAM_test();

#endif