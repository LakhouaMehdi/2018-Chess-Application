/**********************************************************************/
/*               Project 1, EECS 22L, Winter 2018                     */
/*                                                                    */
/*               Author: Team 4                                       */
/*               Date: 1/12/2018                                      */
/*                                                                    */
/*               Move.h: header file for move function                */
/*                                                                    */
/**********************************************************************/

#ifndef MOVE_H
#define MOVE_H
#include <stdbool.h>
#include "Constants.h"
#include "Board.h"
#include "Piece.h"

typedef struct Move {
    int fromX;			// start position X
    int fromY;			// start position Y
    int toX;			// end position X
    int toY;			// end position Y
    PIECETYPE capturedPiece;	// null if no piece is captured. Set when the move is performed 
    bool promotion;		// true if the piece moved can be promoted. Set when the move is performed
    bool enPassant;		// true if it is an en passant capture. Set when the move is performed
    bool castling;		// true if it is a castling move. Set when the move is performed

    bool prevCastleKing;	// stores the castling permition when the move is performed, for undoing
    bool prevCastleQueen;	// stores the queen castling permition when the move is performed, for undoing

    int prevEnPassantX;
    int prevEnPassantY;
} MOVE;

/* Create a move with given coordinates */
MOVE* CreateMove(int fromX, int fromY, int toX, int toY);

/* Delete a move */
void DeleteMove(MOVE* move);

/* Check the move is valid for the player */
bool IsValidMove(BOARD* board,const MOVE* move, const SIDE side);

/* Check if King move is valid */
bool ValidKingMove(const BOARD* baord, const MOVE* move);

/* Check if Queen move is valid */
bool ValidQueenMove(const BOARD* board, const MOVE* move);

/* Check if Pawn move is valid */
bool ValidPawnMove(const BOARD* board, const MOVE* move, const SIDE side);

/* Check if Knight move is valid */
bool ValidKnightMove(const BOARD* board, const MOVE* move);

/* Check if Bishop move is valid */
bool ValidBishopMove(const BOARD* board, const MOVE* move);

/* Check if Rook move is valid */
bool ValidRookMove(const BOARD* board, const MOVE* move);

/* Perform a move on the board */
void PerformMove(BOARD* board, MOVE* move);

/* Undoes a move on the board */
void UndoMove(BOARD* board, MOVE* move);

#endif
