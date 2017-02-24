/*
 * ctrl_macros.h
 *
 *  Created on: 14.12.2016
 *      Author: Asennus
 */

#ifndef CTRL_MACROS_H_
#define CTRL_MACROS_H_

#include "datatypes.h"


//macro to call function from pid control struct with the name of the struct
#define m_execute_fpid_ctrl(X) ((&X)->pid_control((&X)));


#define m_20bit_gain (float)9.536743164062500e-07
#define m_19bit_gain (float)1.907348632812500e-06
#define m_18bit_gain (float)3.814697265625000e-06

#define m_15bit_gain (float)3.051757812500000e-05
#define m_14bit_gain (float)6.103515625000000e-05
#define m_13bit_gain (float)1.220703125000000e-04
#define m_12bit_gain (float)2.442002442002442e-04
#define m_11bit_gain (float)4.882812500000000e-04
#define m_10bit_gain (float)9.765625000000000e-04
#define m_9bit_gain (float)0.001953125000000
#define m_8bit_gain (float)0.003906250000000


#define m_kp ctrl->kp
#define m_ki ctrl->ki
#define m_ref ctrl->ref
#define m_imem ctrl->imem
#define m_ctrl_meas *(Uint16*)*(ctrl->ctrl_meas)


#endif /* CTRL_MACROS_H_ */
