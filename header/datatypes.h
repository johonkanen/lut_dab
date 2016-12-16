/*
 * datatypes.h
 *
 *  Created on: 8.12.2016
 *      Author: Jari
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include "DSP28x_Project.h"

struct measurements
{
	//pri measurements
	Uint16* pri_current_lp;
	Uint16* pri_current_1;
	Uint16* pri_current_2;
	Uint16* pri_voltage;
	//sec measurements
	Uint16* sec_current;
	Uint16* sec_voltage;
};

struct storage
{
	Uint16 rdptr;
	Uint16 wrptr;
	Uint32* mem;
};

struct ext_ad_result
{
	Uint16	first_conv;
	Uint16	second_conv;
};

struct f_pid_ctrl
{
	float ki;
	float kp;
	float kd;
	float kf;
	float imem;
	float dmem;
	//reference and reference
	float ref;
	float ref_mem;
	float scale;
	Uint16* ctrl_meas; //pointer to a measurement struct
	float (*pid_control)(struct f_pid_ctrl*);
};


#endif /* DATATYPES_H_ */
