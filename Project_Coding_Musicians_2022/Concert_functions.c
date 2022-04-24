#include "Concert_header.h"

// This function receives concerts' details, and for each concert the function will try to organize the 
// concert based on given requriments. If a concert's execution was successful the function will print the 
// concert's details, including the chosen musicians, the instruments they will be using and the price they ask.
// Otherwise, if a concert's execution wasn't succssesful the function will printf: "Could not find musicians for 
// the concert" and the concert's name.
void manageConcert(Musician*** players, InstrumentTree inst, int* sizes)
{
	char line[MAX_LINE];
	int logSize = ZERO, phySize = INITIAL;
	Concert* allConcerts = (Concert*)malloc(sizeof(Concert));
	checkAllocation(allConcerts);

	gets(line);

	while (line[ZERO] != '\n') {
		newConcert(allConcerts[logSize], inst, line);
		reorderCollection(allConcerts[logSize], players, sizes);
		setUpConcert(allConcerts[logSize], players, sizes, inst.root);
		logSize++;

		if (logSize == phySize) {
			phySize *= 2;
			allConcerts = (Concert*)realloc(allConcerts, sizeof(Concert) * phySize);
			checkAllocation(allConcerts);
		}

		gets(line);
	}

	//freeConcerts(allConcerts, logSize);
}

// This function receives a concert's details into 'theEvent'.
void newConcert(Concert theEvent, InstrumentTree tools, char* description)
{
	char seps[] = " :", *token;
	bool end = false;

	makeEmptyList(&(theEvent.instrument));
	CIListNode* curr = theEvent.instrument.head;

	token = strtok(description, seps);
	strcpy(theEvent.name, token);
	token = strtok(NULL, seps);
	sscanf(token, "%d", &theEvent.date_of_concert.day);
	token = strtok(NULL, seps);
	sscanf(token, "%d", &theEvent.date_of_concert.month);
	token = strtok(NULL, seps);
	sscanf(token, "%d", &theEvent.date_of_concert.year);
	theEvent.date_of_concert.hour = convertHour(strtok(NULL, seps), strtok(NULL, seps));

	while (end == false) 
	{
		token = strtok(NULL, seps);

		if (token == NULL) 
			end = true;
		else 
		{
			curr = insertDataToEndList(&theEvent.instrument, findId(token, tools), strtok(NULL, seps), strtok(NULL, seps));
			curr = curr->next;
		}
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
CIListNode* insertDataToEndList(CIList* lst, int id, char* sum, char importance) 
{
	int theSum;

	sscanf(sum, "%d", &theSum);
	CIListNode* newTail = createNewListNode(id, sum, importance, NULL);
	insertNodeToEndList(lst, newTail);

	return newTail;
}

// This function creates a new CIListNode, insert it with the given data, and returns it.
CIListNode* createNewListNode(int type, int count, char significance, CIListNode* next) 
{
	CIListNode* res = (CIListNode*)malloc(sizeof(CIListNode));
	checkAllocation(res);

	res->data.num = count;
	res->data.importance = significance;
	res->data.inst = type;
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

// This function receives a instrumen's name and an InstrumentTree, searches the given tree for the given instrument's id, and returns it.
int findId(char* name, InstrumentTree devices) 
{
	int res = FALSE_ID;

	findIdRec(devices.root, name, &res);

	return res;
}

// This function runs recursively over a given tree, searching for a instrument with the given name
// ('type'), and insert the instrument's id into the given pointer to integer 'ID'.
void findIdRec(TreeNode* root, char* type, int* ID) 
{
	int direction;

	if (root == NULL || (*ID) != FALSE_ID) 
		return;

	else 
	{
		direction = strcmp(root->instrument, type);

		if (direction == 0) 
			*ID = root->insId;

		else if (direction > 0)
			findIdRec(root->left, type, ID);

		else if (direction < 0) 
			findIdRec(root->right, type, ID);
	}
}

// This function receives a Concert ('aEvent'), an array of arrays of pointers to Musicians ('artists'), and an array of integers ('sizes').
// For each of the instruments in a given concert, the function will order the array of pointers to musicians who can play the instrument,
//  according to the price the musician ask and the importance of the instroment at that concert.
void reorderCollection(Concert aEvent, Musician*** artists, int* sizes) 
{
	int logSize = ZERO, phySize = 1;
	CIListNode* curr = aEvent.instrument.head;

	while (curr != NULL) 
	{
		reorderMusicians(artists[curr->data.inst], curr->data.importance, sizes[curr->data.inst], curr->data.inst);
		curr = curr->next;
	}
}

// This function orders a given list of pointers to musicians ('players'), at an order based on the given 'direction'.
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
	for (int i = 0; i < size; i++) 
	{
		aidArr[i].pointer = performers[i];
		aidArr[i].askedPrice = findPrice(instId, performers[i]->instruments.head);
	}
}

// This function searches for a given instrument (which 'wanted' is its id) in a given MPIList and returns the insturment's price.
int findPrice(int wanted, MPIListNode* head)
{
	int res = ZERO;
	MPIListNode* curr = head;

	while (curr != NULL && res == ZERO) 
	{
		if (curr->data.insId == wanted)
			res = curr->data.price;

		else 
			curr = curr->next;
	}

	return res;
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
void setUpConcert(Concert show, Musician*** artists, int* sizes, TreeNode* root)
{
	int logSize = ZERO, phySize = 1, i;
	bool proceed = true;
	CIListNode* curr = show.instrument.head;
	Musician* taken = (Musician*)malloc(sizeof(Musician));
	checkAllocation(taken);

	while (curr != NULL && proceed == true)
	{
		for (i = 0; (i < curr->data.num) && (proceed == true); i++)
		{
			proceed = addMusician(artists[curr->data.inst], sizes[curr->data.inst], taken, &logSize, &phySize);
		}
	}

	if (proceed == false)
		printf("Could not find musicians for the concert %s", show.name);
	else // proceed == true
		printConcert(show, taken, logSize, root);

	free(taken);
}

// This function runs on a given list of pointers to musicians ('options'), and checks if a musician doesn't
// exist in the given list of musicians ('busy'). If it doesn't, the musician will be added to 'busy'.
// If the function couldn't find a musician, it will return false, otherwise the function will return true.
bool addMusician(Musician** options, int size, Musician* busy, int* lSize, int* pSize)
{
	bool res = true, found = false;
	int inx = NOT_DEFINED, i, j;

	for (i = 0; i < size; i++)
	{
		for (j = 0; (j < lSize) && (found == true); j++) 
		{
			if (options[i]->name == busy[j].name) 
				found = true;
		}

		if (found == false)
		{
			inx = i;
			i = size;
		}
		else 
			found = false;
	}

	if (inx != -1) 
	{
		busy[*lSize] = *options[inx];
		lSize++;

		if (*lSize == *pSize)
		{
			(*pSize) *= 2;
			busy = (Musician*)realloc(busy, sizeof(Musician) * (*pSize));
		}
	}
	else
		res = false;


	return res;
}

// This function prints a given concert's details, including its name, date and attending 
// players including the instrument they will be using and the price they ask for.
void printConcert(Concert theEvent, Musician* performers, int size, TreeNode* root)
{
	int hours = (int)theEvent.date_of_concert.hour;
	int minutes = (int)(theEvent.date_of_concert.hour) % 1 * HOUR;
	CIListNode* curr = theEvent.instrument.head;
	char* instName;
	int inx = ZERO, tmpPrice;

	printf("%s ", theEvent.name);
	printf("%d ", theEvent.date_of_concert.day);
	printf("%d ", theEvent.date_of_concert.month);
	printf("%d ", theEvent.date_of_concert.year);
	printf("%d:%d\n", hours, minutes);

	while (curr != NULL)
	{
		instName = findInstrumentName(root, curr->data.inst);

		for (int i = inx; i < curr->data.num; inx++, i++) {
			tmpPrice = findAskedPrice(performers[i], curr->data.inst);
			printf("%s ", performers[i].name);
			printf("%s ", instName);
			printf("%d\n", tmpPrice);
		}

		curr = curr->next;
	}
}

//This function runs recursively on a given tree and searches for a node which it's 'insId'
//equals to id, and returns the node's 'instrument' (the instrument' name).
char* findInstrumentName(TreeNode* trNode, int id) {
	char *left, *right;
	if (trNode == NULL) {
		return NULL;
	}
	else {
		if (trNode->insId == id) {
			return trNode->instrument;
		}
		else {
			left = findInstrumentName(trNode->left, id);
			right = findInstrumentName(trNode->right, id);

			if (left != NULL) {
				return left;
			}
			else if (right != NULL) {
				return right;
			}
			else {
				return NULL;
			}
		}
	}
}

//This function searches for a given instrument ('id' is the instrument's id) in
//a given musician list of instruments, and returns the asked price by the musician 
//for performing with this instrument.
int findAskedPrice(Musician artist, int id) {
	MPIListNode* curr = artist.instruments.head;

	while (curr != NULL) {
		if (curr->data.insId == id) 
			return curr->data.price;
		
	}
}

//This dunction free a given list of concerts, and the concerts' instruments lists.
void freeConcerts(Concert* allConcerts, int size) {
	CIListNode *curr, *tmp;

	for (int i = ZERO; i < size; i++) {
		curr = allConcerts->instrument.head;

		while (curr != NULL) {
			tmp = curr->next;
			free(curr);
			curr = tmp;
		}
	}

	free(allConcerts);
}

//This function free all the dynamically allocated memory from the given arrays and tree.
void freeAll(InstrumentTree instruments, Musician** MusiciansGroup, Musician*** MusiciansCollection) {

}