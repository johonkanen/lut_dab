/*
 * control_structures.h
 *
 *  Created on: 30.7.2015
 *      Author: c0341878
 */

#ifndef CONTROL_STRUCTURES_H_
#define CONTROL_STRUCTURES_H_

#include "DSP28x_Project.h"

typedef struct PI_control_gains
{						//mem offsets
	int32	P_gain;		//0
	int32	I_gain;		//2
	int32	mem;		//4
	int16	ref;		//6

//these are needed for reference rampup
	int16	rampup;		//7
	int16	stepsize;	//8
	int32	rampmem;	//10
}PI_controller;


void InitPi(PI_controller*,int32 P_gain, int32 I_gain, int16 ref);

#endif /* CONTROL_STRUCTURES_H_ */
