;****************************************************************
;* FNAME: AUX_ctrl_cuk_rampup.asm       Jhonkanen   12.7.2016  	*
;*                                                             	*
;* DCM flyback Auxiliary supply control sourcefile             	*
;*															   	*
;*	PI-control + antisaturation + PWM1A update				   	*
;*	Uses registers XAR0-XAR4								    *
;*																*
;*	ADCresult0 = AUX voltage									*
;*	PI controller registers										*
;*	+1 P_gain												   	*
;*	+3 I_gain												   	*
;*	+5 mem													   	*
;*	+7 ref													   	*
;*                                                             	*
;****************************************************************
;this is a testi thingie
;	.sect	".text:retain"

; NOTE! SATURATIONS DEFINED IN CORRESPONDING FILES

toggle_gpio34 .macro
    MOVW         DP, #0x1bf
    OR           @0xe, #0x0004
	.endm


    .sect "ramfuncs"
	.global	_AUX_ctrl_cuk_rampup
_AUX_ctrl_cuk_rampup:
        ASP       ; [CPU_]
        PUSH      AR1H:AR0H
        SPM       0
        MOVL      *SP++,XT
        MOVL      *SP++,XAR4
        MOVL      *SP++,XAR5
        MOVL      *SP++,XAR6
        MOVL      *SP++,XAR7
        CLRC      PAGE0,OVM
        CLRC      AMODE

		ZAPA
		SETC	SXM


;read dhb voltage

		MOVW	DP,#_GpioDataRegs ;dp to 6fc0
		OR		@_GpioDataRegs+5,#0x4000
		OR		@_GpioDataRegs+3,#0x8000
		RPT		#3
		||NOP


		MOVW	DP, #_SpiaRegs
		MOV		@_SpiaRegs+9,#0x5555 ;send dummy data to start ad conversion


		.include "AUX_PI_ctrl.asm"
		.include "dc_link_voltage_ctrl_test.asm"


;************************************
;*									*
;* XAR1,#_DC_link_voltage_ctrl		*
;*									*
;************************************

		MOVW	DP,#_ePWM
		MOVL	XAR2,@_ePWM+4
		MOVL	XAR3,@_ePWM+6
		MOV		AR0,#9			;offset to pwm counter compare

		MOVW	DP,#_cuk_rampup

		CMP		@_cuk_rampup,#4095 ; ramp duty to 50%
		BF		RAMPUP_COMPLETE, EQ

		ADD		@_cuk_rampup,#1

;		MOVW	DP, #_AdcResult
;		MOV		AH, @_AdcResult+8

		MOV		T,#3552
		MPYU	P,T,@_cuk_rampup

		MOVL	*+XAR3[AR0],P
		MOVL	*+XAR2[AR0],P


		BF		RAMPUP_INCOMPLETE,UNC



RAMPUP_COMPLETE:
		MOVW	DP,#_cuk_ready
		MOV		@_cuk_ready,#1

		MOVW	DP, #_dhb_meas
		MOVL	XAR4, @_dhb_meas+2
		MOV		AH,*+XAR4[0]

		MOV		T,#3552
		MPYU	P,T,AH

		ADD		PH, #40

		MOV		T,#0x6f00

		MPYU	ACC,T,PL


		SUB		AH,#0x6f00
		NEG		AH


		MOV		PL,AH

;		MOVL	*+XAR3[3],P
;		MOVL	*+XAR2[3],P

RAMPUP_INCOMPLETE:

		;read dhb voltage
		;MOVW	DP, #_SpiaRegs
		;MOV		AH,@_SpiaRegs+7

		;.include "uart_stream.asm"

		toggle_gpio34
		;read heater ad convesion
		MOVW	DP,#_GpioDataRegs ;dp to 6fc0
		OR		@_GpioDataRegs+5,#0x8000
		OR		@_GpioDataRegs+3,#0x4000

		.include "uart_stream.asm"
		MOVW	DP,#_ext_ad
		MOVL	XAR0,#_ext_ad

		MOVW	DP, #_SpiaRegs
		MOV		AL,@_SpiaRegs+7

		MOV		*+XAR0[0], AL

		MOV		@_SpiaRegs+9,#0x5555 ;send dummy data to start ad conversion

;-------------------------------------------



;-------------------------------------------


;PRBS generation

		MOVW	DP,#_sig_prbs
		MOV		AL,@_sig_prbs+0	;load wrptr
		MOV		AH,@_sig_prbs+0

		AND		AH,#1
		LSL		AH,#13
		LSR		AL,#1

		XOR		AL,AH		;AH == new bit field
		LSL		AH,#1
		OR		AL,AH

;prbs is ready
		MOV		@_sig_prbs+0,AL
;save +/- to PRBS struct
		AND		AL,#0x2
		SUB		AL,#1
		MOV		@_sig_prbs+1,AL

;-------------------------------------------
;
;	Wait for SPI to convert heater voltage
;

		MOVW	DP,#_ext_ad
		MOVL	XAR0,#_ext_ad


		RPT		#61
		||NOP

		toggle_gpio34

		MOVW	DP, #_SpiaRegs
		MOV		AL,@_SpiaRegs+7

		;read second conversion
		MOV		*+XAR0[1], AL


;clear pie ack and init for next soc

        MOVW    DP,#_EPwm1Regs+28     	;
        OR      @_EPwm1Regs+28,#0x0001 	;
;Acnowledge interrupt to PIE
        MOVW    DP,#_PieCtrlRegs+1    	;
        MOVB    @_PieCtrlRegs+1,#4,UNC 	;

; full context switch
        MOVL    XAR7,*--SP            	;
        MOVL    XAR6,*--SP            	;
        MOVL    XAR5,*--SP            	;
        MOVL    XAR4,*--SP            	;
        MOVL    XT,*--SP              	;
        POP     AR1H:AR0H             	;
        NASP    ;
        IRET    ;
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
	.global _sig_prbs
	.global	_SCIdata
	.global	_cuk_rampup
	.global _cuk_ready
	.global	_memory
	.global _SpiaRegs
	.global _GpioDataRegs
	.global _ext_ad
	.global _step
	.global _ramptest
	.global _rampmem
	.global _filtergains
	.global _dhb_meas

