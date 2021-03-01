/********************************************************************************/
/********************************************************************************/
/*******PROJECT :                                  ******/
/*******FUNCTION: TLE9104SH DRIVER                 ******/
/*******DATA    : 2019-12-12                       ******/
/*******AUTHOR  : X-Mad                            ******/
/********************************************************************************/
/********************************************************************************/

#ifndef __BCC_SPI_DMA_H__
#define __BCC_SPI_DMA_H__
#include "typedefs.h"
#include "platformTypes.h"

/*************Registers  Adress****************/

/**********************************************/
#define NUMBER_OF_BYTES 1266
#define BCC_SPI_DMA_ISR_EN	0
#define BCC_SPI_ISR_EN		1

/*****************Relay Channel****************/

/**********************************************/
typedef enum
{
	e_spi1_dma  = 0,
	e_spi2_dma
}bccSpiDma_obj;


/****************Relay Status******************/


/**********************************************/
extern unsigned short int spi1DmaRxBuffer[NUMBER_OF_BYTES];
extern unsigned short int spi2DmaRxBuffer[NUMBER_OF_BYTES];

/*******************Function*******************/

/**********************************************/
extern void bccSpiDma_Init(void);
extern void bccSpiDma_task(void);
extern unsigned char bccSpiDma_rxSuccess(uint8_t chl);
extern int bccSpiDma_startTransfer(uint8_t chl);
extern int bccSpiDma_resetSpiDMA(uint8_t chl,unsigned short int bufsize,uint8_t dmaen);
extern void bccSpi1Dma_isr_rxcb(void);
extern void bccSpi2Dma_isr_rxcb(void);
extern void bccSpi1_isrEnable(uint8_t en,uint16_t rx_len);
extern void bccSpi2_isrEnable(uint8_t en, uint16_t rx_len);
extern void bccSpi1_isr_rxcb(void);
extern void bccSpi2_isr_rxcb(void);
#endif 
