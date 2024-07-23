#ifndef GUARD_CITYPEEP_MEMORY_H_
#define GUARD_CITYPEEP_MEMORY_H_

#include <sys/types.h>

void memInit(void);

void *memAlloc(const size_t BYTES);
void memFree(void *block);

#endif // !GUARD_CITYPEEP_MEMORY_H_
