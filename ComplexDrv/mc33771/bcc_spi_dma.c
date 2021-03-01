/********************************************************************************/
/********************************************************************************/
/*******PROJECT :                                  ******/
/*******FUNCTION: TLE9104SH DRIVER                 ******/
/*******DATA    : 2019-12-12                       ******/
/*******AUTHOR  : X-Mad                            ******/
/********************************************************************************/
/********************************************************************************/
#include "MPC5744P.h"
#include "SIUL.h"
#include "defines.h"
#include "INTC.h"
#include "global_var.h"
#include "bcc_spi_dma.h"



unsigned short int spi1DmaRxBuffer[NUMBER_OF_BYTES];
unsigned short int spi2DmaRxBuffer[NUMBER_OF_BYTES];
static unsigned short int  spi1DmaRxBufSize = 0;
static unsigned short int  spi2DmaRxBufSize = 0;
static unsigned short int  spi1RxLen = 0;
static unsigned short int  spi2RxLen = 0;
static char spiDma_rxsuccess[2] = {0};


static void bccSpiDma_chl_arbitration(void);
static void bccSpiDma_tcdSpi1(unsigned short int  bufsize,unsigned char dmaen);
static void bccSpiDma_tcdSpi2(unsigned short int  bufsize,unsigned char dmaen);
/********************************************************************************/

void bccSpiDma_Init(void)
{
	//bridges_config
	AIPS_0.MPRA.R |= 0x77777770;       /* All masters have RW & user level access */
	AIPS_1.MPRA.R |= 0x77777770;       /* All masters have RW & user level access */

#if BCC_SPI_ISR_EN
    INTC_InstallINTCInterruptHandler(bccSpi1_isr_rxcb,272, 20);      //
    INTC_InstallINTCInterruptHandler(bccSpi2_isr_rxcb,281, 20);      //
#endif
#if BCC_SPI_DMA_ISR_EN
	INTC_InstallINTCInterruptHandler(bccSpi1Dma_isr_rxcb, 69, 20);
#endif
}


void bccSpiDma_task(void)
{
#if 0
	int i;
	static uint8_t tempReceiveBuffer2[NUMBER_OF_BYTES];
	if(DMA_0.TCD[15].CSR.B.DONE == 1)
	{
		DMA_0.CDNE.R = 15;
		spi2DmaRxBufSize = NUMBER_OF_BYTES;
		bccSpiDma_tcdSpi2();
		DMA_0.ERQ.B.ERQ15 = 1;

		for(i = 0;i< NUMBER_OF_BYTES;i++)
		{
			tempReceiveBuffer2[i] = spi2DmaRxBuffer[i];
		}
	}

	if(DMA_0.TCD[16].CSR.B.DONE == 1)
	{
		DMA_0.CDNE.R = 16;
		spi1DmaRxBufSize = NUMBER_OF_BYTES;
		bccSpiDma_tcdSpi1();
		DMA_0.ERQ.B.ERQ16 = 1;

		for(i = 0;i< NUMBER_OF_BYTES;i++)
		{
			tempReceiveBuffer2[i] = spi1DmaRxBuffer[i];
		}
	}
#endif
}

int bccSpiDma_startTransfer(uint8_t chl)
{

	switch(chl)
	{
		case e_spi1_dma:
		{
			DMA_0.ERQ.B.ERQ16 = 1;
		}
		break;
		case e_spi2_dma:
		{
			DMA_0.ERQ.B.ERQ15 = 1;
		}
		break;
		default:
		break;
	}

	return 0;
}

int bccSpiDma_resetSpiDMA(uint8_t chl,unsigned short int bufsize,uint8_t dmaen)
{
	if(bufsize > NUMBER_OF_BYTES)
	{
		return -1;
	}

	switch(chl)
	{
		case e_spi1_dma:
		{
			//spi1DmaRxBufSize = bufsize;
			bccSpiDma_tcdSpi1(bufsize,dmaen);
		}
		break;
		case e_spi2_dma:
		{
			//spi2DmaRxBufSize = bufsize;
			bccSpiDma_tcdSpi2(bufsize,dmaen);
		}
		break;
		default:
		break;
	}

	return 0;
}


unsigned char bccSpiDma_rxSuccess(uint8_t chl)
{
	unsigned char success = 0;

	switch(chl)
	{
		case e_spi1_dma:
		{
#if (BCC_SPI_DMA_ISR_EN | BCC_SPI_ISR_EN)
			success = spiDma_rxsuccess[0];
			spiDma_rxsuccess[0]  = 0;
#else
			if(DMA_0.TCD[16].CSR.B.DONE == 1)
			{
				DMA_0.CDNE.R = 16;
				DMA_0.INT.R = 0x10000; /****clear interrupt flag****/
				success = 1;
			}
#endif
		}
		break;
		case e_spi2_dma:
		{
#if (BCC_SPI_DMA_ISR_EN | BCC_SPI_ISR_EN)
			success = spiDma_rxsuccess[1];
			spiDma_rxsuccess[1]  = 0;
#else
			if(DMA_0.TCD[15].CSR.B.DONE == 1)
			{
				DMA_0.CDNE.R = 15;
				success = 1;
			}
#endif
		}
		break;
		default:
		break;
	}

	return success;
}


static void bccSpiDma_chl_arbitration (void)
{ /* Use default fixed arbitration */

  DMA_0.CR.R = 0x00000400; /* Fixed priority arbitration for groups, channels */

  DMA_0.DCHPRI[0].R  = 0x00; /* Grp 0 chan 00, no suspension, no premption */
  DMA_0.DCHPRI[1].R  = 0x01; /* Grp 0 chan 01, no suspension, no premption */
  DMA_0.DCHPRI[2].R  = 0x02; /* Grp 0 chan 02, no suspension, no premption */
  DMA_0.DCHPRI[3].R  = 0x03; /* Grp 0 chan 03, no suspension, no premption */
  DMA_0.DCHPRI[4].R  = 0x04; /* Grp 0 chan 04, no suspension, no premption */
  DMA_0.DCHPRI[5].R  = 0x05; /* Grp 0 chan 05, no suspension, no premption */
  DMA_0.DCHPRI[6].R  = 0x06; /* Grp 0 chan 06, no suspension, no premption */
  DMA_0.DCHPRI[7].R  = 0x07; /* Grp 0 chan 07, no suspension, no premption */
  DMA_0.DCHPRI[8].R  = 0x08; /* Grp 0 chan 08, no suspension, no premption */
  DMA_0.DCHPRI[9].R  = 0x09; /* Grp 0 chan 09, no suspension, no premption */
  DMA_0.DCHPRI[10].R = 0x0A; /* Grp 0 chan 10, no suspension, no premption */
  DMA_0.DCHPRI[11].R = 0x0B; /* Grp 0 chan 11, no suspension, no premption */
  DMA_0.DCHPRI[12].R = 0x0C; /* Grp 0 chan 12, no suspension, no premption */
  DMA_0.DCHPRI[13].R = 0x0D; /* Grp 0 chan 13, no suspension, no premption */
  DMA_0.DCHPRI[14].R = 0x0E; /* Grp 0 chan 14, no suspension, no premption */
  DMA_0.DCHPRI[15].R = 0x0F; /* Grp 0 chan 15, no suspension, no premption */

  DMA_0.DCHPRI[16].R = 0x10; /* Grp 1 chan 00, no suspension, no premption */
  DMA_0.DCHPRI[17].R = 0x11; /* Grp 1 chan 01, no suspension, no premption */
  DMA_0.DCHPRI[18].R = 0x12; /* Grp 1 chan 02, no suspension, no premption */
  DMA_0.DCHPRI[19].R = 0x13; /* Grp 1 chan 03, no suspension, no premption */
  DMA_0.DCHPRI[20].R = 0x14; /* Grp 1 chan 04, no suspension, no premption */
  DMA_0.DCHPRI[21].R = 0x15; /* Grp 1 chan 05, no suspension, no premption */
  DMA_0.DCHPRI[22].R = 0x16; /* Grp 1 chan 06, no suspension, no premption */
  DMA_0.DCHPRI[23].R = 0x17; /* Grp 1 chan 07, no suspension, no premption */
  DMA_0.DCHPRI[24].R = 0x18; /* Grp 1 chan 08, no suspension, no premption */
  DMA_0.DCHPRI[25].R = 0x19; /* Grp 1 chan 09, no suspension, no premption */
  DMA_0.DCHPRI[26].R = 0x1A; /* Grp 1 chan 10, no suspension, no premption */
  DMA_0.DCHPRI[27].R = 0x1B; /* Grp 1 chan 11, no suspension, no premption */
  DMA_0.DCHPRI[28].R = 0x1C; /* Grp 1 chan 12, no suspension, no premption */
  DMA_0.DCHPRI[29].R = 0x1D; /* Grp 1 chan 13, no suspension, no premption */
  DMA_0.DCHPRI[30].R = 0x1E; /* Grp 1 chan 14, no suspension, no premption */
  DMA_0.DCHPRI[31].R = 0x1F; /* Grp 1 chan 15, no suspension, no premption */
}


static void bccSpiDma_tcdSpi1(unsigned short int  bufsize,unsigned char dmaen)        /* This is for DSPI_2 Receive */
{
	// Clear the ENBL and TRIG bits of the DMA channel
    DMAMUX_1.CHCFG[0].B.ENBL = 1;
    DMAMUX_1.CHCFG[0].B.TRIG = 0;
    DMAMUX_1.CHCFG[0].B.SOURCE = 0x04;

    // DMA_0 config
    DMA_0.ERQ.R= 0;
    DMA_0.ERQ.B.ERQ16 = 0;    //The DMA request signal for channel 2 is disabled.
    SPI_1.RSER.R = 0x00030000;
	/* SADDR Note: 1 byte is transferred in this TCD on each DMA request (minor loop). */
	/*     The byte is 4th one in the 32-bit POPR register. */
	/*     Therefore a +3 byte offset is added to POPR address for the source address SADDR */
	DMA_0.TCD[16].SADDR.R = ((unsigned int)&SPI_1.POPR.R) + 2; /* Load address of source data */
	DMA_0.TCD[16].ATTR.B.SSIZE = 1;            /* Read 2**0 = 1 byte per transfer */
	DMA_0.TCD[16].ATTR.B.SMOD = 0;             /* Source modulo feature not used  */
	DMA_0.TCD[16].SOFF.R = 0;                  /* After transfer, add 0 to src addr */
	DMA_0.TCD[16].SLAST.R = 0;                 /* No addr adjustment after major loops  */

	DMA_0.TCD[16].DADDR.R = (unsigned int)spi1DmaRxBuffer;	/* Destination address */
	DMA_0.TCD[16].ATTR.B.DSIZE = 1;            /* Write 2**0 = 1 byte per transfer  */
	DMA_0.TCD[16].ATTR.B.DMOD = 0;             /* Destination modulo feature not used  */
	DMA_0.TCD[16].DOFF.R = 2;                  /* Increment destination addr by 1 */
	/* If repeating major loop, subtract NUMBER_OF_BYTES from dest. addr. */
	DMA_0.TCD[16].DLASTSGA.R = 0;              /* No addr adjustment after major loop */

	/* If repeating major loop, set this to 0 to keep the channel enabled */
	DMA_0.TCD[16].CSR.B.DREQ = 1;              /* Disable channel when major loop is done*/
	DMA_0.TCD[16].NBYTES.MLNO.R = 2;           /* NBYTES - Transfer 1 byte per minor loop */
	DMA_0.TCD[16].CITER.ELINKNO.B.ELINK = 0;   /* No Enabling channel LINKing */
	DMA_0.TCD[16].CITER.ELINKNO.B.CITER = bufsize;  /* Init. current interaction count */
	DMA_0.TCD[16].BITER.ELINKNO.B.ELINK = 0;   /* No Enabling channel LINKing */
	DMA_0.TCD[16].BITER.ELINKNO.B.BITER = bufsize;  /* Minor loop iterations */
	DMA_0.TCD[16].CSR.B.MAJORELINK = 0;        /* Dynamic program is not used */
	DMA_0.TCD[16].CSR.B.ESG = 0;               /* Scatter Gather not Enabled */
	DMA_0.TCD[16].CSR.B.BWC = 0;               /* Default bandwidth control- no stalls */
	DMA_0.TCD[16].CSR.B.INTHALF = 0;           /* No interrupt when major count half complete */
	DMA_0.TCD[16].CSR.B.INTMAJOR = dmaen?1:0;          /* No interrupt when major count completes */
	DMA_0.TCD[16].CSR.B.MAJORLINKCH = 0;       /* No link channel # used */
	DMA_0.TCD[16].CSR.B.START = 0;             /* Initialize status flags START, DONE, ACTIVE */
	DMA_0.TCD[16].CSR.B.DONE = 0;
	DMA_0.TCD[16].CSR.B.ACTIVE = 0;
}

static void bccSpiDma_tcdSpi2(unsigned short int  bufsize,unsigned char dmaen)        /* This is for DSPI_2 Receive */
{
	// Clear the ENBL and TRIG bits of the DMA channel
	DMAMUX_0.CHCFG[15].B.ENBL = 1;
    DMAMUX_0.CHCFG[15].B.TRIG = 0;
    DMAMUX_0.CHCFG[15].B.SOURCE = 0x02;

    // DMA_0 config
    DMA_0.ERQ.R= 0;
    DMA_0.ERQ.B.ERQ15 = 0;    //The DMA request signal for channel 2 is disabled.
    SPI_2.RSER.R = 0x00030000;
  /* SADDR Note: 1 byte is transferred in this TCD on each DMA request (minor loop). */
  /*     The byte is 4th one in the 32-bit POPR register. */
  /*     Therefore a +3 byte offset is added to POPR address for the source address SADDR */
  DMA_0.TCD[15].SADDR.R = ((unsigned int)&SPI_2.POPR.R) + 2; /* Load address of source data */
  DMA_0.TCD[15].ATTR.B.SSIZE = 0;            /* Read 2**0 = 1 byte per transfer */
  DMA_0.TCD[15].ATTR.B.SMOD = 0;             /* Source modulo feature not used  */
  DMA_0.TCD[15].SOFF.R = 0;                  /* After transfer, add 0 to src addr */
  DMA_0.TCD[15].SLAST.R = 0;                 /* No addr adjustment after major loops  */

  DMA_0.TCD[15].DADDR.R = (unsigned int)spi2DmaRxBuffer;	/* Destination address */
  DMA_0.TCD[15].ATTR.B.DSIZE = 0;            /* Write 2**0 = 1 byte per transfer  */
  DMA_0.TCD[15].ATTR.B.DMOD = 0;             /* Destination modulo feature not used  */
  DMA_0.TCD[15].DOFF.R = 1;                  /* Increment destination addr by 1 */
             /* If repeating major loop, subtract NUMBER_OF_BYTES from dest. addr. */
  DMA_0.TCD[15].DLASTSGA.R = 0;              /* No addr adjustment after major loop */

             /* If repeating major loop, set this to 0 to keep the channel enabled */
  DMA_0.TCD[15].CSR.B.DREQ = 1;              /* Disable channel when major loop is done*/
  DMA_0.TCD[15].NBYTES.MLNO.R = 1;           /* NBYTES - Transfer 1 byte per minor loop */
  DMA_0.TCD[15].CITER.ELINKNO.B.ELINK = 0;   /* No Enabling channel LINKing */
  DMA_0.TCD[15].CITER.ELINKNO.B.CITER = bufsize;  /* Init. current interaction count */
  DMA_0.TCD[15].BITER.ELINKNO.B.ELINK = 0;   /* No Enabling channel LINKing */
  DMA_0.TCD[15].BITER.ELINKNO.B.BITER = bufsize;  /* Minor loop iterations */
  DMA_0.TCD[15].CSR.B.MAJORELINK = 0;        /* Dynamic program is not used */
  DMA_0.TCD[15].CSR.B.ESG = 0;               /* Scatter Gather not Enabled */
  DMA_0.TCD[15].CSR.B.BWC = 0;               /* Default bandwidth control- no stalls */
  DMA_0.TCD[15].CSR.B.INTHALF = 0;           /* No interrupt when major count half complete */
  DMA_0.TCD[15].CSR.B.INTMAJOR = dmaen?1:0;          /* No interrupt when major count completes */
  DMA_0.TCD[15].CSR.B.MAJORLINKCH = 0;       /* No link channel # used */
  DMA_0.TCD[15].CSR.B.START = 0;             /* Initialize status flags START, DONE, ACTIVE */
  DMA_0.TCD[15].CSR.B.DONE = 0;
  DMA_0.TCD[15].CSR.B.ACTIVE = 0;
}

void bccSpi1Dma_isr_rxcb(void)
{
	DMA_0.INT.R = 0x10000; /****clear interrupt flag****/
	SPI_1.SR.B.RFDF = 1;
	DMA_0.TCD[16].CSR.B.INTMAJOR = 0;
	DMA_0.CDNE.R = 16;
	spiDma_rxsuccess[0]  = 1;
	//BCC_Receive_EndNotification();
}


void bccSpi2Dma_isr_rxcb(void)
{
	DMA_0.INT.R = 0x8000; /****clear interrupt flag****/
	SPI_2.SR.B.RFDF = 1;
	DMA_0.TCD[15].CSR.B.INTMAJOR = 0; /* Disable an interrupt when major iteration count completes */
	//BCC_Receive_EndNotification();
	DMA_0.CDNE.R = 15;
	spiDma_rxsuccess[1]  = 1;
}

void bccSpiDma_transferAbort(uint8_t chl)
{
	switch(chl)
	{
		case e_spi1_dma:
		{
			DMA_0.INT.R = 0x10000; /****clear interrupt flag****/
			SPI_1.SR.B.RFDF = 1;
			DMA_0.TCD[16].CSR.B.INTMAJOR = 0;
		}
		break;
		case e_spi2_dma:
		{
			DMA_0.INT.R = 0x8000; /****clear interrupt flag****/
			SPI_2.SR.B.RFDF = 1;
			DMA_0.TCD[15].CSR.B.INTMAJOR = 0; /* Disable an interrupt when major iteration count completes */

		}
		break;
		default:
		break;
	}
}

void bccSpi1_isrEnable(uint8_t en, uint16_t rx_len)
{
	if(rx_len > NUMBER_OF_BYTES)
	{
		return;
	}
	if(en)
	{
		SPI_1.RSER.R = 0x00020000;
		spi1DmaRxBufSize = 0;
		spi1RxLen = rx_len;
	}
	else
	{
		SPI_1.RSER.R = 0x00000000;
	}
}

void bccSpi2_isrEnable(uint8_t en, uint16_t rx_len)
{
	if(rx_len > NUMBER_OF_BYTES)
	{
		return;
	}
	if(en)
	{
		SPI_2.RSER.R = 0x00020000;
		spi2DmaRxBufSize = 0;
		spi2RxLen = rx_len;
	}
	else
	{
		SPI_2.RSER.R = 0x00000000;
	}
}


void bccSpi1_isr_rxcb(void)
{
	while(SPI_1.SR.B.RXCTR)			//SR.B.RXCTR)
	{
		spi1DmaRxBuffer[spi1DmaRxBufSize++] = (uint16_t)SPI_1.POPR.R;
	}

	if(spi1DmaRxBufSize == spi1RxLen)
	{
		spiDma_rxsuccess[0]  = 1;
	}
	SPI_1.SR.B.RFDF = 1;
	//BCC_Receive_EndNotification();
}

void bccSpi2_isr_rxcb(void)
{
	while(SPI_2.SR.B.RXCTR)			//SR.B.RXCTR)
	{
		spi2DmaRxBuffer[spi2DmaRxBufSize++] = (uint16_t)SPI_2.POPR.R;
	}

	if(spi2DmaRxBufSize == spi2RxLen)
	{
		spiDma_rxsuccess[1]  = 1;
	}
	SPI_2.SR.B.RFDF = 1;
	//BCC_Receive_EndNotification();
}



