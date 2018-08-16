#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Node.h"

node* nlHead = NULL;
node* nlTail = NULL;
int nodeCount = 0;

int appendNode(node* node1) {
	if (nlHead == NULL) {
		nlHead = node1;
		nlTail = node1;
	}

	nlTail->Next = node1;
	nlTail = node1;
	nodeCount++;

	return 0;
}

void freeNlList() {
	node* current = getNlHead();
	node* tmpNode = NULL;

	while (current != NULL) {
		tmpNode = current;
		free(current);
		current = tmpNode;
	}
}

node* getNlHead() {
	return nlHead;
}

node* getNlTail() {
	return nlTail;
}

void freeNode(node* node1) {
	if (node1 != NULL) {
		freeToken(node1->t1);
		free(node1);
	}
}

node* newNode(token* t1) {
	node* node1 = NULL;

	if ((node1 = (node*)malloc(sizeof(node))) == NULL) {
		return NULL;
	}

	// call token constructor
	if ((node1->t1 = newToken()) == NULL) {
		free(node1);
		return NULL;
	}

	node1->t1 = t1;
	node1->Next = NULL;
	return node1;
}

void printNodes() {
	node* current = getNlHead();
	while (current != NULL) {
		fprintf(stderr, "Node Name:%s\n", getTokenName(current->t1));
		current = current->Next;
	}
}

void setNlTail(node* node1) {
	nlTail = node1;
	nodeCount++;
}