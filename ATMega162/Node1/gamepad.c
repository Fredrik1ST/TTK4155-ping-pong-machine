#define CH_JOYSTICK_X 0
#define CH_JOYSTICK_Y 1
#define CH_SLIDER_LEFT 2
#define CH_SLIDER_RIGHT 3

#include <avr/io.h>
#include <inttypes.h> // Needed for uint8_t, uint16_t, etc.
#include <math.h>
#include "adc.h"
#include "gamepad.h"


void gamepad_init(){
	DDRB &= ~(1 << DDB0); // Set PB1 to 0 (input for joystick button)
}

Gamepad read_gamepad(){
	Gamepad gp;
	gp.pos_x = (int16_t)adc_read(CH_JOYSTICK_X);
	gp.pos_y = (int16_t)adc_read(CH_JOYSTICK_Y);
	gp.pos_left = (int16_t)adc_read(CH_SLIDER_LEFT);
	gp.pos_right = (int16_t)adc_read(CH_SLIDER_RIGHT);
	gp.btn = (PINB & (1<<PB0));

	return gp;
}

Gamepad calibrate_gamepad(Gamepad input){
	Gamepad gp;
	gp.pos_x = (input.pos_x - 127)*(100/127); // Get direction as a percentage between -100 to +100
	gp.pos_y = (input.pos_y - 127)*(100/127);
	gp.pos_left = (input.pos_left - 127)*(100/127);
	gp.pos_right = (input.pos_right - 127)*(100/127);
	
	return gp;
}

Dir getJoystickDir(Gamepad gp){
	int16_t deadzone = 10;
	Dir direction = NONE; // Default

	if (gp.pos_x < deadzone && gp.pos_x > -deadzone && gp.pos_y < deadzone && gp.pos_y > -deadzone){
		direction = NONE;

	} else if (abs(gp.pos_x) >= abs(gp.pos_y)){
		if (gp.pos_x > 0){
			direction = RIGHT;
		} else{
			direction = LEFT;
		}

	} else if (abs(gp.pos_y) > abs(gp.pos_x)){
		if (gp.pos_y > 0){
			direction = UP;
		}else{
			direction = DOWN;
		}
		}
		
	return direction;
}

Gamepad refresh_gamepad(Gamepad gp){
	gp = read_gamepad();
	gp = calibrate_gamepad(gp);
	gp.joy_dir = getJoystickDir(gp);
}

void print_gamepad(Gamepad gp){
	// Print analog values
	printf("X: %02X    -    Y: %02X    -    L: %02X    -    R: %02X\r\n\r\ ", gp.pos_x, gp.pos_y, gp.pos_left, gp.pos_right);

	switch(getJoystickDir(gp)){
		case LEFT: printf("Joystick: LEFT");
		case RIGHT: printf("Joystick: RIGHT");
		case UP: printf("Joystick: UP");
		case DOWN: printf("Joystick: DOWN");
		default: printf("Joystick: NEUTRAL");
	}
	
	if (gp.btn){
		printf("Button pressed")
	}
}