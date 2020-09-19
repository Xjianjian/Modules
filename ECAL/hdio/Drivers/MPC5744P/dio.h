
#ifndef DIO_H_
#define DIO_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "Defines.h"
#include "SIUL.h"


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_dio_input				0
#define m_dio_output				1
#define m_dio_biDir				2

#define m_dio_outNormal			0
#define m_dio_outOpenDrain			1

#define m_dio_pullDisable			0
#define m_dio_pullUp				1
#define m_dio_pullDown				2

/*Attention:during the valid channel ID range, some of them still not exist,please check the datasheet for details*/
#define m_dio_ChanneIdMax			155	
/*every time dioChannel_t been modified, below 6 macros should do some correspongding modification */
#define m_dio_chanPortStartBit		4
#define m_dio_ChanPortBitLen		4
#define m_dio_chanPinStartBit		0
#define m_dio_ChanPinBitLen		4
#define m_dio_ChanPort(id)			(((id) >> m_dio_chanPortStartBit) &  0x0F)
#define m_dio_ChanPin(id)			(((id) >> m_dio_chanPinStartBit) &  0x0F)


#define m_dio_isValidChanId(id)		(((id)>=0) && ((id) <= m_dio_ChanneIdMax))
#define m_dio_isValidDir(dir)			((m_dio_input == (dir)) || (m_dio_output ==(dir)) || (m_dio_biDir ==(dir)) )
#define m_dio_isValidOutType(outType)	((m_dio_outNormal == (outType)) || (m_dio_outOpenDrain == (outType)))
#define m_dio_isPullType(pulltype)		((m_dio_pullDisable == (pulltype)) || (m_dio_pullUp == (pulltype)) || (m_dio_pullDown == (pulltype)))


#define m_dio_setDirFunId					0
#define m_dio_setOutAbilityFunId			1
#define m_dio_safeModeCmdFunId				2
#define m_dio_setPullTypeFunId				3
#define m_dio_readValFunId					4
#define m_dio_writeValFunId					5
#define m_plat_toggleChanFunId				6
#define m_dio_setHysteresisFunId			7
#define m_dio_setSlewCtrlFunId				8



/*relevant registers and bits mask definition*/
#define m_dio_mscrSSSMask			0x0000000Ful
#define m_dio_mscrOBEMask			0x02000000ul
#define m_dio_mscrOIEMask			0x00080000ul
#define m_dio_mscrPUSMask			0x00010000ul
#define m_dio_mscrHYSMask			0x00040000ul
#define m_dio_mscrSRSMask			0x30000000ul

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef uint8_t dioChannel_t;
typedef uint8_t dioDir_t;
typedef uint8_t dioOutType_t;
typedef uint8_t dioPullType_t;
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
extern void dio_setDir(dioChannel_t chanId, dioDir_t dir);
extern void dio_setOutAbility(dioChannel_t chanId, dioOutType_t outType);
extern void dio_safeModeCmd(dioChannel_t chanId,functionSt_t funSt);
extern void dio_setPullType(dioChannel_t chanId,dioPullType_t pullType);
extern void dio_setHysteresis(dioChannel_t chanId,functionSt_t funSt);
extern void dio_setSlewCtrl(dioChannel_t chanId,functionSt_t funSt);
extern uint8_t dio_readVal(dioChannel_t chanId);
extern bool dio_writeVal(dioChannel_t chanId,uint8_t val, bool readBackVerify);
extern bool dio_toggleChan(dioChannel_t chanId,bool readBackVerify);

#endif



