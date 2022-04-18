/*
Students:   Lior Barak   206631418
			Shalev Kedar 314714080
Project Coding Musicians

*/

#include "Concert_header.h"

void main()
{
    FILE* ptr;
    InstrumentTree instruments;
    int instCount;
    ptr = fopen("TestyMctestface.txt", "r");

    if (ptr == NULL) 
    {
        printf("file can't be opened \n");
    }

    instruments = buildInstrumentsTree(ptr, &instCount);

    // freeAll();
	fclose(ptr);
}