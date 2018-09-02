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

// declare structs
// declare defines for alignment, align macro, struct sizes

HANDLE mm_mutex;
size_t mm_BLOCKSIZE = 0;

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