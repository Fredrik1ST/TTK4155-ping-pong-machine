/*
 * @file encoder.h
 */

#include <sam.h>

#ifndef ENCODER_H
#define ENCODER_H

/*
* Initializes the encoder IO
*/
void encoder_init();

/*
* Read encoder index value from register
* Will reset position whenever position is negative
* @return index value
*/
uint32_t encoder_read();

/*
* Set encoder position to 0.
*/
void encoder_reset();

#endif /* ENCODER_H */