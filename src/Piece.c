/**********************************************************************/
/*               Project 1, EECS 22L, Winter 2018                     */
/*                                                                    */
/*               Author: Team 4                                       */
/*               Date: 1/12/2018                                      */
/*                                                                    */
/*               Piece.c: source file for piece functions             */
/*                                                                    */
/**********************************************************************/


#include "Piece.h"
#include <stdlib.h>

//Allocate memory for the piece struct
//Returns a pointer to the piece
PIECE* CreatePiece(PIECETYPE type, SIDE player){

    PIECE* piece = malloc(sizeof(PIECE));
	
    if(piece != NULL) {

	piece->type = type;
	piece->player = player;
    }

    return piece;
}

//Frees the memory for the piece struct
void DeletePiece(PIECE* piece){

    if(piece != NULL){
	free(piece);
  	piece = NULL;
    }
}

//Sets the type of a piece
void SetType(PIECE* piece, PIECETYPE newType){

    if(piece != NULL){

	piece->type = newType;
    }
}
