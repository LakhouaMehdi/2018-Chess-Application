#
##################################################################
# Project 1, for EECS 22L, Winter 2018	                          #
#                                                                 #
# Authors: Team 4                                                 #
# Date: 1/12/2018                                                 #
#                                                                 #
# Makefile: Makefile for Project 1                                #
#                                                                 #
###################################################################

# Variables
FLAGS = -ansi -std=c99 -Wall

# Default target
all: Chess

################# Generate object files  ################## 

# Target for IO.o
IO.o: IO.c IO.h Move.h Constants.h Piece.h Board.h MoveList.h
	gcc -c $(FLAGS) IO.c -o ../bin/IO.o

#Target for Main.o
Main.o: Main.c IO.h AI.h Board.h Piece.h Constants.h Move.h MoveList.h
	gcc -c $(FLAGS) Main.c -o ../bin/Main.o

#Target for Move.o
Move.o: Move.c Move.h Piece.h Board.h Constants.h
	gcc -c $(FLAGS) Move.c -o ../bin/Move.o

#Target for Board.o
Board.o: Board.c Board.h Constants.h AI.h MoveList.h Move.h
	gcc -c $(FLAGS) Board.c -o ../bin/Board.o

#Target for Piece.o
Piece.o: Piece.c Piece.h Constants.h
	gcc -c $(FLAGS) Piece.c -o ../bin/Piece.o

#Target for MoveList.o
MoveList.o: MoveList.c MoveList.h Constants.h Move.h
	gcc -c $(FLAGS) MoveList.c -o ../bin/MoveList.o

#Target for AI.o
AI.o: AI.c AI.h MoveList.h Board.h Constants.h Move.h
	gcc -c $(FLAGS) AI.c -o ../bin/AI.o

################# Generate the executable #################

# Target for Chess
Chess: Main.o IO.o Piece.o Board.o Move.o MoveList.o AI.o
	cd ../bin; gcc $(FLAGS) Main.o IO.o Piece.o Board.o Move.o MoveList.o AI.o -o Chess

# Target to run the Chess program
test:
	cd ../bin; ./Chess

testpvp:
	cd ../bin; ./Chess -gamemode pvp

testcvc:
	cd ../bin; ./Chess -gamemode cvc

####################### Others ############################


# Target for clean-up
clean:
	cd ../bin; rm -f *.o; rm -f *.txt;  rm -f Chess

# Target to create a backup folder
backup:
	rm -rf ~/chessBackup
	cp -r ~/chess ~/chessBackup
