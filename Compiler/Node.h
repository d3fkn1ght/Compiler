#pragma once
#include "Token.h"

typedef struct node node;
//typedef struct nodeList nodeList;

struct node {
	token* t1;
	node* Next;
};

//int addnewnode(node** nlist, node* node1);
int appendNode(node* node1);
node* getNlHead();
node* getNlTail();
void freeNlList();
void freeNode(node* node1);
node* newNode(token* token);
void printNodes();