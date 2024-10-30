/**
 * @file pwm.h
 * @brief Pulse-width modulation
 * 
 * Uses the PWM macrocell of the ATSAM to output PWM signals
 */

#ifndef PWM_H
#define PWM_H

#include <sam.h>

/**
 * Initializes the PWM module for channel 5
 */
void pwm_init(void)


/**
 * Sets a new duty cycle for PWM channel 5
 *
 * @param dutyCycle_us The desired duty cycle in microseconds
 */
void(pwm_setDutyCycle)(uint32_t dutyCycle_us)



#endif /* PWM_H */