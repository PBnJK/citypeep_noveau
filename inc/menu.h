#ifndef GUARD_CITYPEEP_MENU_H_
#define GUARD_CITYPEEP_MENU_H_

#include <sys/types.h>

typedef enum { FNT_SMALL = 0, FNT_BIG = 1 } CP_FontType;

void menuInit(void);

void menuDrawText(CP_FontType type, u_char x, u_char y, const char *TEXT);
void menuDrawBox(u_char x, u_char y, u_char w, u_char h);

#endif // !GUARD_CITYPEEP_MENU_H_
