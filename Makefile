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
all:
	cd src; make

################# Generate the executable #################

# Target to run the Chess program
test:
	cd src; make test

testpvp:
	cd src; make testpvp

testcvc:
	cd src; make testcvc

####################### Others ############################

# Target to tar the source code package
tar:   
	make 
	gtar cvzf Chess_V1.0_src.tar.gz src doc bin Makefile README COPYRIGHT INSTALL --exclude='CVS'	

# Target for clean-up
clean:
	cd src; make clean

# Target to create a backup folder
backup:
	cd src; make backup
