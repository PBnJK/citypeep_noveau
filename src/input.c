/* Citypeep: Input handler */

#include <sys/types.h>

#include <libpad.h>
#include <libgte.h>
#include <libgpu.h>

#include "input.h"

CP_PadData padData[2] = { 0 };

void inputInit(void) {
	PadInitDirect((u_char *)&PAD_P1, (u_char *)&PAD_P2);
	PadStartCom();
}

bool inputIsPadConnected(int port) {
	return PadGetState(port) != PadStateDiscon;
}
