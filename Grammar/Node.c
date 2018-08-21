#include <stdlib.h>
#include <string.h>

#include "Node.h"
#include "Keyword.h"

void appendNode(nodeList* nl1, node* node1) {
	if (nl1->head == NULL) {
		nl1->current = nl1->head = nl1->tail = node1;
	}
	else {
		nl1->tail->next = node1;
		nl1->tail = node1;
		nl1->tail->next = NULL;
	}
}

void freeNode(node* node1) {
	if (node1 != NULL) {
		freeKeyword(node1->kw);
		free(node1);
	}
}

void freeNodeList(nodeList* nl1) {
	node* current = nl1->head;
	node* tmpNode = NULL;

	while (current != NULL) {
		tmpNode = current;
		current = current->next;
		free(tmpNode);
	}

	free(nl1);
}

node* getNode(nodeList* nl1) {
	if (nl1->current == NULL) {
		return NULL;
	}

	node* tmpNode = nl1->current;
	nl1->current = nl1->current->next;

	return tmpNode;
}

node* newNode() {
	node* node1 = NULL;
	node1 = (node*)malloc(sizeof(node));
	if (node1 != NULL) {
		node1->kw = newKeyword();
		if (node1->kw == NULL) {
			return NULL;
		}
		return node1;
	}
	return NULL;
}

nodeList* newNodeList() {
	nodeList* nl1 = (nodeList*)malloc(sizeof(nodeList));
	
	if (nl1 != NULL) {
		nl1->head = nl1->current = nl1->tail = NULL;
	}
	return nl1;
}

void resetHead(nodeList* nl1) {
	nl1->current = nl1->head;
}