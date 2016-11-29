/*
 * control_structures.c
 *
 *  Created on: 30.7.2015
 *      Author: c0341878
 */

#include "control_structures.h"

void InitPi(PI_controller* ctrl,int32 P_gain, int32 I_gain, int16 ref)
{
	//set gains and init memory to zero
	ctrl->P_gain=P_gain;
	ctrl->I_gain=I_gain;
	ctrl->mem	=0;
	ctrl->ref	=ref;
	//zero the rampup variables
	ctrl->rampup = 0;
	ctrl->stepsize = 0;
	ctrl->rampmem = 0;

}
