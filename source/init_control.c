/*
 * init_control.c
 *
 *  Created on: 14.12.2016
 *      Author: Jari
 */



#include "control.h"
#pragma CODE_SECTION(fpid_control, "ramfuncs");

void init_f_pid_control(struct f_pid_ctrl* pid_controller, float kp, float ki, float kd, float kf, float ref,Uint16* ctrl_meas,float (*ctrl_ptr)(struct f_pid_ctrl* pid_controller))
{
	pid_controller->kp 			= kp*m_12bit_gain;
	pid_controller->kd 			= kd*m_12bit_gain;
	pid_controller->ki 			= ki*m_12bit_gain;
	pid_controller->kf			= kf*m_12bit_gain;
	//
	pid_controller->ref 		= ref/m_12bit_gain;
	pid_controller->ref_mem 	= 0;
	pid_controller->scale		= 0;
	pid_controller->pid_control = ctrl_ptr;
	pid_controller->ctrl_meas   = ctrl_meas;
	pid_controller->imem		=0;
	pid_controller->dmem		=0;
}


float fpid_control(struct f_pid_ctrl* ctrl)
{
	float input_current_error= m_ref-(float)m_ctrl_meas;
	float output;

	output = m_kp*input_current_error + m_ki*m_imem;

	if(output>=1)
	{
		m_imem -= (float)0.00001*fabs(input_current_error);
		output = (float)1;
		return output;
	}
	else if(output<=-1)
	{
		m_imem += (float)0.00001*fabs(input_current_error);
		output = (float)-1;
		return output;
	}
	else
	{
		m_imem +=  input_current_error;
		return output;
	}
}
