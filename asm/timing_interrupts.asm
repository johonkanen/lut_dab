;***************************************************************
;* TMS320C2000 C/C++ Codegen                         PC v6.2.0 *
;* Date/Time created: Thu Aug 04 15:58:21 2016                 *
;***************************************************************

gpio34_on .macro
		OR		*+XAR0[0],#0x4
	.endm

gpio34_off .macro
		OR		*+XAR1[0],#0x4
	.endm

    .sect "ramfuncs"
	.global	_AUX_timing

_AUX_timing:
        ASP
        CLRC      PAGE0,OVM            
        CLRC      AMODE                 


        MOVW	DP,#_GpioDataRegs+10
        MOVL	XAR0,#_GpioDataRegs+10
        MOVL	XAR1,#_GpioDataRegs+12

		; delay for PFC pwm

		RPT		#90
		||NOP

       	gpio34_on

        MOVW	DP,#_EPwm4Regs+25     
        OR		@_EPwm4Regs+25,#0x0800 

        AND		AL,@_EPwm4Regs+25,#0xf8ff
        OR		AL,#0x0600 
        
		MOV		@_EPwm4Regs+25,AL     
        OR		@_EPwm4Regs+26,#0x0300 

		gpio34_off
		;delay for heater pwm
		RPT		#95
		||NOP

        gpio34_on

        MOVW	DP,#_EPwm5Regs+25     
        OR		@_EPwm5Regs+25,#0x0800 

        AND		AL,@_EPwm5Regs+25,#0xf8ff 
        OR		AL,#0x0600            
        MOV		@_EPwm5Regs+25,AL    

        AND		AL,@_EPwm5Regs+26,#0xfcff 
        OR		AL,#0x0200           
        MOV		@_EPwm5Regs+26,AL    

		gpio34_off


		RPT		#50
		||NOP

        gpio34_on

        MOVW	DP,#_EPwm2Regs+25    
        OR		@_EPwm2Regs+25,#0x0800 

        AND		AL,@_EPwm2Regs+25,#0xf8ff 
        OR		AL,#0x0600            
        MOV		@_EPwm2Regs+25,AL     

        AND		AL,@_EPwm2Regs+26,#0xfcff 
        OR		AL,#0x0100           
        MOV		@_EPwm2Regs+26,AL     


        MOVW	DP,#_EPwm1Regs+28     
        OR		@_EPwm1Regs+28,#0x0001 

		gpio34_off


		MOVW	DP,#_ad_timing_disable
		MOV		AH,#1
		CMP		AH,@_ad_timing_disable
		BF		AD_TIMING_DIABLED,NEQ

        MOVW	DP,#_ad_timing_complete
        ADD		@_ad_timing_complete,#1

AD_TIMING_DIABLED:
        MOVW	DP,#_PieCtrlRegs+1   
        MOVB	@_PieCtrlRegs+1,#4,UNC
        NASP       
        IRET      

;**************************************************************
;* UNDEFINED EXTERNAL REFERENCES                              *
;**************************************************************
	.global	_PieCtrlRegs
	.global	_GpioDataRegs
	.global	_EPwm2Regs
	.global	_EPwm1Regs
	.global	_EPwm5Regs
	.global	_EPwm4Regs
	.global	_AdcRegs
	.global _ad_timing_complete
	.global _ad_timing_disable
