/*
 * Gpio_init.c
 *
 *  Created on: 29 Nov 2016
 *      Author: Jari
 */
#include "Gpio_init.h"

#define AUX_PWM1B_ENABLE

void init_lut_dab_GPIO(void)
{
	  EALLOW;

	  //24V_ok
	  GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;

	  //TRIP
	  GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;

	  //DAB enable
	  GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;

	  //connect
	  GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;

	  //EMERGENCYSTOP
	  //GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;
	  //GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;
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
	   GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on 	EPWM2A
	   GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as 	EPWM2A

	   GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on 	EPWM2B
	   GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as 	EPWM2B

	   GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up 	EPWM3A
	   GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as 	EPWM3A

	   GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on 	EPWM3B
	   GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as 	EPWM3B



	   EDIS;
}

void init_sec_HB_GPIO(void)
{
	   EALLOW;
	   GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up 	EPWM4A
	   GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;	 // Configure GPIO6 as 	EPWM4A

	   GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // // Disable pull-up 	EPWM4B
	   GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as 	EPWM4B

	   GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up 	EPWM5A
	   GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A

	   GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up 	EPWM5B
	   GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B

	   EDIS;
}

void init_lut_dab_ext_ad_GPIO(void)
{
	  EALLOW;
	  //ext ad enable
	  GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;
	  GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;

	  //ext ad read = output
	  GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;// configure as gpio
	  GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;

	  GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;// configure as gpio
	  GpioCtrlRegs.GPBDIR.bit.GPIO34 = 0;//set as input

	  //D0-D6
	  GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO22 = 0;

	  GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO23 = 0;

	  GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;

	  GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;

	  GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;

	  GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO27 = 0;

	  GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;

	  //D7
	  GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;


	  //D8-D9
	  GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO30 = 0;

	  GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;
	  GpioCtrlRegs.GPADIR.bit.GPIO31 = 0;

	  //D10-D11
	  GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
	  GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;

	  GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;
	  GpioCtrlRegs.GPBDIR.bit.GPIO33 = 0;



	  EDIS;


}

