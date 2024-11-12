 /**
 * @file solenoid.h
 * @brief Controls solenoid on the game board
 */

#ifndef SOLENOID_H
#define SOLENOID_H


/**
* @brief Initializes the solenoid IO
*/
void solenoid_init(void);

/**
* @brief Activates the solenoid pin to kick the ball
*/
void solenoid_kick(void);

/**
* @brief Retract the solenoid by deactivating the pin
*/
void solenoid_retract(void);


#endif /* SOLENOID_H */