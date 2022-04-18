#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE 150
#define LOWER_A 'a'
#define UPPER_A 'A'
#define EROR -1

typedef struct treeNode
{
	char* instrument; 
	unsigned short insId; 
	struct treeNode* left;
	struct treeNode* right;

} TreeNode;

typedef struct tree
{
	TreeNode* root;

} InstrumentTree;


//This function checks if the memory allocation has failed.
void checkAllocation(void* ptr);

//This function creates a new TreeNode and insert it with the given data (and NULL as its
//left and right nodes). The function returns the newly created TreeNode.
TreeNode* newTreeNode(char* data, int* Id);

//This function creates a binary search tree of a given sorted array of strings.
TreeNode* buildInstrumentsTreeRec(char** arr, int left, int right, int* id);

//This function copys the given 'src' array's data into the given 'dest' array.
void copyArr(char** dest, char** src, int size);

//This function checks if a given string ('s1') starts with upper or lower case,
//and changes the second given string ('s2') to have its first letter mutch the case
//of s1's first letter (upper/lower) accordingly.
void upperLowerCase(char* s1, char* s2);

//This function merges two given lexicographically sorted arrays into one sorted array ('res').
void merge(char** a1, int n1, char** a2, int n2, char** res);

//This function lexicographically sorts a given array of strings recorsively.
void mergeInstruments(char** arr, int size);

//This functions creates a lexicographically sorted array of strings of a given text file lines
//(each line in the given text file is a string in the returned array).
char** sortedInstrumentsArr(FILE* txt, int* counter);

//This function creates and returns a binary search tree (while each of its nodes' data is
//a line from the given text file).
InstrumentTree buildInstrumentsTree(FILE* text, int* count);

int findInsId(InstrumentTree tree, char* instrument);

int findInsIdRec(TreeNode* trNode, char* instrument);
