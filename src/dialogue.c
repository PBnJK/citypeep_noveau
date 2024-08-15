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
static u_short _actualchar = 0;
static u_short _lastspace = 0;

/* Ticks until next character should be shown */
static short _speed = DSPEED_NORMAL;
static short _tick = 0;

static void _end(void) {
	_dialogueOn = false;
}

static u_int _dstrlen(const char *STR) {
	const char *S;
	for( S = STR; *S; ++S ) {
		if( *S == '\x8A' ) {
			S += 3;
		}
	}

	return (S - STR);
}

static void _advance(void) {
	if( ++_line == NULL ) {
		_end();
		return;
	}

	_linelen = _dstrlen(_line);
}

static void _processCmd(void) {
	switch( _line[_curchar] ) {
	case '\x80': /* DCMD_SPD_INST */
		_speed = DSPEED_INSTANT;
		break;
	case '\x81': /* DCMD_SPD_FAST */
		_speed = DSPEED_FAST;
		break;
	case '\x82': /* DCMD_SPD_NORM */
		_speed = DSPEED_NORMAL;
		break;
	case '\x83': /* DCMD_SPD_SLOW */
		_speed = DSPEED_SLOW;
		break;
	case '\x84': /* DCMD_WAIT_4V */
		_tick = -4;
		break;
	case '\x85': /* DCMD_WAIT_8V */
		_tick = -8;
		break;
	case '\x86': /* DCMD_WAIT_12V */
		_tick = -12;
		break;
	case '\x87': /* DCMD_WAIT_16V */
		_tick = -16;
		break;
	case '\x8A': /* DCMD_COLOR */
		_curchar += 3;
	}
}

static void _nextchar(void) {
	_tick = 0;

	if( ++_curchar >= _linelen ) {
		_lineDone = true;
		return;
	}

	if( _line[_curchar] < 0 ) {
		_processCmd();
		return;
	}

	if( _line[_curchar] == ' ' ) {
		_lastspace = _curchar;
	}

	if( ++_actualchar == 26 ) {
		if( _lastspace > 0 ) {
			_line[_lastspace] = '\n';
		}

		_actualchar = _curchar - _lastspace;
		_lastspace = 0;
	}
}

void dialogueStart(const char **DIALOGUE) {
	_dialogueOn = true;

	_lines = (char **)DIALOGUE;
	_line = *_lines;
	_linelen = _dstrlen(_line);

	_tick = -4; /* Wait a bit until starting */
}

void dialogueUpdate(void) {
	if( !_dialogueOn || _lineDone ) {
		return;
	}

	if( ++_tick >= _speed ) {
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
