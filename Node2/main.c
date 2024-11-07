#include "def.h"
#include "sam.h"
#include "can.h"
#include "uart.h"
#include "time.h"
#include "pwm.h"
#include "ir.h"
#include "encoder.h"
#include "solenoid.h"
#include <stdio.h>

#define txMailbox 0
#define rxMailbox 1

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
		msgOut.length = 0x2;
		msgOut.dword[0] = 0xAABB;
		can_tx(msgOut);
		//printf("Sent: ID: %02X    -    Len: %02X    -    Dat: %02X %02X \r\n\r\n", msgOut.id, msgOut.length, msgOut.byte[0], msgOut.byte[1]);
		
		CanMsg msgIn;
		if(CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MRDY){
			can_rx(&msgIn);
			//printf("Recv: ID: %02X    -    Len: %02X    -    Dat: %02X %02X %01X %01X \r\n\r\n", msgIn.id, msgIn.length, msgIn.byte[0], msgIn.byte[1], msgIn.byte[2], msgIn.byte[3]);
		}
		
		
		// =================================================
		// Decode gamepad data for motor control
		int8_t gp_pos_x = (int8_t) msgIn.byte[0];
		int8_t gp_pos_y = (int8_t) msgIn.byte[1];
		uint8_t gp_btn = (uint8_t) msgIn.byte[2];
		// y = (x - min) / (max - min) * (new_max - new_min) + new_min
		int16_t servoPwmDutyCycle = -6*(gp_pos_x-29) + 1500;
		//printf("%d %d \r\n\r\n", gp_pos_x, servoPwmDutyCycle);
		pwm_setDutyCycle_servo(servoPwmDutyCycle);
		pwm_setSpeed_motor(gp_pos_x);
		
		//uint8_t prev_gp_btn = gp_btn;
		//uint8_t rTrig_gp_btn = (gp_btn == 1 && prev_gp_btn == 0);
		
		if (gp_btn == 0){
			solenoid_kick();
		}else{
			solenoid_retract();
		}
		
		
		printf("%01X\r\n", gp_btn);
		
		
		// =================================================
		// Read board inputs (IR, encoder)

		//printf("%d\r\n", encoder_read());
		
	}
}