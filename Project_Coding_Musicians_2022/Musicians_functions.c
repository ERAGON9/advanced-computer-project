#include "Concert_header.h"


// The function gets a text file of musicians information, the instrument tree, and a output parameter logicSize.
// It creates and sets the array of pointers to musicians from the information in the file, return that array and 
// returns as a output parameter the number of musicians. (logicSize)
Musician** createMusicianArr(FILE* musiciansFile, InstrumentTree instTree, int* logicSize)
{
	int PhysicSize = INITIAL;
    Musician** MusicianGroup = (Musician**)malloc(sizeof(Musician*) * PhysicSize);
	checkAllocation(MusicianGroup);

	(*logicSize) = fillMusicianGroup(&MusicianGroup, PhysicSize, musiciansFile, instTree);

	return MusicianGroup;
}

// The function gets a pointer to array of pointers to musicians, a int physicSize, a file of musicians information and the instrument tree.
// It updates the pointer to array of pointers to musicians from the information in the file, and returns the number of musicians.
int fillMusicianGroup(Musician*** MusicianGroup, int MusiciansPhysicSize, FILE* musiciansFile, InstrumentTree instTree)
{
	int MusiciansLogicSize = ZERO, namePhysicSize, nameLogicSize;
    Musician** tempMusicianGroup = *MusicianGroup;
    char seps[] = " ,.;:?!-\t'()[]{}<>~_", *token;
	char* tempString = (char*)malloc(sizeof(char) * MAX_LINE);
	checkAllocation(tempString);

	while (fgets(tempString, MAX_LINE, musiciansFile) != NULL) 
	{
		updateMusicianGroupPhysicSizeArray(&tempMusicianGroup, MusiciansLogicSize, &MusiciansPhysicSize);
		Musician* musician = initializeMusician(&namePhysicSize, &nameLogicSize);
        token = strtok(tempString, seps);

        while (token != NULL)                         // run on one line (one string)
		{
			updateNamePhysicSizeArray(musician, nameLogicSize, &namePhysicSize);

			if (nameLogicSize <= 1)                                    // the first two words must be the musician name.
				addMusicianNameToArray(musician, token, &nameLogicSize);

            else
			{
				if (isInstrument(instTree, token) == false)           // not a instrument, so must be a part from the musician name.
					addMusicianNameToArray(musician, token, &nameLogicSize);
				else 
				{                                                    // isInstrument(instTree, token) == true
					MPIListNode* mpiNode = initializeMPINode(instTree, token, seps);
					addNodeToMusicianInstrumentsList(musician, mpiNode);
				}
            }
			token = strtok(NULL, seps);
        }
		musician->name = (char**)realloc(musician->name, sizeof(char*) * nameLogicSize);
		checkAllocation(musician->name);
		musician->nameSize = nameLogicSize;
		tempMusicianGroup[MusiciansLogicSize] = musician;
		MusiciansLogicSize++;
		clearString(tempString);
    }
	tempMusicianGroup = (Musician**)realloc(tempMusicianGroup, sizeof(Musician**) * MusiciansLogicSize);
	checkAllocation(tempMusicianGroup);
	MusicianGroup = &tempMusicianGroup;
	free(tempString);
	return MusiciansLogicSize;            // logic size;
}

// The function gets a pointer to array of pointers to musicians, the array logical size and a pointer to it's phisical size.
// If the logical size equals to the phisical size, it updates the array to be doubled at it's physical size and the output variable
// physicSize also to be doubled.
void updateMusicianGroupPhysicSizeArray(Musician*** tempMusicianGroup, int logSize, int* physicSize)
{
	if (logSize == (*physicSize))
	{
		(*physicSize) *= 2;
		*tempMusicianGroup = (Musician**)realloc(*tempMusicianGroup, sizeof(Musician*) * (*physicSize));
		checkAllocation(*tempMusicianGroup);
	}
}

// The function gets a pointer to the name phisical size, and a pointer to the name logic size.
// It creates new pointer to musician, initializes the musician and returns him,
// also it update and return as output variables namePhysicSize, nameLogicSize.
Musician* initializeMusician(int* namePhysicSize, int* nameLogicSize)
{
	Musician* musician = (Musician*)malloc(sizeof(Musician));
	checkAllocation(musician);

	(*namePhysicSize) = INITIAL;
	(*nameLogicSize) = ZERO;

	musician->name = (char**)malloc(sizeof(char*) * (*namePhysicSize));
	checkAllocation(musician->name);

	musician->instruments.head = musician->instruments.tail = NULL;

	return musician;
}


// The function gets a pointer musician, the name array log size and a pointer to name phisical size.
// If the logical size equals to the physical size it updates the array to be doubled at his physical size and
// updates the variable namePhysicSize also to be doubled.
void updateNamePhysicSizeArray(Musician* musician, int nameLogicSize, int* namePhysicSize)
{
	if (nameLogicSize == (*namePhysicSize))
	{
		(*namePhysicSize) *= 2;
		musician->name = (char**)realloc(musician->name, sizeof(char*) * (*namePhysicSize));
		checkAllocation(musician->name);
	}
}

// The function gets a pointer musician, a string name and a pointer to name logic size.
// It adds a name to the name array and updates the output variable nameLogicSize.
void addMusicianNameToArray(Musician* musician, char* name, int* nameLogicSize)
{
	musician->name[(*nameLogicSize)] = (char*)malloc(sizeof(char) * (strlen(name) + 1)); // + 1 for the '/0'at the end.
	checkAllocation(musician->name[(*nameLogicSize)]);

	strcpy(musician->name[(*nameLogicSize)], name);
	(*nameLogicSize)++;
}

//The function gets a instrument tree and a string (named "string").
// If the string is not a name of an instrument the function returns false, else it returns true.
bool isInstrument(InstrumentTree instTree, char* string)
{
	if (findInsId(instTree, string) == EROR)
		return false;
	else
		return true;
}

// The function gets a pointer to the root of the instruments tree, a string token and a string seps. 
// It creates a new pointer to mpiNode, sets the mpiNode and returns him.
MPIListNode* initializeMPINode(InstrumentTree instTree, char* token, char* seps)
{
	MPIListNode* mpiNode = (MPIListNode*)malloc(sizeof(MPIListNode));
	checkAllocation(mpiNode);

	mpiNode->data.insId = findInsId(instTree, token);

	token = strtok(NULL, seps);
	sscanf(token, "%f", &(mpiNode->data.price));

	mpiNode->next = NULL;

	return mpiNode;
}

// The function get a pointer to musician (musician) and a pointer to MPIListNode (mpiNode).
// It add the list node (mpiNode) to the musician instrument list.
void addNodeToMusicianInstrumentsList(Musician* musician, MPIListNode* mpiNode)
{
	if (musician->instruments.head == NULL)
		musician->instruments.head = musician->instruments.tail = mpiNode;

	else
	{
		musician->instruments.tail->next = mpiNode;
		musician->instruments.tail = mpiNode;
	}
}

// The function gets a char* (string).
// It emptys every char at the string (switch the char with a space).
void clearString(char* string)
{
	int i = 0;

	while (string[i] != '\0')
	{
		string[i] = ' ';
	}
}

// This function creates an array of arrays, each of them contain a pointer to a musician (MusicianCollection),
// while the number of cells at the outer array equal to the number of different existing instruments (from the instrumenTree).
// Each array's index in the outer array equal to an instrument's id, and includes the pointers 
// to all the musicians which can play that instrument.
//('iSize' - The amount of different existing instruments. 'mSize' - The amount of musicians (the amount of all the musician)).
Musician*** constructMCollection(int iSize, Musician** MusicianGroup, int mSize, int* sizes)
{
	int logSize, phySize, i, j;
	Musician*** MusicianCollection = (Musician***)malloc(sizeof(Musician**) * iSize);
	checkAllocation(MusicianCollection);

	for (i = 0; i < iSize; i++) 
	{
		MusicianCollection[i] = NULL;
		logSize = phySize = ZERO;

		for (j = 0; j < mSize; j++) 
		{
			checkMusician(MusicianGroup[j], MusicianCollection[i], i, &logSize, &phySize);
		}

		if (phySize > logSize)
		{
			MusicianCollection[i] = (Musician**)realloc(MusicianCollection[i], sizeof(Musician*) * logSize);
			checkAllocation(MusicianCollection[i]);
		}
		sizes[i] = logSize;
	}

	return MusicianCollection;
}

// This function check if a given instrument's id ('id') matches one of the given musician's instruments' ids,
// if it does a pointer to that musician will be added to the given array of pointers to musicians ('arr').
void checkMusician(Musician* player, Musician** arr, int id, int* lSize, int* pSize) 
{
	MPIListNode* curr = player->instruments.head;
	bool found = false;

	while (curr != NULL && found == false) 
	{
		if (curr->data.insId == id)
			found = true;
		else
			curr = curr->next;
	}

	if (found == true) 
	{
		if (arr == NULL) // (*lSize) == ZERO
		{
			*pSize = INITIAL;
			arr = (Musician**)malloc(sizeof(Musician*) * (*pSize));
			checkAllocation(arr);
		}
		else if (lSize == pSize)
		{
				(*pSize) *= 2;
				arr = (Musician**)realloc(arr, sizeof(Musician*) * (*pSize));
				checkAllocation(arr);
		}
		arr[(*lSize)] = player;
		(*lSize)++;
	}
