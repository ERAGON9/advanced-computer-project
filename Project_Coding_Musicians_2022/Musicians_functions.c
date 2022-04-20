#include "Concert_header.h"


// The function get a text file of musicians information, the instrument tree, and a output parameter logicSize.
// It's create and set the array of pointers to musicians from the information in the file, return that array and return as output parameter the number of musicians. (logicSize)
Musician** createMusicianArr(FILE* text, InstrumentTree instTree,int* logicSize)
{
	int PhysicSize = INITIAL;
    Musician** MusicianGroup = (Musician**)malloc(sizeof(Musician*) * PhysicSize);
	checkAllocation(MusicianGroup);

    char** musiciansFile[20][150]; // will be reading from a file

	(*logicSize) = fillMusicianGroup(&MusicianGroup, PhysicSize, musiciansFile, instTree);

	return MusicianGroup;
}

// The function get a pointer to array of pointers to musicians, a physicSize, a file of musicians information and the instrument tree.
// It's update the pointer to array of pointers to musicians from the information in the file and return the number of musicians.
int fillMusicianGroup(Musician*** MusicianGroup, int physicSize, char** musiciansFile, InstrumentTree instTree)
{
	int fileRow, namePhysicSize, nameLogicSize;
    Musician** tempMusicianGroup = *MusicianGroup;
    char seps[] = " ,.;:?!-\t'()[]{}<>~_", *token;

    for (fileRow = 0; fileRow < 20; fileRow++) { // the file row == the musician logic size
		updateMusicianGroupArray(&tempMusicianGroup, fileRow, &physicSize);
		Musician* musician = initializeMusician(&namePhysicSize, &nameLogicSize);
        token = strtok(musiciansFile[fileRow], seps);

        while (token != NULL) { // run on one line
			updateNameArray(musician, nameLogicSize, &namePhysicSize);

			if (nameLogicSize <= 1) // the first two words must be the musician name.
				addMusicianNameToArray(musician, token, &nameLogicSize);

            else{
				if (isInstrument(instTree, token) == false) // not a instrument, so must be a part from the musician name.
					addMusicianNameToArray(musician, token, &nameLogicSize);
				else { // isInstrument(instTree, token) == true
					MPIListNode* mpiNode = initializeMPINode(instTree.root, token, seps);

					if (musician->instruments.head == NULL)
						musician->instruments.head = musician->instruments.tail = mpiNode;
					else{
						musician->instruments.tail->next = mpiNode;
						musician->instruments.tail = mpiNode;
					}
				}
            }
			token = strtok(NULL, seps);
        }
		musician->name = (char**)realloc(musician->name, sizeof(char*) * namePhysicSize);
		checkAllocation(musician->name);
		tempMusicianGroup[fileRow] = musician;
    }
	tempMusicianGroup = (Musician**)realloc(tempMusicianGroup, sizeof(Musician**) * fileRow);
	checkAllocation(tempMusicianGroup);
	MusicianGroup = &tempMusicianGroup;
	return fileRow; // logic size;
}

// The function get a pointer to array of pointers to musicians, the array log size and a pointer to phisic size.
// If the log size equal to the phisic size it's update the array to be doubled at his logical size and update the variable physicSize also to be doubled.
void updateMusicianGroupArray(Musician*** tempMusicianGroup, int logSize, int* physicSize)
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
	(*nameLogicSize) = Zero;

	musician->name = (char**)malloc(sizeof(char*) * (*namePhysicSize));
	checkAllocation(musician->name);

	musician->instruments.head = musician->instruments.tail = NULL;

	return musician;
}


// The function get a pointer musician, the name array log size and a pointer to name phisic size.
// If the log size equal to the phisic size it's update the array to be doubled at his logical size and update the variable namePhysicSize also to be doubled.
void updateNameArray(Musician* musician, int nameLogicSize, int* namePhysicSize)
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

//The function get a instrument tree and a string (named "string").
// If the string is not a name of a instrument the function return false, else it's return true.
bool isInstrument(InstrumentTree instTree, char* string)
{
	if (findInsIdRec(instTree.root, string) == EROR)
		return false;
	else
		return true;
}









//task 4.........



// This function create an array of arrays, each of them contain a pointer to a musician, while 
// the number of arrays equals the number of available instruments (from the instrumenTree).
// Each array's index in the bigger final array equals a instrument's id, and includes the pointers 
// To all the musicians which can use that instrument.
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

//This function check if a given instrument's id ('id') matches one of the given musician's 
//instruments' ids, if it does a pointer to that musician will be added to the given array of
//pointers to musicians ('arr').
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

//void printMusiciansCollection(Musician*** coll, int size) {
//	for (int i = 0; i < size; i++) {
//		printf("%d", i);
//		for (int j = 0; j < size; j++) {
//			printf("%s ", coll[i][j]->name);
//		}
//		printf("/n");
//	}
//}