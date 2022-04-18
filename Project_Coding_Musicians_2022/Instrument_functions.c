#include "Concert_header.h"

void checkAllocation(void* ptr)
{
	if (ptr == NULL)
	{
		printf("Allocation error\n");
		exit(-1);
	}
}

InstrumentTree buildInstrumentsTree(FILE* text, int* count) 
{
	int size, counter = 0;
	InstrumentTree res;

	char** arr = sortedInstrumentsArr(text, &size);
	res.root = buildInstrumentsTreeRec(arr, 0, size - 1, &counter);
	*count = counter;

	return res;
}

char** sortedInstrumentsArr(FILE* txt, int* counter) 
{
	char** instList = (char**)malloc(sizeof(char*));
	checkAllocation(instList);
	int lSize = 0, pSize = 1;

	instList[lSize] = (char*)malloc(sizeof(char) * MAX_LINE);
	checkAllocation(instList);

	while (fgets(instList[lSize], MAX_LINE, txt) != NULL) 
	{
		lSize++;

		if (lSize >= pSize) 
		{
			pSize *= 2;
			instList = (char**)realloc(instList, sizeof(char*) * pSize);
			checkAllocation(instList);
		}

		instList[lSize] = (char*)malloc(sizeof(char) * MAX_LINE);
		checkAllocation(instList);
	}

	lSize--;

	if (lSize < pSize) 
	{
		instList = (char**)realloc(instList, sizeof(char*) * lSize);
		checkAllocation(instList);
	}

	*counter = lSize;
	mergeInstruments(instList , lSize);

	return instList;
}

void mergeInstruments(char** arr, int size) 
{
	char** tmpArr = NULL;
	if (size <= 1)
		return;

	mergeInstruments(arr, size / 2);
	mergeInstruments(arr + size / 2, size - size / 2);

	tmpArr = (char**)malloc(size * sizeof(char*));
	checkAllocation(tmpArr);

	merge(arr, size / 2, arr + size / 2, size - size / 2, tmpArr);
	copyArr(arr, tmpArr, size); 
	free(tmpArr);
}

void merge(char** a1, int n1, char** a2, int n2, char** res)
{
	int ind1, ind2, resInd;
	ind1 = ind2 = resInd = 0;

	while ((ind1 < n1) && (ind2 < n2)) 
	{
		upperLowerCase(a2[ind2], a1[ind1]);

		if (strcmp(a2[ind2], a1[ind1]) > 0) 
		{
			res[resInd] = a1[ind1];
			ind1++;
		}
		else 
		{
			res[resInd] = a2[ind2];
			ind2++;
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

void upperLowerCase(char* s1, char* s2) 
{
	if (s1[0] >= LOWER_A) 
	{
		if (s2[0] < LOWER_A) 
		{
			s2[0] = s2[0] + (LOWER_A - UPPER_A);
		}
	}
	else 
	{
		if (s2[0] >= LOWER_A) 
		{
			s2[0] = s2[0] - (LOWER_A - UPPER_A);
		}
	}
}

void copyArr(char** dest, char** src, int size)
{
	int i;

	for (i = 0; i < size; i++)
		dest[i] = src[i];
}

TreeNode* buildInstrumentsTreeRec(char** arr, int left, int right, int* id) {
	if (left > right) 
	{
		return NULL;
	}

	int mid = (left + right) / 2;
	TreeNode* root = newTreeNode(arr[mid], id);
	root->left = buildInstrumentsTreeRec(arr, left, mid - 1, id);
	root->right = buildInstrumentsTreeRec(arr, mid + 1, right, id);
	
	return root;
}

TreeNode* newTreeNode(char* data, int* Id) 
{
	TreeNode* res = (TreeNode*)malloc(sizeof(TreeNode));
	checkAllocation(res);

	res->instrument = data;
	res->insId = *Id;
	(*Id)++;
	res->left = res->right = NULL;

	return res;
}

// The function get a tree of instruments and a string (instrument name).
//It's return the insid of the instrument or EROR(-1) if not found.
int findInsId(InstrumentTree tree, char* instrument) 
{
	findInsIdRec(tree.root, instrument);
}

// The function get a treeNode of instrument (the root at the start) and a string (instrument name).
//It's a recursive function that find the instrument insid and return it.
int findInsIdRec(TreeNode* trNode, char* instrument) 
{
	if (trNode == NULL)
		return EROR;

	else
	{
		int compare = strcmp(instrument, trNode->instrument);

		if (compare == 0)
			return trNode->insId;

		else if (compare > 0)
			return findInsIdRec(trNode->right, instrument);

		else // compare < 0
			return findInsIdRec(trNode->left, instrument);
	}
}

