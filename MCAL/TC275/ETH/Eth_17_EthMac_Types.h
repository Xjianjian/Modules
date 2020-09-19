/******************************************************************************
**                                                                           **
** Copyright (C) Infineon Technologies (2013)                                **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to Infineon      **
** Technologies. Passing on and copying of this document, and communication  **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
*******************************************************************************
**                                                                           **
**  $FILENAME   : Eth_17_EthMac_Types.h $                                    **
**                                                                           **
**  $CC VERSION : \main\17 $                                                 **
**                                                                           **
**  $DATE       : 2014-10-31 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains                                         **
**                 - functionality of Eth driver.                            **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/

#ifndef ETH_17_ETHMAC_TYPES_H
#define ETH_17_ETHMAC_TYPES_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/


/* Include Communication Stack Types */
#include "ComStack_Types.h"

/* Pre-compile/static configuration parameters for ETH                        */
#include "Eth_17_EthMac_Cfg.h"  


/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID= DS_AS_ETH_PR5162,DS_AS_ETH_PR5163,DS_AS_ETH_PR5164,
                     DS_AS_ETH_PR5165,DS_AS_ETH_PR5166]                       **
**  [/cover]                                                                  **
*******************************************************************************/
typedef uint16 Eth_FrameType;

typedef uint8 Eth_DataType ;

typedef enum Eth_ModeType
{
  ETH_MODE_DOWN   = 0x0U,
  ETH_MODE_ACTIVE = 0x1U
}Eth_ModeType;

typedef enum Eth_17_EthMac_StateType
{
  ETH_STATE_UNINIT   = 0x0U,
  ETH_STATE_INIT     = 0x1U,
  ETH_STATE_ACTIVE   = 0x2U
}Eth_17_EthMac_StateType;

/* Added for compatibility with AS4.1 */
typedef enum
{
  ETH_RECEIVED = 0x0U,
  ETH_NOT_RECEIVED,
  ETH_RECEIVED_MORE_DATA_AVAILABLE
} Eth_RxStatusType;

typedef enum
{
  ADD_TO_FILTER = 0x0U,
  REMOVE_FROM_FILTER
} Eth_FilterActionType;

/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/
/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/

/*******************************************************************************
**                            LOW Level Driver Definitions                    **
*******************************************************************************/

/*******************************************************************************
**                            LOW Level Driver Definitions                    **
*******************************************************************************/


#endif /* ETH_17_ETHMAC_TYPES_H */

