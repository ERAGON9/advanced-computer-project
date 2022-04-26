#pragma once

#include "Musicians_header.h"
#define FALSE_ID -1
#define NOT_DEFINED -1
#define HOUR 60
#define EMPTY_ROW '\0'
#define END_OF_LINE '\n'

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

char* readLineFromTheUser();

void newConcert(Concert theEvent, InstrumentTree tools, char* description);

float convertHour(char* hours, char* minutes);

void makeEmptyList(CIList* new);

void insertDataToEndList(CIList* lst, int id, char sum, char importance);

CIListNode* createNewListNode(int type, int count, char significance, CIListNode* next);

void insertNodeToEndList(CIList* lst, CIListNode* new);

void reorderCollection(Concert aEvent, Musician*** artists, int* sizes);

void reorderMusicians(Musician** players, int direction, int size, int id);

void createAidArray(MusiciansDetails* aidArr, Musician** performers, int size, int instId);

int findPrice(int wanted, MPIListNode* head);

void mergeMusicians(MusiciansDetails* aidArr, int size, int direct);

void mergeM(MusiciansDetails* a1, int n1, MusiciansDetails* a2, int n2, MusiciansDetails* res, int importance);

void copyMArr(MusiciansDetails* dest, MusiciansDetails* src, int size);

void insertMusicians(Musician** performers, MusiciansDetails* aidArr, int size);

void setUpConcert(Concert show, Musician*** artists, int* sizes, TreeNode* root);

bool addMusician(Musician** options, int size, Musician* busy, int* lSize, int* pSize);

void printConcert(Concert theEvent, Musician* performers, int size, TreeNode* root);

char* findInstrumentName(TreeNode* trNode, int id);

int findAskedPrice(Musician artist, int id);

void freeConcerts(Concert* allConcerts, int size);

void freeAll(InstrumentTree instruments, Musician** MusiciansGroup, int musiciansCount, Musician*** MusiciansCollection, int instCount);

void freeTreeRec(TreeNode* root);