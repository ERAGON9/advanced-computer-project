#include "Concert_header.h"

// This function checks if the memory allocation has failed.
void checkAllocation(void* ptr)
{
	if (ptr == NULL)
	{
		printf("Allocation error\n");
		exit(1);
	}
}

// This function creates and returns a binary search tree (each of its nodes data is a line from the given text file (instrument name)).
// Also the function return as output value the amount of nodes at the tree (the amount of different instruments).
InstrumentTree buildInstrumentsTree(FILE* text, int* count) 
{
	int id = ZERO, len;
	InstrumentTree instrumentTr;
	char* instrumentName = (char*)malloc(sizeof(char) * MAX_LINE);
	checkAllocation(instrumentName);

	fgets(instrumentName, MAX_LINE, text); // Creating the root of the tree.
	len = strlen(instrumentName);
	instrumentName[len - 1] = END_LINE;

	TreeNode* rootNode = newTreeNode(instrumentName, id);
	instrumentTr.root = rootNode;
	id++;

	while (fgets(instrumentName, MAX_LINE, text) != NULL) // Creating the binary tree.
	{
		len = strlen(instrumentName);
		instrumentName[len - 1] = END_LINE;
		addNodeToBinaryTree(instrumentTr, instrumentName, id);
		id++;
	}

	*count = id;
	free(instrumentName);
	return instrumentTr;
}

// The function get instrument tree (instrumentTr), the string to enter and a integer instrument id.
// It's add to the binary tree new node in the correct place with the data (string) and the instrument id (id).
void addNodeToBinaryTree(InstrumentTree instrumentTr, char* string, int id)
{
	addNodeToBinaryTreeRec(instrumentTr.root, string, id);
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

	char* newString = (char*)malloc(sizeof(char) * strlen(data));
	checkAllocation(newString);
	strcpy(newString, data);

	res->instrument = newString;
	res->insId = Id;
	res->left = res->right = NULL;

	return res;
}

// The function gets a tree of instruments and a string (instrument name).
// It returns the insid of the instrument or EROR(-1) if not found.
int findInsId(InstrumentTree tree, char* instrument)
{
	return findInsIdRec(tree.root, instrument);
}

// The function gets a treeNode of instrument (the root at the start) and a string (instrument name).
// It's a recursive function that finds the instrument insid and returns it.
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

// The function get a pointer to file.
// It Check that the file opening was successful.
void checkFile(FILE* pointer) 
{
	if (pointer == NULL)
	{
		printf("file can't be opened \n");
		exit(1);
	}
}