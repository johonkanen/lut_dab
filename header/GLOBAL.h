/*
 * GLOBAL.h
 *
 *  Created on: 1.12.2016
 *      Author: jhonkanen
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

void rampup(void);
void rampup_init(void);

extern void init_cla(void);
/**********************************/
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsLoadSize;

extern volatile struct EPWM_REGS *ePWM[9];

extern Uint16 p1_phase,p2_phase,s1_phase,s2_phase,rxduty1,rxduty2,rxdata,rxphase;


float measgain = (float)0.109890109890110;

struct f_pid_ctrl voltage_ctrl;

struct f_pid_ctrl d1_ctrl;
struct f_pid_ctrl d2_ctrl;

struct storage sig_prbs;
struct measurements meas;
struct ext_ad_result ext_ad;
struct pwm_interface phase_reg;

float current_filter[2] = {0.439900846488443, 0.120198307023115};
float current_filter_mem = 0;
Uint16 current_filter_output = 0;

float current_filter2[6] = {0.187600514608639,0.375201029217278,0.187600514608639,
							1.000000000000000,-0.187996191288173,-0.061601750277270};
float current_filter2_mem[2] = {0,0};
Uint16 current_filter_2_output = 0;


Uint16	SCIdata;

int16 cnt_jee=0;

Uint16* mailbox;
Uint16 system_init;

#endif /* GLOBAL_H_ */




