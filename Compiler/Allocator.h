#pragma once
#include <stdlib.h>

typedef struct mm_allocation mm_allocation;
typedef struct mm_header mm_header;


int appendBlock();
mm_header* get_free_block(size_t size);
int initializeAllocator(size_t size);

#ifdef _DEBUG
	#ifndef _MY_MALLOC_DEBUG
	#define _MY_MALLOC_DEBUG
		void* my_malloc(size_t size, const char *file, int line, const char *func);
	#else
		void* my_malloc(size_t);
	#endif // !_MY_MALLOC_DEBUG
#endif _DEBUG

void* new_mmBlock();