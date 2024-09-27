#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h> // Needed for uint16_t


void SRAM_init() {
	MCUCR = (1 << SRE); // Enable SRAM
	SFIOR = (1 << XMM2); // Masks PC4-PC7 to make sure JTAG remains untouched
	
	// TEST: Add some wait states to address/data pins if ADC is unable to respond due to timing issues
	//	SRW01	SRW00	Function
	//	0		0		No wait states
	//	0		1		Wait one cycle during read/write strobe
	//	1		0		Wait two cycles during read/write strobe
	//	1		1		Wait two cycles during read/write and wait one cycle before driving out new address
	//EMCUCR |= (1<<SRW00);
	//EMCUCR |= (1<<SRW01);
}


void SRAM_test(){
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\r\n");
    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();
    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
            write_errors++;
        }
    }
    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);
    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("SRAM test completed with \r\n%4d errors in write phase and \r\n%4d errors in retrieval phase\r\n\r\n", write_errors, retrieval_errors);
}