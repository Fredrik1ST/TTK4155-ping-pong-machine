
#include "encoder.h"
#include "pwm.h"

#define MOTOR_PID_KP 0
#define MOTOR_PID_KI 0.00001

#define ENCODER_MAX 1800

void motorController_run(int8_t joystick_pos, float integral){
	
		// encoder data
		uint32_t encoder_data = encoder_read();	// from 0 to ENCODER_MAX
		
		float encoder_data_scaled = (float)encoder_data * (200./ 1800.) - 100;	// From -100 to 100 to match joystick
		
		float posError = joystick_pos - encoder_data_scaled;
		
		float iPart = integral + MOTOR_PID_KI * posError;
		
		float u = MOTOR_PID_KP * posError + iPart;
		
		pwm_setSpeed_motor((int8_t)u);
}