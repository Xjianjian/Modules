

#include "MPC5744P.h"
#include "platformTypes.h"
#include "flexCan.h"
#include "sysClk.h"

/*
 * Function:sysClk_init
 * Param<void>:
 * Return<void>:
 * Description: Enable XOSC, PLL0, PLL1 and enter RUN0 with PLL1 as sys clk (200 MHz)
 * Note:
*/
void sysClk_init(void)
{
	//Clk Out
	//m_CGM.AC6_SC.R = 0x02000000;  //Enable PLL0 - clkout0 (PLL0 PHI)
	m_CGM.AC6_DC0.B.DE = 1;       //Enable divider for SYSCLK0
	m_CGM.AC6_DC0.B.DIV = 0;      //Set divider for SYSCLK0
	//Configure RunPeripheralConfiguration regist   ers in ME_RUN_PC0
	m_ME.RUN_PC[0].B.DRUN = 1;    //Enable DRUN mode for all peripherals

	//Enable external oscilator
	m_ME.DRUN_MC.B.XOSCON = 1;

	/* Enable IRC for watchdog */
	m_ME.DRUN_MC.B.IRCON = 1;

	//AUX Clock Selector 3 setup
	m_CGM.AC3_SC.B.SELCTL =0x01;  //connect (8..40MHz) XTALL to the PLL0 input
	m_ME.DRUN_MC.B.PLL0ON = 1;    //Enable PLL0 for DRUN mode
	 /*
	        fPLL0_ref = 16Mhz.Configure PLL0 Dividers for 200 MHz
	        fPLL0_VCO = (fPLL0_ref x PLL0DV[MFD] x 2)/PLL0DV[PREDIV]
	        fPLL0_PHI  = fPLL0_ref x PLL0DV[MFD] / (PLL0DV[PREDIV] x PLL0DV[RFDPHI] --PLL0 clk
	        fPLL0_PHI1 = fPLL0_ref x PLL0DV[MFD] / (PLL0DV[PREDIV] x PLL0DV[RFDPHI1])
	*/
	PLLDIG.PLL0CR.B.CLKCFG = 1;             //Bypass mode PLL0 on
	// RFDPHI1 = 10, RFDPHI = 2, PREDIV = 2, MFD = 50
	PLLDIG.PLL0DV.R = 0x50000000 |0x00020000 |0x00002000 |0x0032;

	// Set PPL0 as system clock
	m_ME.DRUN_MC.B.SYSCLK = 0x2;

	/* CAN-0 Peripheral Control Register */
	//m_ME.PCTL79.R = 0;

	//Mode transition to apply the PLL0 setup and set Normal mode with PLL
	m_ME.MCTL.R = 0x30005AF0;      //DRUN Mode & Key
	m_ME.MCTL.R = 0x3000A50F;      //DRUN Mode & Key

	while(!m_ME.GS.B.S_PLL0);      //ME_GS Wait for PLL stabilization.
	while(m_ME.GS.B.S_MTRANS);     //Waiting for end of transaction
	// ME_GS Check DRUN mode has successfully been entered
	while(m_ME.GS.B.S_CURRENT_MODE != 3);
}

void sysClk_uninit(void)
{
    uint16_t timeout;
    /* DRUN mode detect */
    if (0x03 != m_ME.GS.B.S_CURRENT_MODE)
    {
        /* Not in DRUN mode */
        /* Switch to DRUN mode */
        m_ME.DRUN_MC.B.SYSCLK = 0;
        m_ME.DRUN_MC.B.XOSCON = 0;
        m_ME.DRUN_MC.B.PLL0ON = 0;
        m_ME.DRUN_MC.B.PLL1ON = 0;
        /* Turn back to DRUN mode */
        m_ME.MCTL.R = 0x30005AF0; /* Mode & Key */
        m_ME.MCTL.R = 0x3000A50F; /* Mode & Key inverted */
        timeout = 60000u;
        while((m_ME.GS.B.S_MTRANS == 1) && (timeout > 0))
        {
            timeout--;
        }/* Wait for mode entry complete */
        timeout = 60000u;
        while((m_ME.GS.B.S_CURRENT_MODE != 0x3) && (timeout > 0))
        {
            timeout--;
        }/* Check DRUN mode entered */
    }
}

/*
 * Function: periClkGen_init
 * Param<void>:
 * Return<void>:
 * Description: PBRIDGE0/PBRIDGE1_CLK at syst clk div by 4 ... (50 MHz)
 * Note: No effect for peripheral (eg. CAN, FLay, ADC etc.) clock.
*/
void periClk_init(void)
{
    m_CGM.SC_DC0.R = 0x80030000;    // PBRIDGE0/PBRIDGE1_CLK at syst clk div by 4 ... (50 MHz)//divid 5 40Mhz
    m_CGM.AC0_SC.R = 0x01000000;    // ADC_CLK : extern XOSC CLOCK(16M)
    m_CGM.AC0_DC0.R = 0x80030000;    // MOTC_CLK : Enable aux clk 0 div by 4 (4 MHz)
    m_CGM.AC0_DC1.R = 0x80000000;    // SGEN_CLK : Enable aux clk 0 div by 1 ?(16MHz)
    m_CGM.AC0_DC2.R = 0x80070000;    // ADC_CLK : Enable aux clk 0 div by 8?(2MHz)
    m_CGM.AC6_SC.R = 0x02000000;    // Select PLL0 for auxiliary clock 6
    m_CGM.AC6_DC0.R = 0x80010000;    // CLKOUT0 : Enable aux clk 6 div by 2 ?(100 MHz)
    m_CGM.AC10_SC.R = 0x04000000;    // Select PLL1 for auxiliary clock 10
    m_CGM.AC10_DC0.R = 0x80030000;    // ENET_CLK : Enable aux clk 10 div by 4 ?(50 MHz)
    m_CGM.AC11_SC.R = 0x04000000;    // Select PLL1 for auxiliary clock 11
    m_CGM.AC11_DC0.R = 0x80030000;    // ENET_TIME_CLK : Enable aux clk 11 div by 4 ?(50 MHz)
    m_CGM.AC5_SC.R = 0x02000000;    // Select PLL0 for auxiliary clock 5
    m_CGM.AC5_DC0.R = 0x80000000;    // LFAST_CLK : Enable aux clk 5 div by 1 ?(50 MHz)
    m_CGM.AC2_DC0.R = 0x80040000;    // CAN_PLL_CLK : Enable aux clk 2 (PLL0) div by 5 : (40 MHz)
    m_CGM.AC1_DC0.R = 0x80010000;    // FRAY_PLL_CLK : N aux clk 1 (PLL0) div by 2 ?(25 MHz)
    m_CGM.AC1_DC1.R = 0x80010000;    // SENT_CLK : Enable aux clk 1 (PLL0) div by 2 ?(25 MHz)
}

/*
 * Function: sysSchClk_init
 * Param<void>:
 * Return<void>:
 * Description: Initial system schedule clock to 10 MHz (0.1us each count), and start system timer
 * Note:
*/
void sysSchClk_init(void)
{
    STM_0.CR.R = 0x00000401;
}

uint32_t sys_random(bool seedInit)
{
    static uint32_t randomSeed = 0ul;
    uint32_t curCnt = STM_0_CNT;

    if (TRUE == seedInit)
    {
        randomSeed = curCnt;

        if (0ul == randomSeed)
        {
            randomSeed = 0x1A8D614Cul;
        }
    }
    else
    {
        if ((0 == randomSeed)
            && (0 == curCnt))
        {
            randomSeed = 0x1A8D614Cul;
        }
    }

    randomSeed = (randomSeed * 1971) + 1360 + (curCnt & 0x0A00000Ful);

    return randomSeed;
}

