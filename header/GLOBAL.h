/*
 * GLOBAL.h
 *
 *  Created on: 1.12.2016
 *      Author: jhonkanen
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsLoadSize;

Uint16 startup_voltage;

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


/*********************************/
//int16* mailbox;

// ramp up signals

Uint16 cuk_rampup;
Uint16 heater_rampup;
Uint16 Aux_rampup;

// converter ready signal

Uint16 timing_complete;
Uint16 voltage_has_risen;
Uint16 dclink_ready;
Uint16 cuk_ready;

int32 startup_filter_num[] = {0x00000000,0x40000000, 0x33E70ABC};
int32 startup_filter_den[] = {0x40000000,0xA2822A24, 0x2224AB3C};
int32 startup_filter_mem[] = {0,0,0};

void read_ext_ad(void);

Uint16	SCIdata;

Uint16 cnt_jee=0;

struct storage sig_prbs;
struct measurements meas;
struct ext_ad_result ext_ad;


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


#endif /* GLOBAL_H_ */




