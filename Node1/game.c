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

uint8_t highScores[2];

void end_game() {
    gameActive = 0;
    if (score > highScores[0]) {
        highScores[2] = highScores[1];
        highScores[1] = highScores[0];
        highScores[0] = score;
        return;
    }
    if (score > highScores[1]) {
        highScores[2] = highScores[1];
        highScores[1] = score;
        return;
    }
    if (score > highScores[2]) {
        highScores[2] = score;
        return;
    }
    score = 0;
}

;