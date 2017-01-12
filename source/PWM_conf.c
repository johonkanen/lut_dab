/*																*
;* FNAME: PWM_conf.c					Jhonkanen 28 Jun 2015	*
;*																*
;* PWM configuration sourcefile           						*
;*																*
;*																*
;***************************************************************/



#include "PWM__conf.h"

volatile struct EPWM_REGS *ePWM[9] =
 			 {  &EPwm1Regs, &EPwm1Regs, &EPwm2Regs,	&EPwm3Regs,	&EPwm4Regs,	&EPwm5Regs,	&EPwm6Regs,	&EPwm7Regs,	&EPwm8Regs};
// DTO == 0 => no deadtime
#define DT0 1
#define DAB_V1_LEG2_POL 1


void AUX_PWM1_config(void)
{
	Uint16 channel=1, period = 900-1; //j=1

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


	(*ePWM[channel]).AQCTLA.bit.PRD = AQ_SET;
	(*ePWM[channel]).AQCTLA.bit.CAU = AQ_CLEAR;

	(*ePWM[channel]).AQCTLB.bit.PRD = AQ_CLEAR;
	(*ePWM[channel]).AQCTLB.bit.CBU = AQ_SET;

	EALLOW;
	(*ePWM[channel]).HRCNFG.all = 0x0;
	(*ePWM[channel]).HRCNFG.bit.EDGMODE = HR_REP;          // MEP control on rising edge
	(*ePWM[channel]).HRCNFG.bit.CTLMODE = HR_CMP;          // CMPAHR and TBPRDHR HR control
	(*ePWM[channel]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
	EDIS;

	(*ePWM[channel]).ETSEL.bit.SOCAEN = 0x1;
	(*ePWM[channel]).ETSEL.bit.SOCASEL =ET_CTRU_CMPA;       // Select SOC from from CPMB on upcount
	(*ePWM[channel]).ETPS.bit.SOCAPRD = 0x1;

	(*ePWM[channel]).ETSEL.bit.SOCBEN = 0x1;
	(*ePWM[channel]).ETSEL.bit.SOCBSEL = ET_CTRU_CMPB;       // Select SOC from from CPMB on upcount
	(*ePWM[channel]).ETPS.bit.SOCBPRD = 0x1;

// Assumes ePWM1 clock is already enabled in InitSysCtrl();
	(*ePWM[channel]).ETSEL.bit.INTSEL = ET_CTR_ZERO;//ET_CTR_PRD;// ET_CTR_ZERO;      // Enable INT on Zero event
	(*ePWM[channel]).ETSEL.bit.INTEN = 1;   // Enable INT
	(*ePWM[channel]).ETPS.bit.INTPRD = ET_1ST;            // Generate INT on 1st event
}

void PRI_DAB_PWM23_config(void)
{

	Uint16 channel=2, period = 900-1; //j=1

	for (channel=2; channel<=3; channel++)
	{

		(*ePWM[channel]).TBCTL.bit.PRDLD = TB_SHADOW;	        // set Immediate load
		(*ePWM[channel]).TBPRD = period;		                // PWM frequency = 1 / period
		(*ePWM[channel]).CMPA.half.CMPA = period>>1;          // set duty 18% initially
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
		(*ePWM[channel]).CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;
		(*ePWM[channel]).CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;

		if (channel & 1) //ch3
		{
#if(DAB_V1_LEG2_POL==1)
			//this reverses leg2 voltage polarity. Needed for fixing bug with lut dab v1
			(*ePWM[channel]).AQCTLA.bit.ZRO = AQ_SET;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLA.bit.CAU = AQ_CLEAR;
			(*ePWM[channel]).AQCTLB.bit.ZRO = AQ_CLEAR;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLB.bit.CBU = AQ_SET;
#else
			//this is the normal leg 2 polarity
			(*ePWM[channel]).AQCTLA.bit.ZRO = AQ_CLEAR;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLA.bit.CAU = AQ_SET;
			(*ePWM[channel]).AQCTLB.bit.ZRO = AQ_SET;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLB.bit.CBU = AQ_CLEAR;
#endif


			(*ePWM[channel]).DBCTL.bit.IN_MODE = DBA_ALL;

			(*ePWM[channel]).DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // Enable Dead-band module
			(*ePWM[channel]).DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active High Complementary (AHC)

			(*ePWM[channel]).DBRED = 24*DT0; // RED = 30 TBCLKs initially
			(*ePWM[channel]).DBFED = 24*DT0;

/*
			EALLOW;
			(*ePWM[channel]).HRCNFG.all = 0x0;
			(*ePWM[channel]).HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on both edges
			(*ePWM[channel]).HRCNFG.bit.CTLMODE = HR_PHS;          // CMPAHR and TBPRDHR HR control
			(*ePWM[channel]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
			(*ePWM[channel]).HRCNFG.bit.SELOUTB  = 1; // load on CTR = 0 and CTR = TBPRD
			EDIS;
*/

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

			(*ePWM[channel]).DBRED = 24*DT0;
			(*ePWM[channel]).DBFED = 24*DT0;
/*
			EALLOW;
			(*ePWM[channel]).HRCNFG.all = 0x0;
			(*ePWM[channel]).HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on both edges
			(*ePWM[channel]).HRCNFG.bit.CTLMODE = HR_PHS;          // CMPAHR and TBPRDHR HR control
			(*ePWM[channel]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
			(*ePWM[channel]).HRCNFG.bit.SELOUTB  = 1; // load on CTR = 0 and CTR = TBPRD
			EDIS;
*/
//			(*ePWM[channel]).ETSEL.bit.SOCAEN = 0x1;
//			(*ePWM[channel]).ETSEL.bit.SOCASEL = 0x6;       // Select SOC from from CPMB on upcount
//			(*ePWM[channel]).ETPS.bit.SOCAPRD = 0x1;
		}
	}
}


void SEC_DAB_PWM45_config(void)
{

	Uint16 channel=4, period = 900-1; //j=1

	for (channel=4; channel<=5; channel++)
	{

		(*ePWM[channel]).TBCTL.bit.PRDLD = TB_SHADOW;	        // set Immediate load
		(*ePWM[channel]).TBPRD = period;		                // PWM frequency = 1 / period
		(*ePWM[channel]).CMPA.half.CMPA =  period>>1;          // set duty 18% initially
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
		(*ePWM[channel]).CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;
		(*ePWM[channel]).CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;

		if (channel & 1) //ch5
		{
			(*ePWM[channel]).AQCTLA.bit.ZRO = AQ_CLEAR;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLA.bit.CAU = AQ_SET;
			(*ePWM[channel]).AQCTLB.bit.ZRO = AQ_SET;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLB.bit.CBU = AQ_CLEAR;


			(*ePWM[channel]).DBCTL.bit.IN_MODE = DBA_ALL;

			(*ePWM[channel]).DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // Enable Dead-band module
			(*ePWM[channel]).DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active High Complementary (AHC)

			(*ePWM[channel]).DBRED = 24*DT0; // RED = 30 TBCLKs initially
			(*ePWM[channel]).DBFED = 24*DT0;

/*
			EALLOW;
			(*ePWM[channel]).HRCNFG.all = 0x0;
			(*ePWM[channel]).HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on both edges
			(*ePWM[channel]).HRCNFG.bit.CTLMODE = HR_PHS;          // CMPAHR and TBPRDHR HR control
			(*ePWM[channel]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
			(*ePWM[channel]).HRCNFG.bit.SELOUTB  = 1; // load on CTR = 0 and CTR = TBPRD
			EDIS;
*/

		}
		else			//ch 4
		{

			(*ePWM[channel]).AQCTLA.bit.ZRO = AQ_SET;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLA.bit.CAU = AQ_CLEAR;
			(*ePWM[channel]).AQCTLB.bit.ZRO = AQ_CLEAR;          // PWM toggle high/low
			(*ePWM[channel]).AQCTLB.bit.CBU = AQ_SET;

			(*ePWM[channel]).DBCTL.bit.IN_MODE = DBA_ALL;
			(*ePWM[channel]).DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; // Enable Dead-band module
			(*ePWM[channel]).DBCTL.bit.POLSEL = DB_ACTV_HIC; // Active High Complementary (AHC)

			(*ePWM[channel]).DBRED = 24*DT0; // RED = 30 TBCLKs initially
			(*ePWM[channel]).DBFED = 24*DT0;
/*
			EALLOW;
			(*ePWM[channel]).HRCNFG.all = 0x0;
			(*ePWM[channel]).HRCNFG.bit.EDGMODE = HR_BEP;          // MEP control on both edges
			(*ePWM[channel]).HRCNFG.bit.CTLMODE = HR_PHS;          // CMPAHR and TBPRDHR HR control
			(*ePWM[channel]).HRCNFG.bit.HRLOAD  = HR_CTR_ZERO; // load on CTR = 0 and CTR = TBPRD
			(*ePWM[channel]).HRCNFG.bit.SELOUTB  = 1; // load on CTR = 0 and CTR = TBPRD
			EDIS;
*/

			/*
			(*ePWM[channel]).ETSEL.bit.SOCAEN = 0x1;
			(*ePWM[channel]).ETSEL.bit.SOCASEL = 0x6;       // Select SOC from from CPMB on upcount
			(*ePWM[channel]).ETPS.bit.SOCAPRD = 0x1;
			*/
		}
	}
}

void AUX_PWM7_config(void)
{
	Uint16 channel=7, period = 900-1; //j=1

	(*ePWM[channel]).TBCTL.bit.PRDLD = TB_SHADOW;	        // set Immediate load
	(*ePWM[channel]).TBPRD = period;		                // PWM frequency = 1 / period
	(*ePWM[channel]).CMPA.half.CMPA = 4;          // set duty 18% initially
	(*ePWM[channel]).CMPA.half.CMPAHR = (1 << 8);         // initialize HRPWM extension
	(*ePWM[channel]).CMPB = 222;	                // set duty 50% initially
	(*ePWM[channel]).TBPHS.all = 0;
	(*ePWM[channel]).TBCTR = 0;

	//EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;

	(*ePWM[channel]).TBCTL.bit.CTRMODE = TB_COUNT_UP;
	(*ePWM[channel]).TBCTL.bit.PHSEN = TB_ENABLE;		//master module
	(*ePWM[channel]).TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
	(*ePWM[channel]).TBCTL.bit.HSPCLKDIV = TB_DIV1;
	(*ePWM[channel]).TBCTL.bit.CLKDIV = TB_DIV1;
	(*ePWM[channel]).TBCTL.bit.FREE_SOFT = 11;

	(*ePWM[channel]).CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	(*ePWM[channel]).CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	(*ePWM[channel]).CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;
	(*ePWM[channel]).CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;

	(*ePWM[channel]).ETSEL.bit.SOCAEN = 0x1;
	(*ePWM[channel]).ETSEL.bit.SOCASEL =ET_CTR_ZERO;       // Select SOC from from CPMB on upcount
	(*ePWM[channel]).ETPS.bit.SOCAPRD = 0x1;

	// Assumes ePWM1 clock is already enabled in InitSysCtrl();
////	(*ePWM[channel]).ETSEL.bit.INTSEL = ET_CTR_ZERO;//ET_CTR_PRD;// ET_CTR_ZERO;      // Enable INT on Zero event
//	(*ePWM[channel]).ETSEL.bit.INTEN = 1;   // Enable INT
//	(*ePWM[channel]).ETPS.bit.INTPRD = ET_1ST;            // Generate INT on 1st event
}

void AUX_PWM8_config(void)
{
	Uint16 channel=8, period = 900-1; //j=1

	(*ePWM[channel]).TBCTL.bit.PRDLD = TB_SHADOW;	        // set Immediate load
	(*ePWM[channel]).TBPRD = period;		                // PWM frequency = 1 / period
	(*ePWM[channel]).CMPA.half.CMPA = 4;          // set duty 18% initially
	(*ePWM[channel]).CMPA.half.CMPAHR = (1 << 8);         // initialize HRPWM extension
	(*ePWM[channel]).CMPB = 222;	                // set duty 50% initially
	(*ePWM[channel]).TBPHS.all = 0;
	(*ePWM[channel]).TBCTR = 0;

	//EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;

	(*ePWM[channel]).TBCTL.bit.CTRMODE = TB_COUNT_UP;
	(*ePWM[channel]).TBCTL.bit.PHSEN = TB_ENABLE;		//master module
	(*ePWM[channel]).TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
	(*ePWM[channel]).TBCTL.bit.HSPCLKDIV = TB_DIV1;
	(*ePWM[channel]).TBCTL.bit.CLKDIV = TB_DIV1;
	(*ePWM[channel]).TBCTL.bit.FREE_SOFT = 11;

	(*ePWM[channel]).CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	(*ePWM[channel]).CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	(*ePWM[channel]).CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;
	(*ePWM[channel]).CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;

	(*ePWM[channel]).ETSEL.bit.SOCAEN = 0x1;
	(*ePWM[channel]).ETSEL.bit.SOCASEL =ET_CTRU_CMPA;       // Select SOC from from CPMB on upcount
	(*ePWM[channel]).ETPS.bit.SOCAPRD = 0x1;

	// Assumes ePWM1 clock is already enabled in InitSysCtrl();
////	(*ePWM[channel]).ETSEL.bit.INTSEL = ET_CTR_ZERO;//ET_CTR_PRD;// ET_CTR_ZERO;      // Enable INT on Zero event
//	(*ePWM[channel]).ETSEL.bit.INTEN = 1;   // Enable INT
//	(*ePWM[channel]).ETPS.bit.INTPRD = ET_1ST;            // Generate INT on 1st event
}
