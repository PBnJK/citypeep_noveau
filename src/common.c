/* Citypeep -- Global variables & utils */

#include "common.h"

void *cp_memcpy(void *restrict dst, const void *restrict src, size_t cnt) {
	u_char *_dst = (u_char *)dst;
	const u_char *_src = (const u_char *)src;

	for( ; cnt; cnt-- )
		*(_dst++) = *(_src++);

	return dst;
}

size_t cp_strlen(const char *STR) {
	const char *S;
	for( S = STR; *S; ++S )
		;

	return (S - STR);
}
