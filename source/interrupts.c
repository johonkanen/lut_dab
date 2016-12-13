/*
 * interrupts.c
 *
 *  Created on: 8.12 2016
 *      Author: Jari
 */
#define INTERRUPTS_C

#include "interrupts.h"
#include "datatypes.h"

__interrupt void PWM1_int(void)
{

	read_ext_ad();

	cnt_jee--;



	ph_shift_1 = -224+(measgain * pri_current);
	ph_shift_2 = 224-(measgain * pri_current);

	/*

		include control code here

	*/

//	ph_shift = ((Uint16))(measgain*pri_current);


	EPwm3Regs.TBPHS.half.TBPHS = 224+ph_shift_1;
	EPwm4Regs.TBPHS.half.TBPHS = 224+ph_shift_2;
	EPwm5Regs.TBPHS.half.TBPHS = 224;
	EPwm6Regs.TBPHS.half.TBPHS = 224;

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
