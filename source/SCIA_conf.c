/*
 * SCIA_conf.c
 *
 *  Created on: 30 Jun 2015
 *      Author: Jari
 */
/*
"${CG_TOOL_ROOT}/include"
"${PROJECT_ROOT}"
"${INSTALLROOT_2803x_V130}/DSP2803x_common/lib"
"${INSTALLROOT_IQMATH_V160}/lib"
*/
#include "DSP28x_Project.h"
#include "SCIA_conf.h"


void scia_fifo_init()
{
    SciaRegs.SCIFFTX.all=0xE040;
    SciaRegs.SCIFFRX.all=0x2044;
    SciaRegs.SCIFFCT.all=0x0;
}

void scia_echoback_init()
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

 	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.bit.TXINTENA =1;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;

	SciaRegs.SCIHBAUD    =0x0000;  // 937500 baud @LSPCLK = 15MHz (60 MHz SYSCLK).
    SciaRegs.SCILBAUD    =0x0000;  // C2 = 9600

	SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

void scia_xmit(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
    SciaRegs.SCITXBUF=a;
}

void scia_msg(char* msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scia_xmit(msg[i]);
        i++;
    }
}
