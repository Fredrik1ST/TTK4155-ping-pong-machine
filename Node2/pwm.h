/**
 * @file pwm.h
 * @brief Pulse-width modulation
 * 
 * @file pwm.h
 * Uses the PWM macrocell of the ATSAM to output PWM signals
 */

#ifndef PWM_H
#define PWM_H

#include <sam.h>

/**
 * Initializes the PWM module for channel 1
 */
void pwm_init(void);

/**
 * Sets a new duty cycle for PWM channel 1
 *
 * @param dutyCycle_us The desired duty cycle in microseconds
 */
void pwm_setDutyCycle_servo (uint32_t dutyCycle_us);

/**
 * Sets a new motor speed as a percentage (+/- 100%)
 *
 * @param pct The desired speed
 */
void pwm_setSpeed_motor(int8_t pct);


#endif /* PWM_H */