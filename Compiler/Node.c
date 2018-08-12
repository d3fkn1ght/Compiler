#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"
#include "Node.h"

node* nlHead = NULL;
node* nlTail = NULL;

void freeNlList() {

}

node* getNlHead() {
	return nlHead;
}

node* getNlTail() {
	return nlTail;
}

node* newNode() {
	node* node1 = NULL;

	if ((node1 = (node*)malloc(sizeof(node))) == NULL) {
		return NULL;
	}

	// call token constructor
	if ((node1->token = newToken()) == NULL) {
		free(node1);
		return NULL;
	}

	node1->Next = NULL;
	return node1;
}

int appendNode(node* node1) {
	if (nlHead == NULL) {
		nlHead = node1;
		nlTail = node1;
	}

	nlTail->Next = node1;
	nlTail = node1;

	return 0;
}

void printNodes() {
	int i = 0;
	node* current = getNlHead();
	while (current != NULL) {
		fprintf(stderr, "Node Type: %d\n", current->token->tType);
		fprintf(stderr, "Node Name: %s\n", current->token->Name);
		current = current->Next;
	}
}

int setNodeName(node* node1, char* buf, int len) {
	return strncpy_s(node1->token->Name, maxTokenNameSz, buf, len);
}