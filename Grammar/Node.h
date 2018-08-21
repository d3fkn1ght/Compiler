#pragma once
#include "Keyword.h"

typedef struct node node;
typedef struct nodeList nodeList;

struct node {
	keyword* kw;
	node* next;
};

struct nodeList {
	node* head;
	node* current;
	node* tail;
};

void appendNode(nodeList* parent, node* node);
void freeNode(node* node);
void freeNodeList(nodeList* nl1);
node* getNode(nodeList* nl1);
node* newNode();
nodeList* newNodeList();
void resetHead(nodeList* nl1);