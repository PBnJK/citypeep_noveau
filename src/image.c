/* Citypeep: Image loading */

#include "image.h"
#include "cp_memory.h"
#include "system.h"

void imgLoad(const char *FILENAME, TIM_IMAGE *tim) {
	u_long *data = sysLoadFileFromCD(FILENAME);

	OpenTIM(data);
	ReadTIM(tim);

	/* Transfer TIM from memory to framebuffer
	 * It does it through DMA, so we have to wait with DrawSync...
	 */
	LoadImage(tim->prect, tim->paddr);
	DrawSync(0);

	/* Image has a CLUT? Load it as well */
	if( tim->mode & 0x8 ) {
		LoadImage(tim->crect, tim->caddr);
		DrawSync(0);
	}
}
