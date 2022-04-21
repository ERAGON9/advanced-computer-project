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


Musician** createMusicianArr(FILE* text, InstrumentTree instTree, int* logicSize);

int fillMusicianGroup(Musician*** MusicianGroup, int physicSize, char** musiciansFile, InstrumentTree instTree);

void updateMusicianGroupArray(Musician*** tempMusicianGroup, int logSize, int* physicSize);

Musician* initializeMusician(int* namePhysicSize, int* nameLogicSize);

void updateNameArray(Musician* musician, int nameLogicSize, int* namePhysicSize);

void addMusicianNameToArray(Musician* musician, char* name, int* nameLogicSize);

MPIListNode* initializeMPINode(TreeNode* instTreeRoot, char* token, char* seps);

bool isInstrument(InstrumentTree instTree, char* string);

Musician*** constructMCollection(int iSize, Musician** MusicianGroup, int mSize);

void checkMusician(Musician* player, Musician** arr, int id, int* lSize, int* pSize);
