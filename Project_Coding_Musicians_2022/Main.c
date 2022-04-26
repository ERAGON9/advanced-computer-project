/*
Students:   Lior Barak   206631418
			Shalev Kedar 314714080
Project Coding Musicians

*/

#include "Concert_header.h"

void main(int argc, char* argv[])
{
    FILE *instrumentsFile, *musiciansFile;
    InstrumentTree instruments;
    int instCount, musiciansCount;
    Musician** MusiciansGroup;
    Musician*** MusiciansCollection;

    instrumentsFile = fopen("TestyMctestface.txt", "r");
    //instrumentsFile = fopen(argv[1], "r");
    checkFile(instrumentsFile);

    instruments = buildInstrumentsTree(instrumentsFile, &instCount);

    musiciansFile = fopen("ListOfMusicians.txt", "r");
    //musiciansFile = fopen(argv[2], "r");
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