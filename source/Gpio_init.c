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


void init_AUX_PWM1_GPIO(void)
{
	   EALLOW;

	   GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A

#ifdef AUX_PWM1B_ENABLE
	   GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
#endif
	   EDIS;
}

void init_pri_HB_GPIO(void)
{
	   EALLOW;
	   GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO0 as EPWM1A

	   GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO0 as EPWM1A

	   GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO0 as EPWM1A

	   GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO0 as EPWM1A


	   EDIS;
}

void init_sec_HB_GPIO(void)
{
	   EALLOW;
	   GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO0 as EPWM1A

	   GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO0 as EPWM1A

	   GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO0 as EPWM1A

	   GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO0 as EPWM1A


	   EDIS;
}

void init_PFC_PWM4_GPIO(void)
{
	   EALLOW;

	   GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO0 (EPWM4A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO0 as EPWM4A

#ifdef PFC_PWM4B_ENABLE
	   GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO1 (EPWM4B)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO1 as EPWM4B
#endif
	   EDIS;
}

void init_HEATER_PWM56_GPIO(void)
{
	   EALLOW;

	   GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A

	   GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;    // Disable pull-up on GPIO10 (EPWM6A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A

#ifdef HEATER_PWM5B_ENABLE
	   GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO8 as EPWM5A
#endif

#ifdef HEATER_PWM6B_ENABLE
	   GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;    // Disable pull-up on GPIO10 (EPWM6A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO10 as EPWM6A
#endif
	   EDIS;
}

void enable_DHB_gate_drivers(void)
{
	   EALLOW;
	   GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;  // GPIO6 = GPIO6
	   GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;   // GPIO6 = output

	   GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;  // GPIO6 = GPIO6
	   GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;   // GPIO6 = output

	   GpioDataRegs.GPASET.bit.GPIO3 = 1;   // Load output latch
	   GpioDataRegs.GPASET.bit.GPIO5 = 1;   // Load output latch
	   EDIS;

}
