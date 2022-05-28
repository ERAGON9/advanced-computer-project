#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define MAX_LINE 151 // 150 characters and another 1 for '\0'
#define LOWER_A 'a'
#define UPPER_A 'A'
#define EROR -1
#define INITIAL 2
#define ZERO 0
#define ONE 1
#define FIRST_FILE 1
#define SECOUND_FILE 2
#define END_LINE '\0'

typedef struct treeNode
{
	char* instrument; // instrument name as string
	unsigned short insId; // instrument uniq id
	struct treeNode* left;
	struct treeNode* right;

} TreeNode;

typedef struct tree
{
	TreeNode* root;

} InstrumentTree;


void checkAllocation(void* ptr);

InstrumentTree buildInstrumentsTree(FILE* text, int* count);

TreeNode* newTreeNode(char* data, int Id);

void addNodeToBinaryTree(InstrumentTree res, char* string, int id);

void addNodeToBinaryTreeRec(TreeNode* trNode, char* data, int id);

int findInsId(InstrumentTree tree, char* instrument);

int findInsIdRec(TreeNode* trNode, char* instrument);

void checkFile(FILE* pointer);