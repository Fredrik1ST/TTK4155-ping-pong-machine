/*
#include "ir.h"
#include "game.h"

int failstate = 0;
int Fails = 0; 

void scorecounter(int* Fails){
    int RT = getIR(); 
    if (!failstate) {
		if (RT) {
			Fails++;
			failstate = 1;
		}
	} else if (!RT) {
		failstate = 0;
	}
}
*/

;