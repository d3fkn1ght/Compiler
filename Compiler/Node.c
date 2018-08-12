#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Node.h"

int maxTokenNameSz = 128;
nodeList* nlHead = NULL;
nodeList* nlTail = NULL;

void freeNlList() {

}

nodeList* getNlHead() {
	return nlHead;
}

nodeList* getNlTail() {
	return nlTail;
}

node* newNode() {
	node* node1 = (node*)malloc(sizeof(node));
	if (node1 != NULL) {
		node1->token.tType = NONE;
		if ((node1->tName = (char*)malloc(sizeof(char) * maxTokenNameSz)) == NULL) {
			free(node1);
		}
	}
	return node1;
}

nodeList* newNodeList() {
	nodeList* nl = NULL;
	nl = (nodeList*)malloc(sizeof(nodeList));
	if (nl != NULL) {
		nl->Next = NULL;
		nl->Node = NULL;
	}

	return nl;
}

nodeList* newListNode() {
	nodeList* nl = (nodeList*)malloc(sizeof(nodeList));
	if (nl != NULL) {
		nl->Node = NULL;
		nl->Next = NULL;
	}
	return nl;
}

int addnewnode(nodeList** nlList, node* node1) {
	nodeList* nlNode = NULL;
	
	if ((nlNode = newListNode()) == NULL) {
		// throw error
		return -1;
	}
	
	nlNode->Node = node1;

	if (nlHead == NULL) {
		nlHead = nlNode;
		nlTail = nlNode;
	}
	else {
		nlTail->Next = nlNode;
		nlTail = nlNode;
	}

	return 0;
}

void printNodes() {
	int i = 0;
	nodeList* current = getNlHead();
	while (current != NULL) {
		fprintf(stderr, "Node Type: %d\n", current->Node->token.tType);
		fprintf(stderr, "Node Name: %s\n", current->Node->tName);
		current = current->Next;
	}
}

int setNodeName(node* node1, char* buf, int len) {
	return strncpy_s(node1->tName, maxTokenNameSz, buf, len);
}