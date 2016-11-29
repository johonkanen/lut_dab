/*
 * Gpio_init.c
 *
 *  Created on: 29 Nov 2016
 *      Author: Jari
 */
#include "Gpio_init.h"

void init_lut_dab_GPIO(void)
{
	//insert proper initialization here!
	  EALLOW;
	   GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
	   GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;

	   GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;
	   GpioCtrlRegs.GPADIR.bit.GPIO30 = 1;

	   GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;
	   GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;

	   GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;
	   GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
	   EDIS;

}
