/*
 * @file encoder.h
 */


#ifndef ENCODER_H
#define ENCODER_H

void encoder_init();

struct Encoder {
	int32_t	pos;
};

int32_t encoder_read(Encoder);

void encoder_reset(Encoder);

#endif /* ENCODER_H */