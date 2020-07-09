#ifndef MINIMAX_H

#define MINIMAX_H

#include "generalised_tic_tac_toe.h"

struct Move{ // a move is charactersied by the row and the column
    int row , col;
};

int minimax(Game* game, int depth, bool isMax , int alpha , int beta , int maxdepth);
Move findBestMove(Game* game , int maxdepth);
Move minimax_score(Game* game , int maxdepth);

#endif
