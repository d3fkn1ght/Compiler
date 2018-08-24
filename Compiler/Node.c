#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Node.h"

appendNode(nodeList* ll_nodelist, node* node1) {
	if (ll_nodelist->head == NULL) {
		ll_nodelist->head = node1;
		ll_nodelist->tail = node1;
	}

	ll_nodelist->tail->Next = node1;
	ll_nodelist->tail = node1;
}

void freeNlList(nodeList* ll_nodelist) {
	node* tmpNode = NULL;

	while (ll_nodelist->head != NULL) {
		tmpNode = ll_nodelist->head->Next;
		freeNode(ll_nodelist->head);
		ll_nodelist->head = tmpNode;
	}

	free(ll_nodelist);
}

void freeNode(node* node1) {
	if (node1 != NULL) {
		freeToken(node1->t1);
		free(node1);
	}
}

node* newNode(token* t1) {
	node* node1 = NULL;
	token* t2 = NULL;

	if ((node1 = (node*)malloc(sizeof(node))) == NULL) {
		return NULL;
	}

	if ((t2 = newToken()) == NULL) {
		return NULL;
	}

	t2->tType = t1->tType;
	if ((strncpy_s(t2->str, maxTokenNameSz, t1->str, strlen(t1->str))) == -1) {
		freeToken(t2);
		freeNode(node1);
		return NULL;
	}

	node1->t1 = t2;
	node1->Next = NULL;
	return node1;
}

nodeList* newNodeList() {
	nodeList* nl = NULL;

	if ((nl = (nodeList*)malloc(sizeof(nodeList))) != NULL) {
		nl->head = NULL;
		nl->tail = NULL;
	}

	return nl;
}

void printNodes(nodeList* ll_nodelist) {
	node* current = ll_nodelist->head;

	while (current != NULL) {
		fprintf(stderr, "Node Name: %s\n", getTokenName(current->t1));
		fprintf(stderr, "Node Str:  %s\n", current->t1->str);
		current = current->Next;
	}
}