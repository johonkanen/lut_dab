;****************************************************************
;* FNAME: test_hrpwm_drivers.asm        Jhonkanen   2.3.2016  	*
;*																*
;*                                                             	*
;****************************************************************

;PFC PWM driver test
		MOVW	DP,#_AdcResult
		MOV		AH,@_AdcResult+1
		MOV		DP,#_ePWM
		MOVL	XAR4,@_ePWM+8  ;pwm channel number * 2
		MOVB	XAR0,#0x9		; CMPA offset  == 9


		MOV		PL,#0xD05C
		MOV		PH,#0x09D3

		MOVL	XT,P
		QMPYUL	P,XT,ACC ;1c

;HRPWM UPDATE
		MOV		T,#0x7400
		MPYU	ACC,T,PL
		MOV	 	PL,AH
;HRPWM UPDATE end

        MOVL	*+XAR4[AR0],P
        MOVL	ACC, P
        ASR		AH, #1
        MOVB	XAR0,#0xA
        MOV		*+XAR4[AR0],AH
;PFC PWM driver test end

;Cuk pwm test

;		MOVW	DP,#_AdcResult
;		MOV		AH,@_AdcResult+1
;		MOV		DP,#_ePWM
;		MOVL	XAR4,@_ePWM+4
;		MOVB	XAR0,#0x9		; CMPA offset  == 9
;		MOVB	XAR1,#0xA


;		MOV		PL,#0xD05C
;		MOV		PH,#0x09D3
;		MOVL	XT,P
;		QMPYUL	P,XT,ACC ;1c

;HRPWM UPDATE
;		MOV		T,#0x7400
;		MPYU	ACC,T,PL
;		MOV	 	PL,AH
;HRPWM UPDATE end

;        MOVL	*+XAR4[AR0],P
;        MOV		AH,PH
;        ASR		AH,#1
;        MOV		*+XAR4[AR1],AH

;        ADD		PH,#0xE ;sub 2*deadtime
;        MOVL	XAR4,@_ePWM+6
;        MOVL	*+XAR4[AR0],P
;Cuk pwm test end



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
