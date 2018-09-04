// have thread run in background to handle gc & frees
// have thread run every so often - say every 5 secs
// also can trigger to run based on malloc usage
// put in seprate gc.h and gc.c files


// #2
// 1. implement mm_init
// 2a. implement mm_malloc
/*	  allocate one block of 64K size
	  allocate 50% for small block (1 byte, 4 byte, 8 byte, 16 byte and 32 byte allocators)
	  allocate 25% for medium block (64-byte, 128-byte, 256-byte, 512-byte and 1024-byte)
	  allocate 50% for large block (1024+ allocator)
*/
// 2b. When allocating new block use same size but don't carve up 
// 3. implement get_free_block
// 4. implement mm_free
// 5. implement mm_calloc
// 6. implement mm_realloc

#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>

#include "Allocator.h"

#define TRUE 1
#define FALSE 0

// start here
// declare defines for alignment, align macro, struct sizes
#define ALIGNMENT		  sizeof(size_t)
#define ALIGN(size)		  (((size) + (ALIGNMENT-1)) & ~0x7)
#define HDR_OVERHEAD	  (6 * sizeof(mm_heapListHdr))
#define mm_blockHdrSize    sizeof(mmBlockHdr)
#define mm_heapListHdrSize sizeof(mmHeapListHdr)

//declare structs
struct mm_BlockHdr {
	size_t allocSize;
};

struct mm_heapListHdr {
	size_t heapSize;
	unsigned int mask;
	mm_heapListHdr* next;
	mm_heapListHdr* prev;
};

// Allocator globals
HANDLE mm_mutex;
size_t ll_heapHdrSize = ALIGN(4096);
size_t mm_MemBlockSize = ALIGN(65536);

mm_heapListHdr* sm_Heap_Head = NULL;
mm_heapListHdr* md_Heap_Head = NULL;
mm_heapListHdr* lg_Heap_Head = NULL;

void mm_finalize() {
	// loop through sm_Heap_blocks
	mm_freeHeapHdr(sm_Heap_Head);
	// loop through md_Heap_blocks
	mm_freeHeapHdr(md_Heap_Head);
	// loop through lg_Heap_blocks
	mm_freeHeapHdr(lg_Heap_Head);
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

int mm_init() {
	if (((sm_Heap_Head = (mm_heapListHdr*)malloc(ll_heapHdrSize))) == NULL) {
		// throw error
		return -1;
	}

	if (((md_Heap_Head = (mm_heapListHdr*)malloc(ll_heapHdrSize))) == NULL) {
		// throw error
		return -1;
	}

	if (((lg_Heap_Head = (mm_heapListHdr*)malloc(ll_heapHdrSize))) == NULL) {
		// throw error
		return -1;
	}

	sm_Heap_Head->heapSize = ll_heapHdrSize;
	sm_Heap_Head->next = NULL;
	sm_Heap_Head->prev = NULL;
	sm_Heap_Head->mask = 0;

	md_Heap_Head->heapSize = ll_heapHdrSize;
	md_Heap_Head->next = NULL;
	md_Heap_Head->prev = NULL;
	md_Heap_Head->mask = 0;

	lg_Heap_Head->heapSize = ll_heapHdrSize;
	lg_Heap_Head->next = NULL;
	lg_Heap_Head->prev = NULL;
	lg_Heap_Head->mask = 0;

	return 0;
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