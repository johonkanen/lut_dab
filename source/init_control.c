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
	pid_controller->kp 			= kp;
	pid_controller->kd 			= kd;
	pid_controller->ki 			= ki;
	pid_controller->kf			= kf;
	//
	pid_controller->ref 		= ref;
	pid_controller->ref_mem 	= 0;
	pid_controller->scale		= 0;
	pid_controller->pid_control = ctrl_ptr;
	pid_controller->ctrl_meas   = ctrl_meas;
	pid_controller->imem		= 0;
	pid_controller->dmem		= 0;
}


float fpid_control(struct f_pid_ctrl* ctrl)
{
	float input_current_error= m_ref-(float)m_ctrl_meas*m_12bit_gain+ctrl->ref_mem;
	float output;

	output = m_kp*input_current_error + m_ki*m_imem;

	if(output>=.95)
	{
		m_imem -= (float)0.005*fabs(input_current_error);
		output = (float).95;
		return output;
	}
	else if(output<=-.95)
	{
		m_imem += (float)0.005*fabs(input_current_error);
		output = (float)-.95;
		return output;
	}
	else
	{
		m_imem +=  input_current_error;
		return output;
	}
}

float fpid_vcontrol(struct f_pid_ctrl* ctrl)
{
	float input_current_error= m_ref-(float)m_ctrl_meas*m_12bit_gain+ctrl->ref_mem;
	float output;

	output = m_kp*input_current_error + m_ki*m_imem;

	if(output>=.5)
	{
		m_imem -= (float)0.005*fabs(input_current_error);
		output = (float).5;
		return output;
	}
	else if(output<=-.5)
	{
		m_imem += (float)0.005*fabs(input_current_error);
		output = (float)-.5;
		return output;
	}
	else
	{
		m_imem +=  input_current_error;
		return output;
	}
}

