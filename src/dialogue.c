/* Citypeep: Dialogue handler */

#include <stddef.h>

#include <sys/types.h>

#include "dialogue.h"

#include "common.h"
#include "menu.h"

static bool _dialogueOn = false;
static bool _lineDone = false;

static char **_lines = NULL;
static char *_line = NULL;
static u_short _linelen = 0;

static u_short _curchar = 0;

static u_short _time = 0;

static void _end(void) {
	_dialogueOn = false;
}

static void _advance(void) {
	if( ++_line == NULL ) {
		_end();
		return;
	}

	_linelen = cp_strlen(_line);
}

static void _nextchar(void) {
	_time = 0;

	++_curchar;
	if( _curchar >= _linelen ) {
		_lineDone = true;
	}
}

void dialogueStart(const char **DIALOGUE) {
	_dialogueOn = true;

	_lines = (char **)DIALOGUE;
	_line = *_lines;
	_linelen = cp_strlen(_line);

	_time = 0;
}

void dialogueUpdate(void) {
	if( !_dialogueOn || _lineDone ) {
		return;
	}

	if( ++_time >= 4 ) {
		_nextchar();
	}
}

void dialogueDraw(void) {
	if( !_dialogueOn ) {
		return;
	}

	menuDrawTextN(FNT_BIG, 32, 136, _line, _curchar);
	menuDrawBox(16, 128, 36, 12);
}
