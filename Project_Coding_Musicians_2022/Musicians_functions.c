#include "Concert_header.h"


// The function get a text file of musicians information, the instrument tree, and a output parameter logicSize.
// It's create and set the array of pointers to musicians from the information in the file, return that array and return as output parameter the number of musicians. (logicSize)
Musician** createMusicianArr(FILE* musiciansFile, InstrumentTree instTree, int* logicSize)
{
	int PhysicSize = INITIAL;
	Musician** MusicianGroup = (Musician**)malloc(sizeof(Musician*) * PhysicSize);
	checkAllocation(MusicianGroup);

	(*logicSize) = fillMusicianGroup(&MusicianGroup, PhysicSize, musiciansFile, instTree);

	return MusicianGroup;
}

// The function get a pointer to array of pointers to musicians, a int physicSize, a file of musicians information and the instrument tree.
// It's update the pointer to array of pointers to musicians from the information in the file and return the number of musicians.
int fillMusicianGroup(Musician*** MusicianGroup, int MusiciansPhysicSize, FILE* musiciansFile, InstrumentTree instTree)
{
	int MusiciansLogicSize = ZERO, namePhysicSize, nameLogicSize;
	Musician** tempMusicianGroup = *MusicianGroup;
	char seps[] = " ,.;:?!-\t'()[]{}<>~_", * token;
	char* tempString = (char*)malloc(sizeof(char) * MAX_LINE);
	checkAllocation(tempString);

	while (fgets(tempString, MAX_LINE, musiciansFile) != NULL) {
		updateMusicianGroupPhysicSizeArray(&tempMusicianGroup, MusiciansLogicSize, &MusiciansPhysicSize);
		Musician* musician = initializeMusician(&namePhysicSize, &nameLogicSize);
		token = strtok(tempString, seps);

		while (token != NULL) {                                            // run on one line (one string)
			updateNamePhysicSizeArray(musician, nameLogicSize, &namePhysicSize);

			if (nameLogicSize <= 1)                                        // the first two words must be the musician name.
				addMusicianNameToArray(musician, token, &nameLogicSize);

			else {
				if (isInstrument(instTree, token) == false)                  // not a instrument, so must be a part from the musician name.
					addMusicianNameToArray(musician, token, &nameLogicSize);
				else {                                                       // isInstrument(instTree, token) == true
					MPIListNode* mpiNode = initializeMPINode(instTree.root, token, seps);

					if (musician->instruments.head == NULL)
						musician->instruments.head = musician->instruments.tail = mpiNode;
					else {
						musician->instruments.tail->next = mpiNode;
						musician->instruments.tail = mpiNode;
					}
				}
			}
			token = strtok(NULL, seps);
		}
		musician->name = (char**)realloc(musician->name, sizeof(char*) * nameLogicSize);
		checkAllocation(musician->name);
		tempMusicianGroup[MusiciansLogicSize] = musician;
		MusiciansLogicSize++;
		clearString(tempString);
	}
	tempMusicianGroup = (Musician**)realloc(tempMusicianGroup, sizeof(Musician**) * MusiciansLogicSize);
	checkAllocation(tempMusicianGroup);
	MusicianGroup = &tempMusicianGroup;
	return MusiciansLogicSize;            // logic size;
}

// The function get a pointer to array of pointers to musicians, the array logical size and a pointer to it's phisical size.
// If the logical size equal to the phisical size it's update the array to be doubled at his physical size and update the variable physicSize also to be doubled.
void updateMusicianGroupPhysicSizeArray(Musician*** tempMusicianGroup, int logSize, int* physicSize)
{
	if (logSize == (*physicSize))
	{
		(*physicSize) *= 2;
		*tempMusicianGroup = (Musician**)realloc(*tempMusicianGroup, sizeof(Musician*) * (*physicSize));
		checkAllocation(*tempMusicianGroup);
	}
}

// The function get a pointer to the name phisical size and a pointer to the name logic size.
// It's create new pointer to musician, initialize the musician and return him.
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


// The function get a pointer musician, the name array log size and a pointer to name phisic size.
// If the logical size equal to the physical size it's update the array to be doubled at his py\hysical size and update the variable namePhysicSize also to be doubled.
void updateNamePhysicSizeArray(Musician* musician, int nameLogicSize, int* namePhysicSize)
{
	if (nameLogicSize == (*namePhysicSize))
	{
		(*namePhysicSize) *= 2;
		musician->name = (char**)realloc(musician->name, sizeof(char*) * (*namePhysicSize));
		checkAllocation(musician->name);
	}
}

// The function get a pointer musician, a string name and a pointer to name logic size.
// It's add a name to the name array and update the variable nameLogicSize.
void addMusicianNameToArray(Musician* musician, char* name, int* nameLogicSize)
{
	strcpy(musician->name[(*nameLogicSize)], name);
	strcat(musician->name[(*nameLogicSize)], "\0");
	(*nameLogicSize)++;
}

//The function get a instrument tree and a string (named "string").
// If the string is not a name of a instrument the function return false, else it's return true.
bool isInstrument(InstrumentTree instTree, char* string)
{
	if (findInsIdRec(instTree.root, string) == EROR)
		return false;
	else
		return true;
}

// The function get a pointer to the root of the instrument tree, a string token and a string seps. 
// It's create new pointer to mpiNode, set the mpiNode and return him.
MPIListNode* initializeMPINode(TreeNode* instTreeRoot, char* token, char* seps)
{
	MPIListNode* mpiNode = (MPIListNode*)malloc(sizeof(MPIListNode));
	checkAllocation(mpiNode);

	mpiNode->data.insId = findInsIdRec(instTreeRoot, token);

	token = strtok(NULL, seps);
	sscanf(token, "%f", &(mpiNode->data.price));

	mpiNode->next = NULL;

	return mpiNode;
}

// The function get a char* (string).
// It's empty every char at the string (switch the char with a space).
clearString(char* string)
{
	int i = 0;

	while (string[i] != '\0')
	{
		string[i] = ' ';
	}
}

// This function create an array of arrays, each of them contain a pointer to a musician,
// while the number of arrays equals the number of available instruments (from the instrumenTree).
// Each array's index in the bigger final array equals a instrument's id, and includes the pointers 
// To all the musicians which can play that instrument.
Musician*** constructMCollection(int iSize, Musician** MusicianGroup, int mSize, int** sizes)
{
	int logSize, phySize, i, j;
	Musician*** MusicianCollection = (Musician***)malloc(sizeof(Musician**) * iSize);
	checkAllocation(MusicianCollection);
	int* countSizes = (int*)malloc(sizeof(int) * iSize);
	checkAllocation(countSizes);

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
		countSizes[i] = logSize;
	}

	*sizes = countSizes;
	return MusicianCollection;
}

// This function check if a given instrument's id ('id') matches one of the given musician's 
// instruments' ids, if it does a pointer to that musician will be added to the given array of pointers to musicians ('arr').
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
}

//void printMusiciansCollection(Musician*** coll, int size)
//{
//	for (int i = ZERO; i < size; i++)
//	{
//		printf("%d", i);
//
//		for (int j = ZERO; j < size; j++)
//		{
//			printf("%s ", coll[i][j]->name);
//		}
//
//		printf("/n");
//	}
//}