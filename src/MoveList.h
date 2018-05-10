#ifndef MOVEENTRY_H
#define MOVEENTRY_H

#include "Move.h"

typedef struct MoveEntry MENTRY;
typedef struct MoveList MLIST;

struct  MoveEntry
{
	MLIST* List;
	MENTRY* Next;
	MENTRY* Prev;
	MOVE* Move;
};

struct MoveList
{
	MENTRY* First;
	MENTRY* Last;
	int Length;
};

MENTRY* CreateMoveEntry(MOVE* move);

void DeleteMoveEntry(MENTRY* entry);

MLIST *CreateMoveList();

void DeleteMoveList(MLIST* list);

void AppendMove(MLIST *list, MOVE *move);

#endif
