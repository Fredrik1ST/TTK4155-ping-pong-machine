/*
 * @file motorController.h
 */


#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

typedef struct{
	float integral;
	float prev_e;
	uint64_t prev_t;	
}PID_controller;

PID_controller motorController_run(int8_t input, PID_controller);

#endif /* MOTORCONTROLLER_H */