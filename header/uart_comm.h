/*
 * uart_comm.h
 *
 *  Created on: 5.8.2015
 *      Author: jhonkanen
 */

#ifndef UART_COMM_H_
#define UART_COMM_H_


#include "DSP28x_Project.h"

extern volatile struct EPWM_REGS *ePWM[7];
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
//#define	heater_voltage	AdcResult.ADCRESULT9

#define aux_cmpa		(*ePWM[1]).CMPA.half.CMPA
#define cuk_hi_cmpa		(*ePWM[2]).CMPA.half.CMPA
#define pfc_cmpa		(*ePWM[4]).CMPA.half.CMPA
#define heater_hi_cmpa	(*ePWM[5]).CMPA.half.CMPA

#define dhb_voltage		(Uint16*)&(ext_ad)
#define heater_voltage	(Uint16*)&(ext_ad)+1
*/
extern Uint16* mailbox;
extern Uint16  package;
extern struct ext_ad_result ext_ad;

void init_msg_source(void);

#endif /* UART_COMM_H_ */




