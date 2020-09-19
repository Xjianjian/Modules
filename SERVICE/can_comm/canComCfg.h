
#ifndef _CAN_COM_CFG_H_
#ifndef _CAN_COM_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_hcan_mboxFix   0		/*one message fixed map to one mbox,and never change,both rx and tx*/
#define m_hcan_mboxDyna  1		/*all the rx messages share the Mb0, all the tx messages share the Mb1, and both Rx isr and Tx Isr must be enabled*/


#define m_dLink_callPeriod			(1)						/*unit:ms*/
#define m_dLink_txTimeOut			(20/m_dLink_callPeriod)		/*unit:m_dLink_callPeriod ms*/

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/


typedef struct
{
	bool 	 mboxAssignedOk;
	uint8_t	 MboxAssignWay;
}hcanMboxAssign_t;



/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void nt_recIndication(uint32_t rxId, uint8_t rxDLC,uint8_t * rxBuf);
#endif



