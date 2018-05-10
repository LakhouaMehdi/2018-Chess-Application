/**********************************************************************/
/* Project 1, for EECS 22L, Winter 2018	                              */
/*                                                                    */
/* Authors: Team 4                                                    */
/* Date: 1/12/2018                                                    */
/*                                                                    */
/* Main.c: source file for the main function                          */
/*                                                                    */
/**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IO.h"
#include "Board.h"
#include "Constants.h"
#include "Move.h"
#include "AI.h"
//#include "MoveList.h"

CONTROL GetControl(SIDE currentPlayer);
int ReadCommandLine(int argc, char* argv[]);
void Testing ();

//Game settings
static GAMEMODE specifiedGameMode = pvc;
static SIDE specifiedSide = White;
static DIFFICULTY specifiedDifficulty = Expert;
static CONTROL whiteControl = Human;
static CONTROL blackControl = Computer;

/* main */
int main (int argc, char* argv[]) {

      
    //READ COMMAND LINE
    if (ReadCommandLine(argc, argv) != 0)
    {
	PrintUsage();
	printf("\nAborting due to invalid command line.\n");
	return -1;
    }

    //DETERMINE CONTROL
    switch ( specifiedGameMode ) 
    {
	case pvp:
	        whiteControl = Human;
		blackControl = Human;
		break;
	case pvc:
	        switch (specifiedSide)
		{
		    case Black:
			      blackControl = Human; 
			      whiteControl = Computer;
			      break;
		    case White: 
			      whiteControl = Human; 
			      blackControl = Computer;
			      break;
		}
		break;
	case cvc:
	        whiteControl = Computer;
		blackControl = Computer;
		break;

    }

  //GAME IS READY TO BE PLAYED
    int currentTurn = 0;
    SIDE currentPlayer = White;
    BOARD* gameBoard = CreateBoard();

    int searchDepth;
    switch ( specifiedDifficulty )
    {
	case Beginner:
		searchDepth = 0;
		break;
	case Intermediate:
		searchDepth = 1;
		break;
	case Expert:
		searchDepth = 3;
		break;
    }

    /*game logic*/

    bool gameOver = false;
	
	// Start log
   StartLog();
   PrintBoard(gameBoard); 
  
   while(!gameOver)
   {
    //Start of the turn

    currentTurn ++;
  
    CONTROL cControl = GetControl(currentPlayer);
    
        
    //Print the current turn and the current player
    
    printf("============================================\n\n");
    printf("Turn %d. ", currentTurn);
    PrintPlayer(currentPlayer);    
    printf(".\n");

    MOVE* move = NULL;  //The move for this turn

    if(cControl == Computer)
    {   

        if(IsInCheck(gameBoard, currentPlayer) )
        {
	    PrintPlayer(currentPlayer);
            printf("'s king is in check.\n");
        }

	//Computer finds the move	
        move = GetBestMove(gameBoard,currentPlayer,searchDepth);
	printf("Computer has generated the move ");
	PrintMove(move);
	printf(".\n");
    }
    else
    {
	//Human enters the move    	

	while(move == NULL)
	{ 
	    //let the player know if his king is in check
	    if(IsInCheck(gameBoard, currentPlayer) )
	    {
	        printf("Your king is in check.\n");
	    }

	    //print the menu and get user selection
	    
	    int selection = -1;

            PrintMenu();
            scanf("%d", &selection);
            while(getchar() != '\n');
	    
	    while( selection <1 || selection >5 )
 	    {   
	        printf("Your selection is invalid. Please try again.\n");

                PrintMenu();
                scanf("%d", &selection);
	        while(getchar() != '\n');
 	    }
	    
	    //got the selection
	    switch(selection) 
	    {
	        case 1:
		      move = RequestMove();
 
		      while(!IsValidMove(gameBoard,move,currentPlayer))
        	      {	
		          printf("Your move is invalid. Please try again.\n"); 
		          DeleteMove(move);
		          move = RequestMove();
		      }
		
		      break;
		case 2:
		      PrintBoard(gameBoard);
		      printf("\n");
		      break;
	        case 3:
	  	      printf("I recommend ");
		      MOVE* hintMove = GetBestMove(gameBoard, currentPlayer, searchDepth);
		      PrintMove(hintMove);
		      printf(".\n");
		      DeleteMove(hintMove);
		      break;
		case 4:
		      printf("Valid moves: \n");
		      MLIST* list = GetAllValidMoves(gameBoard,currentPlayer);
		      PrintMoveList(list);
		      DeleteMoveList(list);
		      break;		      
	        case 5:
		      PrintPlayer(currentPlayer);
		      printf(" has forfeited the game.\n");
		      PrintPlayer(GetNextPlayer(currentPlayer));
		      printf(" has won!\n");
		      DeleteBoard(gameBoard);
		      return 0;	    
  	    }
	}
	//Got the move
    
    }//esle

    printf("Performing the move ");
    PrintMove(move);
    printf(".\n");
    
    PerformMove(gameBoard, move);

    gameBoard -> fiftyMoves ++;
    
    if(move != NULL)
    {
        PIECE* movedPiece = GetPiece(gameBoard, move -> toX, move -> toY);

        if(move -> capturedPiece != None || (movedPiece != NULL && movedPiece -> type == Pawn) )
        {
	    //reset the 50 moves counter
	    gameBoard -> fiftyMoves = 0;
        }
    }

    //verify if move is a promotion and promote
    if(move != NULL && move -> promotion == true)
    {
	if(cControl == Human)
	{
	    SetType( GetPiece(gameBoard, move-> toX, move -> toY), RequestPromotion() );
	}
	else
	{
	    SetType( GetPiece(gameBoard, move-> toX, move -> toY), Queen );
	}
    }
 
     //Print the board
    PrintBoard(gameBoard);

    LogMove(move, currentPlayer);
    
    DeleteMove(move); 	//move has been performed

    currentPlayer = GetNextPlayer(currentPlayer);    
        
    if(IsCheckmate(gameBoard, currentPlayer) == true)
    {
	PrintPlayer(currentPlayer);
        printf(" is checkmate.\n");
        PrintPlayer(GetNextPlayer(currentPlayer));
        printf(" has won!\n");
	
        PrintBoard(gameBoard);
   	gameOver = true;
    }
   
    //Verify stalemate
    MLIST* list = GetAllValidMoves(gameBoard, currentPlayer);
    if( list -> First == NULL && !IsInCheck(gameBoard,currentPlayer) && !gameOver )
    {
	PrintPlayer(currentPlayer);
	printf(" has no legal moves.\n");
        printf("Draw game!\n");
        PrintBoard(gameBoard);
	gameOver = true;
    }
    DeleteMoveList(list);
   
    //Verify fifty moves rule
    if(gameBoard -> fiftyMoves > 50)
    {
	printf("Fifty moves with no captures or pawn advancement\n");
	printf("Draw game!\n");
	gameOver = true;
    }
 
/*    if(specifiedGameMode == cvc && !gameOver)
	{   
	    char cont ='y';
	    do{
	    printf("Would you like to continue watching? (y/n): ");
	    scanf("%c", &cont);
	    while(getchar() != '\n');

	    if(cont == 'n')
	    {
		gameOver = true;
	    }
	   
	    if(cont != 'y' && cont != 'n')
	    {
		printf("Invalid input!\n");
	    }
	    }while(cont != 'y' && cont != 'n');
	    
	}
*/

   }
	
    DeleteBoard(gameBoard);
    return 0;

}	


void Testing () {

  // Testing
  //Testing board creation, printing, and deletion
/*    BOARD* board = CreateBoard();    
    PrintBoard(board);
    DeleteBoard(board);
*/
/*
  //Testing Piece creation, printing, and deletion
    PIECE* piece = CreatePiece(Pawn, Black);
    PrintPiece(piece);	//should print bP
    printf("   ");
    SetType(piece,Queen);
    PrintPiece(piece);	//should print bQ
    printf("\n");
    DeletePiece(piece);
*/
/*
  //Testing Move creation, printing and deletion
    MOVE* move1 = CreateMove(0,0,2,2);
    PrintMove(move1);
    DeleteMove(move1);


  //Test parse move
    MOVE* move2 = RequestMove();
    
    PrintMove(move2);
    DeleteMove(move2);
  */ 

/*    MLIST* list = CreateMoveList();

    AppendMove(list, move1);
    AppendMove(list, move2);

    DeleteMoveList(list);
*/

}


//reads the command line and sets the global settings of the game.
//returns non zeron if invalid
int ReadCommandLine(int argc, char* argv[])
{
    int x = 1;

    while(x < argc)
    {
	if (strcmp(argv[x], "-gamemode") == 0) 
        {
	    if(x < argc -1)
	    {
		if( strcmp(argv[x+1], "pvp") == 0)
		{
		    specifiedGameMode = pvp;
		}
		else if( strcmp(argv[x+1], "pvc") == 0)
		{
		     specifiedGameMode = pvc;       
		}
 		else if( strcmp(argv[x+1], "cvc") == 0)
		{
		     specifiedGameMode = cvc;
		}
		else 
		{
		    printf("Invalid argument for -gamemode!\n");
		    return 5;
		}
		x+=2;
		continue;
	    }		
	    else
	    {
		printf("Missing argument for -gamemode!\n");
		return 6;
	    }
			
        }//fi

        
	else if (strcmp(argv[x], "-difficulty") == 0)
	{
	    if(x < argc -1)
	    {
	        if(strcmp(argv[x+1], "beginner") ==0)
		{
		   specifiedDifficulty = Beginner;
		}
		else if(strcmp(argv[x+1], "intermediate") ==0)
		{
 		    specifiedDifficulty = Intermediate;
		}
		else if(strcmp(argv[x+1], "expert") ==0)
		{
		    specifiedDifficulty = Expert;
		}
		else
		{
  		    printf("Invalid argument for -difficulty!\n");
		    return 7;
		    
		}
	        x+=2;
		continue;
	    }
	    else
	    {
		printf("Missing argument for -difficulty!\n");
		return 8;
	    }

	}//fi
	
	else if (strcmp(argv[x], "-side") == 0)
	{
	    if(x < argc - 1)
	    {
	        if(strcmp(argv[x+1], "black") == 0)
		{
		    specifiedSide = Black;
		}
		else if(strcmp(argv[x+1], "white") == 0)
		{
		    specifiedSide = White;
		}
		else
		{
		    printf("Invalid argument for -side!\n");
		    return 9;
		}
		x+=2;
		continue;
	    }
	    else
	    {
		printf("Missing argument for -side!\n");
		return 10;
	    }
	}//fi
	
	else
	{
		return 11;
	}

	x++;
    }//elihw

    return 0;
}


CONTROL GetControl(SIDE player)
{
    switch(player)
    {
   	case White: 
	    return whiteControl; 
	case Black:
	    return blackControl;
	default:
	    return Human;   

    }

}

