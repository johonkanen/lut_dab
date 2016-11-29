;****************************************************************
;* FNAME: _startup_filter               FR SIZE:  12           	*
;*                                                             	*
;* FUNCTION ENVIRONMENT                                        	*
;*                                                             	*
;* ADC_result 1 == Input voltage								*
;* ADC_result 0 == aux voltage                                 	*
;*                            0 Parameter,  0 Auto, 12 SOE     	*
;****************************************************************

;	.sect	".text:retain"
    .sect "ramfuncs"
	.global	_startup_filter

_startup_filter:
        ASP       ; [CPU_]
        PUSH      AR1H:AR0H             ; [CPU_]
        SPM       0                     ; [CPU_]
        MOVL      *SP++,XT              ; [CPU_]
        MOVL      *SP++,XAR4            ; [CPU_]
        MOVL      *SP++,XAR5            ; [CPU_]
        MOVL      *SP++,XAR6            ; [CPU_]
        MOVL      *SP++,XAR7            ; [CPU_]
        CLRC      PAGE0,OVM             ; [CPU_]
        CLRC      AMODE                 ; [CPU_]

;Load Filter parameters
		ZAPA
		SETC	SXM
;		SPM		-2
        MOVW    DP,#_startup_filter_den			;
        MOVL	XAR1,#_startup_filter_den		;XAR1 = den[]

        MOVW    DP,#_startup_filter_num			;
        MOVL	XAR2,#_startup_filter_num		;XAR2 = num[]

        MOVW    DP,#_startup_filter_mem			;
        MOVL	XAR3, #_startup_filter_mem		;XAR3 = mem[]

       	MOVL	ACC,*+XAR3[3]			;ACC = mem[2]
		MOVL	*+XAR3[1],ACC			;mem[1] = mem[2]

;Test for startup voltage
		MOVW	DP,#_startup_voltage
		CMP		AH,@_startup_voltage
		BF		VOLTAGE_NOT_RISEN,LEQ		;if AL - _startup_voltage > 0 jump

;VOLTAGE_HAS_RISEN:
		MOV		AL,#1
		MOVW	DP,#_voltage_has_risen
		MOV		@_voltage_has_risen,AL

VOLTAGE_NOT_RISEN:
		MOVB	ACC,#0					;zero the acc

        MOVW    DP,#_pfc_meas          ; ADCRESULT 0 = AUX voltage
        MOVL	XAR4, @_pfc_meas
 		MOV		AH,*+XAR4[0]		;ACC = error<<16

        SFR		ACC,#3
;        NOP
        MOVL	XT,ACC					;XT = adc0<<16

;Filter:
        QMPYL	ACC,XT,*+XAR2[3]		;ACC    = (adc0*num[2])>>32
        LSL		ACC,#2
;        NOP
		MOVL	*+XAR3[3],ACC			;mem[2] = (adc0*num[2])>>32
		MOVL	ACC,*+XAR3[5]			;ACC    = mem[3]
		ADDL	*+XAR3[3],ACC			;mem[2]+= mem[3]

        QMPYL	ACC,XT,*+XAR2[5]		;ACC  = (adc0*num[3])>>32
		LSL		ACC,#2
;        NOP
		MOVL	*+XAR3[5],ACC			;mem[3] = (adc0*num[3])>>32

		MOVL	XT,*+XAR3[1]			;XT = mem[1] = y

        QMPYL	ACC,XT,*+XAR1[3]		;ACC= mem[1]*den[2]
		LSL		ACC,#2
;        NOP
		SUBL	*+XAR3[3],ACC			;mem[2] += mem[1]*den[2]

        QMPYL	ACC,XT,*+XAR1[5]		;ACC= mem[1]*den[3]
		LSL		ACC,#2
;        NOP
		SUBL	*+XAR3[5],ACC			;mem[3] -= mem[1]*den[3]

NO_OVERFLOW:
;----------------------------------------------------------------------
; 163 | if (SciaRegs.SCIFFTX.bit.TXFFST == 0)
;----------------------------------------------------------------------
;        MOVW      DP,#_SciaRegs+10      ; [CPU_U]
;        AND       AL,@_SciaRegs+10,#0x1f00 ; [CPU_] |163|
;        LSR       AL,8                  ; [CPU_] |163|
;        BF        $C$L5,NEQ             ; [CPU_] |163|

;----------------------------------------------------------------------
; 165 | SciaRegs.SCITXBUF = AdcResult.ADCRESULT0>>8;
;----------------------------------------------------------------------
; scaling = 5227574E
;		MOV		AL,#0x574E
;		MOV		AH,#0x5227
;		MOVL	XT,ACC

;		QMPYL	P,XT,*+XAR3[1]
;		MOVL	ACC,P
;		LSL		ACC,#4
;uncomment these to check raw ad conversion
;*/
;	    MOVW    DP,#_AdcResult        	; [CPU_U]
;       	MOV     AH,@_AdcResult       	; Load upper register of XT /w adc
;/*
;        MOVW    DP,#_SciaRegs+9       ; [CPU_U]

;        LSR     AH,8                  ; [CPU_] |165|

;        MOV     @_SciaRegs+9,AH       ; [CPU_] |165|
;*/
;	    MOVW    DP,#_AdcResult        	; [CPU_U]
;      	MOV     AH,@_AdcResult       	; Load upper register of XT /w adc
;/*
;        MOVW    DP,#_SciaRegs+9       ; [CPU_U]
;		MOVL	ACC, *+XAR3[1]			;ACC = mem[1]
;        MOV     @_SciaRegs+9,AH       ; [CPU_] |166|
;----------------------------------------------------------------------
;
;----------------------------------------------------------------------

;$C$L5:
;----------------------------------------------------------------------
; 182 | AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;         //Clear ADCINT1 flag rein
;     | itialize for next SOC
;----------------------------------------------------------------------
;        MOVW      DP,#_AdcRegs+5        ; [CPU_U]
;        OR        @_AdcRegs+5,#0x0001   ; [CPU_] |182|

$C$L5:


;toggle gpio 34
;		MOVW         DP, #0x1bf
;    	OR           @0xe, #0x0004
;end toggle


;clear pie ack and init for next soc
        MOVW      DP,#_EPwm1Regs+28     ; [CPU_U]
        OR        @_EPwm1Regs+28,#0x0001 ; [CPU_] |235|
		;MOVW         DP, #0x1a0			;dp to epwm regs
		;MOVW    	 DP,#_ePWM+4
		;OR           @0x1c, #0x0001		;offset to event clear
;    	MOVW         DP, #0x33			;dp to piectrl regs
 ;   	MOVB         @0x21, #0x04, UNC	;receive more interrupts from group 3
;Acnowledge interrupt to PIE
        MOVW      DP,#_PieCtrlRegs+1    ; [CPU_U]
        MOVB      @_PieCtrlRegs+1,#4,UNC ; [CPU_] |183|
;----------------------------------------------------------------------
; 185 | return;
;----------------------------------------------------------------------
        MOVL      XAR7,*--SP            ; [CPU_]
        MOVL      XAR6,*--SP            ; [CPU_]
        MOVL      XAR5,*--SP            ; [CPU_]
        MOVL      XAR4,*--SP            ; [CPU_]
        MOVL      XT,*--SP              ; [CPU_]
        POP       AR1H:AR0H             ; [CPU_]
        NASP      ; [CPU_]
        IRET      ; [CPU_]

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
	.global _EPwm1Regs
	.global _pfc_meas
