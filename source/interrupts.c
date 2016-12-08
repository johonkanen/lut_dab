/*
 * interrupts.c
 *
 *  Created on: 8.12 2016
 *      Author: Jari
 */

#include "interrupts.h"
#include "datatypes.h"

__interrupt void PWM1_int(void)
{
	float measgain = (float)0.109890109890110;
	float juttu=0;
	int16 mail;

	read_ext_ad();

	cnt_jee--;

	juttu = measgain * (*meas.pri_current_1);
	mail = (int16)juttu-222;

	/*

		include control code here

	*/

	EPwm4Regs.TBPHS.half.TBPHS = (int16)(juttu-222)+445;
	EPwm5Regs.TBPHS.half.TBPHS = (int16)(juttu-222)+445;

	    if (SciaRegs.SCIFFTX.bit.TXFFST == 0)
	    {
			SciaRegs.SCITXBUF = mail;
			SciaRegs.SCITXBUF = mail>>8;
	    }


	   // Clear INT flag for this timer
	   EPwm1Regs.ETCLR.bit.INT = 1;
	   // Acknowledge this interrupt to receive more interrupts from group 3
	   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
