#pragma once
#include "Token.h"

typedef struct node node;
//typedef struct nodeList nodeList;

struct node {
	s_token* token;
	node* Next;
};

//int addnewnode(node** nlist, node* node1);
int addnewnode(node** node1);
node* getNlHead();
node* getNlTail();
void freeNlList();
node* newListNode();
node* newNode();
node* newNodeList();
void printNodes();
int setNodeName(node* node1, char* buf, int len);