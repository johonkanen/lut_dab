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
float duty1,d1;
float duty2,d2;
float phase;
float ctrl;
float length;
float neg_scale;
Uint16 p1_phase,p2_phase,s1_phase,s2_phase,rxtest1 = 4095*.1,rxtest2 = 4095*.1,rxdata;
__interrupt void PWM1_int(void)
{
	GpioDataRegs.GPASET.bit.GPIO17 = 1;
	read_ext_ad();

	cnt_jee--;

	// macro for calling a function through a pointer

	ctrl = m_execute_fpid_ctrl(voltage_ctrl);

	d1	= m_execute_fpid_ctrl(d1_ctrl);
	d2	= m_execute_fpid_ctrl(d2_ctrl);


	ctrl = -ctrl*.25;
	//ctrl = cnt_jee*3.0518e-05*.25;

	phase = ctrl;
	duty1 =  rxtest1*2.44200e-4;
	duty2 =  rxtest2*2.44200e-4;

	*(phase_reg.p1_phase+6) = 449;
	*(phase_reg.p2_phase+6) = 449;
	*(phase_reg.s1_phase+6) = 449;
	*(phase_reg.s2_phase+6) = 449;


	if(phase>=0)
	{
		if(duty1+duty2>=1.0)
		{
			if(duty1 == duty2)
			{
				p1_phase = 0+900*phase;//0
				p2_phase = 0+450*(1-duty1)+900*phase;

				s1_phase = 0				;
				s2_phase = 0+450*(1-duty1)	;
			}
			else if(duty1>duty2)
			{
				p1_phase = 0+450*(1-duty2)*.5-450*(1-duty1)*.5+900*phase;//0
				p2_phase = 0+450*(1-duty2)*.5+450*(1-duty1)*.5+900*phase;

				s1_phase = 0				;
				s2_phase = 0+450*(1-duty2)	;
			}
			else if(duty1<duty2)
			{
				p1_phase = 0				+ 900*phase;
				p2_phase = 0+450*(1-duty1)	+ 900*phase;

				s1_phase = 0+450*(1-duty1)*.5-450*(1-duty2)*.5;//0
				s2_phase = 0+450*(1-duty1)*.5+450*(1-duty2)*.5;
			}
		}
		else//if(duty1+duty2<1.0)
		{
			if(duty1 == duty2)
			{
				p1_phase = 0				+900*phase*(duty1+duty2);
				p2_phase = 0+450*(1-duty1)	+900*phase*(duty1+duty2);

				s1_phase = 0				;
				s2_phase = 0+450*(1-duty1)	;
			}
			else if(duty1>duty2)
			{
				p1_phase = 0				+450*(duty1-duty2)*.5+900*phase*(duty1+duty2);
				p2_phase = 0+450*(1-duty1)	+450*(duty1-duty2)*.5+900*phase*(duty1+duty2);

				s1_phase = 0;
				s2_phase = 0+450*(1-duty2);
			}
			else if(duty1<duty2) // TODO
			{
				p1_phase = 0+900*phase*(duty1+duty2);
				p2_phase = 0+450*(1-duty1)+900*phase*(duty1+duty2);

				s1_phase = 0+450*(duty2-duty1)*.5;
				s2_phase = 0+450*(1-duty2)+450*(duty2-duty1)*.5;
			}
		}

	}
	else// if(phase<0)
	{
		phase = -phase;
		if(duty1+duty2>=1.0)
		{
			if(duty1 == duty2)
			{
				p1_phase = 0;
				p2_phase = 0+450*(1-duty1);

				s1_phase = 0+900*phase;//0
				s2_phase = 0+450*(1-duty1)+900*phase;
			}
			else if(duty1>duty2)
			{
				p1_phase = 0+450*(1-duty2)*.5-450*(1-duty1)*.5;//0
				p2_phase = 0+450*(1-duty2)*.5+450*(1-duty1)*.5;

				s1_phase = 0				+900*phase;
				s2_phase = 0+450*(1-duty2)	+900*phase;
			}
			else if(duty1<duty2)
			{
				s1_phase = 0+450*(1-duty1)*.5-450*(1-duty2)*.5+ 900*phase;//0
				s2_phase = 0+450*(1-duty1)*.5+450*(1-duty2)*.5+ 900*phase;

				p1_phase = 0				;
				p2_phase = 0+450*(1-duty1)	;
			}
		}
		else//if(duty1+duty2<=1.0)
		{
			if(duty1 == duty2)
			{
				p1_phase = 0				;//0
				p2_phase = 0+450*(1-duty1)	;

				s1_phase = 0				+900*phase*(duty1+duty2);
				s2_phase = 0+450*(1-duty1)	+900*phase*(duty1+duty2);
			}
			else if(duty1>duty2)
			{
				p1_phase = 0				+450*(duty1-duty2)*.5;//0
				p2_phase = 0+450*(1-duty1)	+450*(duty1-duty2)*.5;

				s1_phase = 0				+900*phase*(duty1+duty2);
				s2_phase = 0+450*(1-duty2)	+900*phase*(duty1+duty2);
			}
			else if(duty1<duty2) // TODO
			{
				p1_phase = 0;
				p2_phase = 0+450*(1-duty1);

				s1_phase = 0+450*(duty2-duty1)*.5			   +900*phase*(duty1+duty2);
				s2_phase = 0+450*(1-duty2)+450*(duty2-duty1)*.5+900*phase*(duty1+duty2);
			}
		}
	}


	if(p1_phase==0||p2_phase==0||s1_phase==0||s2_phase==0)
	{
		p1_phase+=1;
		p2_phase+=1;
		s1_phase+=1;
		s2_phase+=1;
	}

	if(p1_phase>448)
	{
		*(phase_reg.p1_phase+6) = 440;
	}

	if(p2_phase>448)
	{
		*(phase_reg.p2_phase+6) = 440;
	}

	if(s1_phase>448)
	{
		*(phase_reg.s1_phase+6) = 440;
	}

	if(s2_phase>448)
	{
		*(phase_reg.s2_phase+6) = 440;
	}



	//update phase shift registers
	*phase_reg.p1_phase = p1_phase;
	*phase_reg.p2_phase = p2_phase;

	*phase_reg.s1_phase = s1_phase;
	*phase_reg.s2_phase = s2_phase;


	if (ScibRegs.SCIFFTX.bit.TXFFST == 0)
	    {
			ScibRegs.SCITXBUF = (Uint16)*meas.sec_current;
			ScibRegs.SCITXBUF = (Uint16)*meas.sec_current>>8;
	    }


	if (ScibRegs.SCIFFRX.bit.RXFFST == 2)
	{
		rxdata =  ScibRegs.SCIRXBUF.all<<8;
		rxdata |=  ScibRegs.SCIRXBUF.all;
		if (rxdata <0x1000)
		{
			rxtest1 = rxdata;
		}
		else if(rxdata < 0x2000)
		{
			rxtest2 = rxdata-4096;
		}
	}

	// Clear INT flag for this timer
	EPwm1Regs.ETCLR.bit.INT = 1;
	// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

	GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;
}
