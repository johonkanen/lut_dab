/*
 * ADC_conf.c
 *
 *  Created on: 28 Jun 2015
 *      Author: Jari
 */

#include "ADC_conf.h"
#include "datatypes.h"

void Adc_config(void)
{
	EALLOW;

	AdcRegs.ADCCTL1.bit.INTPULSEPOS	 = 1;	//ADCINT1 trips after AdcResults latch
	AdcRegs.INTSEL1N2.bit.INT1E      = 1;	//Enabled ADCINT1
	AdcRegs.INTSEL1N2.bit.INT1CONT   = 0;	//Disable ADCINT1 Continuous mode
	AdcRegs.INTSEL1N2.bit.INT1SEL	 = 4;	//setup EOC2 to trigger ADCINT1 to fire

//	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN0 = 0;
//	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN2 = 1;
//	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN4 = 0;
//	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN6 = 1;
//	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN8 = 1;

	AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;


	//dummy adc trigger
	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 0;	//set SOC0/1 channel select to ADCIN A0/B0
	AdcRegs.ADCSOC0CTL.bit.TRIGSEL 	= 0xf;	//set SOC0/1 - ePWM7, ADCSOCA
	AdcRegs.ADCSOC0CTL.bit.ACQPS 	= 6;	//set SOC0/1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC1CTL.bit.CHSEL 	= 0;	//set SOC0/1 channel select to ADCIN A0/B0
	AdcRegs.ADCSOC1CTL.bit.TRIGSEL 	= 0x11;	//set SOC0/1 - ePWM7, ADCSOCA
	AdcRegs.ADCSOC1CTL.bit.ACQPS 	= 6;	//set SOC0/1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	//HF Primary current 1
	AdcRegs.ADCSOC2CTL.bit.CHSEL 	= 9;	//set SOC2 channel select to ADCIN A1
	AdcRegs.ADCSOC2CTL.bit.TRIGSEL 	= 0xf;	//set SOC2/3 start trigger as EPWM1 SOCA
	AdcRegs.ADCSOC2CTL.bit.ACQPS 	= 6;	//set SOC2/3 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	//HF Primary current 2
	AdcRegs.ADCSOC3CTL.bit.CHSEL 	= 9;	//set SOC3 channel select to ADCIN A1
	AdcRegs.ADCSOC3CTL.bit.TRIGSEL 	= 0x10;	//set SOC4 start trigger as EPWM1 SOCB
	AdcRegs.ADCSOC3CTL.bit.ACQPS 	= 6;	//set SOC4 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	//Primary voltage
	AdcRegs.ADCSOC4CTL.bit.CHSEL 	= 2;	//set SOC6/7 channel select to ADCIN A2
	AdcRegs.ADCSOC4CTL.bit.TRIGSEL 	= 0x06;	//set SOC6/7 start trigger as EPWM1 SOCB
	AdcRegs.ADCSOC4CTL.bit.ACQPS 	= 6;	//set SOC6/7 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	// low pass primary current measurements
	AdcRegs.ADCSOC5CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC5CTL.bit.TRIGSEL 	= 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC5CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC6CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC6CTL.bit.TRIGSEL 	= 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC6CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC7CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC7CTL.bit.TRIGSEL 	= 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC7CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC8CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC8CTL.bit.TRIGSEL 	= 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC8CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC9CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC9CTL.bit.TRIGSEL 	= 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC9CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC10CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC10CTL.bit.TRIGSEL = 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC10CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC11CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC11CTL.bit.TRIGSEL = 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC11CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC12CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC12CTL.bit.TRIGSEL = 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC12CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC13CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC13CTL.bit.TRIGSEL = 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC13CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC14CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC14CTL.bit.TRIGSEL = 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC14CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC15CTL.bit.CHSEL 	= 9;	//set SOC5 channel select to ADCIN B1
	AdcRegs.ADCSOC15CTL.bit.TRIGSEL = 0xf;	//set SOC5 start trigger as EPWM6 SOCA
	AdcRegs.ADCSOC15CTL.bit.ACQPS 	= 6;	//set SOC5 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)


	EDIS;
}


void config_measurements(struct measurements* testi)
{
	testi->pri_current_lp 	= (Uint16*)&(AdcResult.ADCRESULT2);	//+0
	testi->pri_current_1	= (Uint16*)&(AdcResult.ADCRESULT3);	//+1
	testi->pri_current_2	= (Uint16*)&(AdcResult.ADCRESULT6); //+2
	testi->pri_voltage 		= (Uint16*)&(AdcResult.ADCRESULT4);	//+3
	// sec_measurements
	testi->sec_voltage		= (Uint16*)&(ext_ad.first_conv);	//+4
	testi->sec_current		= (Uint16*)&(ext_ad.second_conv);	//+5
}
