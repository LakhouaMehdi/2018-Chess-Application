/**********************************************************************/
/*               Project 1, EECS 22L, Winter 2018                     */
/*                                                                    */
/*               Author: Team 4                                       */
/*               Date: 1/12/2018                                      */
/*                                                                    */
/*               Piece.h: header file for piece functions             */
/*                                                                    */
/**********************************************************************/


#ifndef PIECE_H
#define PIECE_H

#include "Constants.h"

typedef struct Piece{
	PIECETYPE type;                   //Piece type of the piece
	SIDE player;                      //The side of the piece
}PIECE;

//Allocate memory for the piece struct
//Returns a pointer to the piece
PIECE* CreatePiece(PIECETYPE type, SIDE player);

//Frees the memory for the piece struct
void DeletePiece(PIECE* piece);

//Sets the type for a piece
void SetType(PIECE* piece, PIECETYPE newType);

#endif
