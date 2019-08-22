#include "optical_resister.h"
#include "../s3c2440_soc.h"

void optical_resister_init(void)
{
	/* optical resister to eint15 */
	GPGCON &= ~(3 << 14);
	GPGCON |= (2 << 14);

	/* Both edge triggered */
	EXTINT1 |= (7 << 28);

	/* EINTMASK */
	EINTMASK &= ~(1 << 15);

	return ;
}

