#include "Concert_header.h"


Musician** createMusicianArr(int size)
{
    size = INITIAL;
    Musician** MusicianGroup = (Musician**)malloc(sizeof(Musician*) * size);
    checkAllocateMusicianArrPtr(MusicianGroup);

    char** musiciansFile[20][150]; // will be reading from a file

    fillMusicianGroup(&MusicianGroup, size, musiciansFile);
}

void checkAllocateMusicianArrPtr(Musician** MusicianGroup) // // The function get a array to pointers to Musicians after try to allocate memory and test if the allocate succeeded.
{
    if (MusicianGroup == NULL)
    {
        printf("Not enough memory to allocate");
        exit(1);
    }
}

void fillMusicianGroup(Musician*** MusicianGroup, int size, char** musiciansFile) // The function...
{
    int fileRows, nameIndex;
    Musician** tempMusicianGroup = *MusicianGroup;

    char seps[] = " ,.;:?!-\t'()[]{}<>~_";
    char* token;

    for (fileRows = 0; fileRows < 20; fileRows++) // the number of lines
    {
        nameIndex = 0;
        token = strtok(musiciansFile[fileRows], seps);

        while (token != NULL) // run on one line
        {
            if (nameIndex == 0)
            {
                strcpy(tempMusicianGroup[fileRows]->name[nameIndex], token);
                strcat(tempMusicianGroup[fileRows]->name[nameIndex], "\0");
                nameIndex++;
            }

            else
            {
                if (isInstrument(token) == true)
                {

                }





            }


            token = strtok(NULL, seps);
        }




    }





}

bool isInstrument(char* string)
{

}











//task 4.........

Musician*** constructMCollection(int iSize, Musician** group, int mSize) 
{
	int logSize, phySize;
	Musician*** res = (Musician***)malloc(sizeof(Musician**) * iSize);
	checkAllocation(res);

	for (int i = 0; i < iSize; i++) 
	{
		res[i] = NULL;
		logSize = phySize = 0;
		for (int j = 0; j < mSize; j++) 
		{
			checkMusician(group[j], res[i], i, &logSize, &phySize);
		}

		if (phySize > logSize) 
		{
			res[i] = (Musician**)realloc(res[i], sizeof(Musician*) * logSize);
			checkAllocation(res[i]);
		}
	}

	return res;
}

void checkMusician(Musician* player, Musician** arr, int id, int* lSize, int* pSize) 
{
	MPIListNode* curr = player->instruments.head;
	bool found = false;

	while (curr != NULL || !found) 
	{
		if (curr->data.insId == id) 
			found = true;
		else 
			curr = curr->next;
	}

	if (found) 
	{
		if (arr == NULL) 
		{
			*lSize = *pSize = 1;
			arr = (Musician**)malloc(sizeof(Musician*) * (*pSize));
			checkAllocation(arr);
			arr[*lSize - 1] = player;
		}
		else 
		{
			if (lSize >= pSize) 
			{
				*pSize = (*pSize) * 2;
				arr = (Musician**)realloc(arr, sizeof(Musician*) * (*pSize));
				checkAllocation(arr);
			}

			arr[*lSize] = player;
			*lSize = *lSize + 1;
		}
	}
}