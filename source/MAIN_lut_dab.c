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

extern volatile struct EPWM_REGS *ePWM[9];
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

   AUX_PWM1_config();
   AUX_PWM7_config();
   AUX_PWM8_config();




   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // start all PWM channels synchronized
   EDIS;
   EPwm1Regs.TBCTL.bit.SWFSYNC = 1;

   init_lut_dab_GPIO();
   init_pri_HB_GPIO();
   init_sec_HB_GPIO();


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



