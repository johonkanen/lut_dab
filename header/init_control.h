/*
 * init_control.h
 *
 *  Created on: 14.12.2016
 *      Author: Asennus
 */

#ifndef INIT_CONTROL_H_
#define INIT_CONTROL_H_
#include "datatypes.h"


void init_f_pid_control(struct f_pid_ctrl* pid_controller, float kp, float ki, float kd, float kf, float ref,void(*)(struct f_pid_ctrl*));

void fpid_control(struct f_pid_ctrl*);

#define execute_fpid_ctrl(X) (X->pid_control(X));


#endif /* INIT_CONTROL_H_ */
