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

Uint16 cnt_jee=0;

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

__interrupt void PWM1_int(void);


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

   //init the measurement structure

   config_measurements(&meas);

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

__interrupt void PWM1_int(void)
{
	float measgain = (float)0.109890109890110;
	float juttu=0;
	int16 mail;


	cnt_jee--;

	juttu = measgain * (*meas.pri_current_1);
	mail = (int16)juttu-222;

	/*

		include control code here

	*/

	EPwm4Regs.TBPHS.half.TBPHS = (int16)(juttu-222)+445;
	EPwm5Regs.TBPHS.half.TBPHS = (int16)(juttu-222)+445;

	    if (SciaRegs.SCIFFTX.bit.TXFFST == 0)
	    {
			SciaRegs.SCITXBUF = mail;
			SciaRegs.SCITXBUF = mail>>8;
	    }


	   // Clear INT flag for this timer
	   EPwm1Regs.ETCLR.bit.INT = 1;
	   // Acknowledge this interrupt to receive more interrupts from group 3
	   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

void config_measurements(struct measurements* testi)
{
	testi->pri_current_lp 	= (Uint16*)&(AdcResult.ADCRESULT4);	//+1
	testi->pri_current_1	= (Uint16*)&(AdcResult.ADCRESULT2);	//+0
	testi->pri_current_2	= (Uint16*)&(AdcResult.ADCRESULT3); 	//+0
	testi->pri_voltage 		= (Uint16*)&(AdcResult.ADCRESULT5);	//+2
	// sec_measurements
	testi->sec_current		= (Uint16*)&(ext_ad.first_conv);
	testi->sec_voltage		= (Uint16*)&(ext_ad.second_conv);
}
