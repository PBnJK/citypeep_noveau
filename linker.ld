/* _gp definition for the exception handler because nugget doesnt define it
 *
 * got it from here (MIT licensed):
 * https://github.com/spicyjpeg/ps1-bare-metal/blob/main/cmake/executable.ld#L83
 */
_gp = ALIGN(16) + 0x7ff0;
