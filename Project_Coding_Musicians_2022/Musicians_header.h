#pragma once

#include "Instrument_header.h"

#define INITIAL 2

typedef struct
{
	unsigned short insId; 
	float price; 

} MusicianPriceInstrument;

typedef struct mpilistnode // -+
{
	MusicianPriceInstrument data;
	struct mpilistnode* next;

}MPIListNode;

typedef struct mpilist // -+
{
	MPIListNode* head;
	MPIListNode* tail;

}MPIList;

typedef struct
{
	char** name; 
	MPIList instruments; 

} Musician;

Musician*** constructMCollection(int iSize, Musician** group, int mSize);
void checkMusician(Musician* player, Musician** arr, int id, int* lSize, int* pSize);
