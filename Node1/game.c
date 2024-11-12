#include <inttypes.h>
#include "def.h"

void update_highscore() {
    if (gScore > gHighScores[0]) {
        gHighScores[2] = gHighScores[1];
        gHighScores[1] = gHighScores[0];
        gHighScores[0] = gScore;
        return;
    }
    if (gScore > gHighScores[1]) {
        gHighScores[2] = gHighScores[1];
        gHighScores[1] = gScore;
        return;
    }
    if (gScore > gHighScores[2]) {
        gHighScores[2] = gScore;
        return;
    }
}