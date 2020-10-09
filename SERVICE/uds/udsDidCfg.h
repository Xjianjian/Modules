/*
* Copyright (c) File,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:udsDidCfg.h
* Description: 
* REQ IDs: 
* History:
* 2017-03-31, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef UDS_DID_CFG_H_
#define UDS_DID_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/

typedef struct
{
	uint16_t did;
	uint16_t dlc;
    void * dtCache;
    uint32_t secLv;
    void (*cb)(void);
}uds_didRdCfg_t;


typedef struct
{
	uint16_t did;
	uint16_t dlc;
	uint8_t * dtCache;
    uint32_t secLv;
    void (*cb)(void);
}uds_didWtCfg_t;


/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/





/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const uint16_t uds_didRdCnt;
extern const uds_didRdCfg_t uds_didRdCfgGrp[];
extern const uint16_t uds_didWtCnt;
extern const uds_didWtCfg_t uds_didWtCfgGrp[];
/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


#endif

