#pragma once

#include "Musicians_header.h"

typedef struct
{
	int day, month, year;
	float hour;

} Date;

typedef struct
{
	int num;          // The amount that needed from this instrument
	int inst;         // The instrument uniq id
	char importance;  // If the instrument important (if important, so there is a badget for expencive musicians) (1 - important, 0 - not important)

} ConcertInstrument;

typedef struct clilistnode
{
	ConcertInstrument data;
	struct clilistnode* next;

}CIListNode;

typedef struct cilist
{
	CIListNode* head;
	CIListNode* tail;

}CIList;

typedef struct
{
	Date date_of_concert; // The date of the concert
	char* name;           // The name of the concert
	CIList instrument;    // A list of the ConcertInstrument

} Concert;



