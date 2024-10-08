/*
* @file joystick.h
* @brief Potentiometer-based joystick read via ADC
*/

#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <inttypes.h> // Needed for uint8_t, uint16_t, etc.
#include "adc.h"


/** @enum Dir
 *  @brief 2D directions
 */
typedef enum {
    NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
} Dir;


/** @struct Gamepad
 *  @brief Controller inputs (joysticks, sliders & buttons)
 */
typedef struct {
	int16_t pos_x; // Joystick horizontal
	int16_t pos_y; // Joystick vertical
	int16_t pos_left; // Left slider
	int16_t pos_right; // Right slider
	uint8_t btn;
	Dir joy_dir;
} Gamepad;


/**
 * Initializes gamepad button(s)
 */
void gamepad_init();


/**
 * Reads all gamepad inputs
 * @returns A Gamepad struct containing (uncalibrated) joystick & slider positions
 */
Gamepad read_gamepad();


/**
 * Calibrates joystick & slider positions by adding an offset
 */
Gamepad calibrate_gamepad(Gamepad gp);


/**
 * Determines joystick's current direction based on x & y positions
 * @returns The current direction of the joystick
 */
Dir getJoyStickDir(Gamepad gp);


/**
 * Read and calibrate all inputs for the gamepad
 * @returns A gamepad struct containing joystick & slider positions & directions, button presses
 */
Gamepad refresh_gamepad();


/**
 * Print all inputs from the gamepad
 */
void print_gamepad(Gamepad gp);

#endif