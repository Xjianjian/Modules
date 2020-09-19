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
**  $FILENAME   : Eth_17_EthMac.h $                                          **
**                                                                           **
**  $CC VERSION : \main\50 $                                                 **
**                                                                           **
**  $DATE       : 2016-09-09 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains                                         **
**                functionality of ETH driver.                               **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: No                                      **
**                                                                           **
******************************************************************************/
/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH026,DS_AS_ETH_PR5166,
     DS_AS_ETH125,DS_AS_ETH020,SAS_AS_ETH001_ETH002,DS_AS_ETH011,
     SAS_NAS_ALL_PR746,SAS_NAS_ALL_PR748,SAS_NAS_ALL_PR630_PR631,
     DS_NAS_ETH_PR643,DS_AS_ETH016,DS_AS_ETH007,DS_NAS_ETH_PR3054]
    [/cover]                                                                  **
*******************************************************************************/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#ifndef ETH_17_ETHMAC_H
#define ETH_17_ETHMAC_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/


/* Inclusion of Platform_Types.h and Compiler.h */
#include "Eth_17_EthMac_Types.h"

/* DEM header file */
#include "Dem.h"

/* Inclusion of Mcal Specific Global Header File */
#include "Mcal.h"

/* Conditional Inclusion of Development Error Tracer File */
#if (ETH_17_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
#if ((ETH_USER_MODE_INIT_API_ENABLE == STD_ON) || \
     (ETH_USER_MODE_RUNTIME_API_ENABLE == STD_ON) || \
     (ETH_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON))
           #include "Eth_17_EthMac_Protect.h"
#endif

/* Conditional Inclusion of Development Error Tracer File */
/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
#if (ETH_17_DEV_ERROR_DETECT == STD_ON)


/*
  Service IDs. Used while reporting development errors.
*/
#define ETH_17_SID_INIT                         ((uint8)0x01)
#define ETH_17_SID_CONTROLLERINIT               ((uint8)0x02)
#define ETH_17_SID_SETCONTROLLERMODE            ((uint8)0x03)
#define ETH_17_SID_GETCONTROLLERMODE            ((uint8)0x04)
#define ETH_17_SID_WRITEMII                     ((uint8)0x05)
#define ETH_17_SID_READMII                      ((uint8)0x06)
#define ETH_17_SID_GETCOUNTERSTATE              ((uint8)0x07)
#define ETH_17_SID_GETPHYADDRESS                ((uint8)0x08)
#define ETH_17_SID_PROVIDETXBUFFER              ((uint8)0x09)
#define ETH_17_SID_TRANSMIT                     ((uint8)0x0A)
#define ETH_17_SID_RECEIVE                      ((uint8)0x0B)
#define ETH_17_SID_TXCONFIRMATION               ((uint8)0x0C)
#define ETH_17_SID_GETVERSIONINFO               ((uint8)0x0D)
#define ETH_17_SID_RXHANDLER                    ((uint8)0x10)
#define ETH_17_SID_TXHANDLER                    ((uint8)0x11)
#define ETH_17_SID_SETPHYSADDRESS               ((uint8)0X13)

#endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */

/*
  Published parameters
*/
/* ETH Vendor ID - Vendor ID of the dedicated implementation of this
module according to the AUTOSAR vendor list */
extern uint8 rec_data[80];//
#define ETH_17_ETHMAC_VENDOR_ID     ((uint16)17)
/*ETH Module ID - Module ID of this module from Module List */
#define ETH_17_ETHMAC_MODULE_ID     ((uint16)88)

/* ETH Instance ID */
#define ETH_17_INSTANCE_ID  ((uint8)ETH_17_INDEX)


#define ETH_ZERO                  (0U)
#define ETH_ONE                   (1U)
#define FULL_DUPLEX               (1U)
#define HALF_DUPLEX               (0U)
#define PHY_RMII                  (0x01000000U)
#define ETH_10MBPS                (ETH_ZERO)
#define ETH_100MBPS               (ETH_ONE)

/*ETH_CLC.bit0 = 0 is Enable the module */
#define ETH_MOD_ENABLE            (0U)

#define ETH_FRAMEHEADER_LENGTH    (14U)
#define ETH_FRAMECHKSUM_LENGTH    (4U)


#define ETH_SHIFT_8BIT            (8U)
#define ETH_SHIFT_16BIT           (16U)
#define ETH_SHIFT_24BIT           (24U)

#define ETH_BYTE0_MASK            (0x000000FFU)
#define ETH_BYTE1_MASK            (0x0000FF00U)
#define ETH_BYTE2_MASK            (0x00FF0000U)
#define ETH_BYTE3_MASK            (0xFF000000U)
#define ETH_HALFWORD_MASK         (0x0000FFFFU)


#define ETH_17_E_DET_NO_ERR       ((uint8)0)
#define ETH_17_E_INV_CTRL_IDX     ((uint8)1)
#define ETH_17_E_NOT_INITIALIZED  ((uint8)2)
#define ETH_17_E_INV_POINTER      ((uint8)3)
#define ETH_17_E_INV_PARAM        ((uint8)4)
#define ETH_17_E_INV_CONFIG       ((uint8)5)
#define ETH_17_E_INV_MODE         ((uint8)6)

#define ETH_MAC_MMCBASE                     (0xF001E000U)
#define ETH_CLC_DISS                        (0x00000002UL)
#define ETH_CLC_DISS_BIT_POS                1U
#define ETH_BUS_MODE_SWR                    (0x00000001UL)
#define ETH_OPERATION_MODE_DFF_BIT_POS      24U
#define ETH_OPERATION_MODE_RSF_BIT_POS      25U
#define ETH_OPERATION_MODE_SR_BIT_POS       1U
#define ETH_OPERATION_MODE_ST_BIT_POS       13U
#define ETH_STATUS_RI_BIT_POS               6U
#define ETH_STATUS_NIS_BIT_POS              16U
#define ETH_OPERATION_MODE_FTF_BIT_POS      20U
#define ETH_KRST0_RSTSTAT_BIT_POS           1U
#define ETH_SRC_SRE_CLEARMASK              (0x7FFFFBFFU)
#define ETH_SRC_SRE_BIT_POS                 10U
#define ETH_SRC_SRR_BITPOS                  24U
#define ETH_GMII_ADDRESS_GB_MASK           (0x00000001U)
#define ETH_OPERATION_MODE_SR_CLEARMASK    (0xFFFFFFFDU)
#define ETH_OPERATION_MODE_ST_CLEARMASK    (0xFFFFDFFFU)
#define ETH_STATUS_NIS_MASK                (0x00010000U)
#define ETH_STATUS_RI_MASK                 (0x00000040U)
#define ETH_STATUS_TI_MASK                 (0x00000001U)
#define ETH_OPERATION_MODE_FTF_CLEARMASK   (0xFFEFFFFFU)
#define ETH_OPERATION_MODE_DFF_CLEARMASK   (0xFEFFFFFFU)
#define ETH_OPERATION_MODE_RSF_CLEARMASK   (0xFDFFFFFFU)
#define ETH_OPERATION_MODE_MASK            (0xFFFFFFFFU)
#define ETH_OPERATION_MODE_RES             (0xFEFFFFFFU)
#define ETH_KRST0_RST_CLEARMASK            (0xFFFFFFFCU)
#define ETH_KRST1_RST_CLEARMASK            (0xFFFFFFFEU)
#define ETH_KRST0_RSTSTAT_MASK             (0x00000002U)
#define ETH_KRSTCLR_CLR_CLRMASK            (0xFFFFFFFEU)
#define ETH_CLC_DISR_CLEARMASK             (0xFFFFFFFEU)
#define ETH_BUS_MODE_SWR_CLEARMASK         (0xFFFFFFFFU)
#define ETH_MAC_FRAME_FILTER_MASK          (0xFFFFFFFFU)
#define ETH_MAC_CONFIGURATION_FD_MASK      (0xFFFFDFFFU)
#define ETH_MAC_CONFIGURATION_MASK         (0xFFFFFFFFU)
#define ETH_MAC_ADDRESS0_HIGH_AE_MASK      (0x80000000U)
#define ETH_FLOW_CONTROL_MASK              (0xFFFFFFFFU)
#define ETH_INTERRUPT_ENABLE_CLEARMASK     (0xFFFFFFFFU)

#define ETH_17_DISABLE_DEM_REPORT          (0U)
#define ETH_17_ENABLE_DEM_REPORT           (1U)

#define ETH_17_CRC_STRIPPING_OFF           (0U)
#define ETH_17_CRC_STRIPPING_ON            (1U)

/*Macros for protected mode support*/
#if (ETH_USER_MODE_INIT_API_ENABLE == STD_ON)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_INIT_RESETENDINIT()                 (ETH_INIT_RESETENDINIT())
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_INIT_SETENDINIT()                   (ETH_INIT_SETENDINIT())
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define ETH_SFR_INIT_WRITE32(reg,value)              ETH_INIT_WRITE32(reg,value)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
#define ETH_SFR_INIT_MODIFY32(reg,clearmask,setmask) \
                                       ETH_INIT_MODIFY32(reg,clearmask,setmask)
#if (ETH_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
    /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
    #define ETH_SFR_INIT_USER_MODE_WRITE32(reg,value) \
                                  (ETH_INIT_USER_MODE_OS_API_WRITE32(reg,value))
     /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
    #define ETH_SFR_INIT_USER_MODE_READ32(reg) \
                                        (ETH_INIT_USER_MODE_OS_API_READ32(reg))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
    #define ETH_SFR_INIT_USER_MODE_MODIFY32(reg,clearmask,setmask) \
                      ETH_INIT_USER_MODE_OS_API_MODIFY32(reg,clearmask,setmask);
#else
    /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define ETH_SFR_INIT_USER_MODE_WRITE32(reg,value)         ((reg) = (value))
    /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
    #define ETH_SFR_INIT_USER_MODE_READ32(reg)                 (reg)
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
    /*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
    #define ETH_SFR_INIT_USER_MODE_MODIFY32(reg,clearmask,setmask) \
     { uint32 val ; val = (uint32)(reg) ; val &= (uint32)(clearmask); \
     val |= (uint32)(setmask); (reg) = (unsigned_int)val;}
#endif /* ETH_RUNNING_IN_USER_MODE_ENABLE == STD_ON  */
#else
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_INIT_RESETENDINIT()                  (Mcal_ResetENDINIT())
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_INIT_WRITE32(reg,value)              ((reg) = (value))
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_INIT_MODIFY32(reg,clearmask,setmask) \
     { uint32 val ; val = (uint32)(reg) ; val &= (uint32)(clearmask); \
     val |= (uint32)(setmask); (reg) = (unsigned_int)val;}
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_INIT_SETENDINIT()                      (Mcal_SetENDINIT())
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_INIT_USER_MODE_WRITE32(reg,value)     ((reg) = (value))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_INIT_USER_MODE_READ32(reg)             (reg)
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_INIT_USER_MODE_MODIFY32(reg,clearmask,setmask)  \
     { uint32 val ; val = (uint32)(reg) ; val &= (uint32)(clearmask); \
     val |= (uint32)(setmask); (reg) = (unsigned_int)val;}
#endif /* ETH_USER_MODE_INIT_API_ENABLE == STD_ON  */

#if (ETH_USER_MODE_RUNTIME_API_ENABLE == STD_ON)
/*ETHUserMode is enabled*/

#if (ETH_RUNNING_IN_USER_0_MODE_ENABLE == STD_ON)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
      #define ETH_SFR_RUNTIME_USER_MODE_WRITE32(reg,value) \
                               ETH_RUNTIME_USER_MODE_OS_API_WRITE32(reg,value)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
      #define ETH_SFR_RUNTIME_USER_MODE_READ32(reg)  \
                               ETH_RUNTIME_USER_MODE_OS_API_READ32(reg)
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
      #define ETH_SFR_RUNTIME_USER_MODE_MODIFY32(reg,clearmask,setmask) \
                  ETH_RUNTIME_USER_MODE_OS_API_MODIFY32(reg,clearmask,setmask);
#else
    /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
  #define ETH_SFR_RUNTIME_USER_MODE_WRITE32(reg,value)      ((reg) = (value))
  /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
    #define ETH_SFR_RUNTIME_USER_MODE_READ32(reg)               (reg)
    /*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
    #define ETH_SFR_RUNTIME_USER_MODE_MODIFY32(reg,clearmask ,setmask) \
      { uint32 val ; val = (uint32)(reg) ; val &= (uint32)(clearmask); \
      val |= (uint32)(setmask); (reg) = (unsigned_int)val;}
#endif /* ETH_RUNNING_IN_USER_MODE_ENABLE == STD_ON  */
#else
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_RUNTIME_USER_MODE_WRITE32(reg,value)    ((reg) = (value))
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_RUNTIME_USER_MODE_READ32(reg)      (reg)
/*IFX_MISRA_RULE_19_04_STATUS=First level of indirection is used to support
modification of OS protected calls, so it cannot be expand to a braced
 initialiser*/
/*IFX_MISRA_RULE_19_07_STATUS=To support user modification of OS protected
  calls definition, it is declared as a function like macro*/
#define ETH_SFR_RUNTIME_USER_MODE_MODIFY32(reg,clearmask,setmask)  \
     { uint32 val ; val = (uint32)(reg) ; val &= (uint32)(clearmask); \
     val |= (uint32)(setmask); (reg) = (unsigned_int)val;}
#endif /* ETH_USER_MODE_RUNTIME_API_ENABLE = STD_ON */

/*******************************************************************************
** Traceability:[cover parentID=DS_AS_ETH106_ETH107_ETH108_ETH136,
                 DS_NAS_ETH_PR128]
**                                                                            **
** Syntax :          Eth_17_GetVersionInfo (VersionInfoPtr)                   **
**                                                                            **
** [/cover]                                                                   **
** Service ID:       0x0D                                                     **
**                                                                            **
** Sync/Async:       Synchronous                                              **
**                                                                            **
** Reentrancy:       non reentrant                                            **
**                                                                            **
** Parameters (in):  None                                                     **
**                                                                            **
** Parameters (out):  versioninfo : Pointer to hold the version values        **
**                                                                            **
** Return value:     none                                                     **
**                                                                            **
** Description : 1. This service will return the version infos of ETH driver  **
*******************************************************************************/
#if ((ETH_17_VERSION_INFO_API == STD_ON) && (ETH_17_DEV_ERROR_DETECT == STD_ON))
/*IFX_MISRA_RULE_19_07_STATUS=GetVersionInfo is declared as a function like
 macro as per AUTOSAR*/
/*IFX_MISRA_RULE_19_04_STATUS=GetVersionInfo cannot be expand to a braced
 initialiser*/
#define Eth_17_EthMac_GetVersionInfo(VersionInfoPtr)                           \
{                                                                              \
  /* Check if the passed parameter is a NULL_PTR.  If so, escalate... */       \
  if((VersionInfoPtr) == NULL_PTR)                                             \
  {                                                                            \
    Det_ReportError(                                                           \
                     ETH_17_ETHMAC_MODULE_ID,                                  \
                     ETH_17_INSTANCE_ID,                                       \
                     ETH_17_SID_GETVERSIONINFO,                                \
                     ETH_17_E_INV_POINTER                                      \
                   );/* End of report to  DET */                               \
  }                                                                            \
  else                                                                         \
  {                                                                            \
    /* (pointer VersionInfoPtr is not checked for NULL pointer) */             \
    /* Get ETH Vendor ID */                                                    \
    ((Std_VersionInfoType*)(VersionInfoPtr))->vendorID =                       \
                                                 ETH_17_ETHMAC_VENDOR_ID;      \
    /* Get ETH Module ID */                                                    \
    ((Std_VersionInfoType*)(VersionInfoPtr))->moduleID =                       \
                                                  ETH_17_ETHMAC_MODULE_ID;     \
    /* Get ETH module Software major version */                                \
    ((Std_VersionInfoType*)(VersionInfoPtr))->sw_major_version =               \
                                       (uint8)ETH_17_ETHMAC_SW_MAJOR_VERSION;  \
    /* Get ETH module Software minor version */                                \
    ((Std_VersionInfoType*)(VersionInfoPtr))->sw_minor_version =               \
                                       (uint8)ETH_17_ETHMAC_SW_MINOR_VERSION;  \
    /* Get ETH module Software patch version */                                \
    ((Std_VersionInfoType*)(VersionInfoPtr))->sw_patch_version =               \
                                       (uint8)ETH_17_ETHMAC_SW_PATCH_VERSION;  \
  }                                                                            \
}/* end of Eth_GetVersionInfo() */
#elif ((ETH_17_VERSION_INFO_API == STD_ON) &&\
       (ETH_17_DEV_ERROR_DETECT == STD_OFF))
/*IFX_MISRA_RULE_19_07_STATUS=GetVersionInfo is declared as a function like
 macro as per AUTOSAR*/
/*IFX_MISRA_RULE_19_04_STATUS=GetVersionInfo cannot be expand to a braced
 initialiser*/
#define Eth_17_EthMac_GetVersionInfo(VersionInfoPtr)                           \
{                                                                              \
  /* (pointer VersionInfoPtr is not checked for NULL pointer) */               \
  /* Get ETH Vendor ID */                                                      \
  ((Std_VersionInfoType*)(VersionInfoPtr))->vendorID = ETH_17_ETHMAC_VENDOR_ID;\
  /* Get ETH Module ID */                                                      \
  ((Std_VersionInfoType*)(VersionInfoPtr))->moduleID = ETH_17_ETHMAC_MODULE_ID;\
  /* Get ETH module Software major version */                                  \
  ((Std_VersionInfoType*)(VersionInfoPtr))->sw_major_version =                 \
                                       (uint8)ETH_17_ETHMAC_SW_MAJOR_VERSION;  \
  /* Get ETH module Software minor version */                                  \
  ((Std_VersionInfoType*)(VersionInfoPtr))->sw_minor_version =                 \
                                       (uint8)ETH_17_ETHMAC_SW_MINOR_VERSION;  \
  /* Get ETH module Software patch version */                                  \
  ((Std_VersionInfoType*)(VersionInfoPtr))->sw_patch_version =                 \
                                       (uint8)ETH_17_ETHMAC_SW_PATCH_VERSION;  \
}/* end of Eth_GetVersionInfo() */
#else
/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Eth_17_EthMac_GetVersionInfo(VersionInfoPtr)                           \
                                        (ERROR_EthVersionInfoApi_NOT_SELECTED)
#endif


/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/

typedef enum Eth_CounterType
{
  ETH_MMC_TX_OCTETCOUNT_GB           =0x00000114U,
  ETH_MMC_TX_FRAMECOUNT_GB           =0x00000118U,
  ETH_MMC_TX_BROADCASTFRAME_G        =0x0000011cU,
  ETH_MMC_TX_MULTICASTFRAME_G        =0x00000120U,
  ETH_MMC_TX_64_OCTETS_GB            =0x00000124U,
  ETH_MMC_TX_65_TO_127_OCTETS_GB     =0x00000128U,
  ETH_MMC_TX_128_TO_255_OCTETS_GB    =0x0000012cU,
  ETH_MMC_TX_256_TO_511_OCTETS_GB    =0x00000130U,
  ETH_MMC_TX_512_TO_1023_OCTETS_GB   =0x00000134U,
  ETH_MMC_TX_1024_TO_MAX_OCTETS_GB   =0x00000138U,
  ETH_MMC_TX_UNICAST_GB              =0x0000013cU,
  ETH_MMC_TX_MULTICAST_GB            =0x00000140U,
  ETH_MMC_TX_BROADCAST_GB            =0x00000144U,
  ETH_MMC_TX_UNDERFLOW_ERROR         =0x00000148U,
  ETH_MMC_TX_SINGLECOL_G             =0x0000014cU,
  ETH_MMC_TX_MULTICOL_G              =0x00000150U,
  ETH_MMC_TX_DEFERRED                =0x00000154U,
  ETH_MMC_TX_LATECOL                 =0x00000158U,
  ETH_MMC_TX_EXESSCOL                =0x0000015cU,
  ETH_MMC_TX_CARRIER_ERROR           =0x00000160U,
  ETH_MMC_TX_OCTETCOUNT_G            =0x00000164U,
  ETH_MMC_TX_FRAMECOUNT_G            =0x00000168U,
  ETH_MMC_TX_EXCESSDEF               =0x0000016cU,
  ETH_MMC_TX_PAUSE_FRAME             =0x00000170U,
  ETH_MMC_TX_VLAN_FRAME_G            =0x00000174U,
  /* MMC RX counter registers */
  ETH_MMC_RX_FRAMECOUNT_GB           =0x00000180U,
  ETH_MMC_RX_OCTETCOUNT_GB           =0x00000184U,
  ETH_MMC_RX_OCTETCOUNT_G            =0x00000188U,
  ETH_MMC_RX_BROADCASTFRAME_G        =0x0000018cU,
  ETH_MMC_RX_MULTICASTFRAME_G        =0x00000190U,
  ETH_MMC_RX_CRC_ERRROR              =0x00000194U,
  ETH_MMC_RX_ALIGN_ERROR             =0x00000198U,
  ETH_MMC_RX_RUN_ERROR               =0x0000019CU,
  ETH_MMC_RX_JABBER_ERROR            =0x000001A0U,
  ETH_MMC_RX_UNDERSIZE_G             =0x000001A4U,
  ETH_MMC_RX_OVERSIZE_G              =0x000001A8U,
  ETH_MMC_RX_64_OCTETS_GB            =0x000001ACU,
  ETH_MMC_RX_65_TO_127_OCTETS_GB     =0x000001b0U,
  ETH_MMC_RX_128_TO_255_OCTETS_GB    =0x000001b4U,
  ETH_MMC_RX_256_TO_511_OCTETS_GB    =0x000001b8U,
  ETH_MMC_RX_512_TO_1023_OCTETS_GB   =0x000001bcU,
  ETH_MMC_RX_1024_TO_MAX_OCTETS_GB   =0x000001c0U,
  ETH_MMC_RX_UNICAST_G               =0x000001c4U,
  ETH_MMC_RX_LENGTH_ERROR            =0x000001c8U,
  ETH_MMC_RX_AUTOFRANGETYPE          =0x000001ccU,
  ETH_MMC_RX_PAUSE_FRAMES            =0x000001d0U,
  ETH_MMC_RX_FIFO_OVERFLOW           =0x000001d4U,
  ETH_MMC_RX_VLAN_FRAMES_GB          =0x000001d8U,
  ETH_MMC_RX_WATCHDOG_ERROR          =0x000001dcU,
  /* IPC*/
  ETH_MMC_RX_IPC_INTR_MASK           =0x00000200U,
  ETH_MMC_RX_IPC_INTR                =0x00000208U,
  /* IPv4*/
  ETH_MMC_RX_IPV4_GD                 =0x00000210U,
  ETH_MMC_RX_IPV4_HDERR              =0x00000214U,
  ETH_MMC_RX_IPV4_NOPAY              =0x00000218U,
  ETH_MMC_RX_IPV4_FRAG               =0x0000021CU,
  ETH_MMC_RX_IPV4_UDSBL              =0x00000220U,

  ETH_MMC_RX_IPV4_GD_OCTETS          =0x00000250U,
  ETH_MMC_RX_IPV4_HDERR_OCTETS       =0x00000254U,
  ETH_MMC_RX_IPV4_NOPAY_OCTETS       =0x00000258U,
  ETH_MMC_RX_IPV4_FRAG_OCTETS        =0x0000025cU,
  ETH_MMC_RX_IPV4_UDSBL_OCTETS       =0x00000260U,

  /* IPV6*/
  ETH_MMC_RX_IPV6_GD_OCTETS          =0x00000264U,
  ETH_MMC_RX_IPV6_HDERR_OCTETS       =0x00000268U,
  ETH_MMC_RX_IPV6_NOPAY_OCTETS       =0x0000026cU,

  ETH_MMC_RX_IPV6_GD                 =0x00000224U,
  ETH_MMC_RX_IPV6_HDERR              =0x00000228U,
  ETH_MMC_RX_IPV6_NOPAY              =0x0000022cU,

  /* Protocols*/
  ETH_MMC_RX_UDP_GD                  =0x00000230U,
  ETH_MMC_RX_UDP_ERR                 =0x00000234U,
  ETH_MMC_RX_TCP_GD                  =0x00000238U,
  ETH_MMC_RX_TCP_ERR                 =0x0000023cU,
  ETH_MMC_RX_ICMP_GD                 =0x00000240U,
  ETH_MMC_RX_ICMP_ERR                =0x00000244U,
  ETH_MMC_RX_UDP_GD_OCTETS           =0x00000270U,
  ETH_MMC_RX_UDP_ERR_OCTETS          =0x00000274U,
  ETH_MMC_RX_TCP_GD_OCTETS           =0x00000278U,
  ETH_MMC_RX_TCP_ERR_OCTETS          =0x0000027cU,
  ETH_MMC_RX_ICMP_GD_OCTETS          =0x00000280U,
  ETH_MMC_RX_ICMP_ERR_OCTETS         =0x00000284U
}Eth_CounterType;

typedef enum Eth_GpctlRegType
{
  ETH_ALTI0_MASK     = 0x00000003U,
  ETH_ALTI1_MASK     = 0x0000000CU,
  ETH_ALTI2_MASK     = 0x00000030U,
  ETH_ALTI3_MASK     = 0x000000C0U,
  ETH_ALTI4_MASK     = 0x00000300U,
  ETH_ALTI5_MASK     = 0x00000C00U,
  ETH_ALTI6_MASK     = 0x00003000U,
  ETH_ALTI7_MASK     = 0x0000C000U,
  ETH_ALTI8_MASK     = 0x00030000U,
  ETH_ALTI9_MASK     = 0x000C0000U,
  ETH_ALTI10_MASK   = 0x00300000U,
  ETH_RMMIMODE_MASK = 0x01000000U,
  ETH_CLKDIV_MASK   = 0x02000000U
}Eth_GpctlRegType;

typedef enum Eth_ConfigReg0Type
{
  ETH_WATCHDOG             = 0x00800000U,
  ETH_WATCHDOGDISABLE      = 0x00800000U,
  ETH_WATCHDOGENABLE       = 0x00000000U,
  ETH_JABBER               = 0x00400000U,
  ETH_JABBERDISABLE        = 0x00400000U,
  ETH_JABBERENABLE         = 0x00000000U,
  ETH_FRAMEBURST           = 0x00200000U,
  ETH_FRAMEBURSTENABLE     = 0x00200000U,
  ETH_FRAMEBURSTDISABLE    = 0x00000000U,
  ETH_JUMBOFRAME           = 0x00100000U,
  ETH_JUMBOFRAMEENABLE     = 0x00100000U,
  ETH_JUMBOFRAMEDISABLE    = 0x00000000U,
  ETH_INTERFRAMEGAP7       = 0x000E0000U,
  ETH_INTERFRAMEGAP6       = 0x000C0000U,
  ETH_INTERFRAMEGAP5       = 0x000A0000U,
  ETH_INTERFRAMEGAP4       = 0x00080000U,
  ETH_INTERFRAMEGAP3       = 0x00040000U,
  ETH_INTERFRAMEGAP2       = 0x00020000U,
  ETH_INTERFRAMEGAP1       = 0x00010000U,
  ETH_INTERFRAMEGAP0       = 0x00000000U,
  ETH_DISABLECRS           = 0x00010000U,
  ETH_MIIGMII              = 0x00008000U,
  ETH_SELECTMII            = 0x00008000U,
  ETH_SELECTGMII           = 0x00000000U,
  ETH_FESPEED100           = 0x00004000U,
  ETH_FESPEED10            = 0x00000000U,
  ETH_RXOWN                = 0x00002000U,
  ETH_DISABLERXOWN         = 0x00002000U,
  ETH_ENABLERXOWN          = 0x00000000U,
  ETH_LOOPBACK             = 0x00001000U,
  ETH_LOOPBACKON           = 0x00001000U,
  ETH_LOOPBACKOFF          = 0x00000000U,
  ETH_DUPLEX               = 0x00000800U,
  ETH_FULLDUPLEX           = 0x00000800U,
  ETH_HALFDUPLEX           = 0x00000000U,
  ETH_RXIPCOFFLOAD         = 0x00000400U,
  ETH_RETRY                = 0x00000200U,
  ETH_RETRYDISABLE         = 0x00000200U,
  ETH_RETRYENABLE          = 0x00000000U,
  ETH_LINKUP               = 0x00000100U,
  ETH_LINKDOWN             = 0x00000100U,
  ETH_PADCRCSTRIP          = 0x00000080U,
  ETH_PADCRCSTRIPENABLE    = 0x00000080U,
  ETH_PADCRCSTRIPDISABLE   = 0x00000000U,
  ETH_BACKOFFLIMIT         = 0x00000060U,
  ETH_BACKOFFLIMIT3        = 0x00000060U,
  ETH_BACKOFFLIMIT2        = 0x00000040U,
  ETH_BACKOFFLIMIT1        = 0x00000020U,
  ETH_BACKOFFLIMIT0        = 0x00000000U,
  ETH_DEFERRALCHECK        = 0x00000010U,
  ETH_DEFERRALCHECKENABLE  = 0x00000010U,
  ETH_DEFERRALCHECKDISABLE = 0x00000000U,
  ETH_TX                   = 0x00000008U,
  ETH_TXENABLE             = 0x00000008U,
  ETH_TXDISABLE            = 0x00000000U,
  ETH_RX                   = 0x00000004U,
  ETH_RXENABLE             = 0x00000004U,
  ETH_RXDISABLE            = 0x00000000U
}Eth_ConfigReg0Type;

#define ETH_FILTERDISABLE   (0x80000000U)
typedef enum Eth_ConfigReg1Type
{
  ETH_FILTERENABLE         = 0x00000000U,
  ETH_HASHPERFECTFILTER    = 0x00000400U,
  ETH_SRCADDRFILTER        = 0x00000200U,
  ETH_SRCADDRFILTERENABLE  = 0x00000200U,
  ETH_SRCADDRFILTERDISABLE = 0x00000000U,
  ETH_SRCINVAADDRFILTER    = 0x00000100U,
  ETH_SRCINVADDRFILTERENA  = 0x00000100U,
  ETH_SRCINVADDRFILTERDIS  = 0x00000000U,
  ETH_PASSCONTROL          = 0x000000C0U,
  ETH_PASSCONTROL3         = 0x000000C0U,
  ETH_PASSCONTROL2         = 0x00000080U,
  ETH_PASSCONTROL1         = 0x00000040U,
  ETH_PASSCONTROL0         = 0x00000000U,
  ETH_BROADCAST            = 0x00000020U,
  ETH_BROADCASTDISABLE     = 0x00000020U,
  ETH_BROADCASTENABLE      = 0x00000000U,
  ETH_MULTICASTFILTER      = 0x00000010U,
  ETH_MULTICASTFILTERENA   = 0x00000010U,
  ETH_MULTICASTFILTERDIS   = 0x00000000U,
  ETH_DESTADDRFILTER       = 0x00000008U,
  ETH_DESTADDRFILTERINV    = 0x00000008U,
  ETH_DESTADDRFILTERNOR    = 0x00000000U,
  ETH_MCASTHASHFILTER      = 0x00000004U,
  ETH_MCASTHASHFILTERON    = 0x00000004U,
  ETH_MCASTHASHFILTEROFF   = 0x00000000U,
  ETH_UCASTHASHFILTER      = 0x00000002U,
  ETH_UCASTHASHFILTERON    = 0x00000002U,
  ETH_UCASTHASHFILTEROFF   = 0x00000000U,
  ETH_PROMISCUOUSMODE      = 0x00000001U,
  ETH_PROMISCUOUSMODEON    = 0x00000001U,
  ETH_PROMISCUOUSMODEOFF   = 0x00000000U
}Eth_ConfigReg1Type;

/*GmacFlowControl    = 0x0018,    Flow control Register   Layout */
typedef enum Eth_ConfigReg6Type
{
  ETH_PAUSELOWTHRESH          = 0x00000030U,
  ETH_PAUSELOWTHRESH3         = 0x00000030U,
  ETH_PAUSELOWTHRESH2         = 0x00000020U,
  ETH_PAUSELOWTHRESH1         = 0x00000010U,
  ETH_PAUSELOWTHRESH0         = 0x00000000U,
  ETH_UNICASTPAUSEFRAME       = 0x00000008U,
  ETH_UNICASTPAUSEFRAMEON     = 0x00000008U,
  ETH_UNICASTPAUSEFRAMEOFF    = 0x00000000U,
  ETH_RXFLOWCONTROL           = 0x00000004U,
  ETH_RXFLOWCONTROLENABLE     = 0x00000004U,
  ETH_RXFLOWCONTROLDISABLE    = 0x00000000U,
  ETH_TXFLOWCONTROL           = 0x00000002U,
  ETH_TXFLOWCONTROLENABLE     = 0x00000002U,
  ETH_TXFLOWCONTROLDISABLE    = 0x00000000U,
  ETH_FLOWCONTROLBACKPRESSURE = 0x00000001U,
  ETH_SENDPAUSEFRAME          = 0x00000001U
}Eth_ConfigReg6Type;

typedef enum Eth_MiiAddrRegType
{
  ETHMIIDEVMASK              = 0x0000F800U,
  ETHMIIDEVSHIFT             = 11U,
  ETHMIIREGMASK              = 0x000007C0U,
  ETHMIIREGSHIFT             = 6U,
  ETHMIICSRCLKMASK           = 0x0000001CU,
  ETHMIICSRCLK5              = 0x00000014U,
  ETHMIICSRCLK4              = 0x00000010U,
  ETHMIICSRCLK3              = 0x0000000CU,
  ETHMIICSRCLK2              = 0x00000008U,
  ETHMIICSRCLK1              = 0x00000004U,
  ETHMIICSRCLK0              = 0x00000000U,
  ETHMIIWRITE                = 0x00000002U,
  ETHMIIREAD                 = 0x00000000U,
  ETHMIIBUSY                 = 0x00000001U
}Eth_MiiAddrRegType;

typedef enum Eth_DmaBusModeRegType
{
  ETH_DMAFIXEDBURSTENABLE     = 0x00010000U,
  ETH_DMAFIXEDBURSTDISABLE    = 0x00000000U,
  ETH_DMATXPRIORITYRATIO11    = 0x00000000U,
  ETH_DMATXPRIORITYRATIO21    = 0x00004000U,
  ETH_DMATXPRIORITYRATIO31    = 0x00008000U,
  ETH_DMATXPRIORITYRATIO41    = 0x0000C000U,
  ETH_DMABURSTLENGTHX8        = 0x01000000U,
  ETH_DMABURSTLENGTH256       = 0x01002000U,
  ETH_DMABURSTLENGTH128       = 0x01001000U,
  ETH_DMABURSTLENGTH64        = 0x01000800U,
  ETH_DMABURSTLENGTH32        = 0x00002000U,
  ETH_DMABURSTLENGTH16        = 0x00001000U,
  ETH_DMABURSTLENGTH8         = 0x00000800U,
  ETH_DMABURSTLENGTH4         = 0x00000400U,
  ETH_DMABURSTLENGTH2         = 0x00000200U,
  ETH_DMABURSTLENGTH1         = 0x00000100U,
  ETH_DMABURSTLENGTH0         = 0x00000000U,
  ETH_DMADESCRIPTOR8WORDS     = 0x00000080U,
  ETH_DMADESCRIPTOR4WORDS     = 0x00000000U,
  ETH_DMADESCRIPTORSKIP16     = 0x00000040U,
  ETH_DMADESCRIPTORSKIP8      = 0x00000020U,
  ETH_DMADESCRIPTORSKIP4      = 0x00000010U,
  ETH_DMADESCRIPTORSKIP2      = 0x00000008U,
  ETH_DMADESCRIPTORSKIP1      = 0x00000004U,
  ETH_DMADESCRIPTORSKIP0      = 0x00000000U,
  ETH_DMAARBITRR              = 0x00000000U,
  ETH_DMAARBITPR              = 0x00000002U,
  /* (SWR)Software Reset DMA engine */
  ETH_RESET_ON                = 0x00000001U,
  ETH_RESET_OFF               = 0x00000000U
}Eth_DmaBusModeRegType;

typedef enum Eth_DmaStatusRegType
{
  ETH_MACPMTINTR              = 0x10000000U,
  ETH_MACMMCINTR              = 0x08000000U,
  ETH_MACLINEINTFINTR         = 0x04000000U,
  ETH_DMAERRORBIT2            = 0x02000000U,
  ETH_DMAERRORBIT1            = 0x01000000U,
  ETH_DMAERRORBIT0            = 0x00800000U,
  ETH_DMATXSTATE              = 0x00700000U,
  ETH_DMATXSTOPPED            = 0x00000000U,
  ETH_DMATXFETCHING           = 0x00100000U,
  ETH_DMATXWAITING            = 0x00200000U,
  ETH_DMATXREADING            = 0x00300000U,
  ETH_DMATXSUSPENDED          = 0x00600000U,
  ETH_DMATXCLOSING            = 0x00700000U,
  ETH_DMARXSTATE              = 0x000E0000U,
  ETH_DMARXSTOPPED            = 0x00000000U,
  ETH_DMARXFETCHING           = 0x00020000U,
  ETH_DMARXWAITING            = 0x00060000U,
  ETH_DMARXSUSPENDED          = 0x00080000U,
  ETH_DMARXCLOSING            = 0x000A0000U,
  ETH_DMARXQUEUING            = 0x000E0000U,
  ETH_DMAINTNORMAL            = 0x00010000U,
  ETH_DMAINTABNORMAL          = 0x00008000U,
  ETH_DMAINTEARLYRX           = 0x00004000U,
  ETH_DMAINTBUSERROR          = 0x00002000U,
  ETH_DMAINTEARLYTX           = 0x00000400U,
  ETH_DMAINTRXWDOGTO          = 0x00000200U,
  ETH_DMAINTRXSTOPPED         = 0x00000100U,
  ETH_DMAINTRXNOBUFFER        = 0x00000080U,
  ETH_DMAINTRXCOMPLETED       = 0x00000040U,
  ETH_DMAINTTXUNDERFLOW       = 0x00000020U,
  ETH_DMAINTRCVOVERFLOW       = 0x00000010U,
  ETH_DMAINTTXJABBERTO        = 0x00000008U,
  ETH_DMAINTTXNOBUFFER        = 0x00000004U,
  ETH_DMAINTTXSTOPPED         = 0x00000002U,
  ETH_DMAINTTXCOMPLETED       = 0x00000001U,
  ETH_DMAINTCLEARALL          = 0x0001E7FFU
}Eth_DmaStatusRegType;


typedef enum Eth_DmaControlRegType
{
  ETH_DMADISABLEDROPTCPCS     = 0x04000000U,
  ETH_DMASTOREANDFORWARD      = 0x00200000U,
  ETH_DMAFLUSHTXFIFO          = 0x00100000U,
  ETH_DMATXTHRESHCTRL         = 0x0001C000U,
  ETH_DMATXTHRESHCTRL16       = 0x0001C000U,
  ETH_DMATXTHRESHCTRL24       = 0x00018000U,
  ETH_DMATXTHRESHCTRL32       = 0x00014000U,
  ETH_DMATXTHRESHCTRL40       = 0x00010000U,
  ETH_DMATXTHRESHCTRL256      = 0x0000C000U,
  ETH_DMATXTHRESHCTRL192      = 0x00008000U,
  ETH_DMATXTHRESHCTRL128      = 0x00004000U,
  ETH_DMATXTHRESHCTRL64       = 0x00000000U,
  ETH_DMATXSTART              = 0x00002000U,
  ETH_DMARXFLOWCTRLDEACT      = 0x00401800U,
  ETH_DMARXFLOWCTRLDEACT1K    = 0x00000000U,
  ETH_DMARXFLOWCTRLDEACT2K    = 0x00000800U,
  ETH_DMARXFLOWCTRLDEACT3K    = 0x00001000U,
  ETH_DMARXFLOWCTRLDEACT4K    = 0x00001800U,
  ETH_DMARXFLOWCTRLDEACT5K    = 0x00400000U,
  ETH_DMARXFLOWCTRLDEACT6K    = 0x00400800U,
  ETH_DMARXFLOWCTRLDEACT7K    = 0x00401000U,
  ETH_DMARXFLOWCTRLACT        = 0x00800600U,
  ETH_DMARXFLOWCTRLACT1K      = 0x00000000U,
  ETH_DMARXFLOWCTRLACT2K      = 0x00000200U,
  ETH_DMARXFLOWCTRLACT3K      = 0x00000400U,
  ETH_DMARXFLOWCTRLACT4K      = 0x00000300U,
  ETH_DMARXFLOWCTRLACT5K      = 0x00800000U,
  ETH_DMARXFLOWCTRLACT6K      = 0x00800200U,
  ETH_DMARXFLOWCTRLACT7K      = 0x00800400U,
  ETH_DMARXTHRESHCTRL         = 0x00000018U,
  ETH_DMARXTHRESHCTRL64       = 0x00000000U,
  ETH_DMARXTHRESHCTRL32       = 0x00000008U,
  ETH_DMARXTHRESHCTRL96       = 0x00000010U,
  ETH_DMARXTHRESHCTRL128      = 0x00000018U,
  ETH_DMAENHWFLOWCTRL         = 0x00000100U,
  ETH_DMADISHWFLOWCTRL        = 0x00000000U,
  ETH_DMAFWDERRORFRAMES       = 0x00000080U,
  ETH_DMAFWDUNDERSZFRAMES     = 0x00000040U,
  ETH_DMATXSECONDFRAME        = 0x00000004U,
  ETH_DMARXSTART              = 0x00000002U
}Eth_DmaControlRegType;

typedef enum Eth_DmaInterruptRegType
{
  ETH_DMAIENORMAL            = ETH_DMAINTNORMAL     ,
  ETH_DMAIEABNORMAL          = ETH_DMAINTABNORMAL   ,
  ETH_DMAIEEARLYRX           = ETH_DMAINTEARLYRX    ,
  ETH_DMAIEBUSERROR          = ETH_DMAINTBUSERROR   ,
  ETH_DMAIEEARLYTX           = ETH_DMAINTEARLYTX    ,
  ETH_DMAIERXWDOGTO          = ETH_DMAINTRXWDOGTO   ,
  ETH_DMAIERXSTOPPED         = ETH_DMAINTRXSTOPPED  ,
  ETH_DMAIERXNOBUFFER        = ETH_DMAINTRXNOBUFFER ,
  ETH_DMAIERXCOMPLETED       = ETH_DMAINTRXCOMPLETED,
  ETH_DMAIETXUNDERFLOW       = ETH_DMAINTTXUNDERFLOW,
  ETH_DMAIERXOVERFLOW        = ETH_DMAINTRCVOVERFLOW,
  ETH_DMAIETXJABBERTO        = ETH_DMAINTTXJABBERTO ,
  ETH_DMAIETXNOBUFFER        = ETH_DMAINTTXNOBUFFER ,
  ETH_DMAIETXSTOPPED         = ETH_DMAINTTXSTOPPED  ,
  ETH_DMAIETXCOMPLETED       = ETH_DMAINTTXCOMPLETED
}Eth_DmaInterruptRegType;

#define ETH_DESCOWNBYDMA     (0x80000000U)
typedef enum Eth_DmaDescStatusType
{
  ETH_DESCDAFILTERFAIL      = 0x40000000U,
  ETH_DESCFRAMELENGTHMASK   = 0x3FFF0000U,
  ETH_DESCFRAMELENGTHSHIFT  = 16U,
  ETH_DESCERROR             = 0x00008000U,
  ETH_DESCRXTRUNCATED       = 0x00004000U,
  ETH_DESCSAFILTERFAIL      = 0x00002000U,
  ETH_DESCRXLENGTHERROR     = 0x00001000U,
  ETH_DESCRXDAMAGED         = 0x00000800U,
  ETH_DESCRXVLANTAG         = 0x00000400U,
  ETH_DESCRXFIRST           = 0x00000200U,
  ETH_DESCRXLAST            = 0x00000100U,
  ETH_DESCRXLONGFRAME       = 0x00000080U,
  ETH_DESCRXCOLLISION       = 0x00000040U,
  ETH_DESCRXFRAMEETHER      = 0x00000020U,
  ETH_DESCRXWATCHDOG        = 0x00000010U,
  ETH_DESCRXMIIERROR        = 0x00000008U,
  ETH_DESCRXDRIBBLING       = 0x00000004U,
  ETH_DESCRXCRC             = 0x00000002U,
  ETH_DESCRXEXTSTS          = 0x00000001U,
  ETH_DESCTXINTENABLE       = 0x40000000U,
  ETH_DESCTXLAST            = 0x20000000U,
  ETH_DESCTXFIRST           = 0x10000000U,
  ETH_DESCTXDISABLECRC      = 0x08000000U,
  ETH_DESCTXDISABLEPADD     = 0x04000000U,
  ETH_DESCTXCISMASK         = 0x00C00000U,
  ETH_DESCTXCISBYPASS       = 0x00000000U,
  ETH_DESCTXCISIPV4HDRCS    = 0x00400000U,
  ETH_DESCTXCISTCPONLYCS    = 0x00800000U,
  ETH_DESCTXCISTCPPSEUDOCS  = 0x00C00000U,
  ETH_TXDESCENDOFRING       = 0x00200000U,
  ETH_TXDESCCHAIN           = 0x00100000U,
  ETH_DESCRXCHKBIT0         = 0x00000001U,
  ETH_DESCRXCHKBIT7         = 0x00000080U,
  ETH_DESCRXCHKBIT5         = 0x00000020U,
  ETH_DESCRXTSAVAIL         = 0x00000080U,
  ETH_DESCRXFRAMETYPE       = 0x00000020U,
  ETH_DESCTXIPV4CHKERROR    = 0x00010000U,
  ETH_DESCTXTIMEOUT         = 0x00004000U,
  ETH_DESCTXFRAMEFLUSHED    = 0x00002000U,
  ETH_DESCTXPAYCHKERROR     = 0x00001000U,
  ETH_DESCTXLOSTCARRIER     = 0x00000800U,
  ETH_DESCTXNOCARRIER       = 0x00000400U,
  ETH_DESCTXLATECOLLISION   = 0x00000200U,
  ETH_DESCTXEXCCOLLISIONS   = 0x00000100U,
  ETH_DESCTXVLANFRAME       = 0x00000080U,
  ETH_DESCTXCOLLMASK        = 0x00000078U,
  ETH_DESCTXCOLLSHIFT       = 3U,
  ETH_DESCTXEXCDEFERRAL     = 0x00000004U,
  ETH_DESCTXUNDERFLOW       = 0x00000002U,
  ETH_DESCTXDEFERRED        = 0x00000001U,
  ETH_RXDESCENDOFRING       = 0x00008000U,
  ETH_RXDESCCHAIN           = 0x00004000U,
  ETH_DESCSIZE2MASK         = 0x1FFF0000U,
  ETH_DESCSIZE2SHIFT        = 16U,
  ETH_DESCSIZE1MASK         = 0x00001FFFU,
  ETH_DESCSIZE1SHIFT        = 0U,
  ETH_DESCRXPTPAVAIL        = 0x00004000U,
  ETH_DESCRXPTPVER          = 0x00002000U,
  ETH_DESCRXPTPFRAMETYPE    = 0x00001000U,
  ETH_DESCRXPTPMESSAGETYPE  = 0x00000F00U,
  ETH_DESCRXPTPNO           = 0x00000000U,
  ETH_DESCRXPTPSYNC         = 0x00000100U,
  ETH_DESCRXPTPFOLLOWUP     = 0x00000200U,
  ETH_DESCRXPTPDELAYREQ     = 0x00000300U,
  ETH_DESCRXPTPDELAYRESP    = 0x00000400U,
  ETH_DESCRXPTPPDELAYREQ    = 0x00000500U,
  ETH_DESCRXPTPPDELAYRESP   = 0x00000600U,
  ETH_DESCRXPTPPDELAYRESPFP = 0x00000700U,
  ETH_DESCRXPTPIPV6         = 0x00000080U,
  ETH_DESCRXPTPIPV4         = 0x00000040U,
  ETH_DESCRXCHKSUMBYPASS    = 0x00000020U,
  ETH_DESCRXIPPAYLOADERROR  = 0x00000010U,
  ETH_DESCRXIPHEADERERROR   = 0x00000008U,
  ETH_DESCRXIPPAYLOADTYPE   = 0x00000007U,
  ETH_DESCRXIPPAYLOADUNKNOWN= 0x00000000U,
  ETH_DESCRXIPPAYLOADUDP    = 0x00000001U,
  ETH_DESCRXIPPAYLOADTCP    = 0x00000002U,
  ETH_DESCRXIPPAYLOADICMP   = 0x00000003U
}Eth_DmaDescStatusType;


typedef enum Eth_KernelRegType
{
  ETH_KERNELRST_ON          = 0x00000001U,  /*(SWR)Software Reset DMA engine */
  ETH_KERNELRST_STATUS_MASK = 0x00000002U,
  ETH_KERNELRST_CLEAR       = 0x00000001U
}Eth_KernelRegType;
/**********************************************************
 * Initial register values
 **********************************************************/
  /* Full-duplex mode with perfect filter on */
#define ETH_CONFIGINITFDX  ((uint32)\
             ((uint32)ETH_WATCHDOGENABLE       |(uint32)ETH_JABBERDISABLE    |\
              (uint32)ETH_FRAMEBURSTDISABLE    |(uint32)ETH_JUMBOFRAMEDISABLE|\
              (uint32)ETH_SELECTMII            |(uint32)ETH_DISABLERXOWN     |\
              (uint32)ETH_LOOPBACKOFF          |(uint32)ETH_RETRYENABLE      |\
              (uint32)ETH_PADCRCSTRIPDISABLE   |(uint32)ETH_BACKOFFLIMIT0    |\
              (uint32)ETH_DEFERRALCHECKDISABLE |(uint32)ETH_TXDISABLE        |\
              (uint32)ETH_RXDISABLE))

  /* Full-duplex mode FRAME FILTER config */
#define ETH_CONFIGFRAMFDX    ((uint32)((uint32)ETH_FILTERDISABLE      |\
                                       (uint32)ETH_MULTICASTFILTERENA |\
                                       (uint32)ETH_PROMISCUOUSMODEON))

  /* Half-duplex mode Flow Control config */
#define ETH_CONFIGFLOWFDX    ((uint32)((uint32)ETH_UNICASTPAUSEFRAMEOFF |\
                                       (uint32)ETH_RXFLOWCONTROLENABLE  |\
                                       (uint32)ETH_TXFLOWCONTROLENABLE))

  /* Half-duplex mode with perfect filter on */
#define ETH_CONFIGINITHDX ((uint32)\
          ((uint32)ETH_WATCHDOGENABLE       |(uint32)ETH_JABBERDISABLE     |\
           (uint32)ETH_FRAMEBURSTDISABLE    |(uint32)ETH_JUMBOFRAMEDISABLE |\
           (uint32)ETH_SELECTMII            |(uint32)ETH_DISABLERXOWN      |\
           (uint32)ETH_LOOPBACKOFF          |(uint32)ETH_RETRYENABLE       |\
           (uint32)ETH_PADCRCSTRIPDISABLE   |(uint32)ETH_BACKOFFLIMIT0     |\
           (uint32)ETH_DEFERRALCHECKDISABLE))

  /* HHalf-duplex mode FRAME FILTER config  */
#define ETH_CONFIGFRAMHDX    ((uint32)((uint32)ETH_FILTERDISABLE      |\
                                       (uint32)ETH_MULTICASTFILTERENA |\
                                       (uint32)ETH_PROMISCUOUSMODEON))

 /* Half-duplex mode Flow Control config */
#define ETH_CONFIGFLOWHDX    ((uint32)((uint32)ETH_UNICASTPAUSEFRAMEOFF |\
                                       (uint32)ETH_RXFLOWCONTROLDISABLE |\
                                       (uint32)ETH_TXFLOWCONTROLDISABLE))
/* DMA Bus mode init configuration.Default use ENH Descriptor of size 4 Words*/
#define ETH_DMAINIT          ((uint32)((uint32)ETH_DMABURSTLENGTH8 |\
                                       (uint32)ETH_DMADESCRIPTORSKIP0))

  /* Initialisation(0x70000000) of Descriptors in Ring Mode except last one */
#define ETH_DMADESCINIT      ((uint32)((uint32)ETH_DESCTXLAST     |\
                                       (uint32)ETH_DESCTXFIRST    |\
                                       (uint32)ETH_DESCTXINTENABLE))

  /* Initialisation(0x70200000) of Last Descriptor in Ring Mode*/
#define ETH_DMADESCINITLAST  ((uint32)((uint32)ETH_DESCTXLAST      |\
                                       (uint32)ETH_DESCTXFIRST     |\
                                       (uint32)ETH_DESCTXINTENABLE |\
                                       (uint32)ETH_TXDESCENDOFRING))

/* Structure for Trasmit buffer table */
typedef struct Eth_TxBuffTableType
{
  uint8  Eth_BuffStatus;  /* Buffer status free or used */
  uint8  Eth_StTxCnfn;    /* Tx confirmation requested */
  uint8  Eth_FrameTxReq;  /* Request for frame transmission */
}Eth_TxBuffTableType;


/*Structure for Controller configuration(for initialization) */
typedef struct Eth_CtrlConfigType
{
  /* Limits the maximum receive buffer length (frame length) in bytes */
  uint16  EthCtrlRxBufLenByte;
  /* Limits the maximum transmit buffer length (frame length) in bytes*/
  uint16  EthCtrlTxBufLenByte;
  /*Wait time for ETH and DMA to become idle once disabled*/
  uint16 EthDmaIdleTransitionWaitTime;
   /* Dem Event Id References */
  /*DEM Id for ETH_E_ACCESS Failure.*/
  Dem_EventIdType EthDemAccessId;
   /* Dem Event Id References */
  /*DEM Id for ETH_E_TIMEOUT Failure.*/
  Dem_EventIdType EthDemTimeout;
  /*DEM Id for ETH_E_FRAMESEQ Failure.*/
  Dem_EventIdType EthDemFrameSeqErr;
  /* Configures the number of receive buffers. */
  uint8 EthRxBufTotal;
  /* Configures the number of transmit buffers */
  uint8 EthTxBufTotal;
  /*Enabling/Disabling Stripping of CRC bytes from ETH frame transerred to 
   Application*/
  uint8 EthCrcStripping;
}Eth_CtrlConfigType;

/*Structure for PB configuration(for initialization) */
typedef struct Eth_17_EthMac_ConfigType
{
   /* Pointer to Controller Configuration structure */
  const Eth_CtrlConfigType* Eth_CtrlConfig;

}Eth_17_EthMac_ConfigType;


/*The Ethernet MAC in Aurix does not support Normal Descriptor Format as it is
    configured to support IEEE1588 PTP time stamping  */
typedef struct Eth_DmaDescType
{
  volatile uint32   Status;  /* Status of Transmit/Receive Frame              */
  volatile uint32   Length;  /* Frame Buffer Length 1 and 2                   */
  uint32   Buffer1;          /* Frame Buffer 1 Address (Dma-able)             */
  uint32   Buffer2;          /* Frame Buffer 2 Address or next descriptor
                               Address (Dma-able)in chain structure          */
} Eth_DmaDescType;

/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/
#define ETH_17_ETHMAC_START_SEC_POSTBUILDCFG
#include "MemMap.h"
extern const struct Eth_17_EthMac_ConfigType \
                            Eth_ConfigRoot[ETH_17_ETHMAC_CONFIG_COUNT];
#define ETH_17_ETHMAC_STOP_SEC_POSTBUILDCFG
#include "MemMap.h"
/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/
/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/

/* Memory Map of the Eth Code */
#define ETH_17_ETHMAC_START_SEC_CODE
#include "MemMap.h"

/*******************************************************************************
** Syntax           : void Eth_17_EthMac_TxIrqHdlr( void )                    **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Frame transmission interrupt handler                    **
**                                                                            **
**                                                                            **
*******************************************************************************/
extern void Eth_17_EthMac_TxRxIrqHandler(void);

#if (ETH_17_ENA_TX_INT == STD_ON)
/*******************************************************************************
** Syntax           : void Eth_17_EthMac_TxIrqHdlr( void )                    **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Frame transmission interrupt handler                    **
**                                                                            **
**                                                                            **
*******************************************************************************/
extern void Eth_17_EthMac_TxIrqHdlr(void);
#else
/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Eth_17_EthMac_TxIrqHdlr(void)\
(ERROR_EthCtrlEnableTxInterrupt_NOT_SELECTED)
#endif

#if (ETH_17_ENA_RX_INT == STD_ON)
/*******************************************************************************
**                                                                            **
** Syntax           : void Eth_17_EthMac_RxIrqHdlr( void )                    **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Frame reception interrupt handler                       **
**                                                                            **
*******************************************************************************/
extern void Eth_17_EthMac_RxIrqHdlr(void);
#else
/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Eth_17_EthMac_RxIrqHdlr(void)\
(ERROR_EthCtrlEnableRxInterrupt_NOT_SELECTED)
#endif


#if ( ETH_17_ENA_MII_API == STD_ON)
/*******************************************************************************
**                                                                            **
** Syntax           : void Eth_17_EthMac_ReadMii(uint8 CtrlIdx,uint8 TrcvIdx, **
**                                         uint8 RegIdx,uint16 *RegVal)       **
**                                                                            **
** Service ID       : 0x06                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx  - Index of the controller                      **
**                    TrcvIdx  - Index of the transceiver on the MII          **
**                    RegIdx   - Index of the transceiver register on the MII **
** Parameters (out) : *RegVal  -Value to be written into the indexed register **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Reads a transceiver register                            **
**                                                                            **
*******************************************************************************/
extern void Eth_17_EthMac_ReadMii(uint8 CtrlIdx,uint8 TrcvIdx,uint8 RegIdx,
                             uint16* RegValPtr);
/*******************************************************************************
** Syntax           : void Eth_17_EthMac_WriteMii(uint8 CtrlIdx,uint8 TrcvIdx,**
                                           uint8 RegIdx,uint16 RegVal)        **
**                                                                            **
** Service ID       : 0x05                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx  - Index of the controller                      **
**                    TrcvIdx  - Index of the transceiver on the MII          **
**                    RegIdx   -Index of the transceiver register on the MII  **
**                    RegVal   -Value to be written into the indexed register **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Configures a transceiver register or triggers a function**
**                    offered by the receiver                                 **
**                                                                            **
*******************************************************************************/
extern void Eth_17_EthMac_WriteMii(uint8 CtrlIdx,uint8 TrcvIdx,uint8 RegIdx,
                              uint16 RegVal);
#else

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Eth_17_EthMac_ReadMii(CtrlIdx,TrcvIdx,RegIdx,RegValPtr)  \
(ERROR_EthCtrlEnableMii_NOT_SELECTED)

/*IFX_MISRA_RULE_19_07_STATUS=Function like macro used to generate compile time
 error in case of incorrect configuration*/
#define Eth_17_EthMac_WriteMii(CtrlIdx,TrcvIdx,RegIdx,RegVal)  \
(ERROR_EthCtrlEnableMii_NOT_SELECTED)

#endif /* End of ETH_17_ENA_MII_API */

/*******************************************************************************
** Syntax           : void Eth_17_EthMac_Init(const Eth_ConfigType* CfgPtr )  **
**                                                                            **
** Service ID       : 0x01                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CfgPtr - Pointer to configuration set                   **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : The function store the access to the configuration      **
**                    parameters for subsequent API calls.                    **
**                                                                            **
*******************************************************************************/

extern void Eth_17_EthMac_Init(const Eth_17_EthMac_ConfigType* CfgPtr );

/*******************************************************************************
** Syntax           : Std_ReturnType                                          **
**                  Eth_17_EthMac_ControllerInit( uint8 CtrlIdx,uint8 CfgIdx )**
**                                                                            **
** Service ID       : 0x02                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx   - Index of the controller                     **
**                    CfgIdx    - Index of the Configuration                  **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : E_OK     - Successfully Initialised the controller      **
**                    E_NOT_OK - Controller could not be initialised          **
** Description      : Initialises the controller                              **
**                                                                            **
*******************************************************************************/
extern Std_ReturnType Eth_17_EthMac_ControllerInit( uint8 CtrlIdx,
                                                                 uint8 CfgIdx );

/*******************************************************************************
** Syntax           : BufReq_ReturnType Eth_17_EthMac_ProvideTxBuffer         **
**                                                              uint8 CtrlIdx,**
**                                                          uint8* BufIdxPtr, **
**                                   Eth_DataType** BufPtr,uint16* LenBytePtr)**
**                                                                            **
** Service ID       : 0x09                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx   - Index of the controller                     **
**                                                                            **
** Parameters (inout): LenBytePtr -In: desired length in bytes,               **
**                                 out: granted length in bytes               **
** Parameters (out) : BufIdxPtr - Index to the granted buffer resource        **
**                    BufPtr    - Pointer to the granted buffer               **
** Return value     : BUFREQ_OK       - Successfully granted buffer           **
**                    BUFREQ_E_NOT_OK - Development error detected            **
                      BUFREQ_E_BUSY   - All Buffers are in USE                **
** Description      : Provides access to reserved transmit buffer of indexed  **
**                    controller                                              **
*******************************************************************************/
extern BufReq_ReturnType Eth_17_EthMac_ProvideTxBuffer(uint8 CtrlIdx,
                    uint8* BufIdxPtr, Eth_DataType** BufPtr,uint16* LenBytePtr);

/*******************************************************************************
** Syntax           :  Std_ReturnType Eth_17_EthMac_Transmit(.................**
**                     uint8 CtrlIdx, uint8 BufIdx,Eth_FrameType FrameType,...**
**                     boolean TxConfirmation, uint16 LenByte,                **
**                     const uint8* PhysAddrPtr)                              **
** Service ID       :  0x0A                                                   **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx  - Index of the controller                      **
**                    BufIdx - Index of the buffer resource                   **
**                    FrameType- Ethernet frame type                          **
**                    TxConfirmation - Activates transmission confirmation    **
**                    LenByte - Data length in byte                           **
**                    PhysAddrPtr - Physical target address                   **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : E_OK     - success                                      **
**                    E_NOT_OK - transmission failed                          **
**                                                                            **
** Description      :Triggers transmission of a previously....................**
**                   filled transmit buffer                                   **
**                                                                            **
**                                                                            **
*******************************************************************************/
extern Std_ReturnType Eth_17_EthMac_Transmit(
              uint8 CtrlIdx, uint8 BufIdx,Eth_FrameType FrameType,
              boolean TxConfirmation, uint16 LenByte, const uint8* PhysAddrPtr);

/*******************************************************************************
** Syntax           : void Eth_17_EthMac_Receive( uint8 CtrlIdx )             **
**                                                                            **
** Service ID       : 0x0B                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx  - Index of the controller                      **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Triggers frame reception                                **
**                                                                            **
*******************************************************************************/
extern void Eth_17_EthMac_Receive( uint8 CtrlIdx );

/*******************************************************************************
** Syntax           : void Eth_17_EthMac_GetPhysAddr( uint8 Eth_CtrlIdx,      **
**                                             uint8* PhysAddrPtr )           **
**                                                                            **
** Service ID       : 0x08                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx   - Index of the controller                     **
**                                                                            **
** Parameters (out) : Physical source address (MAC address) in network byte   **
**                    order.                                                  **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Obtains the physical source address used by the         **
**                    indexed controller                                      **
**                                                                            **
*******************************************************************************/
extern void Eth_17_EthMac_GetPhysAddr(uint8 CtrlIdx, uint8* PhysAddrPtr);
/*******************************************************************************
** Syntax           : void Eth_17_EthMac_SetPhysAddr( uint8 Eth_CtrlIdx,      **
**                                             uint8* PhysAddrPtr )           **
**                                                                            **
** Service ID       : 0x13                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx   - Index of the controller                     **
**                                                                            **
** Parameters (out) : Physical source address (MAC address) in network byte   **
**                    order.                                                  **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Sets the controllers MAC address                        **
**                                                                            **
*******************************************************************************/
extern void Eth_17_EthMac_SetPhysAddr(uint8 CtrlIdx, const uint8* PhysAddrPtr);
/*******************************************************************************
** Syntax           : void Eth_17_EthMac_TxConfirmation(uint8 CtrlIdx)        **
**                                                                            **
** Service ID       : 0x0C                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx  - Index of the controller                      **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Triggers frame transmission confirmation                **
**                                                                            **
**                                                                            **
*******************************************************************************/
extern void Eth_17_EthMac_TxConfirmation( uint8 CtrlIdx);

/*******************************************************************************
** Syntax           : Std_ReturnType Eth_17_EthMac_SetControllerMode(         **
**                                                            uint8 CtrlIdx,  **
**                                   Eth_ModeType CtrlMode )                  **
**                                                                            **
** Service ID       : 0x03                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx   - Index of the controller                     **
**                    CtrlMode  -Controller Mode DOWN/ACTIVE                  **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : E_OK     - Successfully Initialised the controller      **
**                    E_NOT_OK - Controller could not be set                  **
**                                                                            **
** Description      : Enables / Disables the indexed controller               **
**                                                                            **
*******************************************************************************/
extern Std_ReturnType Eth_17_EthMac_SetControllerMode(uint8 CtrlIdx,
                        Eth_ModeType CtrlMode );

/*******************************************************************************
** Syntax           : Std_ReturnType Eth_17_EthMac_GetControllerMode(         **
**                                                            uint8 CtrlIdx,  **
**                                                Eth_ModeType* CtrlModePtr)  **
**                                                                            **
** Service ID       : 0x04                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx   - Index of the controller                     **
**                                                                            **
** Parameters (out) : CtrlMode  -Controller Mode DOWN/ACTIVE                  **
**                                                                            **
** Return value     : E_OK     - Successfully Initialised the controller      **
**                    E_NOT_OK - Controller mode could not be get             **
**                                                                            **
** Description      : Obtains the state of the indexed controller             **
**                                                                            **
*******************************************************************************/
extern Std_ReturnType Eth_17_EthMac_GetControllerMode(uint8 CtrlIdx,
                                                     Eth_ModeType* CtrlModePtr);
/*******************************************************************************
** Syntax           : void Eth_17_EthMac_GetCounterState( uint8 CtrlIdx,      **
**                                         uint16 CtrOffs, uint32* CtrValPtr )**
** Service ID       : 0x07                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx  - Index of the controller                      **
**                    CtrOffs  - offset of the counterregister                **
** Parameters (out) :  CtrValPtr - Content of the specified counter           **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Reads the value of a counter specified with its offset  **
**                                                                            **
*******************************************************************************/
extern void Eth_17_EthMac_GetCounterState( uint8 CtrlIdx, uint16 CtrOffs,
                          uint32* CtrValPtr );

/* Memory Map of the Eth Code */
#define ETH_17_ETHMAC_STOP_SEC_CODE
#include "MemMap.h"

#endif /* ETH_17_ETHMAC_H */

