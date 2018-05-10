/*********************************************************************/
/* Project 1, for EECS 22L, Winter 2018	                             */
/*                                                                   */
/* Authors: Yuki Hayashi                                             */
/* Date: 1/23/2018                                                   */
/*                                                                   */
/* MoveList.c 														*/
/*	1/23/2018 Yuki Hayashi Initial Version							*/
/*                                                                   */
/*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "MoveList.h"

MENTRY *CreateMoveEntry(MOVE *move)
{
	MENTRY *NewMove;
	NewMove = malloc(sizeof(MENTRY));
	NewMove -> List = NULL;
	NewMove -> Next = NULL;
	NewMove -> Prev = NULL;
	NewMove -> Move = move;
	
	return NewMove;
}

	
void DeleteMoveEntry(MENTRY* entry)
{
	if (entry == NULL)
	{
		return;
	}
	
	entry -> List = NULL;
	entry -> Next = NULL;
	entry -> Prev = NULL;
	if (entry -> Move != NULL)
	{
		DeleteMove(entry->Move);
	}
	
	free(entry);
	entry = NULL;
}

MLIST* CreateMoveList()
{
	MLIST *MoveList;
	MoveList = malloc(sizeof(MLIST));
	assert(MoveList);
	MoveList -> Length = 0;
	MoveList -> First = NULL;
	MoveList -> Last = NULL;
	
	return MoveList;
}

void DeleteMoveList(MLIST* list)
{
	if (list == NULL)
	{
		return;
	}
	
	MENTRY *curr;
	MENTRY *next;
	
	curr = list -> First;
	
	list -> First = NULL;
	list -> Last = NULL;
	list -> Length = 0;
	
	while(curr!=NULL)
	{
		next = curr -> Next;
		DeleteMoveEntry(curr);
		curr = next;
	}
	
	free(list);
	list = NULL;
}

void AppendMove(MLIST* list, MOVE* move)
{
	assert(list);
	MENTRY* curr = CreateMoveEntry(move);
	
	assert(curr);
	
	if(list -> Last != NULL)
	{
		curr -> List = list;
		curr -> Next = NULL;
		curr -> Prev = list -> Last;
		list->Last->Next = curr;
		list->Last = curr;
	}
	else
	{
		curr->List = list;
		curr->Next = NULL;
		curr->Prev = NULL;
		list->First = curr;
		list->Last = curr;
	}
	list -> Length++;
}

