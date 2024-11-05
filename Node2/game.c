#include ir.h
#include game.h


int failstate = 0;
int Fails = 0; 
/**
 * @brief counts the score
 * 
 * @param Fails Updates increments this int whenever the IR beam gets broken (Rising Trigger)  
 */
void scorecounter(int*Fails) {
    int RT = readIR(); 
    if (!failstate) {
		if (RT) {
			Fails++;
			failstate = 1;
		}
	} else if (!RT) {
		failstate = 0;
	}
}

