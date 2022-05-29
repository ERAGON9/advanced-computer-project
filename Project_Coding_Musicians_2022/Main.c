/*
Students:   Lior Barak   206631418
			Shalev Kedar 314714080

Project Coding Musicians:

At this project we gets from the user concerts details, for each concert the program find musicians in accordance
to the instruments that needed at the concert details.
If it canot manage a concert show the program will print an appropriate message.
Else the program can manage the concert show and an apropriate massage with details will be printed.

*/

#include "Concert_header.h"

void main(int argc, char* argv[])
{
    FILE *instrumentsFile, *musiciansFile;
    InstrumentTree instruments;
    int instCount, musiciansCount;
    Musician** MusiciansGroup;
    Musician*** MusiciansCollection;

    //instrumentsFile = fopen(argv[FIRST_FILE], "r");
    instrumentsFile = fopen("instruments.txt", "r");
    checkFile(instrumentsFile);

    instruments = buildInstrumentsTree(instrumentsFile, &instCount);

    //musiciansFile = fopen(argv[SECOUND_FILE], "r");
    musiciansFile = fopen("musicians.txt", "r");
    checkFile(musiciansFile);

    int* countSizes = (int*)malloc(sizeof(int) * instCount);
    checkAllocation(countSizes);

    MusiciansGroup = createMusicianArr(musiciansFile, instruments, &musiciansCount);
    MusiciansCollection = constructMCollection(instCount, MusiciansGroup, musiciansCount, countSizes);
    manageConcert(MusiciansCollection, instruments, countSizes);


    freeAll(instruments, MusiciansGroup, musiciansCount, MusiciansCollection, instCount);
    free(countSizes);
	fclose(instrumentsFile);
    fclose(musiciansFile);
}