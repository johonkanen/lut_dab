;****************************************************************
;* FNAME: AUX_ctrl		                Jhonkanen   29.7.2015  	*
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
	.global	_AUX_ctrl
_AUX_ctrl:
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

		.include "AUX_PI_ctrl.asm"
		.include "dc_link_voltage_ctrl_test.asm"
		;Dc link ref rampup

    	MOVW    DP,#_DC_link_voltage_ctrl
		MOV		AH,#0

		MOV		AL,@_DC_link_voltage_ctrl+8

		ADDL	@_DC_link_voltage_ctrl+10,ACC
		MOVL	ACC, @_DC_link_voltage_ctrl+10

		MOV		@_DC_link_voltage_ctrl+7,AH




;************************************
;*									*
;* XAR1,#_DC_link_voltage_ctrl		*
;*									*
;************************************
		.include "uart_stream.asm"
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

		toggle_gpio34

;clear pie ack and init for next soc

        MOVW      DP,#_EPwm1Regs+28     	;
        OR        @_EPwm1Regs+28,#0x0001 	;
;Acnowledge interrupt to PIE
        MOVW      DP,#_PieCtrlRegs+1    	;
        MOVB      @_PieCtrlRegs+1,#4,UNC 	;

; full context switch
        MOVL      XAR7,*--SP            	;
        MOVL      XAR6,*--SP            	;
        MOVL      XAR5,*--SP            	;
        MOVL      XAR4,*--SP            	;
        MOVL      XT,*--SP              	;
        POP       AR1H:AR0H             	;
        NASP      ;
        IRET      ;
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
