#include <string.h>
#include "Allocator.h"

struct pageList {
	page* head;
	page* tail;
};

struct page {
	void* start;
	void* current;
	void* end;
	page* next;
};

const int BLOCKSIZE = 4096 * 16;

void* allocate() {
	return NULL;
}


void freePage(page* p1) {
	if (p1 != NULL) {
		free(p1->start);
	}
	p1->start = p1->current = p1->end = NULL;
	p1->next = NULL;

	free(p1);
//	p1 = NULL;
}


freePageList(pageList* ll_pagelist) {
	page* tmpPage = NULL;

	while (ll_pagelist->head != NULL) {
		tmpPage = ll_pagelist->head->next;
		freePage(ll_pagelist->head);
		ll_pagelist->head = tmpPage;
	}

	free(ll_pagelist);
	ll_pagelist->head = ll_pagelist->tail = NULL;
	
}


void appendPage(pageList* ll_pagelist, page* p1) {
	ll_pagelist->tail->next = p1;
	ll_pagelist->tail = p1;
}

page* newPage() {
	page* p1 = NULL;

	if ((p1 = (page*)malloc(sizeof(page))) == NULL) {
		// throw error
		return NULL;
	}

	if ((p1->start = (void*)malloc(BLOCKSIZE)) == NULL) {
		// throw error
		free(p1);
		return NULL;
	}

	p1->current = p1->start;
	p1->end = (void*)((size_t)p1->start + BLOCKSIZE);
	p1->next = NULL;

	memset(p1->start, 0, BLOCKSIZE);

	return p1;
}

pageList* newPageList() {
	pageList* npl = NULL;
	page* p1 = NULL;

	if ((npl = (pageList*)malloc(sizeof(pageList))) != NULL) {
		if ((p1 = newPage()) == NULL) {
			// throw error
			free(npl);
			return NULL;
		}
	}

	npl->head = npl->tail = p1;
	return npl;
}