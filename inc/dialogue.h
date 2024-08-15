#ifndef GUARD_CITYPEEP_DIALOGUE_H_
#define GUARD_CITYPEEP_DIALOGUE_H_

/* Dialogue commands */

/* Set dialogue speed */
#define DCMD_SPD_INST "\x80"
#define DCMD_SPD_FAST "\x81"
#define DCMD_SPD_NORM "\x82"
#define DCMD_SPD_SLOW "\x83"

/* Wait a bit... */
#define DCMD_WAIT_1V "\x84"
#define DCMD_WAIT_2V "\x85"
#define DCMD_WAIT_4V "\x86"
#define DCMD_WAIT_8V "\x87"

/* Controls dialogue speed, measured in VBlanks */
typedef enum {
	DSPEED_INSTANT = 1,
	DSPEED_FAST = 2,
	DSPEED_NORMAL = 4,
	DSPEED_SLOW = 8,
} CP_DialogueSpeed;

void dialogueStart(const char **DIALOGUE);

void dialogueUpdate(void);
void dialogueDraw(void);

#endif // !GUARD_CITYPEEP_DIALOGUE_H_
