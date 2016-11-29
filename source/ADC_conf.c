/*
 * ADC_conf.c
 *
 *  Created on: 28 Jun 2015
 *      Author: Jari
 */

#include "ADC_conf.h"

void Adc_config(void)
{
	EALLOW;

	AdcRegs.ADCCTL1.bit.INTPULSEPOS	 = 1;	//ADCINT1 trips after AdcResults latch
	AdcRegs.INTSEL1N2.bit.INT1E      = 1;	//Enabled ADCINT1
	AdcRegs.INTSEL1N2.bit.INT1CONT   = 0;	//Disable ADCINT1 Continuous mode
	AdcRegs.INTSEL1N2.bit.INT1SEL	 = 4;	//setup EOC2 to trigger ADCINT1 to fire

	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN0 = 1;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN2 = 1;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN4 = 0;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN6 = 1;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN8 = 1;
	//AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;


	//dummy adc
	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 7;	//set SOC0/1 channel select to ADCIN A4/B4
	AdcRegs.ADCSOC0CTL.bit.TRIGSEL 	= 0x05;	//set SOC0/1 start trigger on  EPWM1SOCA
	AdcRegs.ADCSOC0CTL.bit.ACQPS 	= 6;	//set SOC0/1 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	//PFC adc
	AdcRegs.ADCSOC2CTL.bit.CHSEL 	= 1;	//set SOC2/3 channel select to ADCIN A1/B1
	AdcRegs.ADCSOC2CTL.bit.TRIGSEL 	= 0x0B;	//set SOC2/3 start trigger as EPWM4SOCA
	AdcRegs.ADCSOC2CTL.bit.ACQPS 	= 6;	//set SOC2/3 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	//heater adc
	AdcRegs.ADCSOC4CTL.bit.CHSEL 	= 6;	//set SOC4 channel select to ADCIN A6
	AdcRegs.ADCSOC4CTL.bit.TRIGSEL 	= 0x0D;	//set SOC4 start trigger as EPWM5SOCA
	AdcRegs.ADCSOC4CTL.bit.ACQPS 	= 6;	//set SOC4 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	//dhb adc
	AdcRegs.ADCSOC6CTL.bit.CHSEL 	= 2;	//set SOC6/7 channel select to ADCIN A2/B2
	AdcRegs.ADCSOC6CTL.bit.TRIGSEL 	= 0x07;	//set SOC6/7 start trigger as EPWM2SOCA
	AdcRegs.ADCSOC6CTL.bit.ACQPS 	= 6;	//set SOC6/7 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	AdcRegs.ADCSOC8CTL.bit.CHSEL 	= 4;	//set SOC8/9 channel select to ADCIN A6
	AdcRegs.ADCSOC8CTL.bit.TRIGSEL 	= 0x05;	//set SOC8/9 start trigger as EPWM2SOCA
	AdcRegs.ADCSOC8CTL.bit.ACQPS 	= 6;	//set SOC8/9 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

	EDIS;
}
/*
void config_measurements(struct measurements* testi)
{

	//testi->aux_voltage = (Uint16*)&(AdcResult.ADCRESULT0);
	//meas.aux_voltage = 			(Uint16*)&(AdcResult.ADCRESULT0);

	testi->dc_link_voltage = 		(Uint16*)&AdcResult.ADCRESULT1;
	testi->pfc_current1 = 		(Uint16*)&AdcResult.ADCRESULT0;
	testi->pfc_current2 = 		(Uint16*)&AdcResult.ADCRESULT0;
	testi->mains_voltage = 		(Uint16*)&AdcResult.ADCRESULT0;
	testi->dhb_current = 			(Uint16*)&AdcResult.ADCRESULT0;
	testi->dhb_output_voltage = 	(Uint16*)&AdcResult.ADCRESULT0;
	testi->dhb_cap_voltage =		(Uint16*)&AdcResult.ADCRESULT0;
	testi->heater_voltage = 		(Uint16*)&AdcResult.ADCRESULT0;
	testi->heater_current = 		(Uint16*)&AdcResult.ADCRESULT0;

}

*/
