#pragma once

#include "Musicians_header.h"
#define FALSE_ID -1
#define NOT_DEFINED -1

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
<<<<<<< HEAD
	Date date_of_concert; // The date of the concert
	char* name;           // The name of the concert
	CIList instrument;    // A list of the ConcertInstrument
=======
	Date date_of_concert; 
	char* name; 
	CIList instruments; 
>>>>>>> 06ea239e8c42995fba44915e065c03f62d1e7781

} Concert;



