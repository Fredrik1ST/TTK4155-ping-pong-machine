/**
 * @file def.h
 * @brief System-specific definitions, e.g. CPU clock frequency
 */

#ifndef SRAM_H
#define SRAM_H

#define F_CPU 4915200 // Set clock frequency to 4,9MHz
#define DEF_BAUD 9600
#define DEF_UBBR F_CPU/16/DEF_BAUD - 1 // UART baud rate from datasheet
#define DEF_ADC_CONV_TIME 1000000*9*4*2/F_CPU + 10 // ADC Conversion time [microseconds] with 10us leeway

#define DEF_ADR_SRAM 0x1800			// Start address for the SRAM
#define DEF_ADR_ADC 0x1400			// Start address for the ADC
#define DEF_ADR_OLED_DATA 0x1200		// Start address for the OLED Data
#define DEF_ADR_OLED_CMD 0x1000		// Start address for the OLED Commands

#define DEF_ADC_CHANNELS 4

#endif