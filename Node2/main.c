#include "def.h"
#include "sam.h"
#include "can.h"
#include "uart.h"
#include "time.h"
#include "pwm.h"
#include "ir.h"
#include "encoder.h"
#include "solenoid.h"
#include "motorController.h"
#include <stdio.h>

#define txMailbox 0
#define rxMailbox 1

float integral = 0; // Used by PI motor controller

int main(void) {
	WDT->WDT_MR = WDT_MR_WDDIS; // Disable watchdog timer
	
    SystemInit();
	uart_init(F_CPU, 9600);
	SysTick_Handler(); // Keep track of time for delays

	CanInit canCfg = (CanInit){.brp = 41, .phase1 = 6, .phase2 = 5, .propag = 1}; // Phase values from node 1 (plus one, according to datasheet)
	can_init(canCfg, 0);
	
	IR_init();
	encoder_init();
	solenoid_init();
	pwm_init();

    while (1) {
		
		// =================================================
		// CAN Messages

		CanMsg msgOut;
		msgOut.id = 0xB5;
		msgOut.length = 0x1;
		msgOut.dword[0] = getIR();
		can_tx(msgOut);
		printf("Sent: ID: %02X    -    Len: %02X    -    Dat: %02X\r\n\r\n", msgOut.id, msgOut.length, msgOut.dword[0]);
		
		CanMsg msgIn;
		if(CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MRDY){
			can_rx(&msgIn);
			//printf("Recv: ID: %02X    -    Len: %02X    -    Dat: %02X %02X %01X %01X \r\n\r\n", msgIn.id, msgIn.length, msgIn.byte[0], msgIn.byte[1], msgIn.byte[2], msgIn.byte[3]);
		}
		
		// Decode CAN message
		int8_t gp_pos_x = (int8_t) msgIn.byte[0];
		int8_t gp_pos_y = (int8_t) msgIn.byte[1];
		uint8_t gp_btn = msgIn.byte[2] & (1 << 0);
		#define GAME_ACTIVE  ((msgIn.byte[3] & (1 << 0)) != 0)
		
		
		if GAME_ACTIVE {
			if (gp_pos_y > 50){
				pwm_setDutyCycle_servo(-6*(gp_pos_x-29) + 1500);
			}else{
				pwm_setDutyCycle_servo(1500); // Center
			}
		
			integral = motorController_run(gp_pos_x, integral);
		
			if (gp_btn == 0){
				solenoid_kick();
			}else{
				solenoid_retract();
			}
		}
	
		//printf("%01X\r\n", gp_btn);
		
		
		// =================================================
		// Read board inputs (IR, encoder)

		//printf("%d\r\n", encoder_read());
		
	}
}