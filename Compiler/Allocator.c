// initialize list
// carve into different allocator sizes
// have one allocator for std types
// have two for custom types
// one for objects up to 4096?
// one for larger objects


// when allocation:
// verify size is enough 
// if so allocate
// if not get new block
// allocate

// have thread run in background to handle gc & frees
// have thread run every so often - say every 5 secs
// also can trigger to run based on malloc usage
// put in seprate gc.h and gc.c files


// start here
// 1. initializeAllocator
// 2. get freeblock
// 3. new_mmblock
// 4. free all blocks
// 5. my_malloc

#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>

#include "Allocator.h"

HANDLE mm_mutex;
size_t mm_BLOCKSIZE = 0;

#define TRUE 1
#define FALSE 0

struct mm_allocation {
	size_t size;
	unsigned isValid;
};

struct mm_header {
	size_t size;
	unsigned is_free;
	void* current;
	mm_header *next;
};

mm_header	*mm_head = NULL, 
			*mm_tail = NULL;


int freeAllMemory() {
	return -1;
}

mm_header* get_free_block(size_t size)
{
	// loops through pages and find appropriate page

	mm_header *curr = mm_head;
	while (curr) {
		if (curr->is_free && curr->size >= size)
			return curr;
		curr = curr->next;
	}
	
	return NULL;

}

int initializeAllocator(size_t size) {
	mm_mutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	if (mm_mutex == NULL) {
		// throw error
		return -1;
	}

	if (size == 0) {
		mm_BLOCKSIZE = sizeof(size_t) * 8192;
	}
	else {
		mm_BLOCKSIZE = (size / 4096) * 4096;
	}

	return 0;
}

int my_free(void* block) {
	// find allocation data
	// remove from block
	// memset to 0
	return 0;
}

void* my_malloc(size_t size, const char *file, int line, const char *func) {

	// get a block
	// if none available, add new one
	// add allocator
	// return 
	void *block;
	DWORD dwCount = 0,
		dwWaitResult = 0;
	mm_header *header;

	if (!size)
		return NULL;

	dwWaitResult = WaitForSingleObject(mm_mutex, INFINITE);

	if (dwWaitResult == WAIT_ABANDONED) {
		// abandoned mutex
		return NULL;
	}

	header = get_free_block(size);

	if (!header) {
		// get_free_block returned an existing block so return address to caller
		header->is_free = FALSE;
		if (!ReleaseMutex(mm_mutex)) {
			// throw error
			return NULL;
		}

#ifdef _DEBUG
		fprintf(stderr, "Allocated = %s, %i, %s, %p[%Iu]\n", file, line, func, (void*)(header + 1), size);
#endif // _DEBUG

		return (void*)(header + 1);
	}

	block = malloc(mm_BLOCKSIZE);
	if (!block) {
		// release mutex
		return NULL;
	}

	// new block so must initialize
	header = block;
	header->is_free = FALSE;
	header->next = NULL;

	if (!mm_head)
		mm_head = header;
	if (mm_tail)
		mm_tail->next = header;

	mm_tail = header;
	// release mutex

#ifdef _DEBUG
	fprintf(stderr, "Allocated = %s, %i, %s, %p[%Iu]\n", file, line, func, (void*)(header + 1), size);
#endif // _DEBUG

	return (void*)(header + 1);
}

void* new_mmBlock() {
	// grab mutex
	// malloc memory
	// release mutex

	// add to ll_blocklist

	return NULL;
}