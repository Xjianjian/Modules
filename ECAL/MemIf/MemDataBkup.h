/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:globalVarExport.h
* Description:
* REQ IDs:
* History:
* 2016-11-7, Devon Draft version
* 2017-10-16, Add PIN number description
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef _MEM_DATA_BACKUP_H_
#define _MEM_DATA_BACKUP_H_

/*------------------------------------------------------------------------------
                                  Include files
------------------------------------------------------------------------------*/
#include "importTypes.h"
#include "didcfg_var.h"
#include "dtcInfo.h"

#define EEPROM_BKUP_EN       1//
#define EEPROM_BKUP_NUM      6//


typedef struct
{
    uint8_t* Dt;
    uint16_t len;
}MemCfg_Bkup_Struct;

extern const uint8_t MemDtBkup_Enable;
extern const uint16_t MemDtBkup_Num;
extern const MemCfg_Bkup_Struct MemCfg_Bkup[EEPROM_BKUP_NUM];

#endif
