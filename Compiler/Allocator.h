#pragma once
#include <stdlib.h>

typedef struct mm_blockHdr mm_blockHdr;
typedef struct mm_heapListHdr mm_heapListHdr;

void mm_finalize();
void mm_free(void* address);
void mm_freeHeapHdr(mm_heapListHdr* heapHead);
int mm_init();
int waitForMutex();