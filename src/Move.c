/**********************************************************************/
/*               Project 1, EECS 22L, Winter 2018                     */
/*                                                                    */
/*               Author: Team 4                                       */
/*               Date: 1/12/2018                                      */
/*               Last Updated: 1/25/2018			      */
/*                                                                    */
/*               Move.c: source file for move function                */
/*                                                                    */
/**********************************************************************/

#include "Move.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/* Create a move */
MOVE* CreateMove(int fromX, int fromY, int toX, int toY){

    MOVE* move = malloc(sizeof(MOVE));

    if (move != NULL){
	move->fromX         = fromX;
	move->fromY         = fromY;
	move->toX           = toX;
	move->toY           = toY;
	move->capturedPiece = None; 
	move->promotion     = false;
	move->enPassant     = false;
	move->castling      = false;
    }
    else
	return NULL;

    return move;
}

/* Delete a move */
void DeleteMove(MOVE* move){

    if (move != NULL){
	free(move);
	move = NULL;
    }	
}

/* Check if a move is valid for the given player */
bool IsValidMove(BOARD* board,const  MOVE* move, const SIDE side){
 
 // if board or move are NULL, move is not valid
    if(board == NULL || move == NULL)
    {
        return false;
    }

 // verify coordinate are within the board
   
    // verify from coordinates
    if(move-> fromX < 0  || move-> fromX >= BOARDWIDTH) 
        return false;

    if(move-> fromY < 0  || move-> fromY >= BOARDHEIGHT) 
        return false;

    // verify to coordinates 
    if(move-> toX < 0 || move -> toX >= BOARDWIDTH) 
	return false;

    if(move-> toY < 0 || move -> toY >= BOARDHEIGHT) 
	return false;

    PIECE* fromPiece = GetPiece(board, move-> fromX, move-> fromY);   //piece in the start position
    PIECE* toPiece = GetPiece(board, move->toX, move->toY);	      //piece in the to position

    // verify starting square is occupied by the player
    if ( fromPiece == NULL || fromPiece-> player != side){
	return false;
    }

    // verify ending square is not occupied by the player
    if (toPiece != NULL && toPiece -> player == side){
        return false;
    }

    //check that the move doesnt put the king in check   
    SIDE enemy = GetNextPlayer(side);
    
    //do the move
    SetPiece(board, move-> fromX, move-> fromY, NULL);
    SetPiece(board, move -> toX, move-> toY, fromPiece);

    //if king was moved check if the new location is attacked.
    //if another piece was moved check if the king's location is attacked
    int KingPosX = (fromPiece -> type == King) ? move -> toX : GetKingX(board, side);
    int KingPosY = (fromPiece -> type == King) ? move -> toY : GetKingY(board, side);

    if(IsSquareAttacked(board, KingPosX , KingPosY, enemy) )
    {
	//if it is, place the pieces back and return false
	SetPiece(board, move -> toX, move -> toY, toPiece);
        SetPiece(board, move -> fromX, move -> fromY, fromPiece);
	return false;
    }
    
    //else, place the pieces back
    SetPiece(board, move -> toX, move -> toY, toPiece);
    SetPiece(board, move -> fromX, move -> fromY, fromPiece);
    
    //check if the move is valid for the given piece type
    switch ( fromPiece -> type ) {

	case Pawn: 
	    return ValidPawnMove(board, move, side);
	    break;
	case Rook:
	    return ValidRookMove(board, move);
	    break;
	case Knight:
	    return ValidKnightMove(board, move);
	    break;
	case Bishop:
	    return ValidBishopMove(board, move);
	    break; 
	case King:
	    return ValidKingMove(board, move);
	    break;
	case Queen:
	    return ValidQueenMove(board, move);
	    break;
	default:
	    return false;  // default case, the piece type is not a valid chess piece, false.
    }
}

/* Check if King move is valid */
bool ValidKingMove(const BOARD* board, const MOVE* move) {

    assert(board);
    assert(move);
    assert(GetPiece(board, move-> fromX, move-> fromY) != NULL);
    assert(GetPiece(board, move-> fromX, move-> fromY) -> type == King);

    int c, r;
    int x1 = move->fromX;
    int y1 = move->fromY; 

    int x2 = move-> toX;
    int y2 = move-> toY;

    //moving to one of the surrounding squares	
    for (c = x1-1; c <= x1+1; ++c) {
	for (r = y1-1; r <= y1+1; ++r) {
	    if (c == x2 && r == y2) {
		return true;
	    }
	}
    }

    //the destination is not on the surrounding squares, so it is either a castling move or an invalid move
    PIECE *king = GetPiece(board, move->fromX, move->fromY);

    //if the king is in check, he cannot castle. return false
    if(IsInCheck(board ,king -> player) )
    {
	return false;
    }

    //verify if the move is a valid castle move for the player
    switch (king->player)
    {
	case White:

		return
		  ( board->wCastleQueen										      //permitted queen castle
		  && x2 == 2 && y2 == 7 								   	      //correct coordinates
		  && GetPiece(board, 1, 7) == NULL && GetPiece(board, 2, 7) == NULL && GetPiece(board, 3, 7) == NULL) //no pieces in between
		  ||		  
		  ( board->wCastleKing						      //permitted king castle
		  && x2 == 6 && y2 == 7 					      //correct coordinates
		  && GetPiece(board, 5, 7) == NULL && GetPiece(board, 6, 7) == NULL); //no pieces in between

		break;

	case Black:
		return ( board->bCastleQueen	 								  	    //permitted queen castle
		       && x2 == 2 && y2 == 0 										    //correct coordinates
		       && GetPiece(board, 1, 0) == NULL && GetPiece(board, 2, 0) == NULL && GetPiece(board, 3, 0) == NULL)  //no pieces in between
    		       ||
		       ( board->bCastleKing	 					   //permitted king castle
                       && x2 == 6 && y2 == 0 						   //correct coordinates
		       && GetPiece(board, 5, 0) == NULL && GetPiece(board, 6, 0) == NULL); //no pieces in between
		
		break;
    }

    return false;
}

/* Check if Knight Move is valid */
bool ValidKnightMove(const BOARD* board, const MOVE* move) {

    assert(board);
    assert(move);
    assert(GetPiece(board, move-> fromX, move-> fromY) != NULL);
    assert(GetPiece(board, move-> fromX, move-> fromY) -> type == Knight);

    int a;

    for (a = 0; a < 8; a++){
 	
	if ((move->toX == move->fromX + jumpAttackX[a]) && (move->toY == move->fromY + jumpAttackY[a])){
	    return true;
	}
    }

    return false;
}

/* Check if Pawn move is valid */
bool ValidPawnMove(const BOARD* board, const MOVE* move, const SIDE side) 
{
    assert(board);
    assert(move);
    assert(GetPiece(board, move-> fromX, move-> fromY) != NULL);
    assert(GetPiece(board, move-> fromX, move-> fromY) -> type == Pawn);

    int x1 = move->fromX;
    int y1 = move->fromY;
    int x2 = move->toX;
    int y2 = move->toY;

    switch (side) {
	case White:
	    return 
		   (x2 == x1 && y2 == y1-1 && GetPiece(board,x2,y1-1) == NULL) // move once forward
	       ||  
	           (x2 == x1 && y2 == y1-2 && GetPiece(board,x2,y1-1) == NULL 
		   && GetPiece(board,x2,y1-2) == NULL && y1 == BOARDHEIGHT-2) // move twice forward
	       || 
	  	   (x2 == x1-1 && y2 == y1-1 && GetPiece(board,x1-1,y1-1) != NULL 
	  	   && GetPiece(board,x1-1,y1-1)->player == Black)  // capture diagonally on the left
	       || 
	  	   (x2 == x1+1 && y2 == y1-1 && GetPiece(board,x1+1,y1-1) != NULL 
		   && GetPiece(board,x1+1,y1-1)->player == Black) // capture diagonally on the right
	       ||
		   (x2 == x1-1 && y2 == y1-1 && x2 == board-> enPassantX && y2 == board-> enPassantY) // capture en passant on the left
	       ||
		   (x2 == x1+1 && y2 == y1-1 && x2 == board-> enPassantX && y2 == board-> enPassantY); // capture en passant on the right
	    break;

	case Black:
	    return 
	           (x2 == x1 && y2 == y1+1 && GetPiece(board,x2,y1+1) == NULL) // move once forward
	       ||  
		   (x2 == x1 && y2 == y1+2 && GetPiece(board,x2,y1+1) == NULL 
		   && GetPiece(board,x2,y1+2) == NULL && y1 == BOARDHEIGHT-7) // move twice forward
	       || 
	           (x2 == x1-1 && y2 == y1+1 && GetPiece(board,x1-1,y1+1) != NULL 
		   && GetPiece(board,x1-1,y1+1)->player == White)    // capture diagonally on the left
  	       || 
		   (x2 == x1+1 && y2 == y1+1 && GetPiece(board,x1+1,y1+1) != NULL
		   && GetPiece(board,x1+1,y1+1)->player == White)    // capture diagonally on the right
	       ||
		   (x2 == x1-1 && y2 == y1+1 && x2 == board-> enPassantX && y2 == board-> enPassantY)  // capture en passant on the left

	       ||
		   (x2 == x1+1 && y2 == y1+1 && x2 == board-> enPassantX && y2 == board-> enPassantY); // capture en passant on the right
	    break;
    }

    return false; 
}

/* Check if Bishop move is valid */
bool ValidBishopMove(const BOARD* board, const MOVE* move) {
    
    assert(board);
    assert(move);
    assert(GetPiece(board, move-> fromX, move-> fromY) != NULL);
  
    int c,r;
    int a;
    
    PIECE* piece;

    for(a=0; a<4 ; a++){  // for all 4 diagonal directions  
        c = move-> fromX; // start at fromX,fromY
        r = move-> fromY;

	// while it hasn't reached the end of the board in that direction
        while(c>=0 && c<BOARDWIDTH && r>=0 && r<BOARDHEIGHT){

            c += diagAttackX[a]; // move c and r in the direction
  	    r += diagAttackY[a];   
 
	    // if it encounters a piece and it's not yet at its destination,
	    // break out of the while loop, stop checking this direction;
	    piece = GetPiece(board, c, r);
	    if ( piece != NULL && (c != move -> toX || r != move -> toY) ){
	        break;	
	    }

	    // if it reaches the destination, return true
            if (c == move -> toX && r == move -> toY){
		return true;
	    }

	}
    }

    //check all 4 directions and havn't reached the destination
    return false;
}

/* Check if Rook move is valid */
bool ValidRookMove(const BOARD* board, const MOVE* move) {

    assert(board);
    assert(move);
    assert(GetPiece(board, move-> fromX, move-> fromY) != NULL);

    int c,r;
    int a;

    PIECE* piece;

    for (a = 0; a < 4; ++a) {   // for all 4 linear directions
	// start at fromX, fromY
	c = move-> fromX;
	r = move-> fromY;
	
	// loop until reaching the end of the board in one direction
	while (c>=0 && c<BOARDWIDTH && r>=0 && r<BOARDHEIGHT) {

	// move c and r in the linear direction
	    c += lineAttackX[a]; 
	    r += lineAttackY[a];
	
	// If there is a piece on the way before it reaches the destination,
	// break the while loop and stop checking in that direction
	    piece = GetPiece(board, c, r);
	    if (piece != NULL && (c != move-> toX || r != move-> toY)) {
	    	break;
	    }
	    
	// if destination is reached, return true
	    if (c == move-> toX && r == move-> toY) {
		return true;
	    }

	}
    }
	// checked all 4 directions and destination is not reached
    return false;
}

/* Check if Queen move is valid */
bool ValidQueenMove(const BOARD* board, const MOVE* move) {

    assert(board);
    assert(move);
    assert(GetPiece(board, move-> fromX, move-> fromY) != NULL);
    assert(GetPiece(board, move-> fromX, move-> fromY) -> type == Queen);

// Use rook and bishop function
    return ValidBishopMove(board, move) || ValidRookMove(board, move);
}

/* Perform a move on the board */
void PerformMove(BOARD* board, MOVE* move)
{
   //Note: this function will perform the move. whether it is valid or not.

    if(board == NULL || move == NULL)
    {
        return;
    }

    //initial position
    int x1 = move -> fromX;
    int y1 = move -> fromY;

    //destination
    int x2 = move -> toX;
    int y2 = move -> toY;

    PIECE* fromPiece = GetPiece(board, x1, y1);		//the piece in the from position
    PIECE* toPiece = GetPiece(board, x2, y2);		//the piece in the to position
    
    if(fromPiece == NULL)
    {
	//no piece to move, return;
	return;
    }

    //Verify if there is a piece to capture
    if(toPiece != NULL)
    {
	//if yes, Capture the piece
	move -> capturedPiece = toPiece -> type;	//store the type of the piece for undoing
        DeletePiece(toPiece);
    }

    //The destination spot is now empty, move the piece    
    SetPiece(board, x2, y2, fromPiece);
    SetPiece(board, x1, y1, NULL);
     
    //store the previous castling permissions for undoing
    switch ( fromPiece -> player )
    {
	case White:
		move -> prevCastleKing = board -> wCastleKing;
		move -> prevCastleQueen = board -> wCastleQueen;
		break;
	case Black:
		move -> prevCastleKing = board -> bCastleKing;
		move -> prevCastleQueen = board -> bCastleQueen;
		break;
    }
 
    //store the previous en passant coordinates for undoing
    move -> prevEnPassantX = board-> enPassantX;
    move-> prevEnPassantY = board-> enPassantY;

  
    bool doublePawnMove = false;
   
    switch( fromPiece -> type )
    {   
	//if piece moved is a pawn, need to verify promotion, and en passant capture, and whether to update the en passant coordinates
	case Pawn:	

		//Verify if it is a pawn promotion
		move -> promotion = ( y2 == 0 && fromPiece -> player == White)||
				    ( y2 == BOARDHEIGHT-1 && fromPiece -> player == Black);	//promotion is done outside this function

     		switch (fromPiece -> player)
		{
            	    case White:
			    //if the pawn is moving on the en passant coordinates, capture the pawn below it
	    		    if( x2 == board -> enPassantX && y2 == board -> enPassantY && x1!= x2 )
			    {
		    	        PIECE* passantPawn = GetPiece(board, board->enPassantX, board->enPassantY +1) ;	
			        assert (passantPawn != NULL);
			        assert (passantPawn -> type == Pawn);
 			        assert (passantPawn -> player != fromPiece -> player);
			        DeletePiece( passantPawn );
			        SetPiece(board,board->enPassantX, board->enPassantY +1, NULL);
		  	        move -> enPassant = true;		//store that the move was an en passant capture, for undoing
			    }
				
			    //verify if it is moving twice forward
			    if(move -> fromY == BOARDHEIGHT-2 && move -> toX == move -> fromX && move -> toY == move -> fromY -2)
			    {   
				//if yes, set the new en passant coordinates on the board
				doublePawnMove = true;
			        board->enPassantX = move -> toX;
			        board->enPassantY = move -> toY+1;
			    }
		    break;
	
		    case Black:
	 	            //if the pawn is moving on the en passant coordinates, capture the  pawn above it
	  		    if( x2 == board -> enPassantX && y2 == board -> enPassantY && x1!= x2 )
			    {
			   	 PIECE* passantPawn = GetPiece(board, board->enPassantX, board->enPassantY -1) ;	
			   	 assert (passantPawn != NULL);
			   	 assert (passantPawn -> type == Pawn);
			     	 assert (passantPawn -> player != fromPiece -> player);
			   	 DeletePiece( passantPawn );	
 			   	 SetPiece(board,board->enPassantX, board->enPassantY -1, NULL);
			   	 move -> enPassant = true;
			    }
			
			    //verify if it is moving twice forward to set new en passant coordinates	
			    if(move -> fromY == 1 && move -> toX == move -> fromX && move -> toY == move -> fromY + 2)
		  	    {
				  doublePawnMove = true;
 			  	  board->enPassantX = move -> toX;
				  board->enPassantY = move -> toY-1;		
			    }
  		        break;
		}

	    break;

	//if the piece moved is a king, must set the new king coordinates on the board, move the correct rook if it is a castling
	//move, and set the castling permissions to false
	case King:

		//update the king coordinates stored by the board.
		SetKingCoordinates( board, fromPiece-> player, x2, y2);

		switch( fromPiece -> player)
		{
		case White: 
			if (x1 == 4 && y1 == 0 && x2 == 2 && y2 == 7)
			{
	    	    	    move -> castling = true;
		   	    //perfrom castling for left side
		    	    MOVE* rookMove = CreateMove(0,7,3,7);
		   	    PerformMove(board, rookMove);
		    	    DeleteMove(rookMove);
		 	}
			else if(x1 == 4 && y1 == 0 && x2 == 6 && y2 == 7)
			{
	    	    	   move -> castling = true;
		   	   //perform castling for right sie
		 	   MOVE* rookMove = CreateMove(7,7,5,7);
		 	   PerformMove(board, rookMove);
		 	   DeleteMove(rookMove);
			}
			else;
		
		        board -> wCastleQueen = false;
			board -> wCastleKing  = false;
			break;

		case Black:
			if (x1 == 4 && y1 == 7 && x2 == 2  && y2 == 0)
			{
	    	    	    move -> castling = true;
		   	    //perfrom castling for left side
		    	    MOVE* rookMove = CreateMove(0,0,3,0);
		   	    PerformMove(board, rookMove);
		    	    DeleteMove(rookMove);
		 	}
			else if(x1 == 4 && y1 == 7 && x2 == 6 && y2 == 0)
			{
	    	    	   move -> castling = true;
		   	   //perform castling for right sie
		 	   MOVE* rookMove = CreateMove(7,0,5,0);
		 	   PerformMove(board, rookMove);
		 	   DeleteMove(rookMove);
			}
			else;

		        board -> bCastleQueen = false;
			board -> bCastleKing  = false;
			break;
		}

	    break;
	default: 
	    break;
    }

    // verify if castle permissions of the player impacted by the move (the rook was moved, or captured)
    if ( (x1 == 0 && y1 == BOARDHEIGHT -1)|| (x2 == 0 && y2 == BOARDHEIGHT -1) )
	board -> wCastleQueen = false;

    if ( (x1 == 0 && y1 == 0)|| (x2 == 0 && y2 == 0) )
	board -> bCastleQueen = false;

    if ( (x1 == BOARDWIDTH -1 && y1 == BOARDHEIGHT -1)|| (x2 == BOARDWIDTH-1 && y2 == BOARDHEIGHT -1) )
	board -> wCastleKing = false;

    if ( (x1 == BOARDWIDTH -1 && y1 == 0)|| (x2 == BOARDWIDTH -1 && y2 == 0) )
	board -> bCastleKing = false;


    //if the move was not a doble pawn move, reset the en passant coordinates
    if(!doublePawnMove)
    {
        board-> enPassantX = -1;
        board-> enPassantY = -1;
    }

}

/* Undoes a move on the board */
void UndoMove(BOARD* board, MOVE* move)
{
    if(board == NULL || move == NULL)
    {
        return;
    }

    //store the from and destination piece
    PIECE* fromPiece = GetPiece(board, move-> toX, move-> toY);
    PIECE* toPiece = GetPiece(board, move-> fromX, move-> fromY);

    //coordinates are flipped    
    int toX = move -> fromX;
    int toY = move -> fromY; 
    int fromX = move -> toX;
    int fromY = move -> toY;

    if(fromPiece == NULL)
    {
	//no piece to move
	return;
    }
    
    //if something moved into the original spot delete it to ensure no memory leaks
    if(toPiece != NULL)
    {   
        DeletePiece(toPiece);
    }

    //the destination spot is empty, move the piece back    
    SetPiece(board, toX, toY, fromPiece);
    SetPiece(board, fromX, fromY, NULL);
 
    //if the move was a capture give the piece back to the ennemy
    if( move -> capturedPiece != None)
    {
	SetPiece(board, fromX, fromY, CreatePiece(move-> capturedPiece, GetNextPlayer(fromPiece -> player)));
 	move -> capturedPiece = None;
    }   

    //if the piece moved is a king, update the king coordinates stored by the board.
    if (fromPiece -> type  == King)
    {
	SetKingCoordinates( board, fromPiece-> player, toX, toY);
    }

    //if the move was a pawn promotion, demote the piece to a pawn 
    if( move -> promotion == true )
    {
	fromPiece -> type = Pawn;
	 move -> promotion = false;
    }

    //reset the castling permissions to what they were
    switch ( fromPiece -> player )
    {
	case White:
		board -> wCastleKing = move -> prevCastleKing ;
		board -> wCastleQueen = move -> prevCastleQueen ;
		break;
	case Black:
		board -> bCastleKing = move -> prevCastleKing;
		board -> bCastleQueen = move -> prevCastleQueen;
		break;
    } 
    
    //if the move was a castling move, move the rook back to it's position
    if (move -> castling == true)
    {
	if (fromX == 2 && fromY == 0)
	{
	    assert(GetPiece(board, 3, 0) != NULL);
	    SetPiece(board, 0, 0, GetPiece(board, 3, 0));
	    SetPiece(board, 3, 0, NULL);
	}
	else if (fromX == 6 && fromY == 0)
	{
	    assert(GetPiece(board, 5, 0) != NULL);
	    SetPiece(board, 7, 0, GetPiece(board, 5, 0));
	    SetPiece(board, 5, 0, NULL);
	}
	else if (fromX == 2 && fromY == 7)
	{
	    assert(GetPiece(board, 3, 7) != NULL);
	    SetPiece(board, 0, 7, GetPiece(board, 3, 7));
	    SetPiece(board, 3, 7, NULL);
	}
	else if (fromX == 6 && fromY == 7)
	{
	    assert(GetPiece(board, 5, 7) != NULL);
	    SetPiece(board, 7, 7, GetPiece(board, 5, 7));
	    SetPiece(board, 5, 7, NULL);
	}
	else;
    }

    //reset the en passant coordinates to what they were
    board->enPassantX = move -> prevEnPassantX;
    board->enPassantY = move-> prevEnPassantY;

    //if the move was an en passant capture, give the captured pawn back to the ennemy
    if(move->enPassant == true)
    {
	switch(fromPiece->player)
	{
	    case White:
		//give a piece back below it
		SetPiece(board, fromX, fromY+1, CreatePiece(Pawn, Black));
	        break;
	    case Black:
		//give a piece back above it
		SetPiece(board, fromX, fromY-1, CreatePiece(Pawn, White));
		break;
	}
	move->enPassant = false;
    }


}


