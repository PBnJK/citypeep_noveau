#ifndef GUARD_CITYPEEP_DIALOGUE_H_
#define GUARD_CITYPEEP_DIALOGUE_H_

/* Dialogue commands */

/* Set dialogue speed */
#define DCMD_SPD_INST "\x80"
#define DCMD_SPD_FAST "\x81"
#define DCMD_SPD_NORM "\x82"
#define DCMD_SPD_SLOW "\x83"

/* Wait a bit... */
#define DCMD_WAIT_4V "\x84"
#define DCMD_WAIT_8V "\x85"
#define DCMD_WAIT_12V "\x86"
#define DCMD_WAIT_16V "\x87"

/* Substitute for the player's name */
#define DCMD_PLAYER_NAME "\x88"

/* Color text */
#define DCMD_COLOR(R, G, B) "\x8A" R G B
#define DCMD_RESET_COLOR "\x8A\xFF\xFF\xFF"

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
