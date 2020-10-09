/*
* Copyright (c) 2016,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File: uds_pri.h
* Description: 
* History: 
* 2016-5-20, 1007040, original
*/

#ifndef __UDS_PRI_H__
#define __UDS_PRI_H__

#include "typedefs.h"

#define servm_positiveResponeCode(servID) ((servID) + 0x40)
#define servm_negativeResponseSID         (0x7Fu)

#define servm_servID_empty                (0xFFu)

#define servm_saLv_allLocked              (0xFFu)
#define servm_saLv_allSeedChecked         (0x00u)

#endif /*End of __UDS_PRI_H__ */
/*--------EOF--------*/

