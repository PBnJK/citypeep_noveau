#ifndef GUARD_CITYPEEP_MATH_H_
#define GUARD_CITYPEEP_MATH_H_

#include <sys/types.h>

typedef struct {
	u_char u;
	u_char v;
} CP_UV;

int isin(int x);
int icos(int x);

int hisin(int x);
int hicos(int x);
#endif // !GUARD_CITYPEEP_MATH_H_
