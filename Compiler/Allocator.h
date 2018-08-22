#pragma once
#include <stdlib.h>

extern const int BLOCKSIZE;
typedef struct page page;
typedef struct pageList pageList;

void* allocate();
void appendPage(pageList* ll_pagelist, page* p1);
void freePage(page* p1);
freePageList(pageList* ll_pagelist);
page* newPage();
pageList* newPageList();