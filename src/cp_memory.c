/* Citypeep: Memory allocator */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <libapi.h>

#include "common.h"
#include "cp_memory.h"

#define STACK_SIZE 0x10000

#define B2KB(X) ((X) >> 10)

extern char __heap_start, __sp;

void memInit(void) {
	const u_long HEAP_SIZE = (&__sp - STACK_SIZE) - &__heap_start;

	EnterCriticalSection();
	InitHeap3((u_long *)&__heap_start, HEAP_SIZE);
	ExitCriticalSection();
}

void *memAlloc(const size_t BYTES) {
	void *block = malloc3(BYTES);
	if( !block ) {
		LOG("=== FATAL ERROR: NULL_MALLOC ===\n");
		LOG("malloc3(%d) returned NULL!\n", BYTES);
		exit();
	}

	return block;
}

void memFree(void *block) {
	free3(block);
}
