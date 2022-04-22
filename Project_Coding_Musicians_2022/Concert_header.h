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
	Date date_of_concert; // The date of the concert
	char* name;           // The name of the concert
	CIList instrument;    // A list of the ConcertInstrument

} Concert;

void manageConcert(Musician*** players, InstrumentTree inst, int* sizes);

void newConcert(Concert event, InstrumentTree tools);

float convertHour(char* hours, char* minutes);

void makeEmptyList(CIList* new);

CIListNode* insertDataToEndList(CIList* lst, int id, char* sum, char importance);

CIListNode* createNewListNode(int type, int count, char significance, CIListNode* next);

void insertNodeToEndList(CIList* lst, CIListNode* new);

int findId(char* name, InstrumentTree devices);

void findIdRec(TreeNode* root, char* type, int* ID);

void reorderCollection(Concert aEvent, Musician*** artists, int* sizes);

void reorderMusicians(Musician** players, int direction, int size, int id);

void createAidArray(MusiciansPrices* aidArr, Musician** performers, int size, int instId);

int findPrice(int wanted, MPIListNode* head);

void mergeMusicians(MusiciansPrices* aidArr, int size, int direct);

void mergeM(MusiciansPrices* a1, int n1, MusiciansPrices* a2, int n2, MusiciansPrices* res, int importance);

void copyMArr(MusiciansPrices* dest, MusiciansPrices* src, int size);

void insertMusicians(Musician** performers, MusiciansPrices* aidArr, int size);

void setUpConcert(Concert show, Musician*** artists, int* sizes);

bool addMusician(Musician** options, int size, Musician* busy, int* lSize, int* pSize);

void printConcert(Concert theEvent);
