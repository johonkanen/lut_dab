/*
 * SPI_conf.c
 *
 *  Created on: 12 Nov 2016
 *      Author: Jari
 *		copied from original TI examples
 */

#include "CLA.h"


void init_cla(void)
{
	   //Copy over the CLA code and Tables
	   memcpy(&Cla1funcsRunStart, &Cla1funcsLoadStart, (Uint32)&Cla1funcsLoadSize);
	   //memcpy(&Cla1mathTablesRunStart, &Cla1mathTablesLoadStart, (Uint32)&Cla1mathTablesLoadSize);

	   /*  Compute all CLA task vectors */

	   EALLOW;
	   Cla1Regs.MVECT1 = (Uint16) (&Cla1Task1 - &Cla1Prog_Start)*sizeof(Uint32);
	   /*
	   Cla1Regs.MVECT2 = (Uint16)((Uint32)&Cla1Task2 - (Uint32)&Cla1Prog_Start);
	   Cla1Regs.MVECT3 = (Uint16)((Uint32)&Cla1Task3 - (Uint32)&Cla1Prog_Start);
	   Cla1Regs.MVECT4 = (Uint16)((Uint32)&Cla1Task4 - (Uint32)&Cla1Prog_Start);
	   Cla1Regs.MVECT5 = (Uint16)((Uint32)&Cla1Task5 - (Uint32)&Cla1Prog_Start);
	   Cla1Regs.MVECT6 = (Uint16)((Uint32)&Cla1Task6 - (Uint32)&Cla1Prog_Start);
	   Cla1Regs.MVECT7 = (Uint16)((Uint32)&Cla1Task7 - (Uint32)&Cla1Prog_Start);
	   */
	   Cla1Regs.MVECT8 = (Uint16) (&Cla1Task8 - &Cla1Prog_Start)*sizeof(Uint32);
	   EDIS;

/*
	   EALLOW;
	   Cla1Regs.MMEMCFG.bit.PROGE 	= 1;
	   Cla1Regs.MCTL.bit.IACKE	= 1;
	   Cla1Regs.MMEMCFG.bit.RAM0E	= CLARAM0_ENABLE;
	   Cla1Regs.MMEMCFG.bit.RAM1E	= CLARAM1_ENABLE;
	   EDIS;
*/
}
