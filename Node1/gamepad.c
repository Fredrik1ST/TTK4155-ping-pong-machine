#define CH_JOYSTICK_X 0
#define CH_JOYSTICK_Y 1
#define CH_SLIDER_LEFT 2
#define CH_SLIDER_RIGHT 3

#include <avr/io.h>
#include <inttypes.h> // Needed for uint8_t, uint16_t, etc.
#include <math.h>
#include <stdio.h>
#include "adc.h"
#include "gamepad.h"


void gamepad_init(){
	PORTB |= (1 << PB2); // Set pullup resistor for input
	DDRB &= ~(0 << DDB2); // Set PB2 to 0 (input for joystick button)
}

Gamepad read_gamepad(Gamepad gp){
	gp.pos_x = (int16_t)adc_read(CH_JOYSTICK_X);
	gp.pos_y = (int16_t)adc_read(CH_JOYSTICK_Y);
	gp.pos_left = (int16_t)adc_read(CH_SLIDER_LEFT);
	gp.pos_right = (int16_t)adc_read(CH_SLIDER_RIGHT);
	gp.btn = !((PINB & (1 << PB2)) >> PB2);
	
	// printf("RAW X: %d    -    Y: %d    -    L: %d    -    R: %d\r\n\r\n", gp.pos_x, gp.pos_y, gp.pos_left, gp.pos_right);

	return gp;
}

Gamepad calibrate_gamepad(Gamepad input){
	Gamepad gp;
	
	gp.pos_x = ((input.pos_x - 127) * 100 / 120) - gp.offset_x; // Get direction as a percentage between -100 to +100
	if (gp.pos_x > 100){gp.pos_x = 100;} else if (gp.pos_x < -100){gp.pos_x = -100;}
	gp.pos_y = ((input.pos_y - 127) * 100 / 120) - gp.offset_y;
	if (gp.pos_y > 100){gp.pos_y = 100;} else if (gp.pos_y < -100){gp.pos_y = -100;}
	gp.pos_left = (input.pos_left - 127) * 100 / 127;
	gp.pos_right = (input.pos_right - 127) * 100 / 127;
	
	gp.btn = input.btn;
	
	// printf("CAL X: %d    -    Y: %d    -    L: %d    -    R: %d\r\n\r\n", gp.pos_x, gp.pos_y, gp.pos_left, gp.pos_right);
	
	return gp;
}

Dir getJoystickDir(Gamepad gp){
	int16_t deadzone = 40;
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

/*
Gamepad refresh_gamepad(){
	Gamepad gp = read_gamepad(0,0);
	gp = calibrate_gamepad(gp);
	gp.joy_dir = getJoystickDir(gp);
	return gp;
}
*/

void print_gamepad(Gamepad gp){
	// Print analog values
	
	switch(getJoystickDir(gp)){
		case LEFT: printf("Joystick: LEFT");break;
		case RIGHT: printf("Joystick: RIGHT");break;
		case UP: printf("Joystick: UP");break;
		case DOWN: printf("Joystick: DOWN");break;
		default: printf("Joystick: NEUTRAL");break;
	}
	
	if (gp.btn){
		printf("    -    Button pressed    -    ");
	}else{
		printf("    -    ");
	}
		
	printf("X: %d    -    Y: %d    -    L: %d    -    R: %d\r\n", gp.pos_x, gp.pos_y, gp.pos_left, gp.pos_right);
}