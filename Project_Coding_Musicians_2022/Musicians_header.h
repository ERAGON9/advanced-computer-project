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

//This function create an array of arrays, each of them contain a pointer to a musician, while 
//the number of arrays equals the number of available instruments (from the instrumenTree).
//Each array's index in the bigger final array equals a instrument's id, and includes the pointers 
//To all the musicians which can use that instrument.
Musician*** constructMCollection(int iSize, Musician* group, int mSize);

//This function check if a given instrument's id ('id') matches one of the given musician's 
//instruments' ids, if it does a pointer to that musician will be added to the given array of
//pointers to musicians ('arr').
void checkMusician(Musician player, Musician** arr, int id, int* lSize, int* pSize);


