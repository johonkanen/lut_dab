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
#include "C28x_FPU_FastRTS.h"

#pragma CODE_SECTION(PWM1_int, "ramfuncs");

float ctrl_scaled;
float duty1;
float duty2;
float phase;
float ctrl;
float length;
float neg_scale;
__interrupt void PWM1_int(void)
{
	read_ext_ad();
	//GpioDataRegs.GPASET.bit.GPIO17 = 1;
	cnt_jee--;

	// macro for calling a function through a pointer

	ctrl = m_execute_fpid_ctrl(voltage_ctrl);
	ctrl = -ctrl*.25;

	phase = 0;
	duty1 = 1;
	duty2 = fabs(ctrl)*4;

	if(phase>=0) // note, the logic is backwards when compared to juhamatti modulation!!!
	{

		if(duty1+duty2>0.5)
		{
			if(duty1 == duty2)
			{
				*phase_reg.p1_phase = 0;//0
				*phase_reg.p2_phase = 450*(1-duty1);

				*phase_reg.s1_phase = 0				+900*phase;
				*phase_reg.s2_phase = 450*(1-duty1)	+900*phase;
			}
			else if(duty1>duty2)
			{
				*phase_reg.p1_phase = 0+449*(.5-duty1*.5)+900*phase;//0
				*phase_reg.p2_phase = 0+449*(.5-duty1*.5)+900*phase;

				*phase_reg.s1_phase = 0				+450*(duty2+(1-duty2)*.5);
				*phase_reg.s2_phase = 450*(1-duty2)	+450*(duty2+(1-duty2)*.5);
			}
			else if(duty1<duty2)
			{
				//TODO
			}
		}
	}
	else
	{
		phase = -phase;
		if(duty1+duty2>0.5)
		{
			if(duty1 == duty2)
			{
				*phase_reg.s1_phase = 0;//0
				*phase_reg.s2_phase = 450*(1-duty1);

				*phase_reg.p1_phase = 0+900*phase;
				*phase_reg.p2_phase = 450*(1-duty1)+900*phase;
			}
			else
			{
				*phase_reg.p1_phase = 0;//0
				*phase_reg.p2_phase = 450*(1-duty1);

				*phase_reg.s1_phase = 450*(1-duty1)*.5+900*phase;
				*phase_reg.s2_phase = 450*(1-duty1)*.5+450*(1-duty2)+900*phase;
			}
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
