#include "Instrument_header.h"

// This function checks if the memory allocation has failed.
void checkAllocation(void* ptr)
{
	if (ptr == NULL)
	{
		printf("Allocation error\n");
		exit(-1);
	}
}

// This function creates and returns a binary search tree (while each of its nodes data is a line from the given text file).
InstrumentTree buildInstrumentsTree(FILE* text, int* count)
{
	int size, counter = ZERO, i;
	InstrumentTree res;

	char** arr = sortedInstrumentsArr(text, &size);

	TreeNode* trNode = newTreeNode(arr[ZERO], ZERO);
	res.root = trNode;

	for (i = 1; i < size; i++)
	{
		addNodeToBinaryTree(res, arr[i], i);
	}

	*count = (i - 1);

	return res;
}

// This functions creates a lexicographically sorted array of strings of a given text file lines
// (each line in the given text file is a string in the returned array).
char** sortedInstrumentsArr(FILE* txt, int* counter) {
	char** instList = (char**)malloc(sizeof(char*) * INITIAL);
	checkAllocation(instList);
	int lSize = ZERO, pSize = INITIAL;

	instList[lSize] = (char*)malloc(sizeof(char) * MAX_LINE);
	checkAllocation(instList[lSize]);

	while (fgets(instList[lSize], MAX_LINE, txt) != NULL) {
		lSize++;

		if (lSize == pSize) {
			pSize *= 2;
			instList = (char**)realloc(instList, sizeof(char*) * pSize);
			checkAllocation(instList);
		}

		instList[lSize] = (char*)malloc(sizeof(char) * MAX_LINE);
		checkAllocation(instList[lSize]);
	}

	instList = (char**)realloc(instList, sizeof(char*) * lSize);
	checkAllocation(instList);

	*counter = lSize;

	return instList;
}

// The function get instrument tree (res), the string to enter and a integer instrument id.
// It's add to the binary tree new node in the correct place with the data (string) and the instrument id (id).
void addNodeToBinaryTree(InstrumentTree res, char* string, int id)
{

	addNodeToBinaryTreeRec(res.root, string, id);

}

// The function get tree node (at the first call it's the root of the tree), the string to enter (data) and a integer instrument id (id).
// It's a recursive function that add to the binary tree new node in the correct place with the string (data) and the instrument id (id).
void addNodeToBinaryTreeRec(TreeNode* trNode, char* data, int id)
{
	int compare = strcmp(data, trNode->instrument);

	if (trNode == NULL)
		return;
	else if (compare > ZERO && trNode->right == NULL)
	{
		TreeNode* newNode = newTreeNode(data, id);
		trNode->right = newNode;
	}
	else if (compare < ZERO && trNode->left == NULL)
	{
		TreeNode* newNode = newTreeNode(data, id);
		trNode->left = newNode;
	}
	else
	{
		if (compare > ZERO)
			addNodeToBinaryTreeRec(trNode->right, data, id);
		else
			addNodeToBinaryTreeRec(trNode->left, data, id);
	}
}

// This function creates a new TreeNode and insert it with the given data (and NULL as its left and right nodes).
// The function returns the newly created TreeNode.
TreeNode* newTreeNode(char* data, int Id)
{
	TreeNode* res = (TreeNode*)malloc(sizeof(TreeNode));
	checkAllocation(res);

	res->instrument = data;
	res->insId = Id;
	res->left = res->right = NULL;

	return res;
}

// The function get a tree of instruments and a string (instrument name).
// It's return the insid of the instrument or EROR(-1) if not found.
int findInsId(InstrumentTree tree, char* instrument)
{
	findInsIdRec(tree.root, instrument);
}

// The function get a treeNode of instrument (the root at the start) and a string (instrument name).
// It's a recursive function that find the instrument insid and return it.
int findInsIdRec(TreeNode* trNode, char* instrument)
{
	if (trNode == NULL)
		return EROR;

	else
	{
		int compare = strcmp(instrument, trNode->instrument);

		if (compare == ZERO)
			return trNode->insId;

		else if (compare > ZERO)
			return findInsIdRec(trNode->right, instrument);

		else // compare < ZERO
			return findInsIdRec(trNode->left, instrument);
	}
}