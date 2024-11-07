 /*
 * @file solenoid.h
 * Controls solenoid on the game board
 */

#ifndef SOLENOID_H
#define SOLENOID_H


/*
* Initializes the solenoid IO
*/
void solenoid_init(void);

/*
* Activates the solenoid pin to kick the ball
*/
void solenoid_kick(void);

/*
* Retract the solenoid by deactivating the pin
*/
void solenoid_retract(void);


#endif /* SOLENOID_H */