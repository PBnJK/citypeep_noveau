/* Citypeep: Memory allocator */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <libapi.h>

#include "common.h"
#include "cp_memory.h"

#define HEAP_START_ADDR (void *)0x800F8000
#define HEAP_SIZE 0x00100000

/* static size_t _remainingBytes = HEAP_SIZE; */

void memInit(void) {
	EnterCriticalSection();
	InitHeap3(HEAP_START_ADDR, HEAP_SIZE);
	ExitCriticalSection();
}

void *memAlloc(const size_t BYTES) {
	/*
	_remainingBytes -= BYTES;

	if( _remainingBytes <= 0 ) {
		LOG("=== FATAL ERROR: NO_MEMORY ===\n");
		LOG("Tried to allocate %d bytes, but only had %d remaining!", BYTES,
			_remainingBytes + BYTES);
		exit();
	}
	*/

	void *block = malloc3(BYTES);
	if( !block ) {
		LOG("=== FATAL ERROR: NULL_MALLOC ===\n");
		LOG("malloc3(%d) returned NULL!", BYTES);
		exit();
	}

	return block;
}

void memFree(void *block) {
}
