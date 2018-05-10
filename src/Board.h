#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Constants.h"
#include <stdbool.h>

typedef struct Board {

    int blackKingX;
    int blackKingY;
    int whiteKingX;
    int whiteKingY;  
  

    int enPassantX;
    int enPassantY;

    bool wCastleKing;
    bool wCastleQueen;
    bool bCastleKing;
    bool bCastleQueen;

    int fiftyMoves;

    PIECE* *b;

} BOARD;

//Create a new board
BOARD* CreateBoard();

//Delete a board
void DeleteBoard(BOARD* board);

//Get a Piece at given x and y. null if out of bounds
PIECE* GetPiece(const BOARD* board, int x, int y);

//Set a Piece at given x and y
void SetPiece(BOARD* board, int x, int y, PIECE* piece);

//Creates and returns a default board for chess
void InitializeBoard();

//Returns true if the square at the given coordinate attacked by the given side
bool IsSquareAttacked(const BOARD* board, const int x, const int y, SIDE side);

//returns the number of pawns of the given side attacking the square
int PawnAttackers(const BOARD* board, const int x, const int y, SIDE side);

//returns the number of king of the given side attacking the square
int KingAttackers(const BOARD* board, const int x, const int y, SIDE side);

//returns the number of knights of the given side attacking the square
int KnightAttackers(const BOARD* board, const int x, const int y, SIDE side);

//returns the number of diagonal attackers(bishops or queens) of the given side attacking the square
int DiagonalAttackers(const BOARD* board, const int x, const int y, const SIDE side);

//returns the number of linear attackers(rooks or queens) of the given side attacking the square
int LinearAttackers(const BOARD* board, const int x, const int y, const SIDE side); 

//returns true if the King of the given side is check
bool IsInCheck(const BOARD* board, const SIDE side);

//sets the values of the king's coordinates that are stored in the board
void SetKingCoordinates(BOARD* board, const SIDE side, int x, int y);

//returns the x coordinate of the given King (column)
int GetKingX(const BOARD* board, const SIDE side);

//returns the y coordinate of the given King (row)
int GetKingY(const BOARD* board, const SIDE side);

//returns the next player 
SIDE GetNextPlayer(SIDE currentPlayer);

//returns true if the King of the given side can move to one of the neighboring squares
bool KingCanMove(BOARD* board, const SIDE side);

//Returns true if the King of the given side is in checkmate on the board
bool IsCheckmate(BOARD* board, const SIDE side);

#endif
