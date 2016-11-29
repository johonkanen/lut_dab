;this 'ere is a assembly file

		;.def _pwm1_hrpwm
		;.text ;change this to "ramfuncs" if system is booted from flash
    	.sect "ramfuncs"
    	.global _pwm1_hrpwm
		
_pwm1_hrpwm:

		MOVW	DP,#_AdcRegs
		MOV		AH,@_AdcRegs+8
		MOVW	DP,#_ePWM		
		MOVL	XAR6,@_ePWM+0
		MOV		AR1,#0x9
	
		MOV		T,#1477

		QMPYUL 	P,XT,ACC
		MOV		T,#0x3300
		MPYU	ACC,T,PL
		MOV		PL,AH
		
		MOVL	*+XAR6[AR1],P
		MOVL	ACC,*+XAR6[AR1]
		LSR		AH,#1		
		MOV		AR1,#0xA
		MOV		*+XAR6[AR1],AH		
		
		;return back to interrupt
		LRETR
	
	
; external references:

	.global	_AdcRegs
	.global	_ePWM
