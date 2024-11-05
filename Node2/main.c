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
	SysTick_Handler(); // Keep track of time for delays
	//CanInit canCfg = (CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1}; // Phase values from node 1 (plus one, according to datasheet)
	CanInit canCfg = (CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1}; // Phase values from node 1 (plus one, according to datasheet)
	can_init(canCfg, 0);

    while (1) {

		CanMsg msgOut;
		msgOut.id = 0xB5;
		msgOut.length = 0x2;
		msgOut.dword[0] = 0xAABB;
		can_tx(msgOut);
		printf("Sent: ID: %02X    -    Len: %02X    -    Dat: %02X %02X \r\n\r\n", msgOut.id, msgOut.length, msgOut.byte[0], msgOut.byte[1]);
		
		
		CanMsg* canIn;
		can_rx(canIn);
    }
}