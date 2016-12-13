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

	read_ext_ad();

	cnt_jee--;

//	juttu = measgain * (*meas.pri_current_1);
	mail = (int16)juttu-222;

	/*

		include control code here

	*/

	EPwm3Regs.TBPHS.half.TBPHS = 224+30;
	EPwm4Regs.TBPHS.half.TBPHS = 224-30;
	EPwm5Regs.TBPHS.half.TBPHS = 224+60;
	EPwm6Regs.TBPHS.half.TBPHS = 224-60;

/*
	EPwm4Regs.TBPHS.half.TBPHS = (int16)(juttu-222)+445;
	EPwm5Regs.TBPHS.half.TBPHS = (int16)(juttu-222)+445;
*/
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
