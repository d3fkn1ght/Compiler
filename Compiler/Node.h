#pragma once
#include "Token.h"

typedef struct node node;
//typedef struct nodeList nodeList;

struct node {
	s_token* token;
	node* Next;
};

//int addnewnode(node** nlist, node* node1);
int appendNode(node* node1);
node* getNlHead();
node* getNlTail();
void freeNlList();
node* newNode();
void printNodes();
int setNodeName(node* node1, char* buf, int len);