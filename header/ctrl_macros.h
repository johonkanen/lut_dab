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

// 12bit gain == 1/4095
#define m_12bit_gain (float)2.442002442002442e-04

#define m_kp ctrl->kp
#define m_ki ctrl->ki
#define m_ref ctrl->ref
#define m_imem ctrl->imem


#endif /* CTRL_MACROS_H_ */
