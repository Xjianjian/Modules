/*
 * WKPU.c
 *
 *  Created on: 2020年4月20日

 */

#include "WKPU.h"

void wkup_init(void)
{
	WKPU.NCR.B.NFEE0 = 1;
	WKPU.NCR.B.NLOCK0 = 1;
}

