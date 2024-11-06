/*
 * @file encoder.h
 */


#ifndef ENCODER_H
#define ENCODER_H

void encoder_init();

typedef struct {
	int32_t	pos;
} Encoder;

int32_t encoder_read(Encoder);

void encoder_reset(Encoder);

#endif /* ENCODER_H */