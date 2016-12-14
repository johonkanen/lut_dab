/*
 * init_control.c
 *
 *  Created on: 14.12.2016
 *      Author: Jari
 */



#include "control.h"


void init_f_pid_control(struct f_pid_ctrl* pid_controller, float kp, float ki, float kd, float kf, float ref,struct measurements* ctrl_meas,float (*ctrl_ptr)(struct f_pid_ctrl* pid_controller))
{
	pid_controller->kp 			= kp*m_12bit_gain;
	pid_controller->kd 			= kd*m_12bit_gain;
	pid_controller->ki 			= ki*m_12bit_gain;
	pid_controller->kf			= kf*m_12bit_gain;
	//
	pid_controller->ref 		= ref;
	pid_controller->ref_mem 	= 0;
	pid_controller->scale		= 0;
	pid_controller->pid_control = ctrl_ptr;
	pid_controller->ctrl_meas   = ctrl_meas;
}


float fpid_control(struct f_pid_ctrl* ctrl)
{
	ctrl->ref_mem = 17;

	float input_current_error= (float)*(meas.pri_current_1)-m_ref;

	float output;

	output = m_kp*input_current_error + m_kp*input_current_error;


	return output;
}
