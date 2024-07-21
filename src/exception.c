/* Citypeep: Exception handler */

#include <stdlib.h>

#include "common.h"

#include "exception.h"

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 15

static inline void syscallGPUSync(void) {
	register volatile int n asm("t1") = 0x4E;
	__asm__ volatile("" : "=r"(n) : "r"(n));
	((void (*)(void))0xA0)();
}

/* GPU I/O ports for post-crash drawing */
static volatile unsigned int *GP0 = (volatile unsigned int *)0xBF801810;
static volatile unsigned int *GP1 = (volatile unsigned int *)0xBF801814;

static int charX = 0;
static int charY = 0;

static bool scrDarkened = false;

static void _darken(void) {
	*GP1 = 0x01000000; /* Clear command buffer */
	*GP1 = 0x04000000; /* Disable DMA */

	/* 320x240, no interlace, 15bpp, preserve video mode */
	*GP1 = 0x08000001 | ((((*GP1) & 0x100000) != 0) << 3);
	syscallGPUSync();

	/* 50/50 blend mode, texture don't care, drawing allowed */
	*GP0 = 0xE1000400;
	syscallGPUSync();

	/* Drawing area top-left (0, 0) */
	*GP0 = 0xE3000000;
	syscallGPUSync();

	/* Drawing area bottom-right */
	*GP0 = 0xE4000000 | (256 << 10) | (gSCR_WIDTH);
	syscallGPUSync();

	/* Drawing offset top-left */
	*GP0 = 0xE5000000;
	syscallGPUSync();

	/* Disable masking */
	*GP0 = 0xE6000000;
	syscallGPUSync();

	*GP0 = 0x63000000; /* Black, semitransparent rectangle */
	*GP0 = 0x00000000; /* Top-left */
	*GP0 = (256 << 16) | (gSCR_WIDTH); /* Full screen */
}

static void _printChar(const char C) {
	switch( C ) {
	case '\b':
		charX -= CHAR_WIDTH;
		if( charX < 0 ) {
			charX = gSCR_WIDTH - 8;
			charY -= CHAR_HEIGHT;
		}

		break;
	case '\n':
		charX = 0;
		charY += CHAR_HEIGHT;
		break;
	case '\r':
		charX = 0;
		break;
	default:
		if( C == '\t' ) {
			charX = (charX + CHAR_WIDTH) & 0xFFE0;
		} else if( C == ' ' ) {
			charX += CHAR_WIDTH;
		} else if( C >= 0x21 && C <= 0x7E ) {
			for( int row = 0; row < CHAR_HEIGHT; row++ ) {
				u_int bits
					= *(u_char *)(0xBFC7F8DE + CHAR_HEIGHT * (C - 0x21) + row);
				for( int col = CHAR_WIDTH - 1; col >= 0 && bits != 0; col-- ) {
					if( bits & 1 ) {
						syscallGPUSync();
						*GP0 = 0x69FFFFFF; /* White pixel */
						*GP0 = ((charY + row) << 16) | (charX + col);
					}
					bits >>= 1;
				}
			}
			charX += CHAR_WIDTH;
		} else {
			return;
		}

		if( charX >= gSCR_WIDTH ) {
			charX = 0;
			charY += CHAR_HEIGHT;
		}
	}
}

static void _printHex(u_int x) {
	for( int i = 0; i < 8; i++ ) {
		const int C = x >> 28;
		_printChar(C > 9 ? C - 10 + 'A' : C + '0');
		x <<= 4;
	}
}

void excErrPrint(const char *MSG) {
	if( !scrDarkened ) {
		scrDarkened = true;
		_darken();
	}

	if( MSG == (const char *)0 ) {
		return;
	}

	while( *MSG ) {
		_printChar(*MSG);
		MSG++;
	}
}

void excExit(const char *MSG) {
	excErrPrint(MSG);
	exit();
}

typedef struct {
	long status;
	long mode;
	unsigned long reg[40];
	long system[6];
} TCB;

typedef struct {
	TCB *entry;
	long flag;
} TCBH;

static const TCBH **tcbhPointer = (const TCBH **)0x108;

static void (*oldHandler)();

static void _unresolvedException() {
	/* BIOS exception handler clobbers $gp, restore it */
	__asm__ volatile("la $gp, _gp");

	/* If the exception came from _exit, a0 has the passed exit code */
	TCB *task = (*tcbhPointer)->entry;
	excErrPrint("code=");
	_printHex(task->reg[4]);

	/* cop0r13/cause at the time of the exception */
	excErrPrint(" cause=");
	_printHex(task->reg[36]);

	/* cop0r14/epc at the time of the exception */
	excErrPrint(" pc=");
	_printHex(task->reg[32]);

	/* Dump stack */
	unsigned *sp = (unsigned *)task->reg[29];
	while( (((unsigned)sp) & (~0xE0000000)) >= 0
		&& (((unsigned)sp) & (~0xE0000000)) < 0x200000 && charY < 256 ) {
		_printHex(*sp);
		_printChar(' ');
		_printChar(' ');
		sp++;
	}

	/* Ensure display */
	syscallGPUSync();
	*GP1 = 0x05000000; /* Top-left corner */
	*GP1 = 0x03000000; /* Enable display */

	/* continue to the existing handler
	 * by default it calls the same handler, which since we've redirected that
	 * here, could overflow the stack. a debugger could still hook the same
	 * function though and we'd like to fall through, so check for the default
	 * implementation
	 */
	if( ((unsigned *)oldHandler)[0] != 0x240A00A0
		|| ((unsigned *)oldHandler)[1] != 0x01400008
		|| ((unsigned *)oldHandler)[2] != 0x24090040 ) {
		oldHandler();
	}

	/* If it was the default implementation, or it returned control, hang */
	while( 1 ) {
		/* Some emulators only refresh the display if the GPU is poked */
		volatile int foo = *GP1;
	}
}

static void (**const a0table)() = (void (**)())0x200;

void excInit(void) {
	oldHandler = a0table[0x40];
	a0table[0x40] = _unresolvedException;
}
