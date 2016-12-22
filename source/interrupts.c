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
float ctrl;
float p_offset=1;
float s_offset=1;
float d_offset;
float phase;
__interrupt void PWM1_int(void)
{
	read_ext_ad();
	//GpioDataRegs.GPASET.bit.GPIO17 = 1;
	cnt_jee--;

	// macro for calling a function through a pointer

	ctrl= 0.5;
	duty2 =  m_execute_fpid_ctrl(voltage_ctrl);

	duty2 = duty2*.5+.5;

	if(duty1>duty2)
	{
		ctrl_scaled = (ctrl*223)*duty2+226*(duty2);
		d_offset = 1-duty2;
	}
	else
	{
		ctrl_scaled = (ctrl*223)*duty1+226*(duty1);
		d_offset = 1-duty1;
	}


	if(ctrl>=0)
		ctrl_scaled+=223*d_offset*2;

	// the 223 and 226 are used to scale the +/-1 output of PI control to phase register values

	ph_shift_1 = 225*(1-duty1);
	ph_shift_2 = 225*(1-duty1);

	ph_shift_3 = 225*(1-duty2);
	ph_shift_4 = 225*(1-duty2);

	ph_shift_pri_sec_1 = -225+(ctrl_scaled);
	ph_shift_pri_sec_2 =  225-(ctrl_scaled);



	phase = m_execute_fpid_ctrl(voltage_ctrl);
	phase = phase+1;
	phase = phase*.25;
	duty1 = .3;
	duty2 = .7;

	*phase_reg.p1_phase = 449*duty1		  + 449*duty2-449*duty1*phase;
	*phase_reg.p2_phase = 					449*duty2-449*duty1*phase;

	*phase_reg.s1_phase = 449*duty2;
	*phase_reg.s2_phase = 0;


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
