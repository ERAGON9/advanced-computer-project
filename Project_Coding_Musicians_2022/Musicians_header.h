#pragma once

#include "Instrument_header.h"


typedef struct
{
	unsigned short insId; // The instrument id
	float price;          // The price for play this instrument

} MusicianPriceInstrument;

typedef struct mpilistnode
{
	MusicianPriceInstrument data;
	struct mpilistnode* next;

}MPIListNode;

typedef struct mpilist
{
	MPIListNode* head;
	MPIListNode* tail;

}MPIList;

typedef struct
{
	char** name;         // Array of names (array of strings)
	MPIList instruments; // List of instruments (List of MusicianPriceInstrument)

} Musician;


typedef struct
{
	Musician* pointer;
	float askedPrice;

} MusiciansPrices;


Musician** createMusicianArr(FILE* text, InstrumentTree instTree, int* logicSize);

int fillMusicianGroup(Musician*** MusicianGroup, int physicSize, FILE* musiciansFile, InstrumentTree instTree);

void updateMusicianGroupPhysicSizeArray(Musician*** tempMusicianGroup, int logSize, int* physicSize);

Musician* initializeMusician(int* namePhysicSize, int* nameLogicSize);

void updateNamePhysicSizeArray(Musician* musician, int nameLogicSize, int* namePhysicSize);

void addMusicianNameToArray(Musician* musician, char* name, int* nameLogicSize);

bool isInstrument(InstrumentTree instTree, char* string);

MPIListNode* initializeMPINode(TreeNode* instTreeRoot, char* token, char* seps);

clearString(char* string);

Musician*** constructMCollection(int iSize, Musician** MusicianGroup, int mSize, int** sizes);

void checkMusician(Musician* player, Musician** arr, int id, int* lSize, int* pSize);

void checkMusician(Musician* player, Musician** arr, int id, int* lSize, int* pSize);

