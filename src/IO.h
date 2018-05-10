//*********************************************************************/
/* Project 1, for EECS 22L, Winter 2018	                             */
/*                                                                   */
/* Authors: Team 4                                                   */
/* Date: 1/12/2018                                                   */
/*                                                                   */
/* IO.h: header file for input/output			             */
/*								     */
/*	1/12/2018 Medhi Lakhoua Initial version 		     */
/*	1/23/2018 Yuki Hayashi  Updated				     */		
/*                                                                   */
/*********************************************************************/

#ifndef IO_H
#define IO_H

#include "Move.h"
#include "Board.h"
#include "Piece.h"
#include "MoveList.h"

char fileName[100];


/* Print the command-line arguments usage of the program*/
void PrintUsage();

/* Print the enter move menu */
void PrintMenu();

/* Request the promotion type from a human player */
PIECETYPE RequestPromotion();

/* Request a move from a human player */
MOVE* RequestMove();

/*print the current board*/ 
void PrintBoard(BOARD* board);

void PrintPlayer(SIDE player);

void PrintMove(const MOVE* move);

/*prints every move in the move list */
void PrintMoveList(const MLIST* list);

/*prints the array corrdinates in chess notation (0,0 is a8)*/
void PrintCoordinates(int x, int y);

void StartLog();

void LogMove(MOVE *move, SIDE currentPlayer);

void PrintPiece(PIECE* piece);

void ParseMove(MOVE *move, char* from, char* to);
#endif
