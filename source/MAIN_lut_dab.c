//###########################################################################
// Description:
//! 2kW dual active bridge control
//! control and init of dual active bridge converter
//
//	RX/TX connection(front side of PCB)
// ZZZ----------
//		1		|
//		  *  *	|
//		  *  *	|
//		  *  *	|
//		  *  *	|
//		  *  *	|
//		  * GND	|
//		 RX TX	|
//				N
//				N
//###########################################################################
#include "DSP28x_Project.h"     	// Device Headerfile and Examples Include File
//#include "SFO_V6.h"					// HRpwm calibration
#include "GLOBAL.h"					//pragmas and global variables
#include "interrupts.h"
#include "Gpio_init.h"
#include "ADC_conf.h"				//adc conf declarations
#include "SCIA_conf.h"				//UART declarations
#include "PWM__conf.h"				//pwm conf function declarations
//#include "CLA.h"					//cla header definitions
#include "uart_comm.h"
#include "datatypes.h"
#include "control.h"
#include "ctrl_macros.h"

#define LINK_TO_FLASH 1
#define INTERRUPT_TIMING 0

main(void)
{
#if (LINK_TO_FLASH==1)
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);
	InitFlash();
#endif

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
   InitSysCtrl();

// Step 2. Initialize GPIO:
   InitSciaGpio();
   InitScibGpio();
   init_lut_dab_GPIO();
   init_lut_dab_ext_ad_GPIO();

// Disable CPU interrupts
   DINT;

// Initialize the PIE control registers to their default state.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt

   InitPieVectTable();

   InitAdc();  // init the ADC
   AdcOffsetSelfCal();
   Adc_config();

   scia_fifo_init();	   // Initialize the SCI FIFO
   scia_echoback_init();  // Initialize SCI for echoback

   scib_fifo_init();	   // Initialize the SCI FIFO
   scib_echoback_init();  // Initialize SCI for echoback


   sig_prbs = 0x3;

   //init the measurement structure

   config_measurements(&meas);


   current_samples.a1 = (Uint16*)&(AdcResult.ADCRESULT2);
   current_samples.a2 = (Uint16*)&(AdcResult.ADCRESULT3);
   current_samples.a3 = (Uint16*)&(AdcResult.ADCRESULT5);
   current_samples.a4 = (Uint16*)&(AdcResult.ADCRESULT6);
   current_samples.a5 = (Uint16*)&(AdcResult.ADCRESULT7);
   current_samples.a6 = (Uint16*)&(AdcResult.ADCRESULT8);
   current_samples.a7 = (Uint16*)&(AdcResult.ADCRESULT9);
   current_samples.a8 = (Uint16*)&(AdcResult.ADCRESULT10);
   current_samples.a9 = (Uint16*)&(AdcResult.ADCRESULT11);
   current_samples.a10 = (Uint16*)&(AdcResult.ADCRESULT12);
   current_samples.a11 = (Uint16*)&(AdcResult.ADCRESULT13);
   current_samples.a12 = (Uint16*)&(AdcResult.ADCRESULT14);
   current_samples.a13 = (Uint16*)&(AdcResult.ADCRESULT15);

   meas.pri_current_lp = &mean_filt;

   mailbox = (Uint16*)&meas.pri_current_lp;

	//									   Kp,			Ki			Kd		Kf			ref				measurement					control
   init_f_pid_control(&voltage_ctrl, (float)2, (float)0.0002, (float)0, (float)0, (float)0.06, (Uint16*)&meas.sec_voltage, &fpid_vcontrol);

   init_f_pid_control(&d1_ctrl, (float)15, (float)0.05, (float)0, (float)0, (float)0.025, (Uint16*)&meas.pri_current_lp, &fpid_control);
   d1_ctrl.ref_mem = 1855*2.4420e-04; //primary current offset

   //d1_ctrl.ref_mem = 1350*2.4420e-04;//secondary current offset


   init_f_pid_control(&d2_ctrl, (float)4, (float)0.00, (float)0, (float)0, (float)0.3, (Uint16*)&meas.sec_voltage, &fpid_control);

   phase_reg.p1_phase = &EPwm2Regs.TBPHS.half.TBPHS;
   phase_reg.p2_phase = &EPwm3Regs.TBPHS.half.TBPHS;
   phase_reg.s1_phase = &EPwm4Regs.TBPHS.half.TBPHS;
   phase_reg.s2_phase = &EPwm5Regs.TBPHS.half.TBPHS;

   system_init = 0;

//   init_cla();
   // Configure PWM
   EALLOW;
       SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   PRI_DAB_PWM23_config();
   SEC_DAB_PWM45_config();

   AUX_PWM1_config();
   AUX_PWM6_config();
   AUX_PWM7_config();
   AUX_PWM8_config();

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // start all PWM channels synchronized
   EDIS;
   EPwm1Regs.TBCTL.bit.SWFSYNC = 1;

   init_AUX_PWM1_GPIO();



	EALLOW;  // allow write to protected register
	PieVectTable.EPWM1_INT = &PWM1_int;
	EDIS;    //


	while(1)
	{
		if (ScibRegs.SCIFFRX.bit.RXFFST == 2)
		{
			rxdata =  ScibRegs.SCIRXBUF.all<<8;
			rxdata |=  ScibRegs.SCIRXBUF.all;
			if (rxdata <0x1000)
			{
				rxduty1 = rxdata;
			}
			else if(rxdata < 0x2000)
			{
				rxduty2 = rxdata-0x1000;
			}
			else if(rxdata < 0x3000)
			{
				rxphase= rxdata-0x2000;

				//(duty-0x2000)/2048-1
			}
			else if(rxdata == 0xf999)
			{
				rxphase= 0x800;//phase = 0
				rxduty1= 0;
				rxduty2= 0;

				//(duty-0x2000)/2048-1
			}
			else if(rxdata == 0xf666)
			{
				break;
			}
		}
	}

	IER |= M_INT3;
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt

	while(system_init==0){}; //wait for first pass through interrupt

	DINT;

	init_pri_HB_GPIO();
	init_sec_HB_GPIO();
	EINT;

   for(;;){}
}
