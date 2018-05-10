#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Constants.h"
#include "Move.h"
#include "MoveList.h"

//creates and returns the best move for the given player
MOVE* GetBestMove(BOARD* board,const SIDE player, int searchDepth);

int AlphaBeta(int depth, int alpha, int beta, BOARD* board,const SIDE player);

//evaluates the board and returns a score for the given player
int EvaluateBoard(const BOARD* board,const SIDE player);

int EvaluatePiece(const PIECE* piece, const SIDE player);

void CopyMove(MOVE* move, const MOVE* source);

/*returns a list containing all valid for the given player*/
MLIST* GetAllValidMoves(BOARD* board, SIDE side);


#endif
