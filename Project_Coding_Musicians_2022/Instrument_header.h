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
#define INITIAL 2

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

void checkAllocation(void* ptr);
TreeNode* newTreeNode(char* data, int* Id);
TreeNode* buildInstrumentsTreeRec(char** arr, int left, int right, int* id);
void copyArr(char** dest, char** src, int size);
void upperLowerCase(char* s1, char* s2);
void merge(char** a1, int n1, char** a2, int n2, char** res);
void mergeInstruments(char** arr, int size);
char** sortedInstrumentsArr(FILE* txt, int* counter);
InstrumentTree buildInstrumentsTree(FILE* text, int* count);
int findInsId(InstrumentTree tree, char* instrument);
int findInsIdRec(TreeNode* trNode, char* instrument);
