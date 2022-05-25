#include "Concert_header.h"

// This function receives concerts' details, and for each concert the function will try to organize the 
// concert based on given requriments. If a concert's execution was successful the function will print the 
// concert's details, including the chosen musicians, the instruments they will be using and the price they ask.
// Otherwise, if a concert's execution wasn't succssesful the function will print: "Could not find musicians for 
// the concert" and the concert's name.
void manageConcert(Musician*** MusicianCollection, InstrumentTree inst, int* sizes)
{
	char* line;
	int logSize = ZERO, phySize = INITIAL;
	Concert* allConcerts = (Concert*)malloc(sizeof(Concert) * phySize);
	checkAllocation(allConcerts);

	line = readLineFromTheUser();

	while (line[ZERO] != EMPTY_ROW) 
	{
		newConcert(allConcerts[logSize], inst, line);
		reorderCollection(allConcerts[logSize], MusicianCollection, sizes);
		setUpConcert(allConcerts[logSize], MusicianCollection, sizes, inst.root);
		logSize++;

		if (logSize == phySize) 
		{
			phySize *= 2;
			allConcerts = (Concert*)realloc(allConcerts, sizeof(Concert) * phySize);
			checkAllocation(allConcerts);
		}

		line = readLineFromTheUser();
	}

	freeConcerts(allConcerts, logSize);
}

// The function read a line of chars from the user.
// It return the line of chars as a string.
char* readLineFromTheUser()
{
	int stringLogSize = ZERO, stringPhySize = INITIAL;
	char ch, * string = (char*)malloc(sizeof(char) * stringPhySize);
	checkAllocation(string);

	scanf("%c ", &ch);

	while (ch != END_OF_LINE)
	{
		if (stringLogSize == stringPhySize)
		{
			stringPhySize *= 2;
			string = (char*)realloc(string, sizeof(char) * stringPhySize);
			checkAllocation(string);
		}
		string[stringLogSize] = ch;
		stringLogSize++;

		scanf("%c ", &ch);
	}
	string = (char*)realloc(string, sizeof(char) * (stringLogSize + 1));
	string[stringLogSize] = EMPTY_ROW;

	return string;
}

// This function receives a concert's details into 'theEvent'.
void newConcert(Concert theEvent, InstrumentTree instrumentsTr, char* description)
{
	char seps[] = " :", *token;

	makeEmptyList(&(theEvent.instrument));

	token = strtok(description, seps);
	strcpy(theEvent.name, token);
	token = strtok(NULL, seps);
	sscanf(token, "%d", &theEvent.date_of_concert.day);
	token = strtok(NULL, seps);
	sscanf(token, "%d", &theEvent.date_of_concert.month);
	token = strtok(NULL, seps);
	sscanf(token, "%d", &theEvent.date_of_concert.year);
	theEvent.date_of_concert.hour = convertHour(strtok(NULL, seps), strtok(NULL, seps));

	token = strtok(NULL, seps);

	while (token != NULL)
	{
		insertDataToEndList(&(theEvent.instrument), findInsId(instrumentsTr, token), strtok(NULL, seps), strtok(NULL, seps));
		token = strtok(NULL, seps);
	}
}

// This function receives the strings 'hours' and 'minutes' (self explenatory) and converts them to 
// a float number, represent the given time in decimal form.
float convertHour(char* hours, char* minutes) 
{
	float res, tmp;
	
	sscanf(hours, "%d", &res);
	sscanf(minutes, "%d", &tmp);
	res += (tmp / 60);

	return res;
}

// This function receives a pointer to a newly created CIList, and inserts NULL to its head and tail.
void makeEmptyList(CIList* new) 
{
	new->head = new->tail = NULL;
}

// This function creates a new CIListNode and insert it with the given details.
// The function will insert the newly created node to the end of the given list.
void insertDataToEndList(CIList* lst, int id, char sum, char importance)
{
	int theSum;

	sscanf(sum, "%d", &theSum);
	CIListNode* newTail = createNewListNode(id, theSum, importance, NULL);
	insertNodeToEndList(lst, newTail);

	return newTail;
}

// This function creates a new CIListNode, insert it with the given data, and returns it.
CIListNode* createNewListNode(int type, int count, char significance, CIListNode* next) 
{
	CIListNode* res = (CIListNode*)malloc(sizeof(CIListNode));
	checkAllocation(res);

	res->data.num = count;
	res->data.inst = type;
	res->data.importance = significance;
	res->next = next;

	return res;
}

// This function insert a given CIListNode to the end of a given CIList.
void insertNodeToEndList(CIList* lst, CIListNode* new) 
{
	if (lst->head == NULL) 
		lst->head = lst->tail = new;

	else 
	{
		lst->tail->next = new;
		lst->tail = new;
	}
}

// This function receives a Concert ('aEvent'), an array of arrays of pointers to Musicians ('MusicianCollection'), and an array of integers ('sizes').
// For each of the instruments in a given concert, the function will order the array of pointers to musicians who can play the instrument,
//  according to the price the musician ask and the importance of the instrument at that concert.
void reorderCollection(Concert aEvent, Musician*** MusicianCollection, int* sizes) 
{
	CIListNode* curr = aEvent.instrument.head;

	while (curr != NULL) 
	{
		reorderMusicians(MusicianCollection[curr->data.inst], curr->data.importance, sizes[curr->data.inst], curr->data.inst);
		curr = curr->next;
	}
}

// This function orders a given array of pointers to musicians ('players'), at an order based on the given 'direction'.
void reorderMusicians(Musician** players, int direction, int size, int id)
{
	MusiciansDetails* tmpArr = (MusiciansDetails*)malloc(sizeof(MusiciansDetails) * size);
	checkAllocation(tmpArr);

	createAidArray(tmpArr, players, size, id);
	mergeMusicians(tmpArr, size, direction);
	insertMusicians(players, tmpArr, size);
	free(tmpArr);
}

// This function inserts to a given array ('aidArr') pointers to musicians (from 'performers') and the prices
// those musicians ask for performing with a given instrument ('instId').
void createAidArray(MusiciansDetails* aidArr, Musician** performers, int size, int instId)
{
	int i;

	for (i = 0; i < size; i++) 
	{
		aidArr[i].pointer = performers[i];
		aidArr[i].askedPrice = findPrice(instId, performers[i]->instruments.head);
	}
}

// This function searches for a given instrument (which 'wanted' is its id) in a given MPIList and returns the insturment's price.
// The musician play this instrument (so the if condition must will happen once every call of this function).
int findPrice(int wanted, MPIListNode* head)
{
	MPIListNode* curr = head;

	while (curr != NULL) 
	{
		if (curr->data.insId == wanted)
			return curr->data.price;

		else 
			curr = curr->next;
	}
}

// This function sorts a given array of MusiciansDetails recursively.
void mergeMusicians(MusiciansDetails* aidArr, int size, int direct)
{
	MusiciansDetails* tmpArr = NULL;

	if (size <= 1)
		return;

	mergeMusicians(aidArr, size / 2, direct);
	mergeMusicians(aidArr + size / 2, size - size / 2, direct);

	tmpArr = (char**)malloc(size * sizeof(char*));
	checkAllocation(tmpArr);

	mergeM(aidArr, size / 2, aidArr + size / 2, size - size / 2, tmpArr, direct);
	copyMArr(aidArr, tmpArr, size);

	free(tmpArr);
}

// This funtion merges two array of MusiciansDetails ('a1', 'a2') to a third array ('res') at a chosen order (based on 'importance').
void mergeM(MusiciansDetails* a1, int n1, MusiciansDetails* a2, int n2, MusiciansDetails* res, int importance)
{
	int ind1 = 0, ind2 = 0, resInd = 0;

	while ((ind1 < n1) && (ind2 < n2)) 
	{
		if (importance == 1) // 1 - important
		{
			if (a1->askedPrice > a2->askedPrice) 
			{
				res[resInd] = a1[ind1];
				ind1++;
			}
			else 
			{
				res[resInd] = a2[ind2];
				ind2++;
			}
		}
		else  // importance == 0 // 0 - not important
		{
			if (a1->askedPrice < a2->askedPrice) 
			{
				res[resInd] = a1[ind1];
				ind1++;
			}
			else 
			{
				res[resInd] = a2[ind2];
				ind2++;
			}
		}
		resInd++;
	}

	while (ind1 < n1) 
	{
		res[resInd] = a1[ind1];
		ind1++;
		resInd++;
	}
	while (ind2 < n2) 
	{
		res[resInd] = a2[ind2];
		ind2++;
		resInd++;
	}
}

// This function copys a given array's data ('src') to a second given array ('dest').
void copyMArr(MusiciansDetails* dest, MusiciansDetails* src, int size)
{
	int i;

	for (i = 0; i < size; i++)
		dest[i] = src[i];
}

// This function runs on a given list of pointers to musicians ('performers') and inserts 
// it with pointers to musicians from a given ordered list of MusiciansDetails ('aidArr').
void insertMusicians(Musician** performers, MusiciansDetails* aidArr, int size)
{
	for (int i = 0; i < size; i++) 
	{
		performers[i] = aidArr[i].pointer;
	}
}

// This function choses musicians to perform in a given concert based on the given requirements
// (needed amount from each instrument and its importance, store in a list inside the given concert 'show').
// If the function couldn't find a suitable musician, an error message will be printed.
// Otherwise the function will print the concert details, including its name, date and attending 
// players including the instrument they will be using and the price they ask for.
void setUpConcert(Concert show, Musician*** MusicianCollection, int* sizes, TreeNode* root)
{
	int logSize = ZERO, phySize = INITIAL, i;
	bool proceed = true;
	CIListNode* curr = show.instrument.head;
	Musician* taken = (Musician*)malloc(sizeof(Musician) * phySize);
	checkAllocation(taken);

	while (curr != NULL && proceed == true)
	{
		for (i = ZERO; (i < curr->data.num) && (proceed == true); i++)
		{
			proceed = addMusician(MusicianCollection[curr->data.inst], sizes[curr->data.inst], taken, &logSize, &phySize);
		}

		curr = curr->next;
	}

	if (proceed == false)
		printf("Could not find musicians for the concert %s", show.name);

	else // proceed == true
		printConcert(show, taken, logSize, root);

	free(taken);
}

// This function runs on a given array of pointers to musicians ('options'), and checks if a musician doesn't
// exist in the given array of musicians ('busy'). If it doesn't, the musician will be added to 'busy'.
// If the function couldn't find a musician, it will return false, otherwise the function will return true.
bool addMusician(Musician** options, int optionArrSize, Musician* busy, int* lSize, int* pSize)
{
	bool found = false;
	int i, j;

	for (i = ZERO; i < optionArrSize; i++)
	{
		for (j = ZERO; j < lSize; j++)
		{
			if (options[i]->name != busy[j].name)
				found = true;
		}

		if (found == true)
		{
			busy[*lSize] = *(options[i]);
			lSize++;
			if (*lSize == *pSize)
			{
				(*pSize) *= 2;
				busy = (Musician*)realloc(busy, sizeof(Musician) * (*pSize));
			}
			return found;
		}
	}

	return found;
}

// This function prints a given concert's details, including its name, date and attending 
// players including the instrument they will be using and the price they ask for.
void printConcert(Concert theEvent, Musician* busy, int size, TreeNode* root)
{
	int hours = (int)theEvent.date_of_concert.hour;
	int minutes = ((int)(theEvent.date_of_concert.hour) % 1) * HOUR;
	CIListNode* curr = theEvent.instrument.head;
	char* instName;
	int inx = ZERO, tmpPrice, i, j, sumPrice = ZERO;

	printf("%s ", theEvent.name);
	printf("%d ", theEvent.date_of_concert.day);
	printf("%d ", theEvent.date_of_concert.month);
	printf("%d ", theEvent.date_of_concert.year);
	printf("%d:%d\n", hours, minutes);

	while (curr != NULL)
	{
		instName = findInstrumentName(root, curr->data.inst);

		for (i = inx; i < (curr->data.num + inx); inx++, i++)
		{
			tmpPrice = findAskedPrice(busy[i], curr->data.inst);
			for(j = ZERO; j< busy[i].nameSize; j++)
				printf("%s ", busy[i].name[j]);
			printf("%s ", instName);
			printf("%d\n", tmpPrice);
			sumPrice += tmpPrice;
		}
		curr = curr->next;
	}
	printf("%d\n", sumPrice);
}

// This function runs recursively on a given tree and searches for a node which it's 'insId'
// equals to id, and returns the node's 'instrument' (the instrument' name).
char* findInstrumentName(TreeNode* trNode, int id) 
{
	char *left, *right;
	if (trNode == NULL) 
		return NULL;
	else 
	{
		if (trNode->insId == id) 
			return trNode->instrument;
		else 
		{
			left = findInstrumentName(trNode->left, id);
			right = findInstrumentName(trNode->right, id);

			if (left != NULL) 
				return left;

			return right; // can be the instrument name or NULL
		}
	}
}

// This function searches for a given instrument ('id' is the instrument's id) in a given musician list of instruments.
// It return the asked price by the musician for performing with this instrument.
int findAskedPrice(Musician artist, int id) 
{
	MPIListNode* curr = artist.instruments.head;

	while (curr != NULL) 
	{
		if (curr->data.insId == id) 
			return curr->data.price;
	}
}

// This function free a given list of concerts, and the concerts' instruments lists.
void freeConcerts(Concert* allConcerts, int size) 
{
	int i;
	CIListNode *curr, *tmp;

	for (i = ZERO; i < size; i++) 
	{
		curr = allConcerts->instrument.head;

		while (curr != NULL) 
		{
			tmp = curr->next;
			free(curr);
			curr = tmp;
		}
	}

	free(allConcerts);
}

// This function free all the dynamically allocated memory from the given arrays and tree.
void freeAll(InstrumentTree instruments, Musician** MusiciansGroup, int musiciansCount, Musician*** MusiciansCollection, int instCount)
{
	int i, j;
	MPIListNode *curr, *tmp;

	for (i = ZERO; i < musiciansCount; i++) // free MusiciansGroup
	{
		for (j = ZERO; j < MusiciansGroup[i]->nameSize; j++) // free the name
			free(MusiciansGroup[i]->name[j]);
		free(MusiciansGroup[i]->name);

		curr = MusiciansGroup[i]->instruments.head;
		while (curr != NULL)  // free the instruments list
		{
			tmp = curr->next;
			free(curr);
			curr = tmp;
		}
		free(MusiciansGroup[i]);
	}
	free(MusiciansGroup);

	for (i = ZERO; i < instCount; i++) // free the MusiciansCollection
		free(MusiciansCollection[i]);
	free(MusiciansCollection);

	freeTreeRec(instruments.root); // free the instruments tree
}

// This function free a given tree and its nodes recursively.
void freeTreeRec(TreeNode* root)
{
	if (root == NULL)
		return;
	else
	{
		freeTreeRec(root->left);
		freeTreeRec(root->right);
		free(root->instrument);
		free(root);
	}
}