#include "def.h"
#include "sam.h"
#include "can.h"
#include "uart.h"
#include "time.h"
#include <stdio.h>

int main(void) {
    /* Initialize the SAM system */
    SystemInit();
	uart_init(F_CPU, 9600);
	//CanInit canCfg = (CanInit){.brp = 67, .phase1 = 5, .phase2 = 5, .propag = 1}; // Phase values from node 1 (plus one, according to datasheet)
	//CanInit canCfg = (CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1}; // Phase values from node 1 (plus one, according to datasheet)
	CanInit canCfg = (CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1}; // Phase values from node 1 (plus one, according to datasheet)
	can_init(canCfg, 0);
    /* Replace with your application code */
    while (1) {
		CanMsg *canIn;
		
		uint8_t isNewMsg = can_rx(canIn);
		
		if (isNewMsg == 0){
			//printf("\r\nNo new message\r\n");
			;
		}else{
			
			//printf("\r\nReceived: ID: %02X    -    Len: %02X    -    Dat: %02X %02X", canIn->id, canIn->length, pos_x, pos_y);
		}
    }
}
