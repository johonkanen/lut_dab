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

#define LINK_TO_FLASH 0
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

   //init the measurement structure

   config_measurements(&meas);

	//#include "init_control_gains.txt"
   init_f_pid_control(&voltage_ctrl, (float)4, (float)0.00, (float)0, (float)0, (float)0.3, (Uint16*)&meas.pri_current_1, &fpid_control);
   init_f_pid_control(&d1_ctrl, (float)4, (float)0.00, (float)0, (float)0, (float)0.3, (Uint16*)&meas.sec_current, &fpid_control);
   init_f_pid_control(&d2_ctrl, (float)4, (float)0.00, (float)0, (float)0, (float)0.3, (Uint16*)&meas.sec_voltage, &fpid_control);

   phase_reg.p1_phase = &EPwm2Regs.TBPHS.half.TBPHS;
   phase_reg.p2_phase = &EPwm3Regs.TBPHS.half.TBPHS;
   phase_reg.s1_phase = &EPwm4Regs.TBPHS.half.TBPHS;
   phase_reg.s2_phase = &EPwm5Regs.TBPHS.half.TBPHS;

//   init_cla();
   // Configure PWM
   EALLOW;
       SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   PRI_DAB_PWM23_config();
   SEC_DAB_PWM45_config();

   AUX_PWM1_config();
   AUX_PWM7_config();
   AUX_PWM8_config();

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // start all PWM channels synchronized
   EDIS;
   EPwm1Regs.TBCTL.bit.SWFSYNC = 1;

   init_AUX_PWM1_GPIO();

   init_pri_HB_GPIO();
   init_sec_HB_GPIO();

	EALLOW;  // allow write to protected register
	PieVectTable.EPWM1_INT = &PWM1_int;
	EDIS;    //

	// Enable CPU INT3 which is connected to EPWM1-6 INT:
	IER |= M_INT3;
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt


   for(;;){}
}
