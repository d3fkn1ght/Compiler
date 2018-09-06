// have thread run in background to handle gc & frees
// have thread run every so often - say every 5 secs
// also can trigger to run based on malloc usage
// put in seprate gc.h and gc.c files


// 1. implement get_free_block
// 2. implement mm_calloc
// 3. implement mm_realloc

#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>

#include "Allocator.h"

#define TRUE 1
#define FALSE 0

#define ALIGNMENT			sizeof(size_t)
#define ALIGN(size)			(((size) + (ALIGNMENT-1)) & ~0x7)
#define mm_blockHdrSize		sizeof(mm_blockHdr)
#define mm_heapListHdrSize	sizeof(mm_heapListHdr)

#define mm_BlockSize 65536

size_t sm_BlockSize = mm_BlockSize / 16;
size_t md_BlockSize = mm_BlockSize / 4;
size_t lg_BlockSize = mm_BlockSize;

//declare structs
struct mm_blockHdr {
	size_t allocSize;
};

struct mm_heapListHdr {
	unsigned int mask;
	mm_heapListHdr* next;
	mm_heapListHdr* prev;
	mm_heapListHdr* curr;
};

// Allocator globals
HANDLE mm_mutex;

mm_heapListHdr* sm_Heap = NULL;
mm_heapListHdr* md_Heap = NULL;
mm_heapListHdr* lg_Heap = NULL;

void mm_finalize() {
	// loop through sm_Heap_blocks
	mm_freeHeapHdr(sm_Heap);
	// loop through md_Heap_blocks
	mm_freeHeapHdr(md_Heap);
	// loop through lg_Heap_blocks
	mm_freeHeapHdr(lg_Heap);
}

void mm_free(void* address) {
	// write values for possible gc
	free(address);
}

void mm_freeHeapHdr(mm_heapListHdr* heapHead) {
	mm_heapListHdr* tmp_heapHead = NULL;
	while (heapHead != NULL) {
		tmp_heapHead = heapHead;
		heapHead = heapHead->next;
		mm_free(tmp_heapHead);
	}
}

#define freeSpace(block, size, blockSize) ((block->curr + blockSize) - (block->curr + mm_heapListHdrSize))

void* mm_findBlock(mm_heapListHdr* block, size_t reqSize, size_t blockSize) {
	mm_heapListHdr* start = NULL;

	fprintf(stderr, "%p\n", (void*)freeSpace(block, reqSize, blockSize));


	//if (curr <= ALIGN(reqSize + mm_blockHdrSize)) {
	//	return curr;
	//}

	/*for (; block != start; block = block->next) {

	}*/

	return NULL;
}

int mm_init() {
	sm_BlockSize = ALIGN(sm_BlockSize + mm_heapListHdrSize);
	md_BlockSize = ALIGN(md_BlockSize + mm_heapListHdrSize);
	lg_BlockSize = ALIGN(lg_BlockSize + mm_heapListHdrSize);

	if ((sm_Heap = (mm_heapListHdr*)malloc(sm_BlockSize)) == NULL) {
		// throw error
		return -1;
	}

	if (((md_Heap = (mm_heapListHdr*)malloc(md_BlockSize))) == NULL) {
		// throw error
		return -1;
	}

	if (((lg_Heap = (mm_heapListHdr*)malloc(lg_BlockSize))) == NULL) {
		// throw error
		return -1;
	}

	sm_Heap->mask = 0;
	sm_Heap->next = NULL;
	sm_Heap->prev = NULL;
	sm_Heap->curr = sm_Heap + 1;

	md_Heap->mask = 0;
	md_Heap->next = NULL;
	md_Heap->prev = NULL;
	md_Heap->curr = md_Heap + 1;

	lg_Heap->mask = 0;
	lg_Heap->next = NULL;
	lg_Heap->prev = NULL;
	lg_Heap->curr = lg_Heap + 1;

	return 0;
}

void* mm_malloc(size_t size) {
	mm_findBlock(sm_Heap, size, sm_BlockSize);
	mm_findBlock(md_Heap, size, md_BlockSize);
	mm_findBlock(lg_Heap, size, lg_BlockSize);
	return NULL;
}

void* mm_newBlock(size_t size) {
	// set initial values for block?
	return (void*)malloc(size);
}

int waitForMutex() {
	DWORD dwCount = 0,
		  dwWaitResult = 0;
	
	// add loop to repeatedly check
	dwWaitResult = WaitForSingleObject(mm_mutex, INFINITE);

	if (dwWaitResult == WAIT_ABANDONED) {
		// throw error
		return -1;
	}

	return 0;
}