#pragma once
#include "Token.h"

typedef struct node node;
typedef struct nodeList nodeList;

struct node {
	token* t1;
	node* Next;
};

struct nodeList {
	node* head;
	node* tail;
};

//int addnewnode(node** nlist, node* node1);
appendNode(nodeList* ll_nodelist, node* node1);
void freeNlList(nodeList* ll_nodelist);
void freeNode(node* node1);
node* newNode(token* token);
nodeList* newNodeList();
void printNodes(nodeList* ll_nodelist);