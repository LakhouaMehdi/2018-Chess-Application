#ifndef CONSTANTS_H
#define CONSTANTS_H

#define BOARDWIDTH 8
#define BOARDHEIGHT 8 
#define INFINITY 1000000
typedef enum PieceType{
	Pawn,
	Knight,
	Rook,
	Bishop,
	Queen,
	King,
	None
}PIECETYPE, PT;

typedef enum Side{
	Black,
	White
}SIDE, BW;

typedef enum Control{
	Human,
	Computer
}CONTROL;

typedef enum Difficulty{
	Beginner,
	Intermediate,
	Expert
}DIFFICULTY;

typedef enum GameMode{
	pvp,
	pvc,
	cvc
}GAMEMODE;

//Game Settings
 
static const PT piecePattern[] = {Rook, Knight, Bishop};
static const int patternWidth = sizeof(piecePattern)/sizeof(PT) ;

static const int jumpAttackX[] = {1,2,2,1,-1,-2,-2,-1};
static const int jumpAttackY[] = {-2,-1,1,2,2,1,-1,-2};

static const int diagAttackX[] = {-1,1,1,-1};
static const int diagAttackY[] = {-1,-1,1,1};

static const int lineAttackX[] = {0,1,0,-1};
static const int lineAttackY[] = {-1,0,1,0};

#endif
