/*
 * init_control.h
 *
 *  Created on: 14.12.2016
 *      Author: Jari
 */

#ifndef CONTROL_H_
#define CONTROL_H_
#include "datatypes.h"
#include "ctrl_macros.h"

//pid_controller is the object for the pid controller for a specific control task
void init_f_pid_control(struct f_pid_ctrl* pid_controller, float kp, float ki, float kd, float kf, float ref,Uint16* ctrl_measurement,float(*control_function)(struct f_pid_ctrl*));

float fpid_control(struct f_pid_ctrl*);

extern struct measurements meas;

#endif /* INIT_CONTROL_H_ */
