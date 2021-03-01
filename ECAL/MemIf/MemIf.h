/******************************************************
蹇欓垾鎾拷鐩茬闇叉皳鑱仹鑼傚綍鎷�MemIf.h
蹇欒伀鑱尗椹存幊鑼傚綍
Data			  Vasion			author
2020/8/25		V1.0			liujian
*******************************************************/
#ifndef		MEMIF_H
#define		MEMIF_H
/*******************************************************
description鑼傚綍include the header file
*******************************************************/
#include "MemIf_cfg.h"


/*******************************************************
description鑼傚綍macro definitions
*******************************************************/
/**********姘撳簮鑱皳褰曢埀顑炪儮锟介瞾姘撳簮鎷х洸楣�*******/

/**********姘撳簮鑱皳璧傝祩鑼呴垾梅寰濄儌顔溾埗銇拷*******/



/***********姘撳簮鑱皳閳ッ风煫锔癸拷**********/

/*******************************************************
description鑼傚綍struct definitions
*******************************************************/

/*******************************************************
description鑼傚綍typedef definitions
*******************************************************/
/*****struct definitions*****/

/******enum definitions******/

/******union definitions*****/

/*******************************************************
description鑼傚綍variable External declaration
*******************************************************/

/*******************************************************
description鑼傚綍function External declaration
*******************************************************/
extern void MemIf_Init(void);
extern void MemIf_mainFunction(void);
extern void MemIf_udsStoreImmediately(void);
extern uint8_t MemIf_ReadEE(uint8_t Le_u_Object,uint8_t* Le_u_Data,uint8_t Le_u_Lng);
extern uint8_t MemIf_WriteEE(uint8_t Le_u_Object,uint8_t* Le_u_Data,uint8_t Le_u_Lng,uint8_t imWrFlag);
extern uint8_t MemIf_needRenewSt(void);
#endif
