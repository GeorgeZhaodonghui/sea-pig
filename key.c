#include "key.h"
#include "s3c2440_soc.h"
#include "exception.h"

void key_handle(unsigned int eint)
{
	unsigned int eintpend;
	eintpend = EINTPEND;

	if (eint == 0) {
		/* s2 */
		if ((GPFDAT & (1 << 0)) == 0)
			GPFDAT &= ~(1 << 6);
		else
			GPFDAT |= (1 << 6);
	} else if (eint == 2) {
		/* s3 */
		if ((GPFDAT & (1 << 2)) == 0)
			GPFDAT &= ~(1 << 5);
		else
			GPFDAT |= (1 << 5);
	} else if (eint == 5) {
		/* s4, s5 */
		if (eintpend & (1 << 11)) {
			/* s4 */
			if ((GPGDAT & (1 << 3)) == 0)
				GPFDAT &= ~(1 << 4);
			else
				GPFDAT |= (1 << 4);
		} else if (eintpend & (1 << 19)) {
			/* s5 */
			if ((GPGDAT & (1 << 11)) == 0)
				GPFDAT &= ~((1 << 4) | (1 << 5) | (1 << 6));
			else
				GPFDAT |= (1 << 4) | (1 << 5) | (1 << 6);
		}
	}

	EINTPEND = eintpend;
}

void key_gpio_init(void)
{
	/* s2, s3 to eint0, eint2 */
	GPFCON &= ~((3 << 0) | (3 << 4));
	GPFCON |= ((2 << 0) | (2 << 4));

	/* s4, s5 to eint11, eint19 */
	GPGCON &= ~((3 << 6) | (3 << 22));
	GPGCON |= ((2 << 6) | (2 << 22));

	/* Both edge triggered */
	EXTINT0 |= ((7<< 0) | (7 << 8));
	EXTINT1 |= (7 << 12);
	EXTINT2 |= (7 << 12);

	/* EINTMASK */
	EINTMASK &= ~((1 << 11) | (1 << 19));

	/* EINTPEND */

	/* register irq */
	interrupt_register(key_handle, 0);
	interrupt_register(key_handle, 2);
	interrupt_register(key_handle, 5);
}



	
