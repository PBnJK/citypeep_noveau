/* Citypeep: Dialogue handler */

#include <stddef.h>

#include <sys/types.h>

#include "dialogue.h"

#include "common.h"
#include "menu.h"

static bool _dialogueOn = false; /* Dialogue box up? */
static bool _lineDone = false; /* Current dialogue line done? */

/* Current line info */
static char **_lines = NULL;
static char *_line = NULL;
static u_short _linelen = 0;

static u_short _curchar = 0;
static u_short _lastspace = 0;

/* Ticks until next character should be shown */
static short _nextchar_time = DSPEED_NORMAL;
static short _tick = 0;

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
	_tick = 0;

	++_curchar;
	if( _curchar >= _linelen ) {
		_lineDone = true;
		return;
	}

	if( _line[_curchar] == ' ' ) {
		_lastspace = _curchar;
	}

	if( _curchar % 26 == 0 ) {
		if( !_lastspace ) {
			/* Line too long, no place to break. Break mid-word... */
			_line[_curchar - 2] = '\n';
			return;
		}

		_line[_lastspace] = '\n';
		_lastspace = 0;
	}
}

void dialogueStart(const char **DIALOGUE) {
	_dialogueOn = true;

	_lines = (char **)DIALOGUE;
	_line = *_lines;
	_linelen = cp_strlen(_line);

	_tick = -4; /* Wait a bit until starting */
}

void dialogueUpdate(void) {
	if( !_dialogueOn || _lineDone ) {
		return;
	}

	if( ++_tick >= _nextchar_time ) {
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
