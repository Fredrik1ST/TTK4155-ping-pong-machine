#include "encoder.h"
#include "pwm.h"
#include <stdio.h>

#define MOTOR_PID_KP 0.4
#define MOTOR_PID_KI 0.01

#define ENCODER_MAX 2820

float motorController_run(int8_t joystick_pos, float integral){
	
		// encoder data
		uint32_t encoder_data = encoder_read();	// from 0 to ENCODER_MAX
		
		float encoder_data_scaled = ((float)encoder_data * (200./ ENCODER_MAX) - 100) + 100;	// From -100 to 100 to match joystick
		
		float posError = (-joystick_pos+100) - encoder_data_scaled;
		
		integral = integral + MOTOR_PID_KI * posError;
		
		if (integral > 50){
			integral = 50;
			}else if (integral < -50){
			integral = -50;
		}
		if ((posError > -10) && (posError < 10)){
			integral = 0;
		}
		
		float u = MOTOR_PID_KP * posError + integral;
		
		if (u > 100){
			u = 100;
			}else if (u < -100){
			u = -100;
		}
		
		printf("Enc: %.1f    Joy: %d    Err: %.2f    u: %.2f    i: %.2f\r\n\r\n", encoder_data_scaled, -joystick_pos, posError, u, integral);
		
		pwm_setSpeed_motor((int8_t)-u);
		
		return integral;
}