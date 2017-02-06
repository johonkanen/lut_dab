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

Uint16 p1_phase_m = 449,p2_phase_m = 449,s1_phase_m = 449,s2_phase_m = 449;

float sini[136] = {0, 0.0465253121977467, 0.0929498609293922, 0.139173100960065, 0.185094923044715, 0.230615870742440, 0.275637355816999, 0.320061871756972, 0.363793204953152, 0.406736643075800, 0.448799180200462, 0.489889718238087, 0.529919264233205, 0.568801123102852, 0.606451085398813, 0.642787609686539, 0.677731999145732, 0.711208572010137, 0.743144825477394, 0.773471592733964, 0.802123192755044, 0.829037572555042, 0.854156441580526, 0.877425397954582, 0.898794046299167, 0.918216106880274, 0.935649515839532, 0.951056516295154, 0.964403740114939, 0.975662280184214, 0.984807753012208, 0.991820351541262, 0.996684888044506, 0.999390827019096, 0.999932308003762, 0.998308158271268, 0.994521895368273, 0.988581719497112, 0.980500495755979, 0.970295726275997, 0.957989512315489, 0.943608506393584, 0.927183854566787, 0.908751128973564, 0.888350250793004, 0.866025403784439, 0.841824938595258, 0.815801268044172, 0.788010753606722, 0.758513583348869, 0.727373641573049, 0.694658370458998, 0.660438623999008, 0.624788514543960, 0.587785252292473, 0.549508978070806, 0.510042589765676, 0.469471562785891, 0.427883764941654, 0.385369266142448, 0.342020143325669, 0.297930281038482, 0.253195168104802, 0.207911690817759, 0.162177923105568, 0.116092914125230, 0.0697564737441255, 0.0232689563740566, -0.0232689563740564, -0.0697564737441253, -0.116092914125230, -0.162177923105568, -0.207911690817759, -0.253195168104801, -0.297930281038482, -0.342020143325669, -0.385369266142448, -0.427883764941654, -0.469471562785891, -0.510042589765675, -0.549508978070806, -0.587785252292473, -0.624788514543960, -0.660438623999008, -0.694658370458997, -0.727373641573049, -0.758513583348869, -0.788010753606722, -0.815801268044172, -0.841824938595258, -0.866025403784439, -0.888350250793004, -0.908751128973564, -0.927183854566787, -0.943608506393584, -0.957989512315489, -0.970295726275997, -0.980500495755979, -0.988581719497112, -0.994521895368273, -0.998308158271268, -0.999932308003762, -0.999390827019096, -0.996684888044506, -0.991820351541262, -0.984807753012208, -0.975662280184214, -0.964403740114939, -0.951056516295154, -0.935649515839532, -0.918216106880274, -0.898794046299167, -0.877425397954582, -0.854156441580526, -0.829037572555042, -0.802123192755043, -0.773471592733964, -0.743144825477395, -0.711208572010137, -0.677731999145733, -0.642787609686540, -0.606451085398813, -0.568801123102852, -0.529919264233205, -0.489889718238088, -0.448799180200462, -0.406736643075800, -0.363793204953152, -0.320061871756972, -0.275637355817000, -0.230615870742440, -0.185094923044715, -0.139173100960066, -0.0929498609293923, -0.0465253121977473, -2.44929359829471e-16};

Uint16 i=0;

Uint16 p1_phase,p2_phase,s1_phase,s2_phase,rxduty1 = 4095*.3,rxduty2 = 4095*.12,rxdata=0,rxphase=2048, rx_vref=110;

extern Uint16* mailbox;

extern float current_filter[2];// = {0.7921,0.2079};
extern float current_filter_mem;// = 0;
extern Uint16 current_filter_output;// = 0;


extern float current_filter2[6];
extern float current_filter2_mem[2];
extern Uint16 current_filter_2_output;

Uint16 i_ctrl_send_out;
Uint16 v_ctrl_send_out;


Uint16* mailbox_addr;

Uint16 mean_filtered;
__interrupt void PWM1_int(void)
{
	GpioDataRegs.GPASET.bit.GPIO17 = 1;

	//assembly functions in corresponding assembly files
	read_ext_ad();
	mean_filter();
	prbs_9();

	cnt_jee--;


	current_filter_2_output = mean_filtered	*current_filter2[0] +current_filter2_mem[0];
	current_filter2_mem[0] =  mean_filtered	*current_filter2[1] +current_filter2_mem[1] - current_filter_2_output*current_filter2[1+3];
	current_filter2_mem[1] =  mean_filtered	*current_filter2[2] 						- current_filter_2_output*current_filter2[2+3];


	d1_ctrl.ref = m_execute_fpid_ctrl(voltage_ctrl);
	ctrl= m_execute_fpid_ctrl(d1_ctrl);

	if(i>135)
	{
		i=0;
	}

	ctrl = ctrl*.25;// + sini[i]*.2;
	i++;
	//ctrl = cnt_jee*3.0518e-05*.25;

	//phase =  (rxphase*4.88280e-4-1)*.25;
	phase = ctrl;
	duty1 =  rxduty1*m_12bit_gain;
	duty2 =  rxduty2*m_12bit_gain;

	*(phase_reg.p1_phase+6) = 448;
	*(phase_reg.p2_phase+6) = 448;
	*(phase_reg.s1_phase+6) = 448;
	*(phase_reg.s2_phase+6) = 448;


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
			else if(duty1<duty2)
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
/************************************************************************************/
	if(p1_phase>=*(phase_reg.p1_phase+6) && p1_phase_m<=*(phase_reg.p1_phase+6))
	{
		*(phase_reg.p1_phase+6) =  p1_phase+3;
	}
	else if(p1_phase<=*(phase_reg.p1_phase+6) && p1_phase_m>=*(phase_reg.p1_phase+6))
	{
		*(phase_reg.p1_phase+6) =  p1_phase_m-3;
	}
/************************************************************************************/
	if(p2_phase>=*(phase_reg.p2_phase+6) && p2_phase_m<=*(phase_reg.p2_phase+6))
	{
		*(phase_reg.p2_phase+6) =  p2_phase+3;
	}
	else if(p2_phase<=*(phase_reg.p2_phase+6) && p2_phase_m>=*(phase_reg.p2_phase+6))
	{
		*(phase_reg.p2_phase+6) =  p2_phase_m-3;
	}
/************************************************************************************/
	if(s1_phase>=*(phase_reg.s1_phase+6) && s1_phase_m<=*(phase_reg.s1_phase+6))
	{
		*(phase_reg.s1_phase+6) =  s1_phase+3;
	}
	else if(s1_phase<=*(phase_reg.s1_phase+6) && s1_phase_m>=*(phase_reg.s1_phase+6))
	{
		*(phase_reg.s1_phase+6) =  s1_phase_m-3;
	}
/************************************************************************************/
	if(s2_phase>=*(phase_reg.s2_phase+6) && s2_phase_m<=*(phase_reg.s2_phase+6))
	{
		*(phase_reg.s2_phase+6) =  s2_phase+3;
	}
	else if(s2_phase<=*(phase_reg.s2_phase+6) && s2_phase_m>=*(phase_reg.s2_phase+6))
	{
		*(phase_reg.s2_phase+6) =  s2_phase_m-3;
	}
/************************************************************************************/

	//update measurement triggers

	EPwm1Regs.CMPA.half.CMPA = 900-s2_phase-10;
	EPwm1Regs.CMPB = 900-s2_phase-10;

	EPwm6Regs.CMPA.half.CMPA = 600-sig_prbs;
	EPwm6Regs.CMPB = 600-sig_prbs;//900-sig_prbs-16;

	/*
	EPwm6Regs.CMPA.half.CMPA = *(phase_reg.p2_phase+6);

	EPwm6Regs.TBPHS.half.TBPHS =p2_phase;
*/
	//update phase shift registers
	*phase_reg.p1_phase = p1_phase;
	*phase_reg.p2_phase = p2_phase;

	*phase_reg.s1_phase = s1_phase;
	*phase_reg.s2_phase = s2_phase;

	p1_phase_m = p1_phase;
	p2_phase_m = p2_phase;
	s1_phase_m = s1_phase;
	s2_phase_m = s2_phase;

	i_ctrl_send_out = (Uint16)(225*ctrl+225);
	v_ctrl_send_out = (Uint16)(1855*d1_ctrl.ref+1855);
	meas.pri_current_2 = &i_ctrl_send_out;
	meas.pri_current_1 = &v_ctrl_send_out;

	if (ScibRegs.SCIFFTX.bit.TXFFST == 0)
	    {
		ScibRegs.SCITXBUF = (Uint16)*((Uint16*)*mailbox);
		ScibRegs.SCITXBUF = (Uint16)*((Uint16*)*mailbox)>>8;
	    }


	if (ScibRegs.SCIFFRX.bit.RXFFST > 2|| ScibRegs.SCIRXST.bit.RXERROR == 1)
	{
		ScibRegs.SCICTL1.bit.SWRESET = 0;
		ScibRegs.SCICTL1.bit.SWRESET = 1;
	}

	if (ScibRegs.SCIFFRX.bit.RXFFST == 2)
	{
		rxdata =  ScibRegs.SCIRXBUF.all<<8;
		rxdata |=  ScibRegs.SCIRXBUF.all;
		if (rxdata <0x1000)
		{
			rxduty1 = rxdata;
		}
		else if(rxdata < 0x2000)
		{
			rxduty2 = rxdata-0x1000;
		}
		else if(rxdata < 0x3000)
		{
			rxphase= rxdata-0x2000;

			//(duty-0x2000)/2048-1
		}
		else if(rxdata < 0x6000)
		{
			rx_vref= rxdata-0x5000;
			if(rx_vref >(Uint16)1600)// 600 corresponds to 92.5v voltage
			{
				rx_vref = 1600;
			}

			voltage_ctrl.ref = rx_vref*m_12bit_gain;

			//(duty-0x2000)/2048-1
		}
/*******************************************************************************/
		else if(rxdata < 0x7000)
		{

			Uint16 rx_vkp= rxdata-0x6000;
			if(rx_vkp >(Uint16)1600)// 600 corresponds to 92.5v voltage
			{
				rx_vkp = 1600;
			}

			voltage_ctrl.kp = rx_vkp*m_12bit_gain;

			//(duty-0x2000)/2048-1
		}
		else if(rxdata < 0x8000)
		{
			Uint16 rx_vki= rxdata-0x7000;
			if(rx_vki >(Uint16)1600)// 600 corresponds to 92.5v voltage
			{
				rx_vki = 1600;
			}

			voltage_ctrl.ki = rx_vki*m_12bit_gain*10;

			//(duty-0x2000)/2048-1
		}
		else if(rxdata < 0x9000)
		{
			Uint16 rx_ikp= rxdata-0x8000;
			if(rx_ikp >(Uint16)1600)// 600 corresponds to 92.5v voltage
			{
				rx_ikp = 1600;
			}

			d1_ctrl.kp = rx_ikp*m_12bit_gain;

			//(duty-0x2000)/2048-1
		}
		else if(rxdata < 0xA000)
		{
			Uint16 rx_iki= rxdata-0x9000;
			if(rx_iki >(Uint16)1600)// 600 corresponds to 92.5v voltage
			{
				rx_iki = 1600;
			}

			d1_ctrl.ki = rx_iki*m_12bit_gain;

/*******************************************************************************/
		}
		else if(rxdata == 0xf666)//start modulation command
		{
			/*
			rxphase= 0x800;//phase = 0
			rxduty1= 0;
			rxduty2= 0;
			*/
			EALLOW;
			EPwm2Regs.TZCLR.bit.OST =1;
			EPwm3Regs.TZCLR.bit.OST =1;
			EPwm4Regs.TZCLR.bit.OST =1;
			EPwm5Regs.TZCLR.bit.OST =1;
			EDIS;
			//(duty-0x2000)/2048-1
		}
		else if(rxdata == 0xf001)//stream primary current measurement
		{
			//set mailbox* pri current lp
			mailbox =(Uint16*)&meas.pri_current_lp;
		}

		else if(rxdata == 0xf002)//stream secondary current measurement
		{
			//set mailbox* sec current
			mailbox =(Uint16*)&meas.sec_current;
		}
		else if(rxdata == 0xf003)//stream secondary voltage measurement
		{
			//set mailbox* sec_voltage
			mailbox = (Uint16*)&meas.sec_voltage;
		}
		else if(rxdata == 0xf004)//stream test signal
		{
			//set mailbox* sec_voltage
			mailbox = (Uint16*)&meas.pri_voltage;
		}
		else if(rxdata == 0xf005)//stream test signal
		{
			//set mailbox* sec_voltage
			mailbox = (Uint16*)&meas.pri_current_1;
		}
		else if(rxdata == 0xf006)//stream test signal
		{
			//set mailbox* sec_voltage
			mailbox = (Uint16*)&meas.pri_current_2;
		}
		else if(rxdata == 0xf999)//stop modulation command
		{
			/*
			rxphase= 0x800;//phase = 0
			rxduty1= 0;
			rxduty2= 0;
			*/
			EALLOW;
			EPwm2Regs.TZFRC.bit.OST =1;
			EPwm3Regs.TZFRC.bit.OST =1;
			EPwm4Regs.TZFRC.bit.OST =1;
			EPwm5Regs.TZFRC.bit.OST =1;
			EDIS;
			//(duty-0x2000)/2048-1
		}
	}

	system_init = 1;

	// Clear INT flag for this timer
	EPwm1Regs.ETCLR.bit.INT = 1;
	// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

	GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;
}
