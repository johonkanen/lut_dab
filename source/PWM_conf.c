/*																*
;* FNAME: PWM_conf.c					Jhonkanen 28 Jun 2015	*
;*																*
;* PWM configuration sourcefile           						*
;*																*
;*																*
;***************************************************************/



#include "PWM__conf.h"

volatile struct EPWM_REGS *ePWM[7] =
 			 {  &EPwm1Regs, &EPwm1Regs, &EPwm2Regs,	&EPwm3Regs,	&EPwm4Regs,	&EPwm5Regs,	&EPwm6Regs};

void AUX_PWM1_config(void)
{
	Uint16 channel=1, period = 444-1; //j=1

	(*ePWM[channel]).TBCTL.bit.PRDLD = TB_SHADOW;	        // set Immediate load
	(*ePWM[channel]).TBPRD = period;		                // PWM frequency = 1 / period
	(*ePWM[channel]).CMPA.half.CMPA = 4;          // set duty 18% initially
	(*ePWM[channel]).CMPA.half.CMPAHR = (1 << 8);         // initialize HRPWM extension
	(*ePWM[channel]).CMPB = 222;	                // set duty 50% initially
	(*ePWM[channel]).TBPHS.all = 0;
	(*ePWM[channel]).TBCTR = 0;

	//EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;

	(*ePWM[channel]).TBCTL.bit.CTRMODE = TB_COUNT_UP;
	(*ePWM[channel]).TBCTL.bit.PHSEN = TB_DISABLE;		//master module
	(*ePWM[channel]).TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
	(*ePWM[channel]).TBCTL.bit.HSPCLKDIV = TB_DIV1;
	(*ePWM[channel]).TBCTL.bit.CLKDIV = TB_DIV1;
	(*ePWM[channel]).TBCTL.bit.FREE_SOFT = 11;

	(*ePWM[channel]).CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	(*ePWM[channel]).CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	(*ePWM[channel]).CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	(*ePWM[channel]).CMPCTL.bit.SHDWBMODE = CC_SHADOW;


	(*ePWM[channel]).AQCTLA.bit.PRD = AQ_CLEAR;
	(*ePWM[channel]).AQCTLA.bit.CAU = AQ_SET;

	(*ePWM[channel]).AQCTLB.bit.PRD = AQ_CLEAR;
	(*ePWM[channel]).AQCTLB.bit.CBU = AQ_SET;

	EALLOW;
	(*ePWM[channel]).HRCNFG.all = 0x0;
	(*ePWM[channel]).HRCNFG.bit.EDGMODE = HR_REP;          // MEP control on rising edge
	(*ePWM[channel]).HRCNFG.bit.CTLMODE = HR_CMP;          // CMPAHR and TBPRDHR HR control
	(*ePWM[channel]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
	EDIS;

	(*ePWM[channel]).ETSEL.bit.SOCAEN = 0x1;
	(*ePWM[channel]).ETSEL.bit.SOCASEL = 0x6;       // Select SOC from from CPMB on upcount
	(*ePWM[channel]).ETPS.bit.SOCAPRD = 0x1;

// Assumes ePWM1 clock is already enabled in InitSysCtrl();
	(*ePWM[channel]).ETSEL.bit.INTSEL = ET_CTRU_CMPB;//ET_CTR_PRD;// ET_CTR_ZERO;      // Enable INT on Zero event
	(*ePWM[channel]).ETSEL.bit.INTEN = 1;   // Enable INT
	(*ePWM[channel]).ETPS.bit.INTPRD = ET_1ST;            // Generate INT on 1st event
}

void PRI_DAB_PWM34_config(void)
{

	Uint16 channel=2, period = 900-1; //j=1

	for (channel=3; channel<4+1; channel++)
	{

		(*ePWM[channel]).TBCTL.bit.PRDLD = TB_SHADOW;	        // set Immediate load
		(*ePWM[channel]).TBPRD = period;		                // PWM frequency = 1 / period
		(*ePWM[channel]).CMPA.half.CMPA = period<<1;          // set duty 18% initially
		(*ePWM[channel]).CMPA.half.CMPAHR = (1 << 8);         // initialize HRPWM extension
		(*ePWM[channel]).CMPB = period>>1;	                // set duty 50% initially
		(*ePWM[channel]).TBPHS.all = 0;
		(*ePWM[channel]).TBCTR = 0;

		(*ePWM[channel]).TBCTL.bit.CTRMODE = TB_COUNT_UP;//TB_COUNT_UP;
		(*ePWM[channel]).TBCTL.bit.PHSEN = TB_ENABLE;		//synch to epwm1
		(*ePWM[channel]).TBCTL.bit.SYNCOSEL = TB_SYNC_IN;	//TB_CTR_ZERO;
		(*ePWM[channel]).TBCTL.bit.HSPCLKDIV = TB_DIV1;
		(*ePWM[channel]).TBCTL.bit.CLKDIV = TB_DIV1;
		(*ePWM[channel]).TBCTL.bit.FREE_SOFT = 11;

		(*ePWM[channel]).CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		(*ePWM[channel]).CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
		(*ePWM[channel]).CMPCTL.bit.SHDWAMODE = CC_SHADOW;
		(*ePWM[channel]).CMPCTL.bit.SHDWBMODE = CC_SHADOW;

		if (channel & 1) //ch3
		{

			(*ePWM[channel]).AQCTLA.bit.ZRO = AQ_SET;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLA.bit.CAU = AQ_CLEAR;
			(*ePWM[channel]).AQCTLB.bit.ZRO = AQ_CLEAR;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLB.bit.CBU = AQ_SET;

			(*ePWM[channel]).DBCTL.bit.IN_MODE = DBA_ALL;
			(*ePWM[channel]).DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // Enable Dead-band module
			(*ePWM[channel]).DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active High Complementary (AHC)

			(*ePWM[channel]).DBRED = 23; // RED = 30 TBCLKs initially
			(*ePWM[channel]).DBFED = 23;

			EALLOW;
			(*ePWM[channel]).HRCNFG.all = 0x0;
			(*ePWM[channel]).HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on both edges
			(*ePWM[channel]).HRCNFG.bit.CTLMODE = HR_PHS;          // CMPAHR and TBPRDHR HR control
			(*ePWM[channel]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
			(*ePWM[channel]).HRCNFG.bit.SELOUTB  = 1; // load on CTR = 0 and CTR = TBPRD
			EDIS;


		}
		else			//ch 2
		{
			(*ePWM[channel]).AQCTLA.bit.ZRO = AQ_SET;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLA.bit.CAU = AQ_CLEAR;
			(*ePWM[channel]).AQCTLB.bit.ZRO = AQ_CLEAR;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLB.bit.CBU = AQ_SET;


			(*ePWM[channel]).DBCTL.bit.IN_MODE = DBA_ALL;

			(*ePWM[channel]).DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // Enable Dead-band module
			(*ePWM[channel]).DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active High Complementary (AHC)

			(*ePWM[channel]).DBRED = 23; // RED = 30 TBCLKs initially
			(*ePWM[channel]).DBFED = 23;


			EALLOW;
			(*ePWM[channel]).HRCNFG.all = 0x0;
			(*ePWM[channel]).HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on both edges
			(*ePWM[channel]).HRCNFG.bit.CTLMODE = HR_PHS;          // CMPAHR and TBPRDHR HR control
			(*ePWM[channel]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
			(*ePWM[channel]).HRCNFG.bit.SELOUTB  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
			EDIS;

			(*ePWM[channel]).ETSEL.bit.SOCAEN = 0x1;
			(*ePWM[channel]).ETSEL.bit.SOCASEL = 0x6;       // Select SOC from from CPMB on upcount
			(*ePWM[channel]).ETPS.bit.SOCAPRD = 0x1;
		}
	}
}


void SEC_DAB_PWM56_config(void)
{

	Uint16 channel=5, period = 900-1; //j=1

	for (channel=5; channel<6+1; channel++)
	{

		(*ePWM[channel]).TBCTL.bit.PRDLD = TB_SHADOW;	        // set Immediate load
		(*ePWM[channel]).TBPRD = period;		                // PWM frequency = 1 / period
		(*ePWM[channel]).CMPA.half.CMPA =  period<<1;          // set duty 18% initially
		(*ePWM[channel]).CMPA.half.CMPAHR = (1 << 8);         // initialize HRPWM extension
		(*ePWM[channel]).CMPB = period>>1;	                // set duty 50% initially
		(*ePWM[channel]).TBPHS.all = 0;
		(*ePWM[channel]).TBCTR = 0;

		(*ePWM[channel]).TBCTL.bit.CTRMODE = TB_COUNT_UP;//TB_COUNT_UP;
		(*ePWM[channel]).TBCTL.bit.PHSEN = TB_ENABLE;		//synch to epwm1
		(*ePWM[channel]).TBCTL.bit.SYNCOSEL = TB_SYNC_IN;	//TB_CTR_ZERO;
		(*ePWM[channel]).TBCTL.bit.HSPCLKDIV = TB_DIV1;
		(*ePWM[channel]).TBCTL.bit.CLKDIV = TB_DIV1;
		(*ePWM[channel]).TBCTL.bit.FREE_SOFT = 11;

		(*ePWM[channel]).CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		(*ePWM[channel]).CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
		(*ePWM[channel]).CMPCTL.bit.SHDWAMODE = CC_SHADOW;
		(*ePWM[channel]).CMPCTL.bit.SHDWBMODE = CC_SHADOW;

		if (channel & 1) //ch3
		{

			(*ePWM[channel]).AQCTLA.bit.ZRO = AQ_SET;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLA.bit.CAU = AQ_CLEAR;
			(*ePWM[channel]).AQCTLB.bit.ZRO = AQ_CLEAR;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLB.bit.CBU = AQ_SET;

			(*ePWM[channel]).DBCTL.bit.IN_MODE = DBA_ALL;
			(*ePWM[channel]).DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // Enable Dead-band module
			(*ePWM[channel]).DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active High Complementary (AHC)

			(*ePWM[channel]).DBRED = 23; // RED = 30 TBCLKs initially
			(*ePWM[channel]).DBFED = 23;

			EALLOW;
			(*ePWM[channel]).HRCNFG.all = 0x0;
			(*ePWM[channel]).HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on both edges
			(*ePWM[channel]).HRCNFG.bit.CTLMODE = HR_PHS;          // CMPAHR and TBPRDHR HR control
			(*ePWM[channel]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
			(*ePWM[channel]).HRCNFG.bit.SELOUTB  = 1; // load on CTR = 0 and CTR = TBPRD
			EDIS;


		}
		else			//ch 2
		{
			(*ePWM[channel]).AQCTLA.bit.ZRO = AQ_SET;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLA.bit.CAU = AQ_CLEAR;
			(*ePWM[channel]).AQCTLB.bit.ZRO = AQ_CLEAR;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLB.bit.CBU = AQ_SET;


			(*ePWM[channel]).DBCTL.bit.IN_MODE = DBA_ALL;

			(*ePWM[channel]).DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // Enable Dead-band module
			(*ePWM[channel]).DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active High Complementary (AHC)

			(*ePWM[channel]).DBRED = 5; // RED = 30 TBCLKs initially
			(*ePWM[channel]).DBFED = 5;


			EALLOW;
			(*ePWM[channel]).HRCNFG.all = 0x0;
			(*ePWM[channel]).HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on both edges
			(*ePWM[channel]).HRCNFG.bit.CTLMODE = HR_PHS;          // CMPAHR and TBPRDHR HR control
			(*ePWM[channel]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
			(*ePWM[channel]).HRCNFG.bit.SELOUTB  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
			EDIS;
			/*
			(*ePWM[channel]).ETSEL.bit.SOCAEN = 0x1;
			(*ePWM[channel]).ETSEL.bit.SOCASEL = 0x6;       // Select SOC from from CPMB on upcount
			(*ePWM[channel]).ETPS.bit.SOCAPRD = 0x1;
			*/
		}
	}
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

void init_DHB_PWM23_GPIO(void)
{
	   EALLOW;
	   GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO0 as EPWM1A

#ifdef CUK_PWM2B_ENABLE
	   GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO0 as EPWM1A
#endif

	   GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO0 as EPWM1A

#ifdef CUK_PWM3B_ENABLE
	   GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
	   GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO0 as EPWM1A
#endif


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

void pwm_ad_synch(void)
{
	//pwm2 = duty+1, pwm3 = duty-2*deadtime, ph3 = deadtime
	   (*ePWM[1]).CMPA.half.CMPA = 444-111;//31;
	   (*ePWM[1]).CMPB = 410;//31;

	   (*ePWM[2]).CMPA.half.CMPA = 222;//31; //high side gate
	   (*ePWM[2]).CMPB			 = 215;
	   (*ePWM[3]).CMPA.half.CMPA = 222;//30-6;

	   (*ePWM[4]).CMPA.half.CMPA = 8;//31;
	   (*ePWM[4]).CMPB 			 = 8;//31;

	   (*ePWM[5]).CMPA.half.CMPA = 80+14;//30-6;
	   (*ePWM[5]).CMPB 			 = 215;//31;
	   (*ePWM[6]).CMPA.half.CMPA = 80;

	// PHASE CONTROL OF PWM CHANNELS

	   (*ePWM[2]).TBPHS.half.TBPHS = 111; //0
	   (*ePWM[3]).TBPHS.half.TBPHS = 111; //0+3
	   (*ePWM[4]).TBPHS.half.TBPHS = 0;
	   (*ePWM[5]).TBPHS.half.TBPHS = 40+7; //148
	   (*ePWM[6]).TBPHS.half.TBPHS = 40; //148-7
}
