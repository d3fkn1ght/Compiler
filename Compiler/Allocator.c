// ll_blocklist->current.next = (allocation*)(&(ll_blocklist->current.next)) + size / (sizeof(allocation));
// pointer arithmetic adds sizeof(allocation) currently 24 bytes per value added
// so if i had ptr plus 10 it would add 240 bytes to pointer
// so divide size of block by sizeof(allocation) to get value
// will need to handle times where size is not even divisible by size of allocation


// 	if ((*curr_pos + size) <= (mm_BLOCKSIZE - sizeof(allocation))) {
//  check to see if allocation is beyond length of block

// have thread run in background to handle gc & frees
// have thread run every so often - say every 5 secs
// also can trigger to run based on malloc usage
// put in seprate gc.h and gc.c files


// start here
// 1. free all blocks
// 2. modify all mallocs except in allocator to use my_malloc
// 3. modify new functions to handle all allocs smarter i.e. rather than have token do its own malloc have node assign the addy except when necessary
// 4. modify so allocs are byte aligned
// 5. modify get free block to handle allocation header in calc free space
// 6. will need to handle times where size is not even divisible by size of allocation
// 7. also modify get free block to handle free space in block (circular buffer?)

#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>

#include "Allocator.h"


#define TRUE 1
#define FALSE 0

struct allocation {
	size_t size;
	allocation* next;
};

struct blocklist {
	size_t size;
	mm_state state;
	blocklist* next;
	allocation current;
};

enum mm_state {
	S_ALLOC,
	S_FREE,
	S_FULL,
	S_GC,
	S_INIT,
};

blocklist	*ll_head = NULL,
			*ll_tail = NULL;

HANDLE mm_mutex;
size_t mm_BLOCKSIZE = 0;

void* allocateMemory(blocklist* ll_blocklist, size_t size) {
	void* address = NULL;
	size_t* curr_pos = 0;

	// check if first allocation, if so no need to modify the next pointer
	// but change state to indicate this is not first alloc
	if ((ll_blocklist->state) == S_INIT) {
		ll_blocklist->state = S_ALLOC;
	}

	// set current allocation values
	ll_blocklist->current.size = size;

	// move pointer
	curr_pos = (size_t*)(&(ll_blocklist->current));
	ll_blocklist->size += (sizeof(allocation) * sizeof(size_t)) + size;

	// have to check to see if pointer would go past block boundary
	// 8 bytes short
	if ((*curr_pos + size) <= (mm_BLOCKSIZE - sizeof(allocation))) {
		ll_blocklist->current.next = (allocation*)(&(ll_blocklist->current.next)) + size / (sizeof(allocation));
	}
	else {
		ll_blocklist->current.next = NULL;
		ll_blocklist->state = S_FULL;
	}

	// figure out address
	address = (void*)(&(ll_blocklist->current));
	return address;
}

int appendBlock(blocklist* ll_blocklist) {
	if (!ll_head) {
		ll_head = ll_blocklist;
		ll_tail = ll_blocklist;
	}
	else {
		ll_tail->next = ll_blocklist;
	}

	ll_tail = ll_blocklist;
	ll_tail->next = NULL;
	return 0;
}

void freeBlockList() {
	blocklist* curr = ll_head;
	while (ll_head != NULL) {
		curr = ll_head;
		ll_head = ll_head->next;
		free(curr);
	}

}

blocklist* getFreeBlock(size_t size)
{
	blocklist* curr = ll_head;
	while (curr) {
		// // modify to include allocation header
		if ((mm_BLOCKSIZE - curr->size) >= size )
			return curr;
		curr = curr->next;
	}
	
	return NULL;
}

blocklist* initializeAllocator() {
	blocklist* ll_blocklist = NULL;
	mm_mutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	if (mm_mutex == NULL) {
		// throw error
		return NULL;
	}

	mm_BLOCKSIZE = 65536;
	if ((ll_blocklist = newBlock()) == NULL) {
		// throw error
		return NULL;
	}

	return ll_blocklist;
}

int my_free(void* block) {
	// find allocation data
	// remove from block
	// memset to 0
	return 0;
}

void* my_malloc(size_t size) {
	blocklist* ll_blocklist = NULL;
	void* address = NULL;


	if ((!size) || (size > mm_BLOCKSIZE)) {
		return NULL;
	}

	ll_blocklist = getFreeBlock(size);

	if (waitForMutex() == -1) {
		// throw error
		return NULL;
	}

	if (!ll_blocklist) {
		if ((ll_blocklist = newBlock()) == NULL) {
			// throw error
			return NULL;
		}
	}
		
	if (!ReleaseMutex(mm_mutex)) {
		// throw error
		return NULL;
	}

	return allocateMemory(ll_blocklist, size);
}

blocklist* newBlock() {
	blocklist* ll_blocklist = NULL;

	if ((ll_blocklist = (blocklist*)malloc(mm_BLOCKSIZE)) == NULL) {
		// throw error
		return NULL;
	}

	// add to ll_blocklist
	if ((appendBlock(ll_blocklist)) == -1) {
		// throw error
		return NULL;
	}

	ll_blocklist->size = 0;
	ll_blocklist->state = S_INIT;
	ll_blocklist->next = NULL;
	ll_blocklist->current.size = 0;
	ll_blocklist->current.next = NULL;

	memset(ll_blocklist + 1, 0, (mm_BLOCKSIZE - sizeof(blocklist)));

	return ll_blocklist;
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