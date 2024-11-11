#include "encoder.h"
#include "pwm.h"
#include <stdio.h>
#include "time.h"
#include "motorController.h"

#define MOTOR_PID_KP .44
#define MOTOR_PID_KI 0.03	
#define MOTOR_PID_KD 9000000.

#define ENCODER_MAX 2820

PID_controller motorController_run(int8_t joystick_pos, PID_controller PID){
	
		// encoder data
		uint32_t encoder_data = encoder_read();	// from 0 to ENCODER_MAX
		
		float encoder_data_scaled = ((float)encoder_data * (200./ ENCODER_MAX) - 100) + 100;	// From -100 to 100 to match joystick
		
		float posError = (-joystick_pos+100) - encoder_data_scaled;
		
		PID.integral = PID.integral + MOTOR_PID_KI * posError;
		
		if (PID.integral > 50){
			PID.integral = 50;
			}else if (PID.integral < -50){
			PID.integral = -50;
		}
		if ((posError > -10) && (posError < 10)){
			PID.integral = 0;
		}
		// derviat
		uint64_t timer = time_now();
		uint64_t dt = timer - PID.prev_t;
		float dPart = MOTOR_PID_KD * (posError - PID.prev_e)/dt;
		PID.prev_t = timer;
		PID.prev_e = posError;
		
		float u = MOTOR_PID_KP * posError + PID.integral + dPart;
		
		if (u > 100){
			u = 100;
			}else if (u < -100){
			u = -100;
		}
		
		printf("Enc: %.1f    Joy: %d    Err: %.2f    u: %.2f    i: %.2f   d: %.4f\r\n\r\n", encoder_data_scaled, -joystick_pos, posError, u, PID.integral, dPart);
		
		pwm_setSpeed_motor((int8_t)-u);
		
		return PID;
}