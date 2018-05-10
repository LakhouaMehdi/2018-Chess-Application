#include "Board.h"
#include "Constants.h"
#include "Move.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
//#include "MoveList.h"
#include "AI.h"


//Create a new board
BOARD* CreateBoard(){

	//create the board
	BOARD* board = malloc(sizeof(BOARD));
	assert(board != NULL);

	//create the array
	board-> b = malloc( BOARDWIDTH * BOARDHEIGHT * sizeof(PIECE*) ) ;
	
	if(board -> b == NULL) {
	    printf("Failed to initialize the board. Not enough memory.\n");
	    DeleteBoard(board);
	    return NULL;
	}

	InitializeBoard(board);

	return board;
}

//Delete a board
void DeleteBoard(BOARD* board){

	if(board != NULL)
	{
	    int x, y;

	    if(board -> b != NULL)
	    {   
		//free the pieces in the array
		for(x=0 ; x<BOARDWIDTH ; x++){
		    for(y=0 ; y<BOARDHEIGHT ; y++){
			DeletePiece( GetPiece(board,x,y) );		
		    }
		}

		//free the array	
		free(board->b);
	 	board-> b = NULL;	
	    }
 	    
	    //free the board
	    free(board);
	    board = NULL;
	}
}

//Get a Piece at given x and y. null if out of bounds
PIECE* GetPiece(const BOARD* board, int x, int y)
{
    assert (board     != NULL);
    assert (board-> b != NULL);
	
    if(x<0 || y<0 || x >= BOARDWIDTH || y >= BOARDHEIGHT) 
	return NULL;

    return board->b[x + y * BOARDWIDTH];
}

//Set a Piece at given x and y
void SetPiece(BOARD* board, int x, int y, PIECE* piece)
{	
    assert (board     != NULL);
    assert (board-> b != NULL);
    
    if(x<0 || y<0 || x >= BOARDWIDTH || y >= BOARDHEIGHT) 
	return;

    board->b[x + y * BOARDWIDTH] = piece;
}

//Initialize the default board for chess.
void InitializeBoard(BOARD* board) {
	
	//initialize all pieces to NULL
	int c,r;
	for(c=0; c<BOARDWIDTH ; c++) {
	    for(r=0; r<BOARDHEIGHT ; r++) {
		SetPiece(board, c, r, NULL);
	    }
	}
	
        //place the pieces
        //place pawns
        for(c=0 ; c< BOARDWIDTH ; c++) {
            SetPiece(board, c, 1             , CreatePiece(Pawn, Black) ); 
            SetPiece(board, c, BOARDHEIGHT-2 , CreatePiece(Pawn, White) );
	}

        //place kings
        SetPiece(board, BOARDWIDTH/2, 0            , CreatePiece(King, Black));
        SetPiece(board, BOARDWIDTH/2, BOARDHEIGHT-1, CreatePiece(King, White));

        //place queens
        SetPiece(board, BOARDWIDTH/2-1, 0            , CreatePiece(Queen, Black));
        SetPiece(board, BOARDWIDTH/2-1, BOARDHEIGHT-1, CreatePiece(Queen, White));
	
        //place other pieces(left side of queen)
        for(c=0 ; c< BOARDWIDTH/2-1 ; c++)
        {
            SetPiece(board, c, 0            , CreatePiece( piecePattern[c%patternWidth], Black) );
   	    SetPiece(board, c, BOARDHEIGHT-1, CreatePiece( piecePattern[c%patternWidth], White) );
        }
	    
        //place other pieces(right side of king)	
        for(c=0 ; c< BOARDWIDTH/2-1 ; c++)
        {
            SetPiece(board, BOARDWIDTH -1 -c, 0, CreatePiece( piecePattern[c%patternWidth], Black) );
   	    SetPiece(board, BOARDWIDTH -1 -c, BOARDHEIGHT-1, CreatePiece( piecePattern[c%patternWidth], White) );
	}

        //mark king's coordinates
        board->blackKingX = BOARDWIDTH/2;
        board->blackKingY = 0;	
        board->whiteKingX = BOARDWIDTH/2;
        board->whiteKingY = BOARDHEIGHT - 1 ;

	//set the castling permissions
	board -> wCastleQueen = true;
	board -> wCastleKing = true;
	board -> bCastleQueen = true;
	board -> bCastleKing = true;

	//set the en passant coordinates
	board -> enPassantX = -1;
	board -> enPassantY = -1;

        //set fifty moves
        board -> fiftyMoves = 0;
}

//Returns true if the square at the given coordinate attacked by the given side
//Note, pieces that can attack the square but cannot move because they are defending
//a king from a check are still counted as an attacker.
bool IsSquareAttacked(const BOARD* board, const int x, const int y, SIDE side)
{
	//verify pawn attacks
	if(PawnAttackers(board, x, y, side) > 0) return true;

	//verify knight attacks
	if(KnightAttackers(board, x, y, side) > 0) return true;

	//verify diagonal attacks for bishop and queen
	if(DiagonalAttackers(board, x, y, side) > 0) return true;

	//verify linear attacks	for rook and queen	
	if(LinearAttackers(board, x, y, side) > 0) return true;
	
	//verify king attacks
	if(KingAttackers(board, x, y, side) > 0) return true;
	
        return false;
}

//returns the number of kings of the given side attacking the square
int KingAttackers(const BOARD* board, const int x, const int y, SIDE side)
{
	if (board == NULL) {
	    return 0;
 	}

	if(x<0 || y<0 || x>BOARDWIDTH || y>BOARDHEIGHT) {
	    return 0;
	}
	
	//verify the square is not occupied by the player
	PIECE* piece = GetPiece(board,x,y);
	if(piece != NULL && piece-> player == side){
	    return 0;
	}

	int c,r;
	int count = 0;
	
	for(c = x-1 ; c<= x+1; c++) {
	    for(r = y-1; r <= y+1 ; r++) {
		piece = GetPiece(board,c,r);
		if(piece!= NULL && piece->player == side && piece->type == King)
		{	
		        count++;
		}
	    }
	}	

	return count;
}


// returns the number of pawns of the given side attacking the square
int PawnAttackers(const BOARD* board, const int x, const int y, SIDE side)
{
	if (board == NULL) {
	    return 0;
 	}

	if(x<0 || y<0 || x>BOARDWIDTH || y>BOARDHEIGHT) {
	    return 0;
	}

	//verify the square is not occupied by the player
	PIECE* piece = GetPiece(board,x,y);
	if(piece != NULL && piece-> player == side){
	    return 0;
	}

	int c, r;

	int count = 0;
	
	c=x;
	r=y;
	switch(side){
        
	    case White: 
  			//check the bottom right and bottom left squares for an attacking piece
		  	piece = GetPiece(board,c+1,r+1);
			if(piece != NULL && piece-> player == side && piece-> type == Pawn)
		 	{
			        count++;
			}

			piece = GetPiece(board,c-1,r+1);
			if(piece != NULL && piece-> player == side && piece-> type == Pawn)
			{
			        count++;
			}
			break;
	    case Black:
			//check the top right and top left squares for an attacking piece
			piece = GetPiece(board,c-1,r-1);
			if(piece != NULL && piece-> player == side && piece-> type == Pawn)
		 	{
			        count++;
			}

			piece = GetPiece(board,c+1,r-1);
			if(piece != NULL && piece-> player == side && piece-> type == Pawn)
			{
			        count++;
			}
			break;		
	    default: 
			break;
	}

	return count;
}

// returns the number of knights of the given side attacking the square
int KnightAttackers(const BOARD* board, const int x, const int y, SIDE side)
{
	if (board == NULL) 
	{
	    return 0;
 	}

	if(x<0 || y<0 || x>BOARDWIDTH || y>BOARDHEIGHT) 
	{
	    return 0;
	}
	
	//verify the square is not occupied by the player
	PIECE* piece = GetPiece(board,x,y);
	if(piece != NULL && piece-> player == side)
	{
	    return 0;
	}

	int a;
	int count=0;
	int c,r;		
	for(a = 0; a < 8; a++)
	{
	    c = x + jumpAttackX[a];
	    r = y + jumpAttackY[a];
	       
	    piece = GetPiece(board, c, r);
	    if(piece!= NULL && piece->player == side && piece->type == Knight)
	    {
		    count++;
            }
	}

	return count;
}

// returns the number of linear attacker(rooks or queens) of the given side attacking the square
int LinearAttackers(const BOARD* board, const int x, const int y,const SIDE side)
{
	if (board == NULL) 
	{
	    return 0;
 	}

	if(x<0 || y<0 || x>BOARDWIDTH || y>BOARDHEIGHT) 
	{
	    return 0;
	}

	//verify the square is not occupied by the player
	PIECE* piece = GetPiece(board,x,y);
	if(piece != NULL && piece-> player == side)
	{
	    return 0;
	}

	int c,r;	
	int a;
	int count=0;
	
	for(a = 0; a<4; a++) //for all 4 line directions
	{
	    c = x;
	    r = y;

	    while(c>=0 && c<BOARDWIDTH && r>=0 && r<BOARDHEIGHT) 
	    {
	        piece= GetPiece(board, c, r);
		
    		if(piece != NULL )
	 	{
		    if(piece->player == side && (piece->type == Rook || piece->type == Queen))
		    {
			    count++;
		    }
		    else if(c != x || r != y)	//do not break when checking the square itself.
			break;	    
		}
		
            c+= lineAttackX[a];
	    r+= lineAttackY[a];
	
            }
	}

	return count;
}

// returns the number of diagonal attackers(bishops or queens) of the given side attacking the square
int DiagonalAttackers(const BOARD* board, const int x, const int y, SIDE side)
{
	if (board == NULL) 
	{
	    return 0;
 	}

	if(x<0 || y<0 || x>BOARDWIDTH || y>BOARDHEIGHT) 
	{
	    return 0;
	}

	//verify the square is not occupied by the player
	PIECE* piece = GetPiece(board,x,y);
	if(piece != NULL && piece-> player == side)
	{
	    return 0;
	}

	int c,r;	
	int a;
	int count=0;
	
	for(a = 0; a<4; a++)   //for all 4 diagonal directions
	{
	    c = x;
	    r = y;

	    while(c>=0 && c<BOARDWIDTH && r>=0 && r<BOARDHEIGHT) 
	    {
	        piece= GetPiece(board, c, r);
		if(piece != NULL)
	 	{
		    if(piece->player == side && (piece->type == Bishop || piece->type == Queen))
		    { 
			    count++;
		    }
		    else if(c != x || r != y)
			break;
		}
	
            c+= diagAttackX[a];
	    r+= diagAttackY[a];
	
            }
	}

	return count;
}

//returns the next player
SIDE GetNextPlayer(const SIDE currentPlayer)
{
    if( currentPlayer == White ) 
	return Black; 
    else 
	return White;
}

//returns true if the king of the given side is in check
bool IsInCheck(const BOARD* board, const SIDE side)
{
	int x = GetKingX( board, side);
 	int y = GetKingY( board, side);

	return IsSquareAttacked(board, x, y, GetNextPlayer(side)) ;
} 


//sets the values of the king's coordinates that are stored in the board
void SetKingCoordinates(BOARD* board, const SIDE side, int x, int y)
{
    switch(side)
    {
	case White:
	    board -> whiteKingX = x;
	    board -> whiteKingY = y;
	    break;
	case Black:
	    board -> blackKingX = x;
	    board -> blackKingY = y;
	    break;
    }

}

//returns the x coordinate of the king of the given side (column)
int GetKingX(const BOARD* board, const SIDE side)
{
    switch(side)
    {
	case White:
	    return board -> whiteKingX ;
	    break;
	case Black:
	    return board -> blackKingX;
	    break;
	default:
	    return -1;
    }

}

//returns the y coordinate of the king of the given side (row)
int GetKingY(const BOARD* board, const SIDE side)
{
    switch(side)
    {
	case White:
	    return board -> whiteKingY ;
	    break;
	case Black:
	    return board -> blackKingY;
	    break;
	default:
	    return -1;
    }
}

//returns true if king of the given side can move to one of the neighboring squares
bool KingCanMove (BOARD* board, const SIDE side)
{
	bool canMove = false;

	int x = GetKingX ( board, side);
	int y = GetKingY ( board, side);

	int c, r;
	SIDE enemy =GetNextPlayer(side);
	PIECE* piece;

	for(c= x -1 ; c <= x+1 ; c++)
	{
	    for(r = y -1 ; r <= y +1 ; r++)
	    {
		if( c<0 || r<0 || c>=BOARDWIDTH || r>=BOARDHEIGHT) 
		{
		    continue;
		}

		piece = GetPiece(board, c,r);

		//if there is an ally piece, cannot move there.
		if(piece != NULL && piece -> player == side)
		{
		    continue;
		}

		//remove the king
		PIECE* king = GetPiece(board,x,y);
		SetPiece(board,x,y,NULL);
		
		//there is an empty square
	        //verify that when the king gets removed to be placed on that square, the square is still not attacked
		//if the king is the piece blocking an attack on the square, the square is not safe to move to.		    
		if(piece == NULL && !IsSquareAttacked(board, c, r, enemy)) {
  		    //there is an empty and not attacked square		
		    canMove = true;
		}

		//there is an ennemy piece
		if( piece != NULL && piece-> player == enemy) 
		{
		    //remove it
		    SetPiece(board, c, r, NULL);

		    if ( !IsSquareAttacked(board, c, r, enemy) ) 
	 	    {
		        //the square is safe, put the piece back and return true
	  	        canMove = true;
		    }
		 
  		    // put the piece back 
		    SetPiece(board, c, r, piece); 
		  
		}	

  		//place the king back
		SetPiece(board,x,y, king);
		
	    }
	}

 	return canMove;   
}

//Returns true if king of the given side is in checkmate on the board
bool IsCheckmate( BOARD* board, const SIDE side)
{ 

    if( IsInCheck(board,side) )
    {
        MLIST* list = GetAllValidMoves(board, side);
 	if( list-> First == NULL )
	{
	    DeleteMoveList(list);	
	    return true;
	}
    }

    return false;  //No piece could be placed in between and attacker can't be captured. ==> checkmate.

}

/*
bool IsCheckmateVersion2 ( BOARD* board, const SIDE side)
{

    int kingX = GetKingX ( board, side);
    int kingY = GetKingY ( board, side);

    SIDE enemy =GetNextPlayer(side);	  

    int kingAttackers     = KingAttackers     (board, kingX, kingY, enemy);
    int pawnAttackers     = PawnAttackers     (board, kingX, kingY, enemy);
    int knightAttackers   = KnightAttackers   (board, kingX, kingY, enemy);
    int diagonalAttackers = DiagonalAttackers (board, kingX, kingY, enemy);
    int linearAttackers   = LinearAttackers   (board, kingX, kingY, enemy);
 
    int numberOfAttackers = kingAttackers
			  + pawnAttackers
			  + knightAttackers
			  + diagonalAttackers
			  + linearAttackers ;


    if (numberOfAttackers <= 0 || KingCanMove(board,side))
    {   
	//King is not in check or has a safe square to move to
	return false;
    }
    else if (numberOfAttackers > 1) 
    {
        //King is attacked by multiple enemy pieces and has nowhere to move, return true.
        //The king can only be saved from one of the attacks
	return true;
    }
    
    //King is attacked by only one piece
    assert(numberOfAttackers == 1);

    //Figure out which piece it is.
    //Figure out if the attacking piece can be captured, or can be blocked by own piece.
    
    int c, r;
    PIECE* piece;
    int attackerX= -1 , attackerY = -1;	  //Attacker's position
    
    //Holds Bishop if attack is diagonal, Rook if attack is linear.  
    PIECETYPE possibleAttacker = (pawnAttackers     == 1) ? Pawn :
				 (kingAttackers     == 1) ? King :
				 (knightAttackers   == 1) ? Knight :
				 (diagonalAttackers == 1) ? Bishop :
				 (linearAttackers   == 1) ? Rook : None ;
   
    assert(possibleAttacker != None);

    //Find the attacker
    for(c=0 ; c<BOARDWIDTH ; c++) 
    {
   	for(r=0 ; r<BOARDHEIGHT ; r++) 
        {    
	    piece = GetPiece(board,c,r);
	    if(piece == NULL || piece -> player != enemy || (piece-> type != possibleAttacker && piece -> type != Queen) ) 
	    {    	
		continue; //We don't care about this piece. It cannot be the one attacking.
	    }
	    else
	    {
	        MOVE* move = CreateMove(c,r,kingX,kingY);
		if(IsValidMove(board, move, enemy))
		{
		    attackerX = c;    //attacker has been found
		    attackerY = r;
		}
		DeleteMove(move);
	    }
	}
    }	     

    //We have the attacker's position
    assert(attackerX != -1 && attackerY != -1);

//Need to verify				
    MLIST* list = GetAllValidMoves(board, side);
    MENTRY* curr = list->First;
    
    if( IsInCheck(board,side) && list -> First == NULL )
    {
	return true;
    }

    while(curr != NULL)
    {
	if(IsValidMove(board, curr->Move, side) && curr -> Move -> toX == attackerX && curr -> Move -> toY == attackerY)
	{
	    DeleteMoveList(list);
	    return false;
	}

	curr = curr->Next;
    }
    DeleteMoveList(list);

//    if( IsSquareAttacked(board, attackerX, attackerY, side) )
//    {
//	//Attacker can be captured by own piece.
//	return false;
//    }
//
    
    //We cannot capture the attacker.
    if( possibleAttacker == Knight || possibleAttacker == Pawn || possibleAttacker == King )
    {
	//The attacking piece is a piece that cannot be blocked.
	//The king cannot be saved.
        return true;
    }
   
    //If the attack is a linear attack, see if it can be blocked.
    if( possibleAttacker == Rook )
    {
	assert( (attackerX == kingX) ^ (attackerY == kingY)  );  //xor. Attacker is on same row or same column but not both

	int increment = 0;

	if(attackerX == kingX )
	{
	    //Attacker is on the same column as the king
    	    increment = (attackerY < kingY)? -1 : 1 ; //Above or below
  
	    r = kingY;
            while( r != attackerY ) 
	    {
	        if(IsSquareAttacked(board,kingX, r , side) ) 
		{
		    printf("checked heeeeere");
		    return false;      //A piece can be placed in between to block the attack
		}
		r+= increment;	
	    } 
	}
	else	
	{
	    //Attacker is on the same row as the king
	    increment = (attackerX < kingX)? -1 : 1 ;  //Right or left
	    
	    c = kingX;
	    while( c != attackerX ) 
	    {
		if(IsSquareAttacked(board, c, kingY, side))
		{   printf("checked heeer");
		    return false;	//A piece can be placed in between
		}
		c+= increment;
	    }
	}    
    }

    if( possibleAttacker == Bishop )
    {
        int incrementX = 0;
	int incrementY = 0;

	for(int a= 0 ; a<8 ; a++)
	{
	    if(attackerX == kingX + a) 
	 	incrementX = 1;
	   
	    if(attackerX == kingX - a) 
		incrementX =-1;

	    if(attackerY == kingY + a) 
		incrementY = 1;

	    if(attackerY == kingY - a) 
		incrementY =-1;
	}

	c = kingX;
	r = kingY;

	while( c!= attackerX && r!= attackerY )
	{
	    if(IsSquareAttacked(board, c, r, side))
	    {   printf("checked heere" );   
		return false;		//A piece can be placed in between
	    }
	    
            c+= incrementX;
	    r+= incrementY;
	}
 
    }
    
    return true;  //No piece could be placed in between and attacker can't be captured. ==> checkmate.

}
*/
