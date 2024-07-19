#ifndef GUARD_CITYPEEP_INPUT_H_
#define GUARD_CITYPEEP_INPUT_H_

#include <sys/types.h>

#define PLAYER_1 0
#define PLAYER_2 1

#define PAD_P1 padData[PLAYER_1]
#define PAD_P2 padData[PLAYER_2]

/* Structure with controller data. Taken from here:
 * https://github.com/sparkletron/PSYQ_Examples/blob/master/psxController.md
 */
typedef struct {
	u_char status : 8;

	u_char recvSize : 4;
	u_char type : 4;

	u_char select : 1;
	u_char l3 : 1;
	u_char r3 : 1;
	u_char start : 1;
	u_char up : 1;
	u_char right : 1;
	u_char down : 1;
	u_char left : 1;

	u_char l2 : 1;
	u_char r2 : 1;
	u_char l1 : 1;
	u_char r1 : 1;
	u_char triangle : 1;
	u_char circle : 1;
	u_char cross : 1;
	u_char square : 1;
} CP_PadData;

extern CP_PadData padData[2];

void inputInit(void);

#endif // !GUARD_CITYPEEP_INPUT_H_
