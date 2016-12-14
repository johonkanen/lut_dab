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

	// macro for calling a function through a pointer
	execute_fpid_ctrl(voltage_ctrl);

	ph_shift_1 = -225+(measgain * 2048+30);
	ph_shift_2 =  225-(measgain * 2048-30);

	ph_shift_3 = -225+(measgain * 2048+40);
	ph_shift_4 =  225-(measgain * 2048-40);

	ph_shift_pri_sec_1 = -225+(measgain * pri_current);
	ph_shift_pri_sec_2 =  225-(measgain * pri_current);


	EPwm3Regs.TBPHS.half.TBPHS = 225+ph_shift_1+ph_shift_pri_sec_1;
	EPwm4Regs.TBPHS.half.TBPHS = 225-ph_shift_2+ph_shift_pri_sec_1;

	EPwm5Regs.TBPHS.half.TBPHS = 225+ph_shift_3+ph_shift_pri_sec_2;
	EPwm6Regs.TBPHS.half.TBPHS = 225-ph_shift_4+ph_shift_pri_sec_2;

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
