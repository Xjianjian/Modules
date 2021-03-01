/*
* Copyright (c) 2016,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File: uds_user.h
* Description: 
* History: 
* 2016-5-23, 1007040, original
*/

#ifndef __UDS_USER_H__
#define __UDS_USER_H__

#include "typedefs.h"

/* --------------------------------- Macros ------------------------------- */
/* Service: ECU reset */
#define servm_resetType_hardware                    (0x01u)
#define servm_resetTYpe_keyOffOn                    (0x02u)
#define servm_resetType_software                    (0x03u)
#define servm_resetType_enableRapidPowerShutDown    (0x04u)
#define servm_resetType_disableRapidPowerShutDown   (0x05u)
/* Service: Communication control */
#define servm_comType_normal                        (0x01u)
#define servm_comType_networkManagement             (0x02u)
#define servm_comType_normalAndNm                   (0x03u)

#define servm_comCtrl_enableRxAndTx                 (0x00u)
#define servm_comCtrl_enableRxAndDisableTx          (0x01u)
#define servm_comCtrl_disableRxAndEnableTx          (0x02u)
#define servm_comCtrl_disableRxAndTx                (0x03u)

/* Service: Control DTC setting */
#define servm_dtcSetting_on                         (0x01u)
#define servm_dtcSetting_off                        (0x02u)
/* Service: request download */
#define servm_reqDL_dataFormatOriginal              (0x00u) /* no compressionMethod nor encryptingMethod is used */
#define servm_invalidMta0                           (0xFFFFFFFFul)
/* Service: downloading (transfer data) */
/* Block valid data only, not include service ID and block SN.
 * Must aline with 8 bytes and can be divisible by 1kbyte.
 * And for transfer data stability, do not above 1024. (Actually max buffer size can be 65528) */
#define servm_tdBlockCacheDLC                       (512u)
/* Service: routine control, for eraseMemory */
#define servm_rtEraseMem_allAppProgFlash            (0x0Fu)
#define servm_rtEraseMem_allAppDataFlash            (0x0Eu)
#define servm_rtEraseMem_allExEEPROM                (0xEEu)
/* Service: routine - stayInBoot */
#define servm_stayInBootAcceptFlag                  (0x19A7u)
#define servm_stayInBootRejectFlag                  (0x0000u)


/*enable and disable interrupts*/
#define disableInterrupts  __asm__(" wrteei 0")
#define enableInterrupts   __asm__(" wrteei 1")
/* ---------------------------------- Types -------------------------------- */
/* Program step enum */
typedef enum
{
    progSt_idle = 0,
    progSt_appFingerprintValid,
    progSt_fdDownloadAllowed,
    progSt_fdDownloading,
    progSt_fdDownloadCompelete,
    progSt_fdDownloadCheckPass,
    progSt_eraseSuccess,
    progSt_downloadAllowed,
    progSt_downloading,
    progSt_downloadCompelete,
    progSt_downloadCheckPass, /* Program compelete */
    progErr_appFingerprintSaveFail,
    progErr_flashDriverInvalid,
    progErr_eraseFlashFail,
    progErr_downloadReject,
    progErr_downloadProgFail,
    progErr_downloadUnExpectTerminal,
    progErr_programFlashFail,
    progErr_unExpectTerminal
}progStatus_t;

/* --------------------------------- Exports ------------------------------ */
/* Export: variable */
extern uint8_t didTemp_actSession;
extern uint16_t didTemp_waterTemprature;
extern uint16_t didTemp_batVol;
extern uint16_t didTemp_qTemp1;
extern const uint8_t DntcD341DiagMuxFrame[136];
extern uint16_t didTemp_writeTest;

extern bool normalRxEnable;
extern bool normalTxEnable;
extern bool networkManagerRxEnable;
extern bool networkManagerTxEnable;
extern bool uds_dtcSettingOn;
extern bool uds_prog_allow_flag;
/* Export: service function */
extern void serv_ecuReset(void);
extern void serv_comControl(void);
extern void serv_dtcSetting(void);
extern void serv_requestDownload(void);
extern void serv_transferData(void);
extern void serv_requestTransferExit(void);
extern bool uds_updateAppAllowed(void);
/* Export: DID read, write callback function */
extern uint8_t didrd_activeSession(void);
extern uint8_t didrd_waterTemp(void);
extern uint8_t didrd_batVol(void);
extern uint8_t didrd_qTemp1(void);
/* Export: Routine control */


#endif /*End of __UDS_USER_H__ */
/*--------EOF--------*/


