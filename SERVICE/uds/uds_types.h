/*
* Copyright (c) 2016,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File: uds_types.h
* Description: 
* Note: Do not modify this file by user
* History: 
* 2016-5-19, 1007040, original
*/

#ifndef __UDS_TYPES_H__
#define __UDS_TYPES_H__

#include "typedefs.h"
#include "platformTypes.h"


#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef TRUE
#define TRUE  	(1)
#endif

#ifndef FALSE
#define FALSE	(0)
#endif



/* Service support config type define */
typedef struct
{
    uint8_t sid;    /* Service ID */
    uint8_t ssSptMask; /* service session support mask */
    uint8_t saDeniedMask; /* 0x00 for not need SA unlock, others for need one of any SA unlock */
    uint16_t minDLC; /* not include service name */
    void (* processCallBack) (void); /* Process callback function */
}udsServCfg_t;


typedef struct
{
	uint8_t subCmd;
	uint8_t (*subFunProcess)(void);
}uds_readDtcInfo_t;

/* UDS defined session type */
#define sessionType_std   0x01 /* default session */
#define sessionType_prog  0x02 /* program session */
#define sessionType_ext   0x03 /* extend session */
#define sessionType_safetySystem 0x04 /* For e.g. airbag deployment */
/* Session switch mask (MSB: form, LSB: to), General used */
#define ssSwitchMask(from, to) ((((from) << 8) & 0xFF00u) | ((to) & 0x00FFu))
#define ssSwitch_stdTostd  0x0101u
#define ssSwitch_stdToProg 0x0102u
#define ssSwitch_stdToExt  0x0103u
#define ssSwitch_progToStd 0x0201u
#define ssSwitch_progToSprog 0x0202u
#define ssSwitch_progToExt 0x0203u
#define ssSwitch_extToStd  0x0301u
#define ssSwitch_extToProg 0x0302u
#define ssSwitch_extToext  0x0303u
/* Switch with safetySystem session */
#define ssSwitch_stdToSafety  0x0104u
#define ssSwitch_progToSafety 0x0204u
#define ssSwitch_extToSafety  0x0304u
#define ssSwitch_safetyToStd  0x0401u
#define ssSwitch_safetyToProg 0x0402u
#define ssSwitch_safetyToExt  0x0403u

/* ------------------- UDS service define ------------------- */
/* Service ID define */
/* Diagnostic and communication management */
#define sid_sessionControl                   (0x10u)
#define sid_ecuReset                         (0x11u)
#define sid_securityAccess                   (0x27u)
#define sid_communicationControl             (0x28u)
#define sid_testPresent                      (0x3Eu)
#define sid_accessTimingParamenter           (0x83u)
#define sid_seuredDataTransmission           (0x84u)
#define sid_controlDTCSetting                (0x85u)
#define sid_responseOnEvent                  (0x86u)
#define sid_linkControl                      (0x87u)
/* Data transmission */
#define sid_readDataByIdentifier             (0x22u)
#define sid_readMemoryByAddress              (0x23u)
#define sid_readScalingDataByIdentifier      (0x24u)
#define sid_readDataByPeriodicIdentifier     (0x2Au)
#define sid_dynamicallyDefineDataIdentifier  (0x2Cu)
#define sid_writeDataByIdentifier            (0x2Eu)
#define sid_writeMemoryByAddress             (0x3Du)
/* Store data transmission */
#define sid_clearDiagnosticInformation       (0x14u)
#define sid_readDTCInformation               (0x19u)
/* Input Output Control */
#define sid_inputOutputControlByIdentifier   (0x2Fu)
/* Remote activation of routine function */
#define sid_routineControl                   (0x31u)
/* Upload download */
#define sid_requestDownload                  (0x34u)
#define sid_requestUpload                    (0x35u)
#define sid_transferData                     (0x36u)
#define sid_requestTransferExit              (0x37u)
/* Service sub-function mask */
#define uds_actSubFunctionMask               (0x7Fu) /* get active subfunction code */
#define uds_actResponseHideBySubFunctionMask (0x80u) /* get subfunction active response hide bit */
/* ------------------- End UDS service ID define ------------------- */

/* ------------------- Standard Negative Response Code (ISO-14229-1) ----------------------- */
#define nrsp_positiveResponse                          (0x00u)
#define nrsp_generalReject                             (0x10u)
#define nrsp_serviceNotSupported                       (0x11u)
#define nrsp_subFunctionNotSupported                   (0x12u)
#define nrsp_incorrectMessageLengthOrInvalidFormat     (0x13u)
#define nrsp_responseTooLong                           (0x14u)
#define nrsp_busyRepeatRequest                         (0x21u)
#define nrsp_conditionsNotCorrect                      (0x22u)
#define nrsp_requestSequenceError                      (0x24u)
#define nrsp_noResponseFromSubnetComponent             (0x25u)
#define nrsp_failurePreventsExecutionOfRequestedAction (0x26u)
#define nrsp_requestOutOfRange                         (0x31u)
#define nrsp_securityAccessDenied                      (0x33u)
#define nrsp_invalidKey                                (0x35u)
#define nrsp_exceedNumberOfAttempts                    (0x36u)
#define nrsp_requiredTimeDelayNotExpired               (0x37u)
#define nrsp_uploadDownloadNotAccepted                 (0x70u)
#define nrsp_transferDataSuspended                     (0x71u)
#define nrsp_generalProgrammingFailure                 (0x72u)
#define nrsp_wrongBlockSequenceCounter                 (0x73u)
#define nrsp_requestCorrectlyReceivedResponsePending   (0x78u)
#define nrsp_subFunctionNotSupportedInActiveSession    (0x7Eu)
#define nrsp_serviceNotSupportedInActiveSession        (0x7Fu)
#define nrsp_temperatureTooHigh                        (0x86u)
#define nrsp_temperatureTooLow                         (0x87u)
#define nrsp_voltageTooHigh                            (0x92u)
#define nrsp_voltageTooLow                             (0x93u)
/*0x38 ~ 0x4F for ISO 15764 Extended */
#define nrsp_downloadNotAccepted                       (0x40u)
#define nrsp_improperDownloadType                      (0x41u)
#define nrsp_cantDownloadToSpecialAddress              (0x42u)
#define nrsp_cantDownloadNumberOfBytesRequested        (0x43u)
/* ------------------- End Standard Negative Response Code (ISO-14229-1) ----------------------- */

/* ------------------- Standard DID define ----------------------- */
#define did_bootSoftwareIdentification                  (0xF180u)
#define did_applicationSoftwareIdentification           (0xF181u)
#define did_applicationDataIdentification               (0xF182u)
#define did_bootSoftwareFingerprint                     (0xF183u)
#define did_applicationSoftwareFingerprint              (0xF184u)
#define did_applicationDataFingerprint                  (0xF185u)
#define did_activeDiagnosticSession                     (0xF186u)
#define did_vehicleManufacturerSparePartNumber          (0xF187u)
#define did_vehicleManufacturerECUSoftwareNumber        (0xF188u)
#define did_vehicleManufacturerECUSoftwareVersionNumber (0xF189u)
#define did_systemSupplierIdentifier                    (0xF18Au)
#define did_ECUManufacturingDate                        (0xF18Bu)
#define did_ECUSerialNumber                             (0xF18Cu)
#define did_supportedFunctionalUnits                    (0xF18Du)
#define did_vehicleManufacturerKitAssemblyPartNumber    (0xF18Eu)
#define did_VIN                                         (0xF190u)
#define did_vehicleManufacturerECUHardwareNumber        (0xF191u)
#define did_systemSupplierECUHardwareNumber             (0xF192u)
#define did_systemSupplierECUHardwareVersionNumber      (0xF193u)
#define did_systemSupplierECUSoftwareNumber             (0xF194u)
#define did_systemSupplierECUSoftwareVersionNumber      (0xF195u)
#define did_exhaustRegulationOrTypeApprovalNumber       (0xF196u)
#define did_systemNameOrEngineType                      (0xF197u)
#define did_repairShopCodeOrTesterSerialNumber          (0xF198u)
#define did_programmingDate                             (0xF199u)
#define did_calibrationRepairShopCodeOrEquipmentSN      (0xF19Au) /* calibrationRepairShopCodeOrCalibrationEquipmentSerialNumberDataIdentifier */
#define did_calibrationDate                             (0xF19Bu)
#define did_calibrationEquipmentSoftwareNumber          (0xF19Cu)
#define did_ECUInstallationDate                         (0xF19Du)
#define did_ODXFile                                     (0xF19Eu)
#define did_entity                                      (0xF19Fu)


#define did_WaterTemp									(0xF1A0u)
#define did_batVol										(0xF1A1u)
#define did_qTemp1Vol									(0xF1A2u)
#define did_diagMuxframe								(0xEF82u)

#define did_testWriteDid								(0xF101)


/* ------------------- End standard DID define ------------------- */

/* ------------------- Standard routine ID define ---------------- */
#define rtid_deployLoopRoutineID                        (0xE200u) /* This value shall be used to initiate the deployment of the previously selected ignition loop. */
#define rtid_eraseMemory                                (0xFF00u)
#define rtid_checkProgrammingDependencies               (0xFF01u)
#define rtid_eraseMirrorMemoryDTCs                      (0xFF02u)
#define rtid_programcondCheck							(0x0203u)
/* ------------------- End standard routine ID define ------------ */

/* ------------------- HSAE pre-define routine ID ---------------- */
#define rtid_hs_stayInBoot                              (0xFE00u)
#define rtid_hs_slaveProgramMode                        (0xFE01u)
/* ------------------- HSAE pre-define routine ID ---------------- */


/***********UDS 0X19 service***************/
/*subFunction*/
#define UDS_reportNumberOfDTCByStatusMask                      (0x01)
#define UDS_reportDTCByStatusMask                              (0x02)
#define UDS_reportDTCSnapshotIdentification                    (0x03)
#define UDS_reportDTCSnapshotRecordByDTCNumber                 (0x04)
#define UDS_reportDTCSnapshotRecordByRecordNumber              (0x05)
#define UDS_reportDTCExtendedDataRecordByDTCNumber             (0x06)
#define UDS_reportNumberOfDTCBySeverityMaskRecord              (0x07)
#define UDS_reportDTCBySeverityMaskRecord                      (0x08)
#define UDS_reportSeverityInformationOfDTC                     (0x09)
#define UDS_reportSupportedDTC                                 (0x0A)
#define UDS_reportFirstTestFailedDTC                           (0x0B)
#define UDS_reportFirstConfirmedDTC                            (0x0C)
#define UDS_reportMostRecentTestFailedDTC                      (0x0D)
#define UDS_reportMostRecentConfirmedDTC                       (0x0E)
#define UDS_reportMirroyDTCByStatusMask                        (0x0F)
#define UDS_reportMirrorMemoryDTCExtendedDtatRecordByDTCNumber (0x10)
#define UDS_reportNumberOfMirrorMemoryDTCByStatusMask          (0x11)
#define UDS_reportNumberOfEmissionsRelatedOBDDTCByStatusMask   (0x12)
#define UDS_reportEmissionRelatedOBDDTCByStatusMask            (0x13)
#define UDS_reportDTCFaultDetectionCounter                     (0x14)
#define UDS_reportDTCWithPermanentStatus                       (0x15)
#define uds_reportDtcSupportSubFunNum						   (0x15)
/***********End UDS 0X19 service***************/






/* ------------------- For pre-set frequently used service ------------------------------ */
/* Service: session control */
#define servm_ssSptMask(ss_index)    ((uint8_t)(0x01u << (ss_index))) /* ssMap_i is the session index of ssSpt_t enum (also session map item index) */
/* For security access */
#define servm_saLvSptMask(saLv_index)  ((uint8_t)(0x01u << (saLv_index)))
#define servm_saIsGetSeed(saType)   (0x01u == ((saType) & 0x01u))

/* Service: security access */
typedef struct
{
    uint8_t level;
    uint8_t * seedBuf; /* Seed buffer need to be store when generate for used to check key */
    uint16_t seedLen;  /* Seed data length (Bytes) */
	uint8_t * keyBuf; /* Seed buffer need to be store when generate for used to check key */
    uint16_t keyLen;  /* Seed data length (Bytes) */
}saLevelMap_t;

/* Service: DID read or write */
/* DID access type flags */
#define servm_didReadOnly           (0x01u)
#define servm_didWriteOnly          (0x02u)
#define servm_didReadWrite          (0x03u)

/* DID config struct */


/* DID background call process item define */
#define servm_didRwBkcState_idle   (0x00u)
#define servm_didRwBkcState_Read   (0x01u)
#define servm_didRwBkcState_Write  (0x02u)
#define servm_didRwBkcInvalidIndex (0xFFFFu)


/*Memory read/write background process item define*/
#define servm_memRwBkcState_idle	(0x00u)
#define servm_memRwBkcState_read	(0x01u)
#define servm_memRwBkcState_write	(0x02u)
#define servm_memRwBkcInvalidIndx	(0xFFu)




typedef struct
{
    uint8_t bkcState;    /* 0: for IDLE, 1: for read, 2: for write */
    uint16_t index; /* DID index in DID config array */
}udsDIDrwBkc_t; /* Used bkcState & index to double check background call condition when a DID pending */

typedef struct
{
	uint8_t memOperation;
	uint8_t memIndx;
}udsMemBkc_t;

/* Service: routine control */
/* Routine control sub-function */
#define servm_rtidStartRoutine        0x01
#define servm_rtidStopRoutine         0x02
#define servm_rtidGetRoutineResult    0x03
/* Global mask for routine control sub-function: (0x01 << Sub-function) */
#define servm_rtGlobalMask_start      0x02 /* 0x01 << servm_rtidStartRoutine */
#define servm_rtGlobalMask_stop       0x04 /* 0x01 << servm_rtidStopRoutine */
#define servm_rtGlobalMask_getResult  0x08 /* 0x01 << servm_rtidGetRoutineResult */
#define servm_rtGlobalMask_all        0xFF

/* Routine control config struct */
/* Note:
 *      1. Routine control (Start: rtStart, Stop: rtStop, Get Result: rtGetResult) return value is NRSP code, but only start routine can support response pending NRSP.
 *      2. For the start routine, both return no NRSP (0x00) or response pending (0x78) will be add the rtTask to UDS background task if rtTask is not empty.
 *      3. If start routine with response pending NRSP. You must support rtTask also. And you need send valid response (Pos. or Neg.) when task compelete(return TRUE).
 *         If you not send real response. It will send response pending always by cycle and will be locked by NT layer always, the UDS will can not receive any more request.
 *         In others, if return response pending NRSP, UDS will autmaticly instead it with conditionsNotCorrect NRSP.
 *      4. Routine task will be always call with uds task cycle after rtTask start till stop call or rtTask return TRUE.
 *         That also means, if a routine is runing with response pending, it will can not start any more other routine.
 *         In addition, the routine task can be call only in uds task. Not call in start routine process.
 *      5. Do not response or read any information of uds in routine task callback (rtTask) if the start routine is not return response pending.
 *      6. Only the first 32 routines can have rtTask, after 32 routine, rtTask is in-valid.
 *      7. You can used UDS data push function to push active response data, but do not trig response send in any callback expect rtTask when rtStart return response pending.
 *      9. It will can not stop task when the uds service automaticly return to default session.
 *         If you need stop it when return to default session or session switch to another, please do it in uds_sessionTimeout or uds_sessionSwitch funtion.
*/
typedef struct
{
    uint16_t rtid;                   /* Routine Control ID */
    uint8_t (*rtStart)(void);       /* Start routine callback */
    uint8_t (*rtStop)(void);        /* Stop routine callback */
    uint8_t (*rtGetResult)(void);   /* Get routine result callback */
    bool (*rtTask)(void);           /* routine task process callback */
}udsRtIDcfg_t;



/*uds IO control*/
#define servm_ioCtrl_returnCtrlToEcu		0x00
#define servm_ioCtrl_resetToDefault			0x01
#define servm_ioCtrl_freezeCurrentSt		0x02
#define servm_ioCtrl_shortTermAdjust		0x03

typedef struct
{
	uint16_t did;								/*data id*/
	void 	*addr;								/*buffer to store the physical value */
	uint8_t dtSize;								/*physical value bytes number, can only be 1,2 or 4*/
	const void    *defVal;							/*buffer to store the default value,the *defVal size must be equal to dtSize*/
	bool	*lock;				
	void	*lockVal;							/*buffer to store the lock value,the *lockvalue size must be equal to dtSize*/		
	void (*phy2Comm)(uint32_t phyValue);		/*here need to handle the relation between the physical value and the com value*/
	uint8_t (*comm2Phy)(void * phyValue);			/*here need to handle the relation between the physical value and the com value*/
}udsIoCtrlCfg_t;

typedef struct
{
	uint16_t tick;						/*once tick != 0, the value will be short-item adjusted by background task*/
	uint16_t elapseTick;				/*elpase tick until now*/
	uint32_t targetVal;					/*the target value*/
	uint32_t initVal;
}udsIoCtrlDt_t;





/* ------------------- End Special macros for frequently used service -------------------------- */

#endif /*End of __UDS_TYPES_H__ */
/*--------EOF--------*/

