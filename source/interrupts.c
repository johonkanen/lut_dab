/*
 * interrupts.c
 *
 *  Created on: 8.12 2016
 *      Author: Jari
 */
#define INTERRUPTS_C

#include "interrupts.h"
#include "datatypes.h"
#include "ctrl_macros.h"

#pragma CODE_SECTION(PWM1_int, "ramfuncs");

float ctrl_scaled;
float duty1 = .25;
float duty2 = .25;
float phase;
__interrupt void PWM1_int(void)
{
	read_ext_ad();
	//GpioDataRegs.GPASET.bit.GPIO17 = 1;
	cnt_jee--;

	// macro for calling a function through a pointer

	phase = m_execute_fpid_ctrl(voltage_ctrl);
	duty1 = .2;
	duty2 = .3;

	if (phase >=0)
	{
		if(duty1<duty2)
		{
			*phase_reg.p1_phase = 449*duty1 + 449*duty2*phase;
			*phase_reg.p2_phase = 0+449*duty2*phase;

			*phase_reg.s1_phase = 449*duty2;
			*phase_reg.s2_phase = 0;
		}
		else
		{
			*phase_reg.p1_phase = 449*duty1;
			*phase_reg.p2_phase = 0;

			*phase_reg.s1_phase = 449*duty2+ 449*duty1*phase;
			*phase_reg.s2_phase = 0+ 449*duty1*phase;
		}
	}
	else
	{
		phase = -phase;
		if(duty1<duty2)
		{
			*phase_reg.p1_phase = 449*duty1;
			*phase_reg.p2_phase = 0;

			*phase_reg.s1_phase = 449*duty2+449*duty1*phase;
			*phase_reg.s2_phase = 0+449*duty1*phase;
		}
		else
		{
			*phase_reg.p1_phase = 449*duty1+ 449*duty2*phase;
			*phase_reg.p2_phase = 0+ 449*duty2*phase;

			*phase_reg.s1_phase = 449*duty2;
			*phase_reg.s2_phase = 0;
		}
	}
	//GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;

	if (SciaRegs.SCIFFTX.bit.TXFFST == 0)
	    {
			SciaRegs.SCITXBUF = *meas.pri_current_1;
			SciaRegs.SCITXBUF = *meas.pri_current_1>>8;
	    }

	// Clear INT flag for this timer
	EPwm1Regs.ETCLR.bit.INT = 1;
	// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
