;****************************************************************
;* FNAME: prbs_9.asm                Jhonkanen   15.02.2017 		*
;*                                                             	*
;*	assembly routine for generating 3bit prbs	             	*
;*															   	*
;*	Uses registers AL and AH								    *
;*																*
;*                                                             	*
;****************************************************************
;PRBS generation
    .sect "ramfuncs"
	.global	_prbs_3

_prbs_3:
		MOVW	DP,#_sig_prbs
		MOV		AL,@_sig_prbs+0	;load wrptr
		MOV		AH,@_sig_prbs+0

		LSL		AH,#1 ; LFSR3 = x3+x2, note! bits are arranged x1 x2 x3
		XOR		AH,AL
		AND		AH,#2
		LSR		AL,#1
		LSL		AH,#1

		OR		AL,AH



;prbs is ready
		MOV		@_sig_prbs+0,AL

		LRETR
;**************************************************************
;* UNDEFINED EXTERNAL REFERENCES                              *
;**************************************************************
	.global	_sig_prbs
