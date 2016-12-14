/*
 * init_control.c
 *
 *  Created on: 14.12.2016
 *      Author: Asennus
 */



#include "init_control.h"


void init_f_pid_control(struct f_pid_ctrl* pid_controller, float kp, float ki, float kd, float kf, float ref,void (*ctrl_ptr)(struct f_pid_ctrl* pid_controller))
{
	pid_controller->kp 		= kp;
	pid_controller->kd 		= kd;
	pid_controller->ki 		= ki;
	pid_controller->kf			= kf;
	//
	pid_controller->ref 		= ref;
	pid_controller->ref_mem 	= 0;
	pid_controller->scale		= 0;
	pid_controller->pid_control = ctrl_ptr;
/*
	pid_controller->pid_control(pid_controller);
	execute_fpid_ctrl(pid_controller);
*/
}


void fpid_control(struct f_pid_ctrl* ctrl)
{

	ctrl->ref_mem = 17;
}
