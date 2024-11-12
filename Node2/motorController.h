/**
 * @file motorController.h
 * @brief Motor position control via PID
 */


#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

/**
 * @brief Run the motor controller via PID control
 * @return The current value of the integral term
 */
float motorController_run(int8_t input, float integral);

#endif /* MOTORCONTROLLER_H */