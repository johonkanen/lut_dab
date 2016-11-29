/*
 * uart_comm.c
 *
 *  Created on: 5.8.2016
 *      Author: Jari
 */
#include "uart_comm.h"

/*
#define aux_voltage 	AdcResult.ADCRESULT0
#define dc_link_voltage AdcResult.ADCRESULT1
#define pfc_current 	AdcResult.ADCRESULT2
#define mains_voltage 	AdcResult.ADCRESULT3
#define heater_current 	AdcResult.ADCRESULT4
////////////////////////////////////////////
#define cuk_current	 	AdcResult.ADCRESULT6
#define cuk_cap 		AdcResult.ADCRESULT7
#define Cuk_voltage 	AdcResult.ADCRESULT8
#define	heater_voltage	AdcResult.ADCRESULT9

#define aux_cmpa		(*ePWM[1]).CMPA.half.CMPA
#define cuk_hi_cmpa		(*ePWM[2]).CMPA.half.CMPA
#define pfc_cmpa		(*ePWM[4]).CMPA.half.CMPA
#define heater_hi_cmpa	(*ePWM[5]).CMPA.half.CMPA

#define dhb_voltage		(Uint16*)&(ext_ad)
#define heater_voltage	(Uint16*)&(ext_ad)+1

SpiaRegs.SPIRXBUF
*/

void init_msg_source()
{
	mailbox = (Uint16*)&(ext_ad);
}


