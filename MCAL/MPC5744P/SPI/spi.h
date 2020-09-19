

#ifndef SPI_H_
#define SPI_H_
#include "typedefs.h"
#include "platformTypes.h"
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_spi_module0			0		
#define m_spi_module1			1	
#define m_spi_module2			2	
#define m_spi_module3 			3	

/*Interrupts*/
#define TCFRE	0x80000000
#define EOQFRE	0x10000000
#define TFUFRE	0x08000000
#define TFFFRE	0x02000000
#define RFOFRE	0x00080000
#define RFDFRE	0x00020000


#define m_spi_sendNoErr					0
#define m_spi_sendBusy					1
#define m_spi_sendParaErr				2
#define m_spi_sendWaitOverTimeout		3

#define m_spi_readNoErr					0
#define m_spi_readNoDt					1
#define m_spi_readParaErr				2
#define m_spi_readBufNull				3

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	uint32_t baud;
	uint8_t  frameLen;					/*unit:bit*/	
	bool	 sckHighValid;				/*TRUE: the sclk high at 1, othersie at 0*/	
	bool 	 captOnRiseEdge;			/*TRUE: capture at rise edge, othersie at fall edge*/
	bool	 MsbFirst;					
}spiExSlvCfg_t;

typedef struct
{	
	/*common region*/
	bool 				master;				/*TRUE: Master device, otherwise slave device*/
	uint32_t 			intMsk;				/*interupt mask*/
	bool	 			fifoEna;			/*if fifo enable,both RXFIFO and TXFIFO*/
	bool	 			dmaEna;	
	/*master region*/
	uint8_t				slvCnt;				/*the slave devices count to connected with this master*/
	uint8_t 			cfgInfoCnt;			/*when connected with different slave, the master maybe need  different configurations*/
	const uint8_t		*cfgIndx;			/*the value pointed by this pointer must be less than cfgInfoCnt, and this array length must be equal to slaveCnt*/
	const uint8_t		*csMscr;			/*the mscr index which used to initialize the cs Pin, this array length must be equal to slaveCnt*/
	const uint8_t		*csAltLv;			/*the IMCR index which used to initialize the cs Pin, this array length must be equal to slaveCnt*/
	const spiExSlvCfg_t	*cfgInfo;			/*the array length must be equal to cfgInfoCnt*/

	/*slave region */
	uint8_t				slvFrameLen;		/*unit:bit*/
	bool	 			slvSckHighValid;	/*TRUE: the sclk high at 1, othersie at 0*/
	bool 	 			slvCaptOnRiseEdge;	/*TRUE: capture at rise edge, othersie at fall edge*/			
}spiModuleCfg_t;


/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void spi_init(uint8_t module,spiModuleCfg_t ModcfgInfo);
extern uint8_t spi_sendDt(uint8_t module,uint8_t ctas,uint8_t csMask,uint16_t data,bool csHold, bool waitSendOver);
extern uint8_t spi_readDt(uint8_t module,uint32_t * dt);
extern void spi_switchMasterCfg(uint8_t module,uint8_t cfgIndx);
extern void spi_cmdContinueMode(uint8_t module, functionSt_t funSt);
extern uint8_t spi_readDtBtye(uint8_t module,uint8_t * dt);
#endif


