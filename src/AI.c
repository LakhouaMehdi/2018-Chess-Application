#include <stdlib.h>
#include <stdio.h>
#include "AI.h"
#include <time.h>
#include <assert.h>

//creates and returns the best move for the given player
MOVE* GetBestMove(BOARD* board,const SIDE player, int searchDepth)
{
    srand(time(NULL));

    MLIST* list = GetAllValidMoves(board, player);
 
    if (list == NULL || list -> First == NULL)
    {
	//Player has no valid moves, nothing to return.
	DeleteMoveList(list);
	return NULL;
    } 

    MENTRY* curr = list -> First;
    int bestScore = -INFINITY -1;     
    
    //guaranteed one return value
    MLIST* bestMoves = NULL;		//stores the moves with the best score

    //loop on all possible moves
    while(curr != NULL)
    {
	MOVE* currentMove = curr -> Move;
	if(currentMove != NULL) 
	{
	    //perform the search
	    PerformMove(board, currentMove);

	    //account for promotion
  	    if(curr -> Move -> promotion)
	    {
	        SetType( GetPiece(board, curr->Move->toX, curr->Move->toY) , Queen );
	    }

	    //evaluate the score of the move. (recursively check follow up moves from ennemy player)
	    int newScore = -AlphaBeta(searchDepth, -INFINITY, INFINITY, board,GetNextPlayer(player)) ;

	    if( newScore > bestScore )
	    {  
		//update the best score
		bestScore = newScore;

		//previous best moves are no longer relevant
		DeleteMoveList(bestMoves);
		bestMoves = CreateMoveList();

		//append the new best move
		MOVE* newBestMove = CreateMove(-1,-1,-1,-1);
		CopyMove( newBestMove, currentMove );
		AppendMove(bestMoves, newBestMove);
	    }
	    else if( newScore == bestScore )
	    {  
		//append the new best move
		MOVE* newBestMove = CreateMove(-1,-1,-1,-1);
		CopyMove( newBestMove, currentMove );
		AppendMove(bestMoves, newBestMove);

	    }
	    else;
 
	    UndoMove(board, currentMove);
	}	

	curr = curr -> Next;
    }

	//delete the validMoves list
	DeleteMoveList(list);

	//we have at least one bestMove to return
	assert(bestMoves -> Length > 0);
	assert(bestMoves -> First != NULL);

	//pick at random
	int i = 0;
	int selectedIndex = rand() % (bestMoves -> Length);
	
	curr = bestMoves -> First;

	MOVE* bestMove = CreateMove(-1,-1,-1,-1);
	CopyMove(bestMove, curr -> Move);
	
	while(curr != NULL)
	{
	    if( i == selectedIndex )
	    {   
	        CopyMove(bestMove, curr-> Move);
		break;
	    }

	    i++;
	    curr = curr -> Next;
	}

	DeleteMoveList(bestMoves);

	return bestMove;
}

void CopyMove(MOVE* move, const MOVE* source)
{
   assert( move != NULL);
   assert( source != NULL);

   move -> fromX = source -> fromX;
   move -> fromY = source -> fromY;

   move -> toX = source -> toX;
   move -> toY = source -> toY;
}

int AlphaBeta(int depth, int alpha, int beta, BOARD* board,const SIDE player)
{
    if(depth < 1)
    {
	return EvaluateBoard(board, player);
    }

    MLIST* list = GetAllValidMoves(board, player);
    MENTRY* curr = list -> First;

    int val = -INFINITY;
    while(curr != NULL)
    {
	PerformMove(board, curr -> Move);

	if(curr -> Move -> promotion)
	{
	    SetType( GetPiece(board, curr->Move->toX, curr->Move->toY) , Queen );
	}

	val = - AlphaBeta(depth-1, -beta, -alpha, board, GetNextPlayer(player)) ;

	UndoMove(board, curr -> Move);

        if (val >= beta)
	{
	    DeleteMoveList(list);
	    return val;
	}
	if (val > alpha)
	{
	    alpha = val;	
	}
	curr = curr -> Next;
    }

	DeleteMoveList(list);
	return alpha;
}


//evaluates the board and returns a score for the given player
int EvaluateBoard(const BOARD* board, const SIDE player)
{
    int score = 0;
    PIECE* piece;

    for(int c = 0 ; c< BOARDWIDTH; c++)
    {
	for(int r = 0; r< BOARDHEIGHT; r++)
	{
	    piece = GetPiece(board,c,r);
	    if(piece != NULL)
	    {
		score += EvaluatePiece(piece, player);
	    }
	}	    
    }
    return score;
}

//returns the value of a piece (positive if own side, negative if ennemy piece)
int EvaluatePiece(const PIECE* piece, const SIDE player)
{
    if(piece == NULL)
    {
	return 0;
    }
      
    int value = 0;
    switch(piece -> type)
    {
        case King:   value = 100;
		break;
        case Queen:  value =   9;
		break;
        case Rook:   value =   5;
		break;
        case Knight: value =   3;
		break;
        case Bishop: value =   3;
		break;
        case Pawn:   value =   1;
		break;
        default :    value =   0;
    } 

    return (piece-> player == player)? value : -value ;
}

/*returns a list containing all valid for the given player*/

 MLIST* GetAllValidMoves(BOARD* board,SIDE side)
{
    MLIST* list = CreateMoveList();

    PIECE* piece;
        for(int x= 0; x<BOARDWIDTH; x++)
	{
	    for(int y= 0; y<BOARDHEIGHT; y++)
	    {
	        piece = GetPiece(board,x,y);
	  	if (piece != NULL && piece->player == side)
		{
		    for(int i= 0; i<BOARDWIDTH; i++)
		    {
			for(int j= 0; j<BOARDHEIGHT; j++)
			{		
			    MOVE* move = CreateMove(x,y,i,j);
		 	    if (IsValidMove(board, move, side) == true)
		   	    {
			        AppendMove(list, move);
			    }
		 	    else
		  	    {
		 	        DeleteMove(move);
			    }						
			}
	  	    }
		}
	    }
	}
	return list;
		
}
