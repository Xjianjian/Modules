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
**  $FILENAME   : Eth_17_EthMac.c $                                          **
**                                                                           **
**  $CC VERSION : \main\95 $                                                 **
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
/******************************************************************************
   Traceability   :
  [cover parentID=DS_NAS_ETH_PR228,DS_AS_ETH010,DS_NAS_ETH_PR734,DS_AS_ETH006,
   SAS_AS_ETH001_ETH002,SAS_NAS_ALL_PR749,SAS_NAS_ALL_PR102,SAS_NAS_ALL_PR470,
   SAS_NAS_ALL_PR70,SAS_NAS_ALL_PR1652,SAS_NAS_ALL_PR630_PR631,DS_AS_ETH007,
   DS_NAS_ETH_PR912]
  [/cover]
******************************************************************************/
/******************************************************************************
**                      Includes                                             **
******************************************************************************/

/* Inclusion of Tasking sfr file */
#include "IfxSrc_reg.h"
#include "IfxEth_reg.h"
#include "IfxEth_bf.h"

/* Inclusion of SchM header files */
#include "SchM_17_McalCfg.h"
#include SCHM_ETH_17_ETHMAC_HEADER

/* Own header file, this includes own configuration file also */
#include "Eth_17_EthMac.h"
#include "EthIf_Cbk.h"

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/

/* ETH Header File Version Check */
#ifndef ETH_17_ETHMAC_SW_MAJOR_VERSION
  #error "ETH_17_ETHMAC_SW_MAJOR_VERSION is not defined. "
#endif

#ifndef ETH_17_ETHMAC_SW_MINOR_VERSION
  #error "ETH_17_ETHMAC_SW_MINOR_VERSION is not defined. "
#endif

#ifndef ETH_17_ETHMAC_SW_PATCH_VERSION
  #error "ETH_17_ETHMAC_SW_PATCH_VERSION is not defined. "
#endif

/* AUTOSAR Secification File Version Check */
#ifndef ETH_17_AR_RELEASE_MAJOR_VERSION
  #error "ETH_17_AR_RELEASE_MAJOR_VERSION is not defined. "
#endif

#ifndef ETH_17_AR_RELEASE_MINOR_VERSION
  #error "ETH_17_AR_RELEASE_MINOR_VERSION is not defined. "
#endif

#ifndef ETH_17_AR_RELEASE_REVISION_VERSION
  #error "ETH_17_AR_RELEASE_REVISION_VERSION is not defined. "
#endif

#if (ETH_17_AR_RELEASE_MAJOR_VERSION != 4U)
  #error "ETH_17_AR_RELEASE_MAJOR_VERSION does not match. "
#endif

#if (ETH_17_AR_RELEASE_MINOR_VERSION != 0U)
  #error "ETH_17_AR_RELEASE_MINOR_VERSION does not match. "
#endif

#if (ETH_17_ETHMAC_SW_MAJOR_VERSION != 4U)
  #error "ETH_17_ETHMAC_SW_MAJOR_VERSION does not match. "
#endif

#if (ETH_17_ETHMAC_SW_MINOR_VERSION != 3U)
  #error "ETH_17_ETHMAC_SW_MINOR_VERSION does not match. "
#endif

/*
  VERSION CHECK FOR DET MODULE INCLUSION
*/

#if (ETH_17_DEV_ERROR_DETECT == STD_ON)

#ifndef DET_AR_RELEASE_MAJOR_VERSION
  #error "DET_AR_RELEASE_MAJOR_VERSION is not defined. "
#endif
#ifndef DET_AR_RELEASE_MINOR_VERSION
  #error "DET_AR_RELEASE_MINOR_VERSION is not defined. "
#endif

#if (IFX_DET_VERSION_CHECK == STD_ON)
#if ( DET_AR_RELEASE_MAJOR_VERSION != 4U )
  #error "DET_AR_RELEASE_MAJOR_VERSION does not match. "
#endif
#if ( DET_AR_RELEASE_MINOR_VERSION != 0U )
  #error "DET_AR_RELEASE_MINOR_VERSION does not match. "
#endif
#endif/* End Of IFX_DET_VERSION_CHECK */

#endif/* End Of ETH_17_DEV_ERROR_DETECT */


/*
  VERSION CHECK FOR DEM MODULE INCLUSION
*/
#if((ETH_17_E_ACCESS_DEM_REPORT  == ETH_17_ENABLE_DEM_REPORT) ||\
    (ETH_17_E_TIMEOUT_DEM_REPORT == ETH_17_ENABLE_DEM_REPORT) ||\
    (ETH_17_E_FRAMESEQ_DEM_REPORT == ETH_17_ENABLE_DEM_REPORT))

#ifndef DEM_AR_RELEASE_MAJOR_VERSION
  #error "DEM_AR_RELEASE_MAJOR_VERSION is not defined. "
#endif

#ifndef DEM_AR_RELEASE_MINOR_VERSION
  #error "DEM_AR_RELEASE_MINOR_VERSION is not defined. "
#endif

#if (IFX_DEM_VERSION_CHECK == STD_ON)
#if ( DEM_AR_RELEASE_MAJOR_VERSION != 4U )
  #error "DEM_AR_RELEASE_MAJOR_VERSION does not match. "
#endif

#if ( DEM_AR_RELEASE_MINOR_VERSION != 0U )
  #error "DEM_AR_RELEASE_MINOR_VERSION does not match. "
#endif
#endif/*End For IFX_DEM_VERSION_CHECK*/

#endif/*End for DEM Checks*/


/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

#define ETH_BYTE_MASK                 (0xFFU)
/* Macros to disable interrupts */
#define ETH_DISABLE_IPC_RX_INTPT      (0x3FFF3FFFU)
#define ETH_DISABLE_MMC_RX_INTPT      (0x03FFFFFFU)
#define ETH_DISABLE_MMC_TX_INTPT      (0x03FFFFFFU)
#define ETH_DISABLE_PMT_TTI_INTPT     (0x00000008U)

#define ETH_MASK_CLRR                 (0x02000000U)
#define ETH_MASK_SRE                  (0x80000400U)
#define ETH_MASK_SWSCLR               (0x40000000U)
#define ETH_MASK_IOVCLR               (0x10000000U)
#define ETH_MASK_CLRRSWSCLRIOVCLR  (ETH_MASK_CLRR | ETH_MASK_SWSCLR| \
                                                    ETH_MASK_IOVCLR)

#define ETH_DMA_ACTIVE_STATUS         (0x007E0000U)
#define ETH_DEBUG_ALL_BITS            (0xFFFFFCFFU)
#define ETH_MAC_IDLE                  (0x00000000U)
#define ETH_DMA_IDLE                  (0x00000000U)


/* Macro for KRST wait counts */
#define ETH_KERNEL_RESET_WAIT_COUNTS  (0x00000100UL)
#define ETH_INTERRUPT_SERVICE_TIMEOUT (0x00000100UL)
#define ETH_MTL_TIMEOUT               (0x00001000UL)

/* MACRO for Hardware timeout */
#define ETH_MASK_LOWER14BITS          (0x3FFFU)
#define ETH_BUFF_NOTUSED              (0U)
#define ETH_BUFF_USED                 (1U)
#define ETH_MACADDR_LEN               (6U)
#define ETH_FRAMETYPE_LEN             (2U)
#define ETH_MAX_FRAME_LEN             (1524U)
/* Interrupt node enable */
#define ETH_ENABLE_INTERRUPT          (1U)

/* max number of NIS check loops in ISR */
#define ETH_NIS_PENDING_MAXLOOPCNT    (5U)

/*Clear mask for CST ACS,DM and FES*/
#define ETH_MAC_CONFIGURATION_FES_DM_CST_ACS \
                          (~(0x01UL << IFX_ETH_MAC_CONFIGURATION_FES_OFF) &\
                           ~(0x01UL << IFX_ETH_MAC_CONFIGURATION_DM_OFF) &\
                           ~(0x01UL << IFX_ETH_MAC_CONFIGURATION_CST_OFF) &\
                           ~(0x01UL << IFX_ETH_MAC_CONFIGURATION_ACS_OFF))
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

typedef struct Eth_FrameStructType
{
  uint8 EthDestAddr[ETH_MACADDR_LEN];
  uint8 EthSourceAddr[ETH_MACADDR_LEN];
  uint8 EthFrameType[ETH_FRAMETYPE_LEN];
}Eth_FrameStructType;


/*******************************************************************************
**                 Private Function Declarations:
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
/*[cover parentID=DS_NAS_ETH_PR115]Global and Static variable[/cover]*/
#define ETH_17_ETHMAC_START_SEC_VAR_8BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
/* Global variable to store total number of Rx Buffer */
static uint8 Eth_RxBufTotal;
/* Global variable to store total number of Tx Buffer */
static uint8 Eth_TxBufTotal;
/* Variable to hold the index of Rx Buffer in Rx func */
static uint8 Eth_CurrRxBufIdx;
/* Holds the next Tx buffer pointed by DMA*/
static uint8 Eth_NextTxBuffer;
/* SOF buffer idx */
static uint8 Eth_RxStartIdx;
/* Variable to hold the Confirmed Tx Buffer idx */
static uint8 Eth_ConfirmTxBufIdx;
/*Variable to hold the CRC stripping configuration*/
static uint8 Eth_CrcStripping;
#define ETH_17_ETHMAC_STOP_SEC_VAR_8BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"


#define ETH_17_ETHMAC_START_SEC_VAR_16BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
/* Length of each Rx buffer */
static uint16 Eth_RxBufLength;
/* Length of each Tx buffer */
static uint16 Eth_TxBufLength;
/*Wait time for Eth and DMA to become idle after current frame trasmission once
  disabled*/
static uint16 Eth_DmaIdleTransitionWaitTime;
/* DemEventIds */
static Dem_EventIdType Eth_DemAccessID, Eth_DemTimeout, Eth_DemEthSeq;
#define ETH_17_ETHMAC_STOP_SEC_VAR_16BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

#define ETH_17_ETHMAC_START_SEC_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
/*Maximum Ethernet Receive buffer reserved */
/*IFX_MISRA_RULE_08_07_STATUS=Eth_RxBuffer variable is used via DMA descriptors
 across multiple APIs, hence it cannot be declared as a local variable*/
static uint8 Eth_RxBuffer[ETH_17_RXBUFFER_SIZE];
/*Maximum Ethernet Transmit buffer reserved */
/*IFX_MISRA_RULE_08_07_STATUS=Eth_TxBuffer variable is used via DMA descriptors
 across multiple APIs, hence it cannot be declared as a local variable*/
static uint8 Eth_TxBuffer[ETH_17_TXBUFFER_SIZE];
/* Transmit Descriptors for Each Tx Buffer  */
static Eth_DmaDescType Eth_TxDesc[ETH_17_TXBUFFER_COUNT];
/*Receive Descriptors for Each Receive Buffer  */
static Eth_DmaDescType Eth_RxDesc[ETH_17_RXBUFFER_COUNT];
/*Ethernet Tx Buffer table contains Pointer to Tx Buffer and its status.*/
static Eth_TxBuffTableType Eth_TxBuffTable[ETH_17_TXBUFFER_COUNT];
/* Rx Frame Start pointer */
static uint8* Eth_RxStartPtr;

#if (ETH_17_PB_FIXEDADDR == STD_OFF)
/* To store the driver configuration pointer */
/*IFX_MISRA_RULE_08_07_STATUS=ConfigPtr of the driver cannot be declared as a
 local variable*/
static const Eth_17_EthMac_ConfigType *Eth_17_kConfigPtr;
#endif

#ifdef IFX_ETH_DEBUG
extern volatile uint32 TestEth_DebugMask;
extern volatile uint32 TestEth_DebugMask02;
extern volatile uint32 TestEth_DebugMask03;
extern volatile uint32 TestEth_DebugMask04;
extern volatile uint32 TestEth_DebugMask05;
extern volatile uint32 TestEth_DebugMask06;
extern volatile uint32 TestEth_DebugMask07;
extern volatile uint32 TestEth_DebugMask08;
extern volatile uint32 TestEth_DebugMask09;
extern volatile uint32 TestEth_DebugMask10;
extern volatile uint32 TestEth_DebugMask11;
#endif

#define ETH_17_ETHMAC_STOP_SEC_VAR_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

#define ETH_17_ETHMAC_START_SEC_VAR_INIT_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

#if  ( ETH_17_DEBUG_SUPPORT == STD_ON )
/*Driver state variable.Default state is ETH_STATE_UNINIT(0U) */
/*IFX_MISRA_RULE_08_10_STATUS=Variable Eth_17_EthMac_DriverState is declared as
 extern in Eth_17_EthMac_Dbg.h, which will be included application for
 debugging*/
Eth_17_EthMac_StateType Eth_17_EthMac_DriverState = ETH_STATE_UNINIT;
#else
/*Driver state variable.Default state is ETH_STATE_UNINIT(0U) */
static Eth_17_EthMac_StateType Eth_17_EthMac_DriverState = ETH_STATE_UNINIT;
#endif

#define ETH_17_ETHMAC_STOP_SEC_VAR_INIT_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

#define ETH_17_ETHMAC_START_SEC_CONST_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
#if (ETH_17_PB_FIXEDADDR == STD_ON)
/* [cover parentID=DS_NAS_ETH_PR913] [/cover] */
/* Fixed address feature enabled: Use constant config pointer */
/*IFX_MISRA_RULE_08_07_STATUS=ConfigPtr of the driver cannot be declared as a
 local variable*/
static const Eth_17_EthMac_ConfigType * const Eth_17_kConfigPtr =\
                                                        &Eth_ConfigRoot[0];
#endif
#define ETH_17_ETHMAC_STOP_SEC_CONST_32BIT
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"


/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/


/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/* Memory Map of the Eth Code */
#define ETH_17_ETHMAC_START_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"

static void Eth_17_lReceiveFrames(void);

static void Eth_17_lConfirmTxFrames(void);

static void Eth_17_lResetTxRXStatus(void);

static Std_ReturnType Eth_17_lResetKernels(void);

static Std_ReturnType Eth_17_lDisableEthDma(void);

IFX_LOCAL_INLINE Std_ReturnType Eth_17_lDemtimeout(uint32 Timeout);

IFX_LOCAL_INLINE void Eth_17_lDemEthSeqErr(void);

IFX_LOCAL_INLINE Eth_ModeType Eth_17_lGetMode(void);

IFX_LOCAL_INLINE void Eth_17_lSetPhysAddr(const uint8 *MacAddr);

IFX_LOCAL_INLINE void Eth_17_lGetPhysAddr(uint8 *MacAddr);
IFX_LOCAL_INLINE uint8 Eth_17_lIncrRxBufIdx     (uint8 RxBufIdx);

IFX_LOCAL_INLINE uint8 Eth_17_lIncrTxBufIdx     (uint8 TxBufIdx);

#if ( ETH_17_DEV_ERROR_DETECT == STD_ON)
static uint32 Eth_17_lDetCheckInvalidPointer(const void * Ptr,
                                                          uint8 ServiceId);
IFX_LOCAL_INLINE uint32 Eth_17_lDetCheckInvalidCtrlIdx(uint8 CtrlIdx,
                                                          uint8 ServiceId);

IFX_LOCAL_INLINE uint32 Eth_17_lDetCheckInvalidCfgIdx(uint8 CfgIdx,
                                                          uint8 ServiceId);

static uint32 Eth_17_lDetCheckNoInit(uint8 ServiceId);

IFX_LOCAL_INLINE uint32 Eth_17_lDetCheckMode(uint8 ServiceId);

#endif /* End for ETH_17_DEV_ERROR_DETECT  */

/*******************************************************************************
**                            Low Level Driver Functions                      **
*******************************************************************************/

/*******************************************************************************
**  Traceability   :                                                          **
**              [cover parentID=DS_AS_ETH014_ETH032,DS_AS_ETH031,DS_AS_ETH027,
                    DS_AS_ETH028_ETH029_ETH030_ETH032][/cover]                **
**                                                                            **
** Syntax           : void Eth_17_EthMac_Init                                 **
**                       (const Eth_17_EthMac_ConfigType* CfgPtr )            **
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
void Eth_17_EthMac_Init(const Eth_17_EthMac_ConfigType* CfgPtr )
{
  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;

    /* ETH_17_E_INV_POINTER */
  DevError = Eth_17_lDetCheckInvalidPointer(CfgPtr, ETH_17_SID_INIT) ;

  /* Return if a development error occured */
  if (DevError == ETH_17_E_DET_NO_ERR)
  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    #if (ETH_17_PB_FIXEDADDR == STD_OFF)
    /* Store the configuration data address in a global variable to enable
    subsequent API calls to access the configuration data. */
    Eth_17_kConfigPtr = CfgPtr ;
    #else
    UNUSED_PARAMETER(CfgPtr)
    #endif

    /*Store the configuration data in global variables */
    Eth_RxBufTotal   = Eth_17_kConfigPtr->Eth_CtrlConfig->EthRxBufTotal;
    Eth_TxBufTotal   = Eth_17_kConfigPtr->Eth_CtrlConfig->EthTxBufTotal;
    Eth_RxBufLength  = Eth_17_kConfigPtr->Eth_CtrlConfig->EthCtrlRxBufLenByte;
    Eth_TxBufLength  = Eth_17_kConfigPtr->Eth_CtrlConfig->EthCtrlTxBufLenByte;
    Eth_DemAccessID  = Eth_17_kConfigPtr->Eth_CtrlConfig->EthDemAccessId;
    Eth_DemTimeout   = Eth_17_kConfigPtr->Eth_CtrlConfig->EthDemTimeout;
    Eth_DemEthSeq    = Eth_17_kConfigPtr->Eth_CtrlConfig->EthDemFrameSeqErr;
    Eth_DmaIdleTransitionWaitTime = Eth_17_kConfigPtr->Eth_CtrlConfig->\
                                                  EthDmaIdleTransitionWaitTime;
    Eth_CrcStripping = Eth_17_kConfigPtr->Eth_CtrlConfig->EthCrcStripping;
    
    /*change the state of the driver from ETH_STATE_UNINIT to ETH_STATE_INIT.*/
    Eth_17_EthMac_DriverState = ETH_STATE_INIT;
  }

}

/*******************************************************************************
**  Traceability   :                                                          **
  [cover parentID=DS_NAS_ETH_PR759_PR760_PR761,
  DS_AS_ETH077_ETH078_ETH079_ETH080_ETH081_ETH082_ETH083_ETH084_ETH085_ETH086]**
  [/cover]                                                                    **
**                                                                            **
** Syntax           : BufReq_ReturnType Eth_17_EthMac_ProvideTxBuffer(        **
**                                                            uint8 CtrlIdx,  **
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
BufReq_ReturnType Eth_17_EthMac_ProvideTxBuffer(uint8 CtrlIdx, uint8* BufIdxPtr,
                                      Eth_DataType** BufPtr,uint16* LenBytePtr)
{
  BufReq_ReturnType BufferStatus = BUFREQ_E_BUSY;
  Eth_TxBuffTableType* CurrBuffPtr;
  uint32 DescStatus;
  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError;

  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_PROVIDETXBUFFER);

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(
                                   CtrlIdx, ETH_17_SID_PROVIDETXBUFFER);
  }

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /* ETH_17_E_INV_POINTER */
    DevError = Eth_17_lDetCheckInvalidPointer(
                              (void *)BufIdxPtr, ETH_17_SID_PROVIDETXBUFFER) ;
  }

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /* ETH_17_E_INV_POINTER */
    DevError = Eth_17_lDetCheckInvalidPointer(
                              (void *)(BufPtr), ETH_17_SID_PROVIDETXBUFFER) ;
  }

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /* ETH_17_E_INV_POINTER */
    DevError = Eth_17_lDetCheckInvalidPointer(
                              (void *)LenBytePtr, ETH_17_SID_PROVIDETXBUFFER);
  }

  if(DevError != ETH_17_E_DET_NO_ERR)
  {
    BufferStatus = BUFREQ_E_NOT_OK;
  }
  else
  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    #if (ETH_17_ENA_TX_INT == STD_ON)
    SchM_Enter_Eth_17_EthMac_ProvideTxBuffer();
    #endif
    /* delayed release of buffer in case no TX confirmation was requested
       and no TX isr is used */
    CurrBuffPtr = &Eth_TxBuffTable[Eth_NextTxBuffer];
    if( (CurrBuffPtr->Eth_BuffStatus == ETH_BUFF_USED)&&\
        (CurrBuffPtr->Eth_FrameTxReq == ETH_ONE)&&\
        (CurrBuffPtr->Eth_StTxCnfn   == ETH_ZERO) )
    {
      DescStatus = Eth_TxDesc[Eth_NextTxBuffer].Status;
      #ifdef IFX_ETH_DEBUG
      DescStatus |= (uint32)TestEth_DebugMask05;
      #endif
      if((DescStatus&(uint32)ETH_DESCOWNBYDMA) == ETH_ZERO)
      {
        /* buffer has been transmitted, release buffer now */
        CurrBuffPtr->Eth_FrameTxReq = ETH_ZERO;
        CurrBuffPtr->Eth_BuffStatus = ETH_BUFF_NOTUSED;
        /* this buffer is the next one to wait for being released? */
        #ifdef IFX_ETH_DEBUG
        if(TestEth_DebugMask06 == ETH_ONE)
        {
          Eth_ConfirmTxBufIdx = Eth_17_lIncrTxBufIdx(Eth_ConfirmTxBufIdx);
        }
        #endif
        if(Eth_ConfirmTxBufIdx == Eth_NextTxBuffer)
        {
          /* implicitly confirmed, therefore move to next buffer */
          Eth_ConfirmTxBufIdx = Eth_17_lIncrTxBufIdx(Eth_ConfirmTxBufIdx);
        }
      }
    }
    if(CurrBuffPtr->Eth_BuffStatus == ETH_BUFF_NOTUSED)
    {
      *BufIdxPtr = Eth_NextTxBuffer;
      CurrBuffPtr->Eth_FrameTxReq = ETH_ZERO;
      CurrBuffPtr->Eth_StTxCnfn = ETH_ZERO;
      CurrBuffPtr->Eth_BuffStatus = ETH_BUFF_USED;
      /* DesAdd(6)+SrcAdd(6) + Type/Length(2) + Payload/Data(46-1500)+ CS(4)*/
      /*Upper layer only fill the data(Payload), Frame Header has to be filled
      by Eth_Transmit function so BufPtr points Payload Pointer   */
      /*IFX_MISRA_RULE_11_01_STATUS= Address of Buffer is taken from DMA
        descriptors and stored to the pointer "BufPtr"*/
      /*IFX_MISRA_RULE_11_03_STATUS= Address of Buffer is taken from DMA
        descriptors and stored to the pointer "BufPtr"*/
      *BufPtr = \
      (uint8 *)(Eth_TxDesc[Eth_NextTxBuffer].Buffer1 + ETH_FRAMEHEADER_LENGTH);

      /* if the requested buffer size should not exceed (Res Tx Buffer - Frame
         Header(12) - Frame CRC(4))i.e (reserved size - 18 bytes)           */
      if( (*LenBytePtr) > (Eth_TxBufLength-\
          (uint16)(ETH_FRAMEHEADER_LENGTH + ETH_FRAMECHKSUM_LENGTH )) )
      {
        *LenBytePtr = Eth_TxBufLength-\
                      (ETH_FRAMEHEADER_LENGTH + ETH_FRAMECHKSUM_LENGTH);
      }

      /* move to next buffer index */
      Eth_NextTxBuffer = Eth_17_lIncrTxBufIdx(Eth_NextTxBuffer);

      BufferStatus = BUFREQ_OK;
    }
    #if (ETH_17_ENA_TX_INT == STD_ON)
    SchM_Exit_Eth_17_EthMac_ProvideTxBuffer();
    #endif
  }

  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif

  return(BufferStatus);
}

/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=
    DS_AS_ETH033_ETH034_ETH035_ETH036_ETH037_ETH038_ETH039_ETH040][/cover]    **
**                                                                            **
** Syntax           : Std_ReturnType                                          **
**                    Eth_17_EthMac_ControllerInit(                           **
**                                           uint8 CtrlIdx, uint8 CfgIdx )    **
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
Std_ReturnType Eth_17_EthMac_ControllerInit( uint8 CtrlIdx, uint8 CfgIdx )
{
  Std_ReturnType RetValue = E_NOT_OK;
  uint8 *BuffPtr;
  uint32 RegVal;
  uint32 WaitCount;
  uint32 LoopCount;
  uint8  TempVar;

  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError;

  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_CONTROLLERINIT);

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(
                                   CtrlIdx, ETH_17_SID_CONTROLLERINIT);
  }

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
   DevError = Eth_17_lDetCheckInvalidCfgIdx(
                                   CfgIdx, ETH_17_SID_CONTROLLERINIT);
  }

  if(DevError != ETH_17_E_DET_NO_ERR)
  {
    RetValue = E_NOT_OK;
  }
  else
  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Reset End Init Protection to access regsiters */
    ETH_SFR_INIT_RESETENDINIT();
    /* Enable the ETH Module (reset DISR bit)*/
    ETH_SFR_INIT_MODIFY32(ETH_CLC.U,ETH_CLC_DISR_CLEARMASK,\
                                                  (uint32)ETH_MOD_ENABLE)
    /* set End Init Protection to protect regsiters */
    ETH_SFR_INIT_SETENDINIT();
    /* Read back DISS bit to ensure that ETH Module is enabled */
    RegVal = (uint32)((ETH_SFR_INIT_USER_MODE_READ32(ETH_CLC.U)& ETH_CLC_DISS) \
                       >> (uint32)ETH_CLC_DISS_BIT_POS);
    /* DEM_E_ACCESS error detection */
    #ifdef IFX_ETH_DEBUG
    RegVal |= TestEth_DebugMask02;
    #endif
    if(RegVal != (uint32) ETH_MOD_ENABLE)
    {
      if(Eth_DemAccessID != ETH_17_DISABLE_DEM_REPORT)
      {
        /*DEM should be reported only if DEM has been enabled  */
        Dem_ReportErrorStatus(Eth_DemAccessID,DEM_EVENT_STATUS_FAILED);
      }
    }
    else
    {
      /* Perform Kernel reset */
      RetValue = Eth_17_lResetKernels();

      if (RetValue == E_OK)
      {
        /* MAC supports both RMII and MII . selection has been done here */
        /*Before this all Port/Pin configuration has been completed and all clk
          domains available for PHY(Transceiver) because during ETH module
          reset,all PHY Signal should be available then only module reset would
          be completed*/
        /* MDIO and REFCLK alternate input selection.RMII/MII is selected */
        ETH_SFR_INIT_WRITE32(ETH_GPCTL.U,(unsigned_int)ETH_17_GPCTL_CONFIG);
        /* To select RMII it is required to trigger module reset by setting SWR
        in DMA register 0 */
        ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_BUS_MODE.U,\
                             ETH_BUS_MODE_SWR_CLEARMASK,(uint32)ETH_RESET_ON)

        /* Wait until Enabling is completed */
        WaitCount = (uint32)ETH_17_MAXTIMEOUT_COUNT;
        do
        {
          WaitCount-- ;
          #ifdef IFX_ETH_DEBUG
          RegVal = (uint32)((ETH_SFR_INIT_USER_MODE_READ32(ETH_BUS_MODE.U)
                            & ETH_BUS_MODE_SWR) |TestEth_DebugMask);
          #else
          RegVal = (uint32)(ETH_SFR_INIT_USER_MODE_READ32(ETH_BUS_MODE.U) & \
                                                    ETH_BUS_MODE_SWR);
          #endif
        }while((RegVal==(uint32)ETH_RESET_ON) && (WaitCount > ETH_ZERO));

        /* DEM_E_TIMEOUT error detection */
        if(Eth_17_lDemtimeout(WaitCount)==E_OK)
        {
          RegVal = \
          (((uint32)ETH_17_SPEED << IFX_ETH_MAC_CONFIGURATION_FES_OFF) |
           ((uint32)ETH_17_OPMODE << IFX_ETH_MAC_CONFIGURATION_DM_OFF));
                   
          if(Eth_CrcStripping == ETH_17_CRC_STRIPPING_ON)
         {
            RegVal |= ((1UL << IFX_ETH_MAC_CONFIGURATION_CST_OFF) |
                       (1UL << IFX_ETH_MAC_CONFIGURATION_ACS_OFF));
          }
         /* Ethernet Speed Selection(writing to FES bit) based on
               Ethspeed(10/100Mbps)configuration*/
          /* Ethernet operation FULL/HALF Duplex(writing to DM bit) based on
              EthOperation config */
          /* Enabling or Disabling of stripping of CRC bytes from ETH frame 
           transferred to Application (writing to CST and ACS bits) based on
              EthCrcStripping config */
          ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_MAC_CONFIGURATION.U, \
                ETH_MAC_CONFIGURATION_FES_DM_CST_ACS,RegVal)
          /*Disable Flushing of Received Frames. When DFF bit is set, the Rx DMA
            does not flush any frames because of the unavailability of receive
            descriptors or buffers */
          ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_OPERATION_MODE.U,\
                     ETH_OPERATION_MODE_DFF_CLEARMASK,(uint32)ETH_ONE << \
                     ETH_OPERATION_MODE_DFF_BIT_POS)
          /*Store and forward mode. Frames with errors are dropped by MAC.*/
          ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_OPERATION_MODE.U,\
                   ETH_OPERATION_MODE_RSF_CLEARMASK,(uint32)ETH_ONE << \
                   ETH_OPERATION_MODE_RSF_BIT_POS)

          #if (ETH_17_OPMODE == FULL_DUPLEX)
            /* Full duplex default configuration  */
            ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_MAC_CONFIGURATION.U,\
                    ETH_MAC_CONFIGURATION_FD_MASK,((uint32)((unsigned_int)\
                    (ETH_CONFIGINITFDX) & ETH_MAC_CONFIGURATION_FD_MASK)))
            ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_MAC_FRAME_FILTER.U, \
                    ETH_MAC_FRAME_FILTER_MASK,(uint32)(unsigned_int)\
                    (ETH_CONFIGFRAMFDX))
            ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_FLOW_CONTROL.U,\
                ETH_FLOW_CONTROL_MASK,(uint32)(unsigned_int)(ETH_CONFIGFLOWFDX))
          #else
            /* Half duplex default configuration  */
            ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_MAC_CONFIGURATION.U, \
                   ETH_MAC_CONFIGURATION_MASK,(unsigned_int)(ETH_CONFIGINITHDX))
            ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_MAC_FRAME_FILTER.U, \
                ETH_MAC_FRAME_FILTER_MASK,(unsigned_int)(ETH_CONFIGFRAMHDX))
            ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_FLOW_CONTROL.U, \
                ETH_FLOW_CONTROL_MASK,(unsigned_int)(ETH_CONFIGFLOWHDX))
          #endif /* End for ETH_17_OPMODE */

          /* Set the configured MAC address */
          ETH_SFR_INIT_USER_MODE_WRITE32(ETH_MAC_ADDRESS0_HIGH.U, \
                            ((unsigned_int)ETH_17_DEFAULT_MACADDRESS_HIGH | \
                            ETH_MAC_ADDRESS0_HIGH_AE_MASK));
          ETH_SFR_INIT_USER_MODE_WRITE32(ETH_MAC_ADDRESS0_LOW.U, \
                            (unsigned_int)ETH_17_DEFAULT_MACADDRESS_LOW);

          /* Disable interrupts for GMAC CORE and GMAC MTL */
          ETH_SFR_INIT_USER_MODE_WRITE32(ETH_MMC_IPC_RECEIVE_INTERRUPT_MASK.U, \
                                        (unsigned_int)ETH_DISABLE_IPC_RX_INTPT);
          ETH_SFR_INIT_USER_MODE_WRITE32(ETH_MMC_RECEIVE_INTERRUPT_MASK.U, \
                                      (unsigned_int)ETH_DISABLE_MMC_RX_INTPT);
          ETH_SFR_INIT_USER_MODE_WRITE32(ETH_MMC_TRANSMIT_INTERRUPT_MASK.U, \
                               (unsigned_int)ETH_DISABLE_MMC_TX_INTPT);
          ETH_SFR_INIT_USER_MODE_WRITE32(ETH_INTERRUPT_MASK.U, \
                                      (unsigned_int)ETH_DISABLE_PMT_TTI_INTPT);

          /* If Transmit or Receive Interrupt is enabled then Enable the SRN*/
          #if ((ETH_17_ENA_TX_INT == STD_ON) || (ETH_17_ENA_RX_INT == STD_ON))
          /* Service Request Enable */
           ETH_SFR_INIT_MODIFY32(SRC_ETH.U, ETH_SRC_SRE_CLEARMASK, \
                    (uint32)ETH_ENABLE_INTERRUPT << (uint32)ETH_SRC_SRE_BIT_POS)
          /* Clearall Interrupts */
          ETH_SFR_INIT_USER_MODE_WRITE32(ETH_STATUS.U, \
                                   (unsigned_int)ETH_DMAINTCLEARALL);
          /*Enable the Interrupt NIE */
          ETH_SFR_INIT_USER_MODE_WRITE32(ETH_INTERRUPT_ENABLE.U, \
                               (unsigned_int)ETH_DMAIENORMAL);

          #if (ETH_17_ENA_TX_INT == STD_ON)
          ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_INTERRUPT_ENABLE.U, \
              ETH_INTERRUPT_ENABLE_CLEARMASK,(unsigned_int)ETH_DMAIETXCOMPLETED)
          #endif/* End for ETH_17_ENA_TX_INT */

          #if (ETH_17_ENA_RX_INT == STD_ON)
          ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_INTERRUPT_ENABLE.U, \
              ETH_INTERRUPT_ENABLE_CLEARMASK,(unsigned_int)ETH_DMAIERXCOMPLETED)
          #endif/* End for ETH_17_ENA_RX_INT*/

          #endif /* End for ETH_17_ENA_TX_INT and ETH_17_ENA_RX_INT*/

          /* Enable the required DMA BUS MODE options */
          ETH_SFR_INIT_USER_MODE_WRITE32(ETH_BUS_MODE.U, \
                                        (unsigned_int)(ETH_DMAINIT));

          /*GMII Busy bit should read logic 0 before writing to GMIIAddress
          Register and GMII Data Register.  During a PHY or RevMII register
          access the software sets this bit to 1 to indicate that a Read  or
          Write access is in progress.*/
          WaitCount = (uint32)ETH_17_MAXTIMEOUT_COUNT;
          do
          {
            WaitCount-- ;
            #ifdef IFX_ETH_DEBUG
            RegVal =(uint32)((ETH_SFR_INIT_USER_MODE_READ32(ETH_GMII_ADDRESS.U)\
                             & ETH_GMII_ADDRESS_GB_MASK) | TestEth_DebugMask07);
            #else
            RegVal = (uint32)(ETH_SFR_INIT_USER_MODE_READ32(ETH_GMII_ADDRESS.U)\
                              & ETH_GMII_ADDRESS_GB_MASK);
            #endif
          } while((RegVal == (uint32)ETH_ONE) && (WaitCount > ETH_ZERO));
          /* DEM_E_TIMEOUT error detection */
          if(Eth_17_lDemtimeout(WaitCount)==E_OK)
          {
            /*Total Buffer allocated as linear array(RxBufTotal*RxBufLenByte)*/
            /*So we have to spilit the buffer per frame and get the address
              for eachframe buffer.Also initialize the buffer with zeros.*/

            /* Clear Rx Buffer*/
            for(LoopCount=ETH_ZERO;LoopCount<ETH_17_RXBUFFER_SIZE;LoopCount++)
            {
              Eth_RxBuffer[LoopCount] = ETH_ZERO;
            }
            BuffPtr = &Eth_RxBuffer[0];
            /*Update the RxDesc address register with current Descriptor
            address*/
            ETH_SFR_INIT_USER_MODE_WRITE32 \
                     (ETH_RECEIVE_DESCRIPTOR_LIST_ADDRESS.U, \
                     (unsigned_int)(&Eth_RxDesc[0]));
            for(TempVar = ETH_ZERO; TempVar<Eth_RxBufTotal; TempVar++)
            {
              /* Assign the Receive Buffer pointer to the Descriptor */
              /*IFX_MISRA_RULE_11_01_STATUS="Eth_RxDesc"-DMA descriptor is
                updated with the address of the RAM buffer */
              /*IFX_MISRA_RULE_11_03_STATUS="Eth_RxDesc"-DMA descriptor is
                updated with the address of the RAM buffer */
              /*IFX_MISRA_RULE_17_04_STATUS=Pointer atithemtic is done on
              BuffPtr to split a linear buffer array into multiple small
              buffers*/
              Eth_RxDesc[TempVar].Buffer1=
            (uint32)(BuffPtr + (((uint32)(TempVar))*((uint32)Eth_RxBufLength)));
              /* Clear Status of previous Frame Recetion*/
              Eth_RxDesc[TempVar].Status = (uint32)ETH_DESCOWNBYDMA;
              /*Receive Buffer length is the maximum Configured Length */
              Eth_RxDesc[TempVar].Length = Eth_RxBufLength;
            }
            /* Last Frame of the ring buffer is of MAX frame length */
            Eth_RxDesc[Eth_RxBufTotal - 1U].Length = \
              (uint32)((uint32)ETH_MAX_FRAME_LEN | (uint32)ETH_RXDESCENDOFRING);

            /* Clear Tx Buffer*/
            for(LoopCount=ETH_ZERO; LoopCount<ETH_17_TXBUFFER_SIZE; LoopCount++)
            {
              Eth_TxBuffer[LoopCount] = ETH_ZERO;
            }

            BuffPtr = &Eth_TxBuffer[0];
            ETH_SFR_INIT_USER_MODE_WRITE32 \
                             (ETH_TRANSMIT_DESCRIPTOR_LIST_ADDRESS.U, \
                             (unsigned_int)(&Eth_TxDesc[0]));
            for(TempVar = ETH_ZERO;TempVar < Eth_TxBufTotal;TempVar++)
            {
              /* Clear Buffer Table Parameters */
              Eth_TxBuffTable[TempVar].Eth_BuffStatus = ETH_BUFF_NOTUSED;
              Eth_TxBuffTable[TempVar].Eth_StTxCnfn = ETH_ZERO;
              Eth_TxBuffTable[TempVar].Eth_FrameTxReq = ETH_ZERO;

              /* Clear Status of previous Frame Recetion*/
              Eth_TxDesc[TempVar].Status = (uint32)ETH_DMADESCINIT;
              /* Assign the Receive Buffer pointer to the Descriptor */
              /*IFX_MISRA_RULE_11_01_STATUS="Eth_TxDesc"-DMA descriptor is
                 updated with the address of the RAM buffer */
              /*IFX_MISRA_RULE_11_03_STATUS="Eth_TxDesc"-DMA descriptor is
                  updated with the address of the RAM buffer */
              /*IFX_MISRA_RULE_17_04_STATUS=Pointer atithemtic is done on
                 BuffPtr to split a linear buffer array into multiple small
                 buffers*/
              Eth_TxDesc[TempVar].Buffer1=\
                 (uint32)(BuffPtr + ((uint32)TempVar*(uint32)Eth_TxBufLength));
            }
            /* End of the Transmit buffer ring */
            Eth_TxDesc[Eth_TxBufTotal - 1U].Status |= \
                                                    (uint32)ETH_TXDESCENDOFRING;

            /* Intialize all buffer index to 0. */
            Eth_NextTxBuffer = ETH_ZERO;
            Eth_CurrRxBufIdx = ETH_ZERO;
            Eth_ConfirmTxBufIdx = ETH_ZERO;
           /*initialize Rx Start index to out of bounds and invalid buffer ptr*/
            Eth_RxStartIdx = Eth_RxBufTotal;
            Eth_RxStartPtr = NULL_PTR;

            /* Enable Rx and Tx DMA */
            ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_OPERATION_MODE.U, \
                          ETH_OPERATION_MODE_SR_CLEARMASK, (uint32)ETH_ONE << \
                          ETH_OPERATION_MODE_SR_BIT_POS)
            ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_OPERATION_MODE.U, \
                          ETH_OPERATION_MODE_ST_CLEARMASK, (uint32)ETH_ONE << \
                          ETH_OPERATION_MODE_ST_BIT_POS)

            RetValue = E_OK;
            Eth_17_EthMac_DriverState = ETH_STATE_ACTIVE;
          }
        }
      }
    }
  }
  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  UNUSED_PARAMETER(CfgIdx)
  #endif
return(RetValue);
}

/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH041_ETH042_ETH043_ETH044_ETH045_ETH137_ETH138]
    [/cover]                                                                  **
**                                                                            **
** Syntax           : Std_ReturnType Eth_17_EthMac_SetControllerMode(         **
**                                    uint8 CtrlIdx, Eth_ModeType CtrlMode )  **
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
Std_ReturnType Eth_17_EthMac_SetControllerMode(uint8 CtrlIdx,
                                     Eth_ModeType CtrlMode )
{
  Std_ReturnType RetValue ;
  Eth_ModeType Eth_ModeStatus;

  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;

  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_SETCONTROLLERMODE) ;

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(
                              CtrlIdx, ETH_17_SID_SETCONTROLLERMODE);
  }
  if(DevError != ETH_17_E_DET_NO_ERR)
  {
    RetValue = E_NOT_OK;
  }
  else
  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    if(CtrlMode == ETH_MODE_ACTIVE)
    {
      Eth_ModeStatus = Eth_17_lGetMode();
      if(Eth_ModeStatus == ETH_MODE_DOWN)
      {
        Eth_17_lResetTxRXStatus();
        /* Enable MAC Tx and Rx*/
        ETH_SFR_RUNTIME_USER_MODE_MODIFY32(ETH_MAC_CONFIGURATION.U , \
            ETH_MAC_CONFIGURATION_MASK, (unsigned_int)(((uint32)ETH_TXENABLE | \
            (uint32)ETH_RXENABLE)))

        /* Enable Rx and Tx DMA */
        ETH_SFR_RUNTIME_USER_MODE_MODIFY32(ETH_OPERATION_MODE.U, \
             ETH_OPERATION_MODE_MASK,(unsigned_int)(((uint32)ETH_DMATXSTART | \
             (uint32)ETH_DMARXSTART)))

        /* Force Restart of suspended DMA Rx operation*/
        ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_RECEIVE_POLL_DEMAND.U, \
                                 (unsigned_int)ETH_ONE);
      }
      else
      {
        /* Driver alredy in active state Do Nothing*/
      }
    }
    else
    {
      /* Disable MAC Rx and Tx */
      ETH_SFR_RUNTIME_USER_MODE_MODIFY32(ETH_MAC_CONFIGURATION.U, \
           (unsigned_int)(~((uint32)ETH_TXENABLE | (uint32)ETH_RXENABLE)),0UL)

      /* Disable Rx and Tx DMA */
      ETH_SFR_RUNTIME_USER_MODE_MODIFY32(ETH_OPERATION_MODE.U, \
        (unsigned_int)(~((uint32)ETH_DMATXSTART | (uint32)ETH_DMARXSTART)),0UL)

      Eth_17_lResetTxRXStatus();
    }
    RetValue = E_OK;
  }

  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif

return(RetValue);
}

/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH046_ETH047_ETH048_ETH049_ETH050_ETH051][/cover]  **
**                                                                            **
** Syntax           : Std_ReturnType Eth_17_EthMac_GetControllerMode(         **
**                                                            uint8 CtrlIdx,  **
                                                  Eth_ModeType* CtrlModePtr)  **
**                                                                            **
** Service ID       : 0x04                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx   - Index of the controller                     **
**                                                                            **
** Parameters (out) : CtrlModePtr  -Controller Mode DOWN/ACTIVE               **
**                                                                            **
** Return value     : E_OK     - Successfully obtained controller Mode        **
**                    E_NOT_OK - Controller mode could not be get             **
**                                                                            **
** Description      : Obtains the state of the indexed controller             **
**                                                                            **
*******************************************************************************/
Std_ReturnType Eth_17_EthMac_GetControllerMode(uint8 CtrlIdx,
                                                      Eth_ModeType* CtrlModePtr)
{
  Std_ReturnType RetValue = E_OK;

  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;

  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_GETCONTROLLERMODE) ;

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(
                                   CtrlIdx, ETH_17_SID_GETCONTROLLERMODE);
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_POINTER */
    DevError = Eth_17_lDetCheckInvalidPointer(
                                   CtrlModePtr, ETH_17_SID_GETCONTROLLERMODE);
  }
  if(DevError != ETH_17_E_DET_NO_ERR)
  {
    RetValue = E_NOT_OK;
  }
  else
  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    *CtrlModePtr = Eth_17_lGetMode();
    RetValue = E_OK;
  }
  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif
return(RetValue);
}

/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH052_ETH053_ETH054_ETH055_ETH056_ETH057][/cover]  **
**                                                                            **
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
void Eth_17_EthMac_GetPhysAddr( uint8 CtrlIdx, uint8* PhysAddrPtr)
{

  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;

  /* ETH_17_E_INV_POINTER */
  DevError = Eth_17_lDetCheckInvalidPointer(
                              (void *)PhysAddrPtr, ETH_17_SID_GETPHYADDRESS) ;

  /* Return if a development error occured */
  if (DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_NOT_INITIALIZED */
    DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_GETPHYADDRESS) ;
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(
                                   CtrlIdx, ETH_17_SID_GETPHYADDRESS);
  }

  if(DevError == ETH_17_E_DET_NO_ERR)

  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    Eth_17_lGetPhysAddr(PhysAddrPtr);
  }

  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif
}

#if ( ETH_17_ENA_MII_API == STD_ON)
/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH058_ETH059_ETH060_ETH061_ETH062_ETH063] [/cover] **
**                                                                            **
** Syntax           : void Eth_17_EthMac_WriteMii(uint8 CtrlIdx,uint8 TrcvIdx,**
**                       uint8 RegIdx,uint16 RegVal)                          **
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
void Eth_17_EthMac_WriteMii(uint8 CtrlIdx,uint8 TrcvIdx,uint8 RegIdx,
                                                                  uint16 RegVal)
{
  uint32 WaitCount, AddressVal, GmiiVal;
  Std_ReturnType RetVal;

  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;
  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_WRITEMII) ;

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(CtrlIdx, ETH_17_SID_WRITEMII);
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
    /*  ETH_17_E_INV_MODE */
    (void)Eth_17_lDetCheckMode(ETH_17_SID_WRITEMII);
    #endif
    ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_GMII_DATA.U, (unsigned_int)RegVal);
    AddressVal = (uint32)((((uint32)TrcvIdx << (uint32)ETHMIIDEVSHIFT) &\
                                                    (uint32)ETHMIIDEVMASK) |\
        (((uint32)RegIdx << (uint32)ETHMIIREGSHIFT) & (uint32)ETHMIIREGMASK) |\
        (uint32)ETHMIIWRITE | (uint32) ETHMIIBUSY);

    ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_GMII_ADDRESS.U, \
                                     (unsigned_int)AddressVal);
    /* Wait until Enabling is completed */
    WaitCount = (uint32)ETH_17_MAXTIMEOUT_COUNT;
    do
    {
      WaitCount-- ;
      #ifdef IFX_ETH_DEBUG
      GmiiVal =(uint32)((ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_GMII_ADDRESS.U) &\
                                 ETH_GMII_ADDRESS_GB_MASK) | TestEth_DebugMask);
      #else
      GmiiVal =(uint32)(ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_GMII_ADDRESS.U) & \
                                                 ETH_GMII_ADDRESS_GB_MASK);
      #endif
    }while((GmiiVal == (uint32)ETH_ONE) && (WaitCount > ETH_ZERO));

    /*Timeout error incase of Hardware issue*/
    RetVal = Eth_17_lDemtimeout(WaitCount);
    UNUSED_PARAMETER(RetVal)
  }

  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif
}

/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH064_ETH065_ETH066_ETH067_ETH068_ETH069_ETH070]
    [/cover]                                                                  **
**                                                                            **
** Syntax           : void Eth_17_EthMac_ReadMii(uint8 CtrlIdx,uint8 TrcvIdx, **
**                                         uint8 RegIdx,uint16 *RegValPtr)    **
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
void Eth_17_EthMac_ReadMii(
                  uint8 CtrlIdx, uint8 TrcvIdx, uint8 RegIdx, uint16* RegValPtr)
{
  uint32 WaitCount, AddressVal, GmiiVal;

  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;
  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_READMII) ;

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(CtrlIdx, ETH_17_SID_READMII);
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_POINTER */
    DevError = Eth_17_lDetCheckInvalidPointer(RegValPtr, ETH_17_SID_READMII);
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
     /*  ETH_17_E_INV_MODE */
    (void)Eth_17_lDetCheckMode(ETH_17_SID_READMII);
    #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
    AddressVal = (uint32)((((uint32)TrcvIdx << (uint32)ETHMIIDEVSHIFT) &\
                                                      (uint32)ETHMIIDEVMASK) |\
        (((uint32)RegIdx << (uint32)ETHMIIREGSHIFT) & (uint32)ETHMIIREGMASK) |\
                (uint32)ETHMIIBUSY);

    ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_GMII_ADDRESS.U, \
                                     (unsigned_int)AddressVal);
    /* Wait until Enabling is completed */
    WaitCount = (uint32)ETH_17_MAXTIMEOUT_COUNT;
    do
    {
      WaitCount-- ;
      #ifdef IFX_ETH_DEBUG
      GmiiVal =(uint32)((ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_GMII_ADDRESS.U) &\
                         ETH_GMII_ADDRESS_GB_MASK) | TestEth_DebugMask);
      #else
      GmiiVal =(uint32)(ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_GMII_ADDRESS.U) & \
                         ETH_GMII_ADDRESS_GB_MASK);
      #endif
    }while((GmiiVal == (uint32)ETH_ONE) && (WaitCount > ETH_ZERO));

    /*Timeout error incase of Hardware issue*/
    if(Eth_17_lDemtimeout(WaitCount) == E_OK)
    {
      /* After Successful read from PHY, Data available at WORD Register*/
      *RegValPtr =(uint16)(ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_GMII_DATA.U) & \
                           ETH_HALFWORD_MASK);
    }
  }

  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif
}
#endif /* End of ETH_17_ENA_MII_API */

/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH071_ETH072_ETH073_ETH074_ETH075_ETH076][/cover]  **
**                                                                            **
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
void Eth_17_EthMac_GetCounterState( uint8 CtrlIdx, uint16 CtrOffs,
                                                             uint32* CtrValPtr )
{

  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;

  /* ETH_17_E_INV_POINTER */
  DevError = Eth_17_lDetCheckInvalidPointer(
                              (void *)CtrValPtr, ETH_17_SID_GETCOUNTERSTATE);

  /* Return if a development error occured */
  if (DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_NOT_INITIALIZED */
    DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_GETCOUNTERSTATE);
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(
                                   CtrlIdx, ETH_17_SID_GETCOUNTERSTATE);
  }

  if(DevError == ETH_17_E_DET_NO_ERR)
  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    /*IFX_MISRA_RULE_11_01_STATUS=The address of the counter is type-cast to a
     pointer to access the value of the counter */
    /*IFX_MISRA_RULE_11_03_STATUS=The address of the counter is type-cast to a
     pointer to access the value of the counter */
    *CtrValPtr = ETH_SFR_RUNTIME_USER_MODE_READ32(*(uint32 *)\
                                   ((uint32)ETH_MAC_MMCBASE + (uint32)CtrOffs));
  }

  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif
}

/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH100_ETH101_ETH102_ETH103_ETH104_ETH105][/cover]  **
**                                                                            **
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
void Eth_17_EthMac_TxConfirmation(uint8 CtrlIdx)
{

  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;
  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_TXCONFIRMATION) ;

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(
                                   CtrlIdx, ETH_17_SID_TXCONFIRMATION);
  }

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_MODE */
    DevError = Eth_17_lDetCheckMode(ETH_17_SID_TXCONFIRMATION);
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    /* If Tx Isr is enabled then confirmation is done in ISR */
    #if (ETH_17_ENA_TX_INT == STD_OFF)

    /* Local function to confirm all successfully trasmitted frames */
    Eth_17_lConfirmTxFrames();
    /* Force Start the DMA Tx*/
    ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_TRANSMIT_POLL_DEMAND.U, \
                                     (unsigned_int)ETH_ONE);

    #endif
  }
  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif
}

/*******************************************************************************
**  Traceability   :                                                          **
**[cover parentID=DS_AS_ETH087_ETH088_ETH089_ETH090_ETH091_ETH092_ETH093_ETH094,
                     DS_AS_ETH119,DS_AS_ETH004,DS_NAS_ETH_PR759_PR760_PR761]
                     [/cover]                                                 **
**                                                                            **
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
/*******************************************************************************
                           Ethernet Frame Format
+----------+-------------+--------------+-----------------------+--------------+
+  DesAddr +  SourceAddr + FrameLen /   + Frame Data /          +   CheckSum   +
+ (6Bytes) +   (6Bytes)  + Type(2Bytes) + Payload(46-1500Bytes) +    (4Bytes)  +
+----------+-------------+--------------+-----------------------+--------------+
*******************************************************************************/
Std_ReturnType Eth_17_EthMac_Transmit(
             uint8 CtrlIdx, uint8 BufIdx,Eth_FrameType FrameType,
             boolean TxConfirmation, uint16 LenByte, const uint8* PhysAddrPtr)
{
  Eth_FrameStructType *TempEthFrame;
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 TxLength = ETH_ZERO;
  uint8 LoopCount;

  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;
  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_TRANSMIT) ;

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(CtrlIdx, ETH_17_SID_TRANSMIT);
  }

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_PARAM if the BufIdx is greater then Tx buff total */
    if(BufIdx >= Eth_TxBufTotal)
    {
      Det_ReportError(ETH_17_ETHMAC_MODULE_ID, ETH_17_INSTANCE_ID,
                      ETH_17_SID_TRANSMIT,ETH_17_E_INV_PARAM);
      DevError = ETH_17_E_INV_PARAM;
    }
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_POINTER */
    DevError = Eth_17_lDetCheckInvalidPointer((const void *)PhysAddrPtr,
                                                   ETH_17_SID_TRANSMIT);
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_MODE */
    DevError = Eth_17_lDetCheckMode(ETH_17_SID_TRANSMIT);
  }
  if (DevError == ETH_17_E_DET_NO_ERR)
  #endif /* End for ETH_17_DEV_ERROR_DETECT */
  {

    /*IFX_MISRA_RULE_11_01_STATUS=Address of Buffer is taken from DMA
     descriptors and stored to the pointer TempEthFrame*/
    /*IFX_MISRA_RULE_11_03_STATUS=Address of Buffer is taken from DMA
     descriptors and stored to the pointer TempEthFrame*/
    TempEthFrame = (Eth_FrameStructType *)(void *)(Eth_TxDesc[BufIdx].Buffer1);

    /* Fill the Destination and Source MAC Address in TX/Frame Buffer */
    for(LoopCount=(uint8)0U;LoopCount<(uint8)ETH_MACADDR_LEN ;LoopCount++)
    {
      /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on PhysAddrPtr
        to access the next byte of the network address passed*/
      TempEthFrame->EthDestAddr[LoopCount]   = *(PhysAddrPtr + LoopCount);
    }
    /* Copy PHYs MAC address as source address in TX EthFrame */
    Eth_17_lGetPhysAddr(TempEthFrame->EthSourceAddr);
    /* Fill the Frame Type in TX/Frame Buffer */
    TempEthFrame->EthFrameType[0] = \
                      (uint8)((FrameType >> ETH_SHIFT_8BIT) & ETH_BYTE_MASK);
    TempEthFrame->EthFrameType[1] = (uint8)(FrameType & ETH_BYTE_MASK);

    /*Buffer2 size would be zero, single buffer and single frame at the time
     Here Header length(ETH_FRAMEHEADER_LENGTH) is added because
     provideTxbuffer API only provides the Payload buffer.*/
    TxLength = (uint32)(((uint32)LenByte+ETH_FRAMEHEADER_LENGTH) &\
                    (uint32)ETH_DESCSIZE1MASK);

    /* Critical Section As Global Variables used in ISR also. */
    #if (ETH_17_ENA_TX_INT == STD_ON)
    SchM_Enter_Eth_17_EthMac_Transmit();
    #endif
    if(TxConfirmation == (boolean)TRUE)
    {
      /* Tx confirmation request is updated in BuffTable,Based on this
         status buffers will be cleared during Eth_Txconfirmation Call */
      Eth_TxBuffTable[BufIdx].Eth_StTxCnfn = ETH_ONE;
    }
    else
    {
      Eth_TxBuffTable[BufIdx].Eth_StTxCnfn = ETH_ZERO;
    }

    Eth_TxDesc[BufIdx].Length = TxLength;

    /* Start of Transmission */
    Eth_TxDesc[BufIdx].Status  = (uint32)((Eth_TxDesc[BufIdx].Status &\
                 (~(uint32)ETH_DESCOWNBYDMA))|(uint32)ETH_DESCOWNBYDMA);

    /* Frame Transmit Request Indication*/
    Eth_TxBuffTable[BufIdx].Eth_FrameTxReq = ETH_ONE;

    ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_TRANSMIT_POLL_DEMAND.U, \
                                     (unsigned_int)ETH_ONE);

    #if (ETH_17_ENA_TX_INT == STD_ON)
    SchM_Exit_Eth_17_EthMac_Transmit(); /* Critical Section Exit*/
    #endif
    RetVal = E_OK;
  }
  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif

return(RetVal);
}


/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH095_ETH096_ETH097_ETH098_ETH099]                 **
**                                                                            **
** Syntax           : void Eth_17_EthMac_Receive( uint8 CtrlIdx )             **
**                                                                            **
**  [/cover]                                                                  **
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
** Description      : Reads Received frame                                    **
**                                                                            **
*******************************************************************************/
/*******************************************************************************
                           Ethernet Frame Format
+----------+-------------+--------------+-----------------------+--------------+
+  DesAddr +  SourceAddr + FrameLen /   + Frame Data /          +   CheckSum   +
+ (6Bytes) +   (6Bytes)  + Type(2Bytes) + Payload(46-1500Bytes) +    (4Bytes)  +
+----------+-------------+--------------+-----------------------+--------------+
*******************************************************************************/
void Eth_17_EthMac_Receive( uint8 CtrlIdx )
{

  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError;
  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_RECEIVE) ;

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(CtrlIdx, ETH_17_SID_RECEIVE);
  }

  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_MODE */
    DevError = Eth_17_lDetCheckMode(ETH_17_SID_RECEIVE);
  }

  if(DevError == ETH_17_E_DET_NO_ERR)
  #endif /* End for ETH_17_DEV_ERROR_DETECT */
  {
    /* If Rx ISR is enabled then Polling mode disabled */
    #if (ETH_17_ENA_RX_INT == STD_OFF)

    /* Local function to notify all received frames */
    Eth_17_lReceiveFrames();

    #endif /* end of ETH_17_ENA_RX_INT */
    /* Force Restart of DMA Rx operation, if suspended */
    ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_RECEIVE_POLL_DEMAND.U, \
                                     (unsigned_int)ETH_ONE);
  }

  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif
}

#if (ETH_17_ENA_TX_INT == STD_ON)
/*******************************************************************************
** Syntax           : void Eth_17_EthMac_TxIrqHdlr( void )                    **
**                                                                            **
** Service ID       : 0x11                                                    **
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
void Eth_17_EthMac_TxIrqHdlr( void )
{
  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;
  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_TXHANDLER);

  if(DevError == ETH_17_E_DET_NO_ERR)
  #endif /* End for ETH_17_DEV_ERROR_DETECT */
  {
    /* Local function to confirm all successfully trasmitted frames */
    Eth_17_lConfirmTxFrames();
    /* Force Start the DMA Tx*/
    ETH_TRANSMIT_POLL_DEMAND.U = (unsigned_int)ETH_ONE;
  }
}
#endif

/* if Receive Interrupt handlers enabled then Enable ISRs*/
#if (ETH_17_ENA_RX_INT == STD_ON)
/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH109_ETH110_ETH111_ETH112_ETH113][/cover]         **
**                                                                            **
** Syntax           : void Eth_17_EthMac_RxIrqHdlr( void )                    **
**                                                                            **
** Service ID       : 0x10                                                    **
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
void Eth_17_EthMac_RxIrqHdlr( void )
{
  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError ;
  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_RXHANDLER) ;
  if(DevError == ETH_17_E_DET_NO_ERR)
  #endif /* (ETH_17_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Local function to notify all received frames */
    Eth_17_lReceiveFrames();
    /* Force Restart of DMA Rx operation, if suspended */
    ETH_RECEIVE_POLL_DEMAND.U = (unsigned_int)ETH_ONE;
  }
}
#endif

/*******************************************************************************
**  Traceability   :                                                          **
[cover parentID=DS_NAS_ETH_PR2989,DS_NAS_ETH_PR2990,......................... **
DS_NAS_ETH_PR2991_PR2992_PR2993_PR2994][/cover]                               **
**                                                                            **
** Syntax           : void Eth_17_EthMac_SetPhysAddr( uint8 CtrlIdx,          **
**                                         const uint8* PhysAddrPtr )         **
**                                                                            **
**  [/cover]                                                                  **
**                                                                            **
** Service ID       : 0x13                                                    **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : Non-Rentrant                                            **
**                                                                            **
** Parameters (in)  : CtrlIdx  - Index of the controller                      **
**                    PhysAddrPtr - MAC address in network byte order         **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Sets the controllers MAC address                        **
**                                                                            **
*******************************************************************************/
void Eth_17_EthMac_SetPhysAddr( uint8 CtrlIdx, const uint8* PhysAddrPtr)
{
  #if (ETH_17_DEV_ERROR_DETECT == STD_ON)
  uint32 DevError;
  /*  ETH_17_E_NOT_INITIALIZED */
  DevError = Eth_17_lDetCheckNoInit(ETH_17_SID_SETPHYSADDRESS) ;
  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    /*  ETH_17_E_INV_CTRL_IDX */
    DevError = Eth_17_lDetCheckInvalidCtrlIdx(CtrlIdx,\
                                               ETH_17_SID_SETPHYSADDRESS);
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  {
    DevError = Eth_17_lDetCheckInvalidPointer((const void *)PhysAddrPtr,\
                                                   ETH_17_SID_SETPHYSADDRESS);
  }
  if(DevError == ETH_17_E_DET_NO_ERR)
  #endif /* End for ETH_17_DEV_ERROR_DETECT */
  {
    Eth_17_lSetPhysAddr(PhysAddrPtr);
  }
  #if (ETH_17_DEV_ERROR_DETECT == STD_OFF)
  UNUSED_PARAMETER(CtrlIdx)
  #endif
}

/*******************************************************************************
**  Traceability   :                                                          **
**  [cover parentID=DS_AS_ETH114_ETH115_ETH116_ETH117_ETH118,
                    DS_NAS_ETH_PR759_PR760_PR761]                  [/cover]   **
**                                                                            **
** Syntax           : void Eth_17_EthMac_TxRxIrqHandler( void )               **
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
** Description      : Frame TX/RX interrupt handler                           **
                      (Note: EthMAC has single interrupt for Tx/Rx )          **
**                  Note: loop to check NIS is required as no new interrupt   **
**                        is generated, if a new RX/TX event occurs           **
**                        while NIS is still set                              **
*******************************************************************************/
void Eth_17_EthMac_TxRxIrqHandler(void)
{
  #if ((ETH_17_ENA_TX_INT == STD_ON) || (ETH_17_ENA_RX_INT == STD_ON))
  /* Local copy for entire interrupt status*/
  Ifx_ETH_STATUS TempInterruptStatus;
  uint8 MaxLoops = 0U;

  TempInterruptStatus.U = ETH_STATUS.U;

  while (TempInterruptStatus.B.NIS == ETH_ONE)
  {
    /* normal interrupts are pending */
    #if (ETH_17_ENA_RX_INT == STD_ON)
    if (TempInterruptStatus.B.RI == ETH_ONE)
    {
      /* Call Rx Interrupt handler */
      Eth_17_EthMac_RxIrqHdlr();

      /* clear signalled RX interrupt */
      ETH_STATUS.U = (unsigned_int)((uint32)ETH_DMAINTNORMAL |\
                                    (uint32)ETH_DMAINTRXCOMPLETED);
    }
    #endif/* End for ETH_17_ENA_RX_INT */

    #if (ETH_17_ENA_TX_INT == STD_ON)
    if (TempInterruptStatus.B.TI == ETH_ONE)
    {
      /* Call Tx Interrupt handler */
      Eth_17_EthMac_TxIrqHdlr();

      /* clear signalled TX interrupt */
      ETH_STATUS.U = (unsigned_int)((uint32)ETH_DMAINTNORMAL |\
                                   (uint32)ETH_DMAINTTXCOMPLETED);
    }
    #endif/* End for ETH_17_ENA_TX_INT */

    /* update interrupt status to check,
     whether meanwhile new RX or TX event occurred */
    TempInterruptStatus.U = ETH_STATUS.U;

    #ifdef IFX_ETH_DEBUG
    /*Debug switch to simulate continuous trigger of HW interrupts*/
    if(TestEth_DebugMask03 == ETH_ONE)
    {
      MaxLoops = ETH_NIS_PENDING_MAXLOOPCNT;
    }
    #endif
    MaxLoops++;
    /* limit the check loops to avoid endless loop in case NIS does not
       reset (due to any HW failure) */
    if (MaxLoops > ETH_NIS_PENDING_MAXLOOPCNT)
    {
      break;
    }
  }
  #endif /* ETH_17_ENA_RX_INT or ETH_17_ENA_TX_INT */
}

/*******************************************************************************
** Syntax           : static void Eth_17_lConfirmTxFrames(void)               **
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
** Description      : check transmit buffers and confirm transmitted frames   **
**                                                                            **
*******************************************************************************/
static void Eth_17_lConfirmTxFrames (void)
{
  Eth_TxBuffTableType *CurrBuffPtr;
  uint32 DescStatus;
  uint8 Count;

  for (Count=ETH_ZERO; Count<Eth_TxBufTotal; Count++)
  {
    DescStatus = Eth_TxDesc[Eth_ConfirmTxBufIdx].Status;
    CurrBuffPtr = &Eth_TxBuffTable[Eth_ConfirmTxBufIdx];
    /* is this buffer used already ? */
    if((CurrBuffPtr->Eth_BuffStatus == ETH_BUFF_USED) &&\
       (CurrBuffPtr->Eth_FrameTxReq == ETH_ONE)&&\
       ((DescStatus & (uint32)ETH_DESCOWNBYDMA) == ETH_ZERO))
    {
      /* is a transmission requested already ? */
      /* Check if buffer released by DMA */
      /* Call Back function from EthIf for Tx Confirmation required ?*/
      if (CurrBuffPtr->Eth_StTxCnfn == ETH_ONE)
      {
        EthIf_Cbk_TxConfirmation(ETH_ZERO, Eth_ConfirmTxBufIdx);
        CurrBuffPtr->Eth_StTxCnfn = ETH_ZERO;
      }
      /*Clear Frame Tx Request Indication */
      CurrBuffPtr->Eth_FrameTxReq = ETH_ZERO;
      /* Free the Buffer */
      CurrBuffPtr->Eth_BuffStatus = ETH_BUFF_NOTUSED;
      /* Point to next buffer in the buffer ring */
      Eth_ConfirmTxBufIdx = Eth_17_lIncrTxBufIdx(Eth_ConfirmTxBufIdx);
    }
    else
    {
      /* Buffer unused, break the for Loop */
      break;
    }
  }
}
/*******************************************************************************
** Traceability     : [cover parentID=DS_NAS_HE2_ETH_PR2930,                  **
                                      DS_NAS_HE2_ETH_PR2931,                  **
                                      DS_NAS_EP_ETH_PR2930,...................**
                                      DS_NAS_EP_ETH_PR2931]                   **
**                                                                            **
** Syntax           : static void Eth_17_lReceiveFrames( void )               **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to scan all Rx Buffers for new Frames.         **
*******************************************************************************/
uint8 rec_data[80] = {0};//
static void Eth_17_lReceiveFrames(void)
{
  uint32 DescStatus;
  uint16 RxLength;
  uint8 BufferCount,TempVar;
  /* Loop for all Rx buffers */
  for(BufferCount=ETH_ZERO; BufferCount<Eth_RxBufTotal; BufferCount++)
  {
    DescStatus = Eth_RxDesc[Eth_CurrRxBufIdx].Status;
    if((DescStatus & (uint32)ETH_DESCOWNBYDMA) == ETH_ZERO)
    {
      if((DescStatus & (uint32)ETH_DESCRXFIRST) == (uint32)ETH_DESCRXFIRST)
      {
        /* remember index and buffer start */
        Eth_RxStartIdx = Eth_CurrRxBufIdx;
      /*IFX_MISRA_RULE_11_01_STATUS=Address of Buffer is taken from DMA
        descriptors and stored to the pointer "Eth_RxStartPtr"*/
      /*IFX_MISRA_RULE_11_03_STATUS=Address of Buffer is taken from DMA
        descriptors and stored to the pointer "Eth_RxStartPtr"*/
        Eth_RxStartPtr = (uint8*)Eth_RxDesc[Eth_RxStartIdx].Buffer1;
        /**************add Rxbuffer***************************/
              uint32      i;//

              for (i = 0; i < 60; i++)//
              {
                   rec_data[i] = Eth_RxStartPtr[i];//
              }//
              /**************end***************************/
      }
      else
      {
        #ifdef IFX_ETH_DEBUG
        /*Debug switch to simulate the HW sequence error on DMA*/
        if(TestEth_DebugMask03 == ETH_ONE)
        {
          Eth_RxStartIdx = Eth_RxBufTotal;
        }
        #endif
        /* no first RX event seen yet ? */
        if (Eth_RxStartIdx == Eth_RxBufTotal)
        {
          /* remember index of the unexpected frame, reset Buffer pointer */
          Eth_RxStartIdx = Eth_CurrRxBufIdx;
          Eth_RxStartPtr = NULL_PTR;
          /* flag a frame sequence error to DEM */
          Eth_17_lDemEthSeqErr();
        }
      }
      if((DescStatus & (uint32)ETH_DESCRXLAST) == (uint32)ETH_DESCRXLAST)
      {
        #ifdef IFX_ETH_DEBUG
        /*Debug switch to simulate the HW sequence error on DMA*/
        if(TestEth_DebugMask04 == ETH_ONE)
        {
          Eth_RxStartPtr = NULL_PTR;
        }
        #endif
        if(Eth_RxStartPtr != NULL_PTR)
        {
          /* Length of the received frame */
          RxLength = (uint16)((DescStatus >> (uint32)ETH_DESCFRAMELENGTHSHIFT)&\
                              ETH_MASK_LOWER14BITS);
          /* Application indication of Rx frame */
          EthIf_Cbk_RxIndication(ETH_ZERO,Eth_RxStartPtr,RxLength);
        }
        else
        {
          /* flag an error to DEM */
          Eth_17_lDemEthSeqErr();
          /* no start index seen, at least return this buffer to DMA */
          Eth_RxStartIdx = Eth_CurrRxBufIdx;
        }
        /* Free all Descriptors holding the ETH frame*/
        for(TempVar=Eth_RxStartIdx; TempVar<=Eth_CurrRxBufIdx; TempVar++)
        {
          Eth_RxDesc[TempVar].Status = (uint32)ETH_DESCOWNBYDMA;
        }
        /* expect next start index, set start index to invalid value */
        Eth_RxStartIdx = Eth_RxBufTotal;
        Eth_RxStartPtr = NULL_PTR;
      }
      /* Increment Rx Buffer position */
      Eth_CurrRxBufIdx = Eth_17_lIncrRxBufIdx(Eth_CurrRxBufIdx);
    }
    else
    {
      /* Descriptor still owned by DMA, break the loop */
      break;
    }
  }
}
/*******************************************************************************
** Traceability     :                                                         **
**                                                                            **
** Syntax           : static void Eth_17_lResetTxRXStatus( void )             **
**                                                                            **
** [/cover]                                                                   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Function to reset all TX and RX variables               **
*******************************************************************************/
static void Eth_17_lResetTxRXStatus(void)
{
  uint8 LoopCount;

  #if ((ETH_17_ENA_TX_INT == STD_ON) || (ETH_17_ENA_RX_INT == STD_ON))
  /* Protect variables in interrupt mode*/
  SchM_Enter_Eth_17_EthMac_SetControllerMode();
  #endif
  /* Flush DMA Tx FIFO */
  ETH_SFR_RUNTIME_USER_MODE_MODIFY32(ETH_OPERATION_MODE.U, \
                              ETH_OPERATION_MODE_FTF_CLEARMASK, \
                              (uint32)ETH_ONE << ETH_OPERATION_MODE_FTF_BIT_POS)

  /* Reset all Tc Buffer Table entries */
  for(LoopCount=ETH_ZERO; LoopCount<Eth_TxBufTotal; LoopCount++)
  {
    Eth_TxBuffTable[LoopCount].Eth_BuffStatus = ETH_BUFF_NOTUSED;
    Eth_TxBuffTable[LoopCount].Eth_StTxCnfn = ETH_ZERO;
    Eth_TxBuffTable[LoopCount].Eth_FrameTxReq = ETH_ZERO;
  }

  /* Reset Tx DMA Descriptor and Tx Buffer Ptr to begining of the list */
  ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_TRANSMIT_DESCRIPTOR_LIST_ADDRESS.U, \
                                   (unsigned_int)(&Eth_TxDesc[0]));
  /* Tx Dma to start from 0 index again*/
  Eth_NextTxBuffer = ETH_ZERO;
  Eth_ConfirmTxBufIdx = ETH_ZERO;

  /* Reset Rx DMA Descriptor and Rx Buffer Ptr to begining of the list */
  ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_RECEIVE_DESCRIPTOR_LIST_ADDRESS.U,
                                   (unsigned_int)(&Eth_RxDesc[0]));
  /* Reset Rx index to 0 */
  Eth_CurrRxBufIdx = ETH_ZERO;
  Eth_RxStartIdx = Eth_RxBufTotal;
  Eth_RxStartPtr = NULL_PTR;
  for(LoopCount = ETH_ZERO; LoopCount<Eth_RxBufTotal; LoopCount++)
  {
    /* Ignore all buffers filled by DMA. All buffers with DMA */
    Eth_RxDesc[LoopCount].Status = (uint32)ETH_DESCOWNBYDMA;
  }

  #if ((ETH_17_ENA_TX_INT == STD_ON) || (ETH_17_ENA_RX_INT == STD_ON))
  /* Protect variables in interrupt mode*/
  SchM_Exit_Eth_17_EthMac_SetControllerMode();
  #endif
}
/*******************************************************************************
** Syntax           : static Std_ReturnType Eth_17_lResetKernels(void)        **
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
** Return value     : E_OK     - Kernel reset successful                      **
**                    E_NOT_OK - Kernel reset failed                          **
**                                                                            **
** Description      : Resets Kernel                                           **
**                                                                            **
*******************************************************************************/
static Std_ReturnType Eth_17_lResetKernels(void)
{
  Std_ReturnType RetValue = E_OK;
  volatile uint32 Readback = 0U;
  uint32 WaitCount;
  uint32 RstStatus;
  uint8 IntSerRequest;

  if(Eth_17_lDisableEthDma() == E_OK)
  {
    WaitCount = ETH_INTERRUPT_SERVICE_TIMEOUT;
    
    /*Checking in a loop if any interrupt service request is pending*/
    do
    {
      WaitCount--;
      #ifdef IFX_ETH_DEBUG
      IntSerRequest =
                  (uint8)(((uint32)(ETH_SFR_INIT_USER_MODE_READ32(SRC_ETH.U) >>
                           ETH_SRC_SRR_BITPOS) & 0x01UL) | TestEth_DebugMask09);
      #else
      IntSerRequest = 
                   (uint8)((uint32)(ETH_SFR_INIT_USER_MODE_READ32(SRC_ETH.U) >>
                                        ETH_SRC_SRR_BITPOS) & 0x01UL);
      #endif
    }while((IntSerRequest != 0U) && (WaitCount > ETH_ZERO));
    
    if(Eth_17_lDemtimeout(WaitCount) != E_OK)
    {
      RetValue = E_NOT_OK;
    }
    else
    {  
      /* Service Request Disable */
      ETH_SFR_INIT_MODIFY32(SRC_ETH.U,~(ETH_MASK_SRE),ETH_MASK_CLRRSWSCLRIOVCLR)
      /* Clearall DMA Status Register 5 */
      ETH_SFR_INIT_USER_MODE_WRITE32(ETH_STATUS.U, \
                                            (unsigned_int)ETH_DMAINTCLEARALL);
      
      /* Command to reset the Kernel */
      ETH_SFR_INIT_RESETENDINIT();
      ETH_SFR_INIT_MODIFY32(ETH_KRST0.U,ETH_KRST0_RST_CLEARMASK,ETH_ONE)
      Readback = ETH_SFR_INIT_USER_MODE_READ32(ETH_KRST0.U);
      ETH_SFR_INIT_MODIFY32(ETH_KRST1.U,ETH_KRST1_RST_CLEARMASK,ETH_ONE)
      Readback = ETH_SFR_INIT_USER_MODE_READ32(ETH_KRST1.U);
      ETH_SFR_INIT_SETENDINIT();
      
      /* Wait for Timeout*/
      WaitCount = ETH_KERNEL_RESET_WAIT_COUNTS;
      do
      {
        WaitCount--;
        #ifdef IFX_ETH_DEBUG
        RstStatus = (uint32)(((ETH_SFR_INIT_USER_MODE_READ32(ETH_KRST0.U) & \
                    ETH_KRST0_RSTSTAT_MASK) >> ETH_KRST0_RSTSTAT_BIT_POS) & \
                    TestEth_DebugMask08) ;
        #else
        RstStatus = (uint32)((ETH_SFR_INIT_USER_MODE_READ32(ETH_KRST0.U) & \
                    ETH_KRST0_RSTSTAT_MASK) >> ETH_KRST0_RSTSTAT_BIT_POS);
        #endif
      }while((RstStatus == ETH_ZERO)&&(WaitCount > ETH_ZERO));
      
      if(Eth_17_lDemtimeout(WaitCount)==E_OK)
      {
        /* Clear Reset Status */
        ETH_SFR_INIT_RESETENDINIT();
        ETH_SFR_INIT_MODIFY32(ETH_KRSTCLR.U,ETH_KRSTCLR_CLR_CLRMASK, ETH_ONE)
        Readback = ETH_SFR_INIT_USER_MODE_READ32(ETH_KRSTCLR.U);
        ETH_SFR_INIT_SETENDINIT();
      }
      else
      {
         RetValue = E_NOT_OK;
      }
    }
  }
  else
  {
    RetValue = E_NOT_OK;
  }
  UNUSED_PARAMETER(Readback)
  return (RetValue);
}

/*******************************************************************************
** Syntax           : static Std_ReturnType Eth_17_lDisableEthDma(void)       **
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
** Return value     : E_OK     - ETH and DMA disabled successfully            **
**                    E_NOT_OK - ETH or DMA still active                      **
**                                                                            **
** Description      : Disables transmission and reception of ETH and DMA      **
**                                                                            **
*******************************************************************************/
static Std_ReturnType Eth_17_lDisableEthDma(void)
{
  Std_ReturnType RetValue;
  uint32 WaitCountEthDma;
  uint32 MacStatus;
  uint32 DmaStatus;

  /*Enable Flushing of Received if Receive descriptors are unavailable.*/
  ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_OPERATION_MODE.U, \
                               ETH_OPERATION_MODE_DFF_CLEARMASK,0UL)
                           
   /* Disable DMA Tx*/
  ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_OPERATION_MODE.U , \
     (unsigned_int)(~((uint32)ETH_DMATXSTART)),0UL)
  
  /* Disable MAC Rx  */
  ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_MAC_CONFIGURATION.U, \
       (unsigned_int)(~((uint32)ETH_RXENABLE)),0UL)
       
  WaitCountEthDma = ETH_MTL_TIMEOUT;
  /*Delay for DMA to flush MTL RxFIFO if Receive descriptors are not available*/
  do{
      WaitCountEthDma--;
      NOP();
  }while(WaitCountEthDma>(uint32)ETH_ZERO);
  
   /* Disable DMA Rx */
  ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_OPERATION_MODE.U , \
     (unsigned_int)(~((uint32)ETH_DMARXSTART)),0UL)

   /* Disable MAC Tx */
  ETH_SFR_INIT_USER_MODE_MODIFY32(ETH_MAC_CONFIGURATION.U, \
       (unsigned_int)(~((uint32)ETH_TXENABLE)),0UL)

  WaitCountEthDma = Eth_DmaIdleTransitionWaitTime;

  /*Checking in a time out loop if ETH and DMA has become IDLE*/
  do{
      WaitCountEthDma--;
      #ifdef IFX_ETH_DEBUG
      MacStatus = (uint32)((ETH_SFR_INIT_USER_MODE_READ32(ETH_DEBUG.U) & \
                                 ETH_DEBUG_ALL_BITS) | TestEth_DebugMask10);
      DmaStatus = (uint32)((ETH_SFR_INIT_USER_MODE_READ32(ETH_STATUS.U) & \
                                 ETH_DMA_ACTIVE_STATUS) | TestEth_DebugMask11);
      #else
      MacStatus = (uint32)(ETH_SFR_INIT_USER_MODE_READ32(ETH_DEBUG.U) & \
                                                        ETH_DEBUG_ALL_BITS);
      DmaStatus = (uint32)(ETH_SFR_INIT_USER_MODE_READ32(ETH_STATUS.U) & \
                                                        ETH_DMA_ACTIVE_STATUS);
      #endif
  }while(((MacStatus != ETH_MAC_IDLE) || (DmaStatus != ETH_DMA_IDLE)) \
                                  && (WaitCountEthDma > (uint32)ETH_ZERO));

  /*Report Dem if timeout happened*/
  RetValue = (Std_ReturnType)Eth_17_lDemtimeout(WaitCountEthDma);
  return (RetValue);
}


/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Eth_17_lSetPhysAddr               **
**                    ( const uint8* MacAddr )                                **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : MacAddr  - Physical source address (MAC address) in     **
**                    network byte order.                                     **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Sets the physical source address for the                **
**                    indexed controller                                      **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Eth_17_lSetPhysAddr(const uint8 *MacAddr)
{
  uint32 DataVal;
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   access the next byte of the network address passed*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   access the next byte of the network address passed*/
  DataVal = ((uint32)MacAddr[5] << ETH_SHIFT_8BIT) | (uint32)MacAddr[4];
  /*Write upper 16 bit of Physical address to MAC Address0 High Register*/
  ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_MAC_ADDRESS0_HIGH.U, \
                ((unsigned_int)DataVal | ETH_MAC_ADDRESS0_HIGH_AE_MASK));
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   access the next byte of the network address passed*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   access the next byte of the network address passed*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   access the next byte of the network address passed*/
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   access the next byte of the network address passed*/
  DataVal = (uint32)((((uint32)MacAddr[3]) << ETH_SHIFT_24BIT)  |\
                     (((uint32)MacAddr[2]) << ETH_SHIFT_16BIT)  |\
                     (((uint32)MacAddr[1]) << ETH_SHIFT_8BIT)   |\
                       MacAddr[0]);
  /*Write lower 4 bytes of Physical address to MAC Address0 low Register*/
  ETH_SFR_RUNTIME_USER_MODE_WRITE32(ETH_MAC_ADDRESS0_LOW.U, \
                                   (unsigned_int)DataVal);
}
/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Eth_17_lGetPhysAddr(uint8*        **
**                                                         MacAddr )          **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : None                                                    **
**                                                                            **
** Parameters (out)   MacAddr  - Physical source address (MAC address) in     **
**                    network byte order.                                     **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : Obtains the physical source address for the             **
**                    indexed controller                                      **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Eth_17_lGetPhysAddr(uint8 *MacAddr)
{
  uint32 DataVal;

  DataVal = (uint32)ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_MAC_ADDRESS0_HIGH.U);
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   store the next byte of the network address*/
  MacAddr[5] = (uint8)((DataVal >> ETH_SHIFT_8BIT) & ETH_BYTE0_MASK);
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   store the next byte of the network address*/
  MacAddr[4] = (uint8)(DataVal & ETH_BYTE0_MASK);

  DataVal = (uint32)ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_MAC_ADDRESS0_LOW.U);
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   store the next byte of the network address*/
  MacAddr[3] =(uint8)((DataVal >> ETH_SHIFT_24BIT) & ETH_BYTE0_MASK);
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   store the next byte of the network address*/
  MacAddr[2] =(uint8)((DataVal >> ETH_SHIFT_16BIT) & ETH_BYTE0_MASK);
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   store the next byte of the network address*/
  MacAddr[1] =(uint8)((DataVal >> ETH_SHIFT_8BIT ) & ETH_BYTE0_MASK);
  /*IFX_MISRA_RULE_17_04_STATUS=Pointer arithmentic is done on MacAddr to
   store the next byte of the network address*/
  MacAddr[0] =(uint8)(DataVal & ETH_BYTE0_MASK);
}
/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE uint8 Eth_17_lIncrRxBufIdx             **
**                     (uint8 RxBufIdx )                                      **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : RxBufIdx - Receive Buffer Id                            **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : Id of the next Receive Buffer                           **
**                                                                            **
** Description      : move RX buffer index to next index in ring buffer       **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Eth_17_lIncrRxBufIdx (uint8 RxBufIdx)
{
  return ((RxBufIdx+1U) % Eth_RxBufTotal);
}

/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE uint8 Eth_17_lIncrTxBufIdx             **
**                                                      (uint8 TxBufIdx)      **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : TxBufIdx - Transmit buffer Id                           **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : Id of the next Transmit Buffer                          **
**                                                                            **
** Description      : move TX buffer index to next index in ring buffer       **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE uint8 Eth_17_lIncrTxBufIdx (uint8 TxBufIdx)
{
  return ((TxBufIdx+1U) % Eth_TxBufTotal);
}
/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE Std_ReturnType Eth_17_lDemtimeout      **
**                    (uint32 Timeout)                                        **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : Timeout - Time out value                                **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : E_OK -  Timeout Dem error not detected                  **
**                    E_NOT_OK -  Timeout Dem error detected                  **
**                                                                            **
** Description      : Report Timeout DEM Error                                **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE Std_ReturnType Eth_17_lDemtimeout(uint32 Timeout)
{
  Std_ReturnType RetVal = E_OK;
  /* DEM_E_ACCESS error detection */
  if(Timeout == ETH_ZERO)
  {
    RetVal=E_NOT_OK;
    if(Eth_DemTimeout != ETH_17_DISABLE_DEM_REPORT)
    {
      /*DEM should be reported only if DEM has been enabled  */
      Dem_ReportErrorStatus(Eth_DemTimeout,DEM_EVENT_STATUS_FAILED);
    }
  }
return(RetVal);
}
/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE void Eth_17_lDemEthSeqErr(void)        **
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
** Description      : Report Frame sequence DEM Error                         **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE void Eth_17_lDemEthSeqErr(void)
{
  /* DEM_E_ACCESS error detection */
  if (Eth_DemEthSeq != ETH_17_DISABLE_DEM_REPORT)
  {
    /*DEM should be reported only if DEM has been enabled  */
    Dem_ReportErrorStatus(Eth_DemEthSeq,DEM_EVENT_STATUS_FAILED);
  }
}

/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE Eth_ModeType Eth_17_lGetMode(void)     **
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
** Return value     : ETH_MODE_ACTIVE - Controller is in active state         **
**                    ETH_MODE_DOWN   - Controler is down                     **
**                                                                            **
** Description      : Get the mode of controller                              **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE Eth_ModeType Eth_17_lGetMode(void)
{
  Eth_ModeType Status;
  uint32 DataVal;
  uint32 DataVal1;

  DataVal = (uint32)ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_MAC_CONFIGURATION.U);
  DataVal1 =  (uint32)ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_OPERATION_MODE.U);

  if( ((DataVal & ((uint32)ETH_TX |(uint32)ETH_RX)) == \
                    ((uint32)ETH_TXENABLE |(uint32) ETH_RXENABLE)) &&\
        ((DataVal1 & ((uint32)ETH_DMATXSTART | (uint32)ETH_DMARXSTART)) ==\
                     ((uint32)ETH_DMATXSTART |(uint32)ETH_DMARXSTART)) )
    {
      Status = ETH_MODE_ACTIVE;
    }
  else
    {
      Status = ETH_MODE_DOWN;
    }

  return Status ;
}
/*[cover parentID=DS_AS_ETH120,
   DS_AS_ETH127_ETH128_ETH129_ETH132_ETH134_ETH135_ETH136]*/
/*******************************************************************************
**                  Inline functions for DET checks                          **
*******************************************************************************/
/*[/cover]*/

#if (ETH_17_DEV_ERROR_DETECT == STD_ON)
/*******************************************************************************
** Syntax           : static uint32 Eth_17_lDetCheckInvalidPointer            **
**                      (const void * Ptr,uint8 ServiceId)                    **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : Ptr - Ponits to the address which is to be validated    **
**                    ServiceId - Id of API which calls this function         **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : ETH_17_E_INV_POINTER - Invalid pointer error detected   **
**                    ETH_17_E_DET_NO_ERR - Pointer is valid                  **
**                                                                            **
** Description      : DET check for ETH_17_E_INV_POINTER                      **
**                                                                            **
*******************************************************************************/
static uint32 Eth_17_lDetCheckInvalidPointer(const void * Ptr,
                                                          uint8 ServiceId)
{
  uint32 DevError;

  DevError = ETH_17_E_DET_NO_ERR;

  #if (ETH_17_PB_FIXEDADDR == STD_OFF)
    if (Ptr == NULL_PTR)
  #elif (ETH_17_PB_FIXEDADDR == STD_ON)
    if (((ServiceId == ETH_17_SID_INIT) && (Ptr != Eth_ConfigRoot)) ||\
         (Ptr == NULL_PTR) )
  #endif
    {
      Det_ReportError(ETH_17_ETHMAC_MODULE_ID, ETH_17_INSTANCE_ID, ServiceId,
                                                    ETH_17_E_INV_POINTER);
      DevError = ETH_17_E_INV_POINTER;
    }
  return DevError;
}

/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE uint32 Eth_17_lDetCheckInvalidCtrlIdx  **
**                              (uint8 CtrlIdx,uint8 ServiceId)               **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CtrlIdx - Controller Id                                 **
**                    ServiceId - Id of API which calls this function         **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : ETH_17_E_INV_CTRL_IDX - Invalid Controller Id error     **
**                                            detected                        **
**                    ETH_17_E_DET_NO_ERR -  Controller Id is valid           **
**                                                                            **
**                                                                            **
** Description      : DET check for ETH_17_E_INV_CTRL_IDX                     **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Eth_17_lDetCheckInvalidCtrlIdx(uint8 CtrlIdx,
                                                      uint8 ServiceId)
{
  uint32 DevError;

  DevError = ETH_17_E_DET_NO_ERR;

  /* Only one communication controller exists */
  if (CtrlIdx != ETH_ZERO)
  {
    Det_ReportError(ETH_17_ETHMAC_MODULE_ID, ETH_17_INSTANCE_ID, ServiceId,
                                                    ETH_17_E_INV_CTRL_IDX);
    DevError = ETH_17_E_INV_CTRL_IDX;
  }
  return DevError ;
}
/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE uint32 Eth_17_lDetCheckInvalidCfgIdx   **
**                              (uint8 CfgIdx,uint8 ServiceId)                **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : CfgIdx - Configuration Id                               **
**                    ServiceId - Id of API which calls this function         **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : ETH_17_E_INV_CONFIG - Invalid Configuration Id error    **
**                                          detected                          **
**                    ETH_17_E_DET_NO_ERR - Configuration Id is valid         **
**                                                                            **
**                                                                            **
** Description      : DET check for ETH_17_E_INV_CONFIG                       **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Eth_17_lDetCheckInvalidCfgIdx(uint8 CfgIdx,
                                                     uint8 ServiceId)
{
  uint32 DevError;

  DevError = ETH_17_E_DET_NO_ERR;

  /* Only one Configuration sets are allowed */
  if (CfgIdx != ETH_ZERO)
  {
    Det_ReportError(ETH_17_ETHMAC_MODULE_ID, ETH_17_INSTANCE_ID, ServiceId,
                                                    ETH_17_E_INV_CONFIG);
    DevError = ETH_17_E_INV_CONFIG;
  }
  return DevError ;
}

/*******************************************************************************
** Syntax           : IFX_LOCAL_INLINE uint32 Eth_17_lDetCheckMode            **
**                                                        (uint8 ServiceId)   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : ServiceId - Id of API which calls this function         **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : ETH_17_E_INV_MODE - Invalid controller mode error       **
**                                        detected                            **
**                    ETH_17_E_DET_NO_ERR - Controller mode is valid          **
**                                                                            **
**                                                                            **
** Description      : DET check for ETH_17_E_INV_MODE                         **
**                                                                            **
*******************************************************************************/
IFX_LOCAL_INLINE uint32 Eth_17_lDetCheckMode(uint8 ServiceId)
{
  uint32 DevError;
  uint32 DataVal;
  uint32 DataVal1;
  DevError = ETH_17_E_DET_NO_ERR;
  /* Eth_17_Init API was NOT called */


  DataVal = (uint32)ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_MAC_CONFIGURATION.U);
  DataVal1 = (uint32)ETH_SFR_RUNTIME_USER_MODE_READ32(ETH_OPERATION_MODE.U);
  /* Read the configuration register whether the Transmit and Receive engine is
     enabled to decise the Mode of the Driver */
  if( !(((DataVal & ((uint32)ETH_TX |(uint32)ETH_RX)) == \
              ((uint32)ETH_TXENABLE |(uint32) ETH_RXENABLE)) &&\
        ((DataVal1 & ((uint32)ETH_DMATXSTART | (uint32)ETH_DMARXSTART)) ==\
                    ((uint32)ETH_DMATXSTART |(uint32)ETH_DMARXSTART))) )
    {
      DevError = ETH_17_E_INV_MODE;
      Det_ReportError(ETH_17_ETHMAC_MODULE_ID, ETH_17_INSTANCE_ID, ServiceId,
                                                    ETH_17_E_INV_MODE);
    }
  return (DevError);
}

/*******************************************************************************
** Syntax           : static uint32 Eth_17_lDetCheckNoInit(uint8 ServiceId)   **
**                                                                            **
** Service ID       : NA                                                      **
**                                                                            **
** Sync/Async       : Synchronous                                             **
**                                                                            **
** Reentrancy       : NA                                                      **
**                                                                            **
** Parameters (in)  : ServiceId - Id of API which calls this function         **
**                                                                            **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : ETH_17_E_NOT_INITIALIZED - Driver not initialized       **
**                    ETH_17_E_DET_NO_ERR - Driver is Initialized             **
**                                                                            **
** Description      : DET check for ETH_17_E_NOT_INITIALIZED                  **
**                                                                            **
*******************************************************************************/
static uint32 Eth_17_lDetCheckNoInit(uint8 ServiceId)
{
  uint32 DevError;

  DevError = ETH_17_E_DET_NO_ERR;
  /* Eth_17_Init API was NOT called */

  if((Eth_17_EthMac_DriverState == ETH_STATE_UNINIT)&&
      (ServiceId == ETH_17_SID_CONTROLLERINIT))
  {
    DevError = ETH_17_E_NOT_INITIALIZED;

    Det_ReportError(ETH_17_ETHMAC_MODULE_ID, ETH_17_INSTANCE_ID, ServiceId,
                                                   ETH_17_E_NOT_INITIALIZED);
  }
  else if((Eth_17_EthMac_DriverState != ETH_STATE_ACTIVE)&&
          (ServiceId != ETH_17_SID_CONTROLLERINIT))
  {
    DevError = ETH_17_E_NOT_INITIALIZED;

    Det_ReportError(ETH_17_ETHMAC_MODULE_ID, ETH_17_INSTANCE_ID, ServiceId,
                                                 ETH_17_E_NOT_INITIALIZED);
  }
  else
  {
    /* Do Nothing */
  }
return (DevError);
}

#endif /* End for ETH_17_DEV_ERROR_DETECT */
/* Memory Map of the Eth Code */
#define ETH_17_ETHMAC_STOP_SEC_CODE
/*IFX_MISRA_RULE_19_01_STATUS=File inclusion after pre-processor directives is
 allowed only for MemMap.h*/
#include "MemMap.h"
