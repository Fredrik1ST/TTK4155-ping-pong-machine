#ifndef SRAM_H_
#define SRAM_H_


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