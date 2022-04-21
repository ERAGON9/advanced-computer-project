#include "Concert_header.h"

void manageConcert(Musician*** players, InstrumentTree inst, int* sizes) {
	int counter, i;

	printf("Please enter number of concerts: ");
	scanf("%d", &counter);
	Concert* allConcerts = (Concert*)malloc(sizeof(Concert) * counter);
	checkAllocation(allConcerts);

	for (i = 0; i < counter; i++) {
		newConcert(allConcerts[i], inst);
		reorderCollection(allConcerts[i], players, sizes);
		setUpConcert(allConcerts[i], players, sizes);
	}
}

void newConcert(Concert event, InstrumentTree tools) {
	char seps[] = " :", description[MAX_LINE];
	char* token;	bool end = false;

	makeEmptyList(&event.instruments);
	CIListNode* curr = event.instruments.head;

	gets(description);
	token = strtok(description, seps);
	strcpy(event.name, token);
	token = strtok(NULL, seps);
	sscanf(token, "%d", &event.date_of_concert.day);
	token = strtok(NULL, seps);
	sscanf(token, "%d", &event.date_of_concert.month);
	token = strtok(NULL, seps);
	sscanf(token, "%d", &event.date_of_concert.year);
	event.date_of_concert.hour = convertHour(strtok(NULL, seps), strtok(NULL, seps));

	while (!end) {
		token = strtok(NULL, seps);

		if (token == NULL) {
			end = true;
		}
		else {
			curr = insertDataToEndList(&event.instruments, findId(token, tools), strtok(NULL, seps), strtok(NULL, seps));
			curr = curr->next;
		}
	}
	
}

float convertHour(char* hours, char* minutes) {
	float res, tmp;
	
	sscanf(hours, "d", &res);
	sscanf(minutes, "d", &tmp);
	res += (tmp / 60);
	return res;
}

void makeEmptyList(CIList* new) {
	new->head = new->tail = NULL;
}

CIListNode* insertDataToEndList(CIList* lst, int id, char* sum, char importance) {
	int theSum;
	sscanf(sum, "%d", &theSum);
	CIListNode* newTail = createNewListNode(id, sum, importance, NULL);
	insertNodeToEndList(lst, newTail);

	return newTail;
}

CIListNode* createNewListNode(int type, int count, char significance, CIListNode* next) {
	CIListNode* res = (CIListNode*)malloc(sizeof(CIListNode));
	checkAllocation(res);
	res->data.num = count;
	res->data.importance = significance;
	res->data.inst = type;
	res->next = next;

	return res;
}

void insertNodeToEndList(CIList* lst, CIListNode* new) {
	if (lst->head == NULL) {
		lst->head = lst->tail = new;
	}
	else {
		lst->tail->next = new;
		lst->tail = new;
	}
}

int findId(char* name, InstrumentTree devices) {
	int res = FALSE_ID;
	findIdRec(devices.root, name, &res);

	return res;
}

void findIdRec(TreeNode* root, char* type, int* ID) {
	if (root == NULL || (*ID) != FALSE_ID) {
		return;
	}
	else {
		if (strcmp(root->instrument, type) == 0) {
			*ID = root->insId;
		}
		else {
			findIdRec(root->left, type, ID);
			findIdRec(root->right, type, ID);
		}
	}
}

void reorderCollection(Concert event, Musician*** artists, int* sizes) {
	int logSize = 0, phySize = 1;
	CIListNode* curr = event.instruments.head;

	while (curr != NULL) {
		reorderMusicians(artists[curr->data.inst], curr->data.importance, sizes[curr->data.inst], curr->data.inst);
		curr = curr->next;
	}
}

void reorderMusicians(Musician** players, int direction, int size, int id) {
	MusiciansPrices* tmpArr = (MusiciansPrices*)malloc(sizeof(MusiciansPrices)*size);
	checkAllocation(tmpArr);
	createAidArray(tmpArr, players, size, id);
	mergeMusicians(tmpArr, size, direction);
	insertMusicians(players, tmpArr, size);
	free(tmpArr);
}

void createAidArray(MusiciansPrices* aidArr, Musician** performers, int size, int instId) {
	for (int i = 0; i < size; i++) {
		aidArr[i].pointer = performers[i];
		aidArr[i].askedPrice = findPrice(instId, performers[i]->instruments.head);
	}
}

int findPrice(int wanted, MPIListNode* head) {
	int res = 0;
	MPIListNode* curr = head;

	while (curr != NULL || !res) {
		if (curr->data.insId == wanted) {
			res = curr->data.price;
		}
		else {
			curr = curr->next;
		}
	}

	return res;
}

void mergeMusicians(MusiciansPrices* aidArr, int size, int direct) {
	MusiciansPrices* tmpArr = NULL;
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

void mergeM(MusiciansPrices* a1, int n1, MusiciansPrices* a2, int n2, MusiciansPrices* res, int importance)
{
	int ind1, ind2, resInd;
	ind1 = ind2 = resInd = 0;

	while ((ind1 < n1) && (ind2 < n2)) {
		if (importance) {
			if (a1->askedPrice > a2->askedPrice) {
				res[resInd] = a1[ind1];
				ind1++;
			}
			else {
				res[resInd] = a2[ind2];
				ind2++;
			}
		}
		else {
			if (a1->askedPrice < a2->askedPrice) {
				res[resInd] = a1[ind1];
				ind1++;
			}
			else {
				res[resInd] = a2[ind2];
				ind2++;
			}
		}
		resInd++;
	}

	while (ind1 < n1) {
		res[resInd] = a1[ind1];
		ind1++;
		resInd++;
	}
	while (ind2 < n2) {
		res[resInd] = a2[ind2];
		ind2++;
		resInd++;
	}
}

void copyMArr(MusiciansPrices* dest, MusiciansPrices* src, int size)
{
	int i;

	for (i = 0; i < size; i++)
		dest[i] = src[i];
}

void insertMusicians(Musician** performers, MusiciansPrices* aidArr, int size) {
	for (int i = 0; i < size; i++) {
		performers[i] = aidArr[i].pointer;
	}
}

void setUpConcert(Concert show, Musician*** artists, int* sizes) {
	int logSize = 0, phySize = 1, i;
	bool proceed = true;
	CIListNode* curr = show.instruments.head;
	Musician* taken = (Musician*)malloc(sizeof(Musician));
	checkAllocation(taken);

	while (curr != NULL && proceed) {
		for (i = 0; i < curr->data.num && proceed; i++) {
			proceed = addMusician(artists[curr->data.inst], sizes[curr->data.inst], taken, &logSize, &phySize);
		}
	}

	if (!proceed) {
		printf("Could not find musicians for the concert %s", show.name);
	}
	else {
		printConcert(show, taken, logSize);
	}

	free(taken);
}

bool addMusician(Musician** options, int size, Musician* busy, int* lSize, int* pSize) {
	bool res = true, found = false;
	int inx = NOT_DEFINED;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < lSize && !found; j++) {
			if (options[i]->name == busy[j].name) {
				found = true;
			}
		}

		if (!found) {
			inx = i;
			i = size;
		}
		else {
			found = false;
		}
	}

	if (inx != -1) {
		busy[*lSize] = *options[inx];
		lSize++;

		if (*lSize >= *pSize) {
			*pSize = 2 * (*pSize);
			busy = (Musician*)realloc(busy, sizeof(Musician) * (*pSize));
		}
	}
	else {
		res = false;
	}


	return res;
}

void printConcert(Concert theEvent) {
	int hours = (int)theEvent.date_of_concert.hour;
	int minutes = (int)(theEvent.date_of_concert.hour) % 1;
	CIListNode* curr = theEvent.instruments.head;

	printf("%s ", theEvent.name);
	printf("%d ", theEvent.date_of_concert.day);
	printf("%d ", theEvent.date_of_concert.month);
	printf("%d ", theEvent.date_of_concert.year);
	printf("%d:%d ", hours, minutes);

	

}