#pragma once

#include "Instrument_header.h"



typedef struct
{
	unsigned short insId; // the instrument id
	float price;          // the price for play this instrument

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
	char** name; // array of names (array of strings)
	MPIList instruments; // List of instruments (List of MusicianPriceInstrument)

} Musician;

Musician*** constructMCollection(int iSize, Musician** group, int mSize);
void checkMusician(Musician* player, Musician** arr, int id, int* lSize, int* pSize);
