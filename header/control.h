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
void init_f_pid_control(struct f_pid_ctrl* pid_controller,//pointer to the controller "object"
						float kp,//p gain
						float ki,//I gain
						float kd,//D gain
						float kf,//D filter gain
						float ref,// reference
						Uint16* ctrl_measurement,//Uint16* pointer to measurement
						float(*control_function)(struct f_pid_ctrl*)//control function pointer
						);

float fpid_control(struct f_pid_ctrl*);
float fpid_vcontrol(struct f_pid_ctrl*);

extern struct measurements meas;

#endif /* INIT_CONTROL_H_ */
