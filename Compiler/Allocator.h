#pragma once
#include <stdlib.h>

typedef enum mm_state mm_state;
typedef struct allocation allocation;
typedef struct blocklist blocklist;


void* allocateMemory(blocklist* ll_blocklist, size_t size);
int appendBlock(blocklist* ll_blocklist);
void freeBlockList();
blocklist* getFreeBlock(size_t size);
blocklist* initializeAllocator();
void* my_malloc(size_t);
blocklist* newBlock();
int waitForMutex();