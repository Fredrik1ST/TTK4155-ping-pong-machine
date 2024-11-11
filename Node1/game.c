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

#include <inttypes.h>
#include "def.h"

uint8_t highScores[2];

void end_game() {
    if (gScore > highScores[0]) {
        highScores[2] = highScores[1];
        highScores[1] = highScores[0];
        highScores[0] = gScore;
        return;
    }
    if (gScore > highScores[1]) {
        highScores[2] = highScores[1];
        highScores[1] = gScore;
        return;
    }
    if (gScore > highScores[2]) {
        highScores[2] = gScore;
        return;
    }
    gScore = 0;
}