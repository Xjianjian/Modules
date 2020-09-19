
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "MPC5744P.h"
#include "SIUL.h"
#include "defines.h"
#include "math.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "spi_pri.h"
#include "spi.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static uint32_t spi_roundBaudRate(uint32_t pureBR);
static bool spi_isValidModuleCs(uint8_t module, uint8_t cs);


/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void spi_init(uint8_t module,spiModuleCfg_t ModcfgInfo)		
{
	uint32_t p1 = 0;
    uint32_t realBR = 0;
    uint32_t realBR1 = 0;
    uint32_t realBR2 = 0;
    uint32_t realBR3 = 0;
    uint32_t realBR4 = 0;

    int16_t errorBR = 32767;/*undesirable value of error*/ 
    int16_t errorBR1 = 0;
    int16_t errorBR2 = 0;
    int16_t errorBR3 = 0;
    int16_t errorBR4 = 0;
    uint8_t binaryPBR = 0;
    uint8_t binaryBR = 0;

	uint8_t slvIndx = 0;
	volatile struct SPI_tag *p_DSPI = NULL;
	uint8_t       slvCfgIndx;
	uint8_t       CfgInfoIndx;
	spiExSlvCfg_t slvCfgInfo;
	
	m_InParaCheck(m_spi_isValidModule(module), m_plat_spiModuleId, m_spi_initFunId,	m_plat_spiModuleErr);

	//m_InParaCheck(m_spi_isValidFrameBits(cfgInfo.frameBits), m_plat_spiModuleId, m_spi_initFunId,	m_plat_spiFrameSizeErr);
	switch(module)
	{
		case 0:p_DSPI = &SPI_0;break;
		case 1:p_DSPI = &SPI_1;break;
		case 2:p_DSPI = &SPI_2;break;
		case 3:p_DSPI = &SPI_3;break;
		default:p_DSPI = NULL;break;
	}
	if(NULL != p_DSPI)
	{
		p_DSPI->MCR.B.MSTR = ModcfgInfo.master;	/* Set DSPIx in mode Slave-0 or Master-1*/
	    p_DSPI->MCR.B.MDIS=0;    				/* Enable clock*/
	    p_DSPI->MCR.B.HALT=0;					/* Allow transfer*/
	    p_DSPI->MCR.B.PCSIS = 0x7F;				/* CS0-6 active Low*/
	    p_DSPI->RSER.R = ModcfgInfo.intMsk;		/*interrupt mask*/
		p_DSPI->MCR.B.FRZ = 0;
		/*FIFOs*/
	    if(TRUE == ModcfgInfo.fifoEna)
	    {
			p_DSPI->MCR.B.DIS_TXF = 0;		/*enable Tx FIFOs*/
	    	p_DSPI->MCR.B.DIS_RXF = 0;		/*enable Rx FIFOs*/
		}
		else
		{
			p_DSPI->MCR.B.DIS_TXF = 1;		/*disable Tx FIFOs*/
	    	p_DSPI->MCR.B.DIS_RXF = 1;		/*disable Rx FIFOs*/
		}
		
		if(TRUE == ModcfgInfo.master)
	    {
			for(slvIndx=0;slvIndx < ModcfgInfo.slvCnt;slvIndx ++)
			{
				slvCfgInfo = ModcfgInfo.cfgInfo[ModcfgInfo.cfgIndx[slvIndx]];
				slvCfgIndx = ModcfgInfo.cfgIndx[slvIndx];
				p1 = m_spi_inClkFreq/slvCfgInfo.baud;
				/*prescaler PBR = 00*/ 
				errorBR = 32767;/*undesirable value of error*/
				if(p1>=RATIO_MIN1 && p1<=RATIO_MAX1)
		        {						
		            realBR1 = p1*(1/PBR_VALUE1);
		            realBR1 = spi_roundBaudRate(realBR1);
		            errorBR1 = slvCfgInfo.baud - (m_spi_inClkFreq/(PBR_VALUE1*realBR1));
		            errorBR1 = (int16_t)fabs(errorBR1);
		            if(errorBR1<errorBR)
		            {
		                errorBR = errorBR1;
		                realBR = realBR1;	
		                binaryPBR = PBR1;		
		            }
		        }		

		        /*prescaler PBR = 01*/
		        if(p1>=RATIO_MIN2 && p1<=RATIO_MAX2)
		        {
		            realBR2 = p1*(1/PBR_VALUE2);	
		            realBR2 = spi_roundBaudRate(realBR2);
		            errorBR2 = slvCfgInfo.baud - (m_spi_inClkFreq/(PBR_VALUE2*realBR2));	
		            errorBR2 = (int16_t)fabs(errorBR2);
		            if(errorBR2<errorBR)
		            {
		                errorBR = errorBR2;
		                realBR = realBR2;	
		                binaryPBR = PBR2;		
		            }
		        }

		        /*prescaler PBR = 10*/
		        if(p1>=RATIO_MIN3 && p1<=RATIO_MAX3)
		        {
		            realBR3 = p1*(1/PBR_VALUE3);
		            realBR3 = spi_roundBaudRate(realBR3);
		            errorBR3 = slvCfgInfo.baud - (m_spi_inClkFreq/(PBR_VALUE3*realBR3));
		            errorBR3 = (int16_t)fabs(errorBR3);
		            if(errorBR3<errorBR)
		            {
		                errorBR = errorBR3;
		                realBR = realBR3;
		                binaryPBR = PBR3;			
		            }
		        }

		        /*prescaler PBR = 11*/
		        if(p1>=RATIO_MIN4 && p1<=RATIO_MAX4)
		        {
		            realBR4 = p1*(1/PBR_VALUE4);
		            realBR4 = spi_roundBaudRate(realBR4);
		            errorBR4 = slvCfgInfo.baud - (m_spi_inClkFreq/(PBR_VALUE4*realBR4));
		            errorBR4 = (int16_t)fabs(errorBR4);
		            if(errorBR4<errorBR)
		            {
		                errorBR = errorBR4;
		                realBR = realBR4;	
		                binaryPBR = PBR4;		
		            }
		        }

		        /*choose the BR value*/
		        if(p1<RATIO_MIN1 || p1>RATIO_MAX4)
		        {
		        /*out of bounds*/
		            if(p1<RATIO_MIN1)
		            {							
		                binaryBR = BR1;										/*lower than minimum*/
		                binaryPBR = PBR1;
		            }
		            else
		            {
		                binaryBR = BR16;									/*greater than maximum*/
		                binaryPBR = PBR4;
		            }
		        }
		        else
		        {															/*in the bounds*/
		            switch(realBR)
		            {          
		                case BR_VALUE1	: binaryBR = BR1;	break;
		                case BR_VALUE2  : binaryBR = BR2;	break;
		                case BR_VALUE3  : binaryBR = BR3;	break;	
		                case BR_VALUE4  : binaryBR = BR4;	break;
		                case BR_VALUE5  : binaryBR = BR5;	break;
		                case BR_VALUE6  : binaryBR = BR6;	break;
		                case BR_VALUE7  : binaryBR = BR7;	break;
		                case BR_VALUE8  : binaryBR = BR8;	break;
		                case BR_VALUE9  : binaryBR = BR9;	break;
		                case BR_VALUE10	: binaryBR = BR10;	break;
		                case BR_VALUE11	: binaryBR = BR11;	break;
		                case BR_VALUE12	: binaryBR = BR12;	break;
		                case BR_VALUE13	: binaryBR = BR13;	break;
		                case BR_VALUE14	: binaryBR = BR14;	break;
		                case BR_VALUE15	: binaryBR = BR15;	break;
		                case BR_VALUE16	: binaryBR = BR16;	break;
		            }		
		        }

			    p_DSPI->MODE.CTAR[slvCfgIndx].B.DBR = 0; 	/* double baud rate disable and with 50/50 duty*/
				/* Frame size (value+1)*/
			    p_DSPI->MODE.CTAR[slvCfgIndx].B.FMSZ = slvCfgInfo.frameLen - 1; 
				/* Polarity CPOL=0=>sck is valid @1*/
			    p_DSPI->MODE.CTAR[slvCfgIndx].B.CPOL = 
			    ((TRUE == slvCfgInfo.sckHighValid)?(0):(1)); 
				/* Phase CPHA=0=>data captured on leading edge */
			    p_DSPI->MODE.CTAR[slvCfgIndx].B.CPHA = 
			    ((TRUE == slvCfgInfo.captOnRiseEdge)?(0):(1));
				p_DSPI->MODE.CTAR[slvCfgIndx].B.LSBFE = ((TRUE == slvCfgInfo.MsbFirst)?(0):(1));; 							/* MSB first if 0*/

				p_DSPI->MODE.CTAR[slvCfgIndx].B.PBR = binaryPBR;					/*Baud rate*/
		        p_DSPI->MODE.CTAR[slvCfgIndx].B.BR = binaryBR;
				p_DSPI->MODE.CTAR[slvCfgIndx].B.PCSSCK = PRESC_VAL1;				/*tcsc*/
			    p_DSPI->MODE.CTAR[slvCfgIndx].B.CSSCK = SCALER3;

			    p_DSPI->MODE.CTAR[slvCfgIndx].B.PDT = PRESC_VAL3;					/*tdt*/
			    p_DSPI->MODE.CTAR[slvCfgIndx].B.DT = SCALER4;

			    p_DSPI->MODE.CTAR[slvCfgIndx].B.PASC = PRESC_VAL1;					/*tasc*/
			    p_DSPI->MODE.CTAR[slvCfgIndx].B.ASC = SCALER3;
			}
			p_DSPI->PUSHR.PUSHR.B.CTAS = 0;											/*begin with the first external slave device*/
	    }
	    /*Configuration of ports*/
	    switch(module)
	    {
			case 0 :
				if(TRUE == ModcfgInfo.master)
	            {   /* as a Master*/
	                SIUL_DigitalInput(SIUL_PC7, SIUL_IN_PC7, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      /* SIN_0 	->IN*/
	                SIUL_DigitalOutput(SIUL_PC6, SIUL_ALT1);                    							/* SOUT_0	->OUT*/
	                SIUL_DigitalOutput(SIUL_PC5, SIUL_ALT1);                    							/* SCK_0	->OUT*/
	            }
	            else
	            {	/*as a Slave*/

	            }
			break;
		
	        case 1 	:
	            if(TRUE == ModcfgInfo.master)
	            {	/*as a Master*/
	                SIUL_DigitalInput(SIUL_PA8, SIUL_IN_PA8, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      /* SIN_1 	->IN*/
	                SIUL_DigitalOutput(SIUL_PA7, SIUL_ALT1);                    							/* SOUT_1	->OUT*/
	                SIUL_DigitalOutput(SIUL_PA6, SIUL_ALT1);                  	  							/* SCK_1	->OUT*/
	            }
	            else
	            {
	            	/*as a Slave*/
	            }
	            break;
	        case 2 	:
	            break;	
			case 3 :
	            break;	
	        default : 
			break;
	    }
		if(TRUE == ModcfgInfo.master)
		{
			for(slvIndx=0;slvIndx < ModcfgInfo.slvCnt;slvIndx ++)
			{
				SIUL_DigitalOutput(ModcfgInfo.csMscr[slvIndx], ModcfgInfo.csAltLv[slvIndx]);                   /* CS0_0	->OUT*/
			}
			for(CfgInfoIndx = 0;CfgInfoIndx < ModcfgInfo.cfgInfoCnt;CfgInfoIndx ++)
			{
				p_DSPI->MODE.CTAR[CfgInfoIndx].B.PCSSCK = PRESC_VAL1;			/*tcsc*/
			    p_DSPI->MODE.CTAR[CfgInfoIndx].B.CSSCK = SCALER3;

			    p_DSPI->MODE.CTAR[CfgInfoIndx].B.PDT = PRESC_VAL3;			/*tdt*/
			    p_DSPI->MODE.CTAR[CfgInfoIndx].B.DT = SCALER4;

			    p_DSPI->MODE.CTAR[CfgInfoIndx].B.PASC = PRESC_VAL1;			/*tasc*/
			    p_DSPI->MODE.CTAR[CfgInfoIndx].B.ASC = SCALER3;
			}
		}
		else
		{
			slvCfgInfo = ModcfgInfo.cfgInfo[ModcfgInfo.cfgIndx[0]];

		    p_DSPI->MODE.CTAR[0].B.FMSZ = slvCfgInfo.frameLen - 1;
		    p_DSPI->MODE.CTAR[0].B.CPOL = ((TRUE == slvCfgInfo.sckHighValid)?(0):(1));
		    p_DSPI->MODE.CTAR[0].B.CPHA = ((TRUE == slvCfgInfo.captOnRiseEdge)?(0):(1));

			p_DSPI->MODE.CTAR[0].B.PCSSCK = PRESC_VAL1;			/*tcsc*/
		    p_DSPI->MODE.CTAR[0].B.CSSCK = SCALER3;

		    p_DSPI->MODE.CTAR[0].B.PDT = PRESC_VAL3;			/*tdt*/
		    p_DSPI->MODE.CTAR[0].B.DT = SCALER4;

		    p_DSPI->MODE.CTAR[0].B.PASC = PRESC_VAL1;			/*tasc*/
		    p_DSPI->MODE.CTAR[0].B.ASC = SCALER3;

			p_DSPI->PUSHR.PUSHR.B.CTAS = 0;					/*for the slave module, only CTAS =0  is allowed*/
		}
	}					
}


uint8_t spi_sendDt(uint8_t module,uint8_t ctas,uint8_t csMask,uint16_t data,bool csHold, bool waitSendOver)
{
	uint8_t sendRslt = m_spi_sendNoErr;
	uint32_t sendDt = 0;
	volatile struct SPI_tag * p_DSPI = NULL;
	//bool bufEmpty = FALSE;
	uint16_t timeLeft = 100;
	m_InParaCheck(m_spi_isValidModule(module), m_plat_spiModuleId, m_spi_sendFunId,	m_plat_spiModuleErr);
	m_InParaCheck(m_spi_isValidCtas(ctas),     m_plat_spiModuleId, m_spi_sendFunId,	m_plat_spiModuleErr);

	switch(module)
	{
		case 0:p_DSPI = &SPI_0;break;
		case 1:p_DSPI = &SPI_1;break;
		case 2:p_DSPI = &SPI_2;break;
		case 3:p_DSPI = &SPI_3;break;
		default:p_DSPI = NULL;break;
	}
	if((TRUE == spi_isValidModuleCs(module,csMask)) &&  (NULL != p_DSPI))
	{
#if 0
		if(0 == p_DSPI->MCR.B.DIS_TXF) 
		{
			if(p_DSPI->SR.B.TXCTR < 5)
			{
				p_DSPI->SR.B.TCF = 1;
				bufEmpty = TRUE;		
			}
		}
		else
		{
			bufEmpty = TRUE;
		}
		if(TRUE == bufEmpty)
		{
#endif
			(void)p_DSPI->POPR.R;

			p_DSPI->SR.R = 0x90020000;

			(void)p_DSPI->POPR.R;

			sendDt =((TRUE == csHold)?(0x80000000ul):(0));
			sendDt |= (csMask << 16) & 0xFF0000;
			sendDt |= ((uint32_t)ctas << 28);
			sendDt |= data;
			p_DSPI->PUSHR.PUSHR.R = (p_DSPI->PUSHR.PUSHR.R & 0x0F000000) | sendDt;
			if(TRUE == waitSendOver)
			{
				while((p_DSPI->SR.B.TCF != 1) && (timeLeft > 0))
				{
					timeLeft --;
				}
				p_DSPI->SR.B.TCF = 1;
				if(0 == timeLeft)
				{
					sendRslt= m_spi_sendWaitOverTimeout;
				}
			}

#if 0
		}
		else
		{
			sendRslt = m_spi_sendBusy;
		}
#endif
	}
	else
	{
		sendRslt = m_spi_sendParaErr;
	}
	return sendRslt;
}

uint8_t spi_readDt(uint8_t module,uint32_t * dt)
{
	uint32_t readDt = 0;
	uint8_t readRslt = m_spi_readNoErr;
	volatile struct SPI_tag * p_DSPI = NULL;
	m_InParaCheck(m_spi_isValidModule(module), m_plat_spiModuleId, m_spi_readFunId,	m_plat_spiModuleErr);
	switch(module)
	{
		case 0:p_DSPI = &SPI_0;break;
		case 1:p_DSPI = &SPI_1;break;
		case 2:p_DSPI = &SPI_2;break;
		case 3:p_DSPI = &SPI_3;break;
		default:p_DSPI = NULL;break;
	}
	if(NULL != p_DSPI)
	{
		if(1 == p_DSPI->SR.B.RFDF)
		{
			readDt = p_DSPI->POPR.R;					/*get received data*/
			/*clear receive flag*/
			p_DSPI->SR.B.RFDF = 1;
			p_DSPI->SR.B.TCF = 1;
			if(NULL != dt)
			{
				*dt = readDt;
			}
			else
			{
				readRslt= m_spi_readBufNull;
			}
		}
		else
		{
			readRslt= m_spi_readNoDt;
		}
	}
	else
	{
		readRslt= m_spi_readParaErr;
	}
	return readRslt;
}

uint8_t spi_readDtBtye(uint8_t module,uint8_t * dt)
{
	uint32_t readDt = 0;
	uint8_t readRslt = m_spi_readNoErr;
	volatile struct SPI_tag * p_DSPI = NULL;
	m_InParaCheck(m_spi_isValidModule(module), m_plat_spiModuleId, m_spi_readFunId,	m_plat_spiModuleErr);
	switch(module)
	{
		case 0:p_DSPI = &SPI_0;break;
		case 1:p_DSPI = &SPI_1;break;
		case 2:p_DSPI = &SPI_2;break;
		case 3:p_DSPI = &SPI_3;break;
		default:p_DSPI = NULL;break;
	}
	if(NULL != p_DSPI)
	{
		if(1 == p_DSPI->SR.B.RFDF)
		{
			readDt = p_DSPI->POPR.R;					/*get received data*/
			/*clear receive flag*/
			p_DSPI->SR.B.RFDF = 1;
			p_DSPI->SR.B.TCF = 1;
			if(NULL != dt)
			{
				*dt = readDt;
			}
			else
			{
				readRslt= m_spi_readBufNull;
			}
		}
		else
		{
			readRslt= m_spi_readNoDt;
		}
	}
	else
	{
		readRslt= m_spi_readParaErr;
	}
	return readRslt;
}


void spi_switchMasterCfg(uint8_t module,uint8_t cfgIndx)
{
	volatile struct SPI_tag * p_DSPI = NULL;
	m_InParaCheck(m_spi_isValidModule(module), m_plat_spiModuleId, m_spi_swMasterCfgFunId,	m_plat_spiModuleErr);
	switch(module)
	{
		case 0:p_DSPI  = &SPI_0;break;
		case 1:p_DSPI = &SPI_1;break;
		case 2:p_DSPI = &SPI_2;break;
		case 3:p_DSPI = &SPI_3;break;
		default:p_DSPI = NULL;break;
	}
	if(NULL != p_DSPI) 
	{

		//pcs = p_DSPI->PUSHR.PUSHR.B.PCS;
		p_DSPI->PUSHR.PUSHR.B.PCS = 0;
		//p_DSPI->MCR.B.HALT = 1;
		if(p_DSPI->MCR.B.MSTR >0)
		{
			p_DSPI->PUSHR.PUSHR.B.CTAS = cfgIndx;
		}
		else
		{
			p_DSPI->PUSHR.PUSHR.B.CTAS = 0;
		}

		//p_DSPI->MCR.B.HALT = 0;
		//p_DSPI->PUSHR.PUSHR.B.PCS = pcs;
	}
}

void spi_cmdContinueMode(uint8_t module, functionSt_t funSt)
{
	volatile struct SPI_tag * p_DSPI = NULL;
	m_InParaCheck(m_spi_isValidModule(module), m_plat_spiModuleId, m_spi_cmdContinueMode,	m_plat_spiModuleErr);
	m_InParaCheck(m_plat_isFunctionState(funSt), m_plat_spiModuleId, m_spi_cmdContinueMode,	m_plat_funStErr);
	switch(module)
	{
		case 0:p_DSPI  = &SPI_0;break;
		case 1:p_DSPI = &SPI_1;break;
		case 2:p_DSPI = &SPI_2;break;
		case 3:p_DSPI = &SPI_3;break;
		default:p_DSPI = NULL;break;
	}
	if(NULL != p_DSPI) 
	{
		if(em_funSt_on == funSt)
		{
			p_DSPI->PUSHR.PUSHR.B.CONT = 1;
		}
		else
		{
			p_DSPI->PUSHR.PUSHR.B.CONT = 0;
		}
	}
}


/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static uint32_t spi_roundBaudRate(uint32_t pureBR)/*IN - computed baud rate  OUT - baud rate from a list*/
{					
	uint32_t BR = 0;
	if(pureBR>=2			&& pureBR<3)			BR = 2;
	if(pureBR>=3			&& pureBR<5)			BR = 4;
	if(pureBR>=5			&& pureBR<7)			BR = 6;
	if(pureBR>=7			&& pureBR<12)			BR = 8;
	if(pureBR>=12			&& pureBR<24)			BR = 16;
	if(pureBR>=24			&& pureBR<48)			BR = 32;
	if(pureBR>=48			&& pureBR<96)			BR = 64;
	if(pureBR>=96			&& pureBR<192)			BR = 128;
	if(pureBR>=192			&& pureBR<384)			BR = 256;
	if(pureBR>=384			&& pureBR<768)			BR = 512;
	if(pureBR>=768			&& pureBR<1536) 		BR = 1024;
	if(pureBR>=1536 		&& pureBR<3072) 		BR = 2048;
	if(pureBR>=3072 		&& pureBR<6144) 		BR = 4096;
	if(pureBR>=6144 		&& pureBR<12288)		BR = 8192;
	if(pureBR>=12288		&& pureBR<24576)		BR = 16384;
	if(pureBR>=24576		&& pureBR<32768)		BR = 32768;
	return BR;
}

static bool spi_isValidModuleCs(uint8_t module, uint8_t cs)
{
	bool retVal = TRUE;
	switch(module)
	{
		case 0:
		case 1:
		case 2:
			retVal = ((cs > 0) && (cs <= 0xFF))?(TRUE):(FALSE);
			break;
		default:	
			break;
	}
	return retVal;
}



