/*******************************************************************************
**                                                                            **
** Copyright (C) Infineon Technologies (2016)                                 **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This is a demo file and the contents of this file has to be modified by the**
** customer                                                                   **
**                                                                            **
********************************************************************************
**                                                                            **
**  $FILENAME   : Eth_17_EthMac_Protect.h $                                  **
**                                                                            **
**   $CC VERSION : \main\4 $                                                  **
**                                                                            **
**   $DATE       : 2016-05-07 $                                               **
**                                                                            **
**   AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                            **
**   VENDOR      : Infineon Technologies                                      **
**                                                                            **
**  DESCRIPTION  : This file file define all the first level                  **
**                 indirection macros for Protected mode support              **
**                                                                            **
**                                                                            **
**  MAY BE CHANGED BY USER [Yes/No]: Yes                                      **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                                                                            **
** Traceabilty      :                                                         **
**                                                                            **
*******************************************************************************/

#ifndef ETH_PROTECT_CFG_H
#define ETH_PROTECT_CFG_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/* Inclusion of McalOsConfig.h */
#include "McalOsConfig.h"
#include MCAL_OS_HEADER
/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
#define OS_AREA_DRVETH                  (88U)

#if (ETH_USER_MODE_INIT_API_ENABLE == STD_ON)
#define ETH_INIT_RESETENDINIT()              TRUSTED_ApplResetEndInit()
#define ETH_INIT_SETENDINIT()                TRUSTED_ApplSetEndInit()
#define ETH_INIT_WRITE32(reg,value)  \
                      OS_WritePeripheral32((OS_AREA_DRVETH),(volatile uint32*)\
                                             (volatile void*)(&(reg)),(value))
#define ETH_INIT_MODIFY32(reg,clearmask,setmask)   \
                                       OS_ModifyPeripheral32((OS_AREA_DRVETH),\
              (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
#if (ETH_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
  #define ETH_INIT_USER_MODE_OS_API_WRITE32(reg,value) \
                                        OS_WritePeripheral32((OS_AREA_DRVETH),\
                             (volatile uint32*)(volatile void*)(&(reg)),(value))
  #define ETH_INIT_USER_MODE_OS_API_READ32(reg)  \
                                         OS_ReadPeripheral32((OS_AREA_DRVETH),\
                                    (volatile uint32*)(volatile void*)(&(reg)))
  #define ETH_INIT_USER_MODE_OS_API_MODIFY32(reg,clearmask,setmask) \
                                      OS_ModifyPeripheral32((OS_AREA_DRVETH),\
             (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
#endif
#endif /* ETH_USER_MODE_INIT_API_ENABLE == STD_ON  */

#if (ETH_USER_MODE_RUNTIME_API_ENABLE == STD_ON)

#if (ETH_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
  #define ETH_RUNTIME_USER_MODE_OS_API_WRITE32(reg, value) \
                                       OS_WritePeripheral32((OS_AREA_DRVETH),\
                            (volatile uint32*)(volatile void*)(&(reg)),(value))
  #define ETH_RUNTIME_USER_MODE_OS_API_READ32(reg) \
                                         OS_ReadPeripheral32((OS_AREA_DRVETH),\
                                     (volatile uint32*)(volatile void*)(&(reg)))
  #define ETH_RUNTIME_USER_MODE_OS_API_MODIFY32(reg,clearmask,setmask)  \
                                       OS_ModifyPeripheral32((OS_AREA_DRVETH),\
              (volatile uint32*)(volatile void*)(&(reg)),(clearmask),(setmask));
#endif
#endif /* ETH_USER_MODE_RUNTIME_API_ENABLE == STD_ON  */
 #endif
