//###########################################################################
// Description:
//! 2kW dual active bridge control
//! control and init of dual active bridge converter
//
//	RX/TX connection(front side of PCB)
// zzzz----------
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
#include "Gpio_init.h"
#include "ADC_conf.h"				//adc conf declarations
#include "SCIA_conf.h"				//UART declarations
#include "PWM__conf.h"				//pwm conf function declarations
//#include "CLA.h"					//cla header definitions
#include "GLOBAL.h"					//pragmas and global variables
#include "control_structures.h"		//controller structures
#include "controller_objects.h"		//controllers are defined here
#include "uart_comm.h"

/**** HW COMPILE ENABLE *****/

//	TODO


/****************************/
#define LINK_TO_FLASH 0
#define INTERRUPT_TIMING 0

extern volatile struct EPWM_REGS *ePWM[7];
extern Uint16 cuk_rampup;
extern Uint16 cuk_ready;

extern void init_cla(void);
/**********************************/
void rampup(void);
void rampup_init(void);
//extern void config_measurements(measurements*);

Uint16 ref1,ref2,step,ramptest;
Uint32	rampmem,scale;

#define START_VAL 	3000
#define END_VAL		4000
//#define SCALE		37836 /* == 1/56757*2^31 == 450ms rampup */
#define SCALE		15891 /* 1s rampup*/
//#define SCALE		317816

int32 filtergains[] = {0x15B91CE5, 0x95B91CE5,0};

void config_measurements(struct measurements*);
/*********************************/
//int16* mailbox;

Uint16* mailbox;
Uint16  package;
Uint16 ad_timing_complete;
Uint16 ad_timing_disable;
Uint16 memory;

Uint16* aux_meas;
Uint16* pfc_meas;
Uint16* dhb_meas;
Uint16* heater_meas;


Uint16 ref1, ref2,step;
Uint32 rampmem;
Uint16 ramptest;

struct ext_ad_result
{
	Uint16	first_conv;
	Uint16	second_conv;
}ext_ad;

struct storage sig_prbs;
struct measurements meas;
//Uint16 memtest[170];

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsLoadSize;


float testitesti = 7.56;

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

   config_measurements(&meas);

   aux_meas = meas.aux_voltage;
   pfc_meas = meas.dc_link_voltage;
   dhb_meas = meas.dhb_current;
   heater_meas = meas.heater_voltage;

//   init_cla();
   // Configure PWM
   EALLOW;
       SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   PRI_DAB_PWM34_config();
   SEC_DAB_PWM56_config();

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // start all PWM channels synchronized
   EDIS;
   EPwm1Regs.TBCTL.bit.SWFSYNC = 1;

/******************************************************************
*
*	setup AD conversions and PWM channels
*
*******************************************************************/
   pwm_ad_synch();

   startup_voltage 		= 1870; //=80V meas gain is 1/553 and filter gain is 3.12
   voltage_has_risen 	= 0;
   ad_timing_complete	= 0;
   ad_timing_disable 	= 1;

#if (INTERRUPT_TIMING==1)
   ad_timing_disable 	= 0;
#endif

   EALLOW;
   GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
   GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
   EDIS;


#if (INTERRUPT_TIMING==1)
   init_AUX_PWM1_GPIO();
   init_DHB_PWM23_GPIO();
   init_PFC_PWM4_GPIO();
   init_HEATER_PWM56_GPIO();
#endif
/******************************************************************
*
*	ad conversion timing
*
*******************************************************************/

   // timing adc soc events
   EALLOW;
   PieVectTable.EPWM1_INT = &AUX_timing;
   EDIS;
   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
   IER = M_INT3;
   EINT;          						// Enable Global interrupt INTM
   ERTM;          						// Enable Global realtime interrupt DBGM


//   do //Nothing
   //{}while(ad_timing_complete < 10);

/******************************************************************
*
*	controller initializations found in .txt files
*
*******************************************************************/

#include "config_control_gains.txt"
   // MAP startup filter to pwm1 isr
   EALLOW;
   PieVectTable.EPWM1_INT = &startup_filter;
   EDIS;

   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

   IER = M_INT3;
   EINT;          						// Enable Global interrupt INTM
   ERTM;          						// Enable Global realtime interrupt DBGM

   // zero the sci memory pointers
   sig_prbs.rdptr=0x7fff;
   sig_prbs.wrptr=0;
   sig_prbs.mem	= 0;

   cuk_rampup 	= 40;
   cuk_ready 	= 0;
/*
   do //Nothing
   {}while(voltage_has_risen < 1);
*/

   /*************************************************************
   *															*
   *	initialization ready, disable charge resistor			*
   *															*
   *************************************************************/
   init_lut_dab_GPIO();
   init_pri_HB_GPIO();
   init_pri_HB_GPIO();

   //toggle relay
   GpioDataRegs.GPASET.bit.GPIO24 = 1;
   //GpioDataRegs.GPASET.bit.GPIO31 = 1; //set data read to dhb ad converter

   init_msg_source();

   scale = SCALE;

   rampup_init();

   DINT;
// After init re-map the proper interrupt
   EALLOW;  // This is needed to write to EALLOW protected register
//   PieVectTable.EPWM1_INT = &AUX_ctrl;
   EDIS;    // This is needed to disable write to EALLOW protected registers
   GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
   EINT;

// Loop forever and enjoy :)
   for(;;){}
}


void config_measurements(struct measurements* testi)
{
	testi->aux_voltage = 		(Uint16*)&(AdcResult.ADCRESULT8);	//+0
	//
	testi->dc_link_voltage = 	(Uint16*)&(AdcResult.ADCRESULT9); 	//+0
	testi->pfc_current1 = 		(Uint16*)&(AdcResult.ADCRESULT2);	//+1
	testi->pfc_current2 = 		(Uint16*)&(AdcResult.ADCRESULT3);	//+2
	testi->mains_voltage = 		(Uint16*)&(AdcResult.ADCRESULT3);	//+3
	//
	testi->dhb_current = 		(Uint16*)&(AdcResult.ADCRESULT6); 	//+0
	testi->dhb_output_voltage = (Uint16*)&(ext_ad.first_conv);		//+1
	testi->dhb_cap_voltage =	(Uint16*)&(AdcResult.ADCRESULT7);	//+2
	//
	testi->heater_voltage = 	(Uint16*)&(ext_ad.second_conv);		//+0
	testi->heater_current = 	(Uint16*)&(AdcResult.ADCRESULT4); 	//+1
}



