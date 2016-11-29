;****************************************************************
;* FNAME: dc_link_voltage_ctrl_test.asm  Jhonkanen   6.3.2016  	*
;*																*
;*	XAR0 = pwm register offset									*
;*	XAR1 = _DC_link_voltage_ctrl								*
;*	XAR4 = EPWM pointer                                       	*
;****************************************************************

pfc_max_duty .set 125
sat_pfc_high .set 124

pfc_min_duty .set 10
sat_pfc_min  .set 9

;Load controller register
        MOVW    DP,#_DC_link_voltage_ctrl			;
        MOVL	XAR1,#_DC_link_voltage_ctrl			;XAR1 = PI control pointer
 		MOV		AH,@_DC_link_voltage_ctrl+7			;AH = 16 bit reference, even since 16bit

;calculate error = ref-adc1
        MOVW    DP,#_pfc_meas          ; ADCRESULT 0 = AUX voltage
        MOVL	XAR2, @_pfc_meas
 		SUB		AH,*+XAR2[0]		;ACC = error<<16
; 		LSL		ACC,#1
; 		NOP
 		MOVL	XT,ACC					;XT = err

;PI controller static equation
		QMPYL	P,XT,*+XAR1[1]			;P = e*P_gain
		MOVL	ACC,*+XAR1[4]			;ACC = mem[32bit] [4] and [5] is the same if MOVL is used
		ADDL	P,ACC					;P = output

;check over/underflow
		CMP		PH, #pfc_max_duty
		BF		SAT_PFC_HIGH,GEQ

		CMP		PH, #pfc_min_duty
		BF		SAT_PFC_LOW,LEQ

;NORMAL OPERATION
		QMPYL	ACC,XT,*+XAR1[2]
		NOP
		ADDL	*+XAR1[4],ACC
		BF		UPDATE_PFC_PWM,UNC
SAT_PFC_HIGH:
		MOVL	ACC,XT
		ABS		ACC
		MOV		T,#15
		ASRL	ACC,T
		NEG		ACC

		ADDL	*+XAR1[4],ACC
		MOV		PH,#sat_pfc_high
		BF		UPDATE_PFC_PWM,UNC

SAT_PFC_LOW:
		MOVL	ACC,XT
		ABS		ACC
;//		NEG		ACC

		MOV		T,#15
		ASRL	ACC,T
		NOP
		ADDL	*+XAR1[4],ACC
		MOV		PH,#sat_pfc_min
		BF		UPDATE_PFC_PWM,UNC

UPDATE_PFC_PWM:
;PFC PWM driver test
		MOV		DP,#_ePWM
		MOVL	XAR4,@_ePWM+8  ;pwm channel number * 2
		MOVB	XAR0,#0x9		; CMPA offset  == 9
;HRPWM UPDATE
		MOV		T,#0x6f00
		MPYU	ACC,T,PL
		MOV 	PL,AH			;MOVB
;HRPWM UPDATE end

		MOVL	ACC,P
		SUB		AH,#147
		NEG		AH

		SUB		AL,#0x6f00
		NEG		AL

        MOVL	*+XAR4[AR0],ACC
        ADD		AH,#6

        MOVB	XAR0,#0xA
        MOV		*+XAR4[AR0],AH
;PFC PWM driver test end

;**************************************************************
;* UNDEFINED EXTERNAL REFERENCES                              *
;**************************************************************
	.global	_SciaRegs
	.global	_PieCtrlRegs

	.global	_AdcResult
	.global	_Voltage1
	.global	_Voltage2
	.global	_AdcRegs
	.global	_ConversionCount
	.global	_startup_filter_mem
	.global _startup_filter_den
	.global _startup_filter_num
	.global _voltage_has_risen
	.global _startup_voltage
	.global _ePWM
	.global _AUX_voltage_ctrl
	.global _EPwm1Regs
	.global	_GPIO
	.global _DC_link_voltage_ctrl
	.global _pfc_meas
