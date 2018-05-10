/*********************************************************************/
/* Project 1, for EECS 22L, Winter 2018	                             */
/*                                                                   */
/* Authors: Team 4                                                   */
/* Date: 1/12/2018                                                   */
/*                                                                   */
/* IO.c: source file for input/output			             */
/*								     */
/*	1/12/2018 Medhi Lakhoua Initial version 		     */
/*	1/23/2018 Yuki Hayashi  Updated				     */		
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include "IO.h"
#include "Constants.h"
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/* Print the command-line arguments usage of the program*/
void PrintUsage()
{
    printf("\nUsage: ./Chess -gamemode <gamemode> [options]\n"
	   "Options:\n"
	   "-gamemode <gamemode>           The player picks the gamemode.\n"
	   "-side <side>                   The player picks his side.\n"
	   "-difficulty <difficulty>       The player picks the AI difficulty.\n"
	   "-h                             Display this usage information.\n"
	   "Game modes: pvp, pvc, cvc.\n"
 	   "Sides: black, white.\n"
	   "Difficulties: beginner, intermediate, expert.\n\n");
}


/* Print the enter move menu */
void PrintMenu()
{
    printf("1. Enter a move\n"
	   "2. Show the board\n"
  	   "3. Get a hint\n"
	   "4. Show valid moves\n"
	   "5. Forfeit the game\n");
}


//Print a piece from 
void PrintPiece(PIECE* piece)
{
	if(piece == NULL)
	{
		printf("  ");
		return;	
	}

	//print w (white) or b (black) based on the side
	switch(piece -> player)
	{
	    case Black:
		    printf("b");
		    break;
	    case White:	
		    printf("w");
		    break;
	    default: 
		    break;
	}

	//print K,R,Q,N,B or P based on the type
	switch(piece -> type)
	{
	    case Pawn: 
		    printf("P"); 
		    break;
	    case Rook: 
		    printf("R");
		    break;
	    case Bishop: 
		    printf("B");
		    break;
	    case Knight:
		    printf("N");
		    break;
	    case King: 
		    printf("K");
		    break;
	    case Queen:
		    printf("Q");
		    break;
	    default: 
		    break;
	} 
}


/*print the current board*/ 
void PrintBoard(BOARD* board)
{
	//return if there is no board
	if(board == NULL) 
	{
	    printf("Board is null!\n");
	    return;
	}	

	printf(" +----+----+----+----+----+----+----+----+\n"); //line
 	
	for(int r=0; r<BOARDHEIGHT; r++){
	    printf("%d", BOARDHEIGHT- r);
	    
	    for(int c=0; c<BOARDWIDTH; c++)
	    {
		printf("|");
		printf(" ");
		PrintPiece(GetPiece(board, c, r)); //print the piece
		printf(" ");
		
	    }
		printf("| \n");
	    
	    printf(" +----+----+----+----+----+----+----+----+\n");
	}
	
	printf("   a    b    c    d    e    f    g    h   \n");

	printf("\n");

	printf("Black King: ");
	PrintCoordinates( board->blackKingX, board->blackKingY);
	printf(".\n");
	
	printf("White King: ");
	PrintCoordinates( board->whiteKingX, board-> whiteKingY);
	printf(".\n");

	printf("En Passant: ");
	PrintCoordinates( board->enPassantX, board->enPassantY);
	printf("              Fifty moves: %d", board -> fiftyMoves);
	printf(".\n");
/*
	printf("           Castling permissions:\n");
	printf("QueenSide:");
	printf( (board -> wCastleQueen) ? "true" : "false");
	printf("     White     KingSide: ");
	printf( (board -> wCastleKing)? "true\n" : "false\n");

	printf("QueenSide:");
	printf( (board -> bCastleQueen) ? "true" : "false");
	printf("     Black     KingSide: ");
	printf( (board -> bCastleKing)? "true\n" : "false\n");
*/
}

/*prints the player to the screen (Black or White)*/
void PrintPlayer(SIDE side)
{
    switch(side)
    {
        case White: printf("White");
		    break;

        case Black: printf("Black");
		    break;
	default:
		    break;
    }

}

/*print move's from and to coordinates*/
void PrintMove(const MOVE* move)
{    
	if(move == NULL) 
	{ 
 	    printf("Null");
  	    return;
	}

	PrintCoordinates(move -> fromX, move-> fromY);
  	printf( "  to  " );
	PrintCoordinates(move -> toX, move -> toY);
}

/*prints the array corrdinates in chess notation (0,0 is a8)*/
void PrintCoordinates(int x, int y)
{
	if(x>=0 && x<BOARDWIDTH && y>=0 && y<BOARDHEIGHT)
	{
	    printf( "%c", x + 97 );
 	    printf( "%d", 8 - y  );
	}
	else
	{
	    printf( "xx" );
	}
}

/*prints every move in the move list */
void PrintMoveList(const MLIST* list)
{
        printf("Number of moves = %d.\n", list -> Length);
	
	if (list == NULL)
	{
		printf("Null");
		return;
	}
	
	MENTRY *curr = list -> First;
        int currentIndex = 0;
	
	while (curr != NULL)
	{
		currentIndex ++;
		PrintMove(curr->Move);
		
		printf("    ");
		if(currentIndex %5 == 0)
		{
	 		printf("\n");
		}
		curr= curr -> Next;
	}
	
	if(currentIndex % 5 != 0)
	{
		printf("\n");
	}

}

/* Request the promotion type from a human player */
PIECETYPE RequestPromotion()
{
	char promotion = 'p';
	printf("Q for queen, B for bishop, R for rook, N for knight.\n");
	printf("please enter the promotion type: ");

	scanf("%c", &promotion);
	while(getchar() != '\n');

	while(true)
	{
	    switch (promotion)
	    {
	        case 'q':
	        case 'Q': 
	  	         return Queen;
	                 break;
	        case 'b':
	        case 'B':
	 	         return Bishop;
	   	         break;
	        case 'r':
	        case 'R':
		         return Rook;
		         break;
	        case 'n':
	        case 'N': 
		         return Knight;
		         break;
	        default:
		         printf("Invalid promotion type. Please try again.\n");
			 printf("Q for queen, B for bishop, R for rook, N for knight.\n");
	   		 printf("please enter the promotion type: ");
		 	 scanf("%c", &promotion);
		  	 while(getchar() != '\n');
	    }
       }

}

/* Request a move from a human player */
MOVE *RequestMove() 
{	
	// Creates two temporary variables to store the user inputs.
	char tempFrom [3];
	char tempTo[3] ;
	
	// Prompts to enter move
	printf("Please enter starting position: ");
	scanf("%2s", tempFrom);
	while(getchar() != '\n');
 		
	if( tempFrom[0] < 'a' || tempFrom[0] > 'h' || tempFrom[1] < '1' || tempFrom[1] > '8' )
	{
	    //from invalid. abort.
	    return NULL;
	}	   
		
	printf("Please enter ending position: ");	
	scanf("%2s", tempTo);	
	while(getchar() != '\n');
 	
	if( tempFrom[0] < 'a' || tempFrom[0] > 'h' || tempFrom[1] < '1' || tempFrom[1] > '8' )
	{
	    //to invalid. abort.
	    return NULL;
	}
		    
	MOVE* tempMove = CreateMove(-1,-1,-1,-1);
	ParseMove(tempMove, tempFrom, tempTo);
		   
	return tempMove;			
}


void ParseMove(MOVE *move, char* from, char* to)
{
						
	// Breaks the string down into individual characters
	char charFromX;
	int  intFromY;	
	char charToX;
	int  intToY;
	
	// if move is null
	if( move == NULL )
		return;
			
	
	// Checks the given input string and places them into temporary variables. Checks it is valid and then sets it in the move if ok
	if (sscanf(from, "%c%d" , &charFromX, &intFromY) == 2 )
	{
		move -> fromX = charFromX - 97;
		move -> fromY = 8 - intFromY;	
	}
	
	if (sscanf(to, "%c%d" , &charToX, &intToY) == 2 )
	{
		move -> toX = charToX - 97;
		move -> toY = 8 - intToY;
		
	}

	
	
/*	//Debug
	printf("transformed %c%d to %c%d into %d,%d to %d,%d\n", 
		charFromX, intFromY, charToX, intToY, 
		move->fromX, move -> fromY, move -> toX, move->toY);
*/
		
}


void  StartLog()
{
	
	// Get the raw time from system.
  time_t rawtime;
  
  // Returns pointer to a tm struct with time information
  struct tm * timeinfo;
  time (&rawtime);
  
  timeinfo = localtime ( &rawtime );
  
  char day[10], month[10], year[10], hour[10], min[10], sec[10];
  
  sprintf(day,"%d",   timeinfo -> tm_mday);
  sprintf(month,"%d", timeinfo -> tm_mon + 1);
  sprintf(year,"%d",  timeinfo -> tm_year + 1900);
  sprintf(hour,"%d",  timeinfo -> tm_hour);
  sprintf(min,"%d",   timeinfo -> tm_min);
  sprintf(sec,"%d",   timeinfo -> tm_sec);
  char name[100];

  strcpy(name,"QuickChess-");
  strcat(name, month);
  strcat(name, "-");
  strcat(name, day);
  strcat(name, "-");
  strcat(name, year);
  strcat(name, "--");
  strcat(name, hour);
  strcat(name, ":");
  strcat(name, min);
  strcat(name, ":");
  strcat(name, sec);
  strcat(name, ".txt");
  
  
  FILE *file = NULL;
  file = fopen (name, "wb");
  
  if (!file)
  {
    printf("Failed to create file!\n");
  }
  
  fclose(file);
  
  strcpy(fileName, name);

}



void LogMove(MOVE *move, SIDE currentPlayer)
{

  // Character Array for buffer
  char buffer[240];

  // For current player
  char player[6];
  
  PIECETYPE PT;
 
  if (currentPlayer == White)
  {
	  strcpy(player, "White");
  }
  
  else if (currentPlayer == Black)
  {
	  strcpy(player, "Black");
  }
  
  // Converts numerical values back to ASCII
  char fromx = (move -> fromX) + 97;
  char fromy = 8 - (move -> fromY);
  char tox = (move -> toX) + 97;
  char toy = 8 - (move -> toY);
 
	PT = move -> capturedPiece;
  
  switch (PT)
	  {
		case Pawn:
		{
			sprintf(buffer,"%s: Moved from %c%d -> %c%d | Captured a pawn", player, fromx, fromy, tox, toy);
			break;
		}
		
		case Knight:
		{
			sprintf(buffer,"%s: Moved from %c%d -> %c%d | Captured a Knight", player, fromx, fromy, tox, toy);
			break;
		}
		
		case Rook:
		{
			sprintf(buffer,"%s: Moved from %c%d -> %c%d | Captured a Rook", player, fromx, fromy, tox, toy);
			break;
		}
		
		case Bishop:
		{
			sprintf(buffer,"%s: Moved from %c%d -> %c%d | Captured a Bishop", player, fromx, fromy, tox, toy);
			break;
		}
		
		case Queen:
		{
			sprintf(buffer,"%s: Moved from %c%d -> %c%d | Captured a queen", player, fromx, fromy, tox, toy);
			break;
		}
		
		default:
		{
			sprintf(buffer,"%s: Moved from %c%d -> %c%d", player, fromx, fromy, tox, toy);
			break;
		}
	  
	  
  }

    if (move -> promotion == true)
  {
	sprintf(buffer + strlen(buffer)," | Promoted a pawn.\n");
  }
  
  else if (move -> enPassant == true)
  {
	sprintf(buffer + strlen(buffer)," | EnPassant performed\n");

  }
  
  else if (move -> castling == true)
  {
	sprintf(buffer + strlen(buffer)," | Castling performed\n");

  }
  
  sprintf(buffer + strlen(buffer), "\n");
  

  
	// Opens file for append and editing.
	FILE* file;	
	file = fopen (fileName, "a");
  
  if(file == NULL) 
  
  {
    perror("Error opening file.");
  }
  else 
  {
	  // Prints the string buffer to the file. 
        fprintf(file, "%s", buffer);
  }
	

	fclose(file);

}


