/******************************************************
闁哄倸娲ｅ▎銏ゅ触瀹ュ繒绐�	MemIf.c
闁硅绻楅崼顏堟晬閿燂拷
Data			Vasion			author
2020/8/25		V1.0			liujian
*******************************************************/

/*******************************************************
description闁挎冻鎷穒nclude the header file
*******************************************************/
#include "MemIf.h"


/*******************************************************
description闁挎冻鎷穏lobal variable definitions
*******************************************************/


/*******************************************************
description闁挎冻鎷穝tatic variable definitions
*******************************************************/
static uint8_t s_bkupBuf[EE_BKUP_BUF_LEN];
static uint8_t s_ttbuf[EE_OBJECT_TOTALBUF];
static uint8_t s_ttbuf_validFlag;
static uint8_t s_ttbuf_renewFlag;

static uint8_t s_ccp_validFlag;
static uint8_t s_uds_validFlag;
/*******************************************************
description闁挎冻鎷穎unction declaration
*******************************************************/
/*Global function declaration*/

/*Static function declaration*/
static uint32_t MemIf_checkSum(uint8_t *Data_p, uint16_t auwDataLength);
static uint8_t MemIf_checkDtValid(uint32_t s_addr);
static uint8_t MemIf_ccp_checksum(void);
static uint8_t MemIf_varCheck(uint8_t* sdt,uint8_t* ddt ,uint8_t len);
static void MemIf_BTDtSave(void);
static void MemIf_ccpDtSave(void);
static void MemIf_udsDtSave(void);
static uint8_t MemIf_uds_checksum(void);
static void MemIf_backupRestore(void);
static void MemIf_importDtbackup(void);
/******************************************************
description闁挎冻鎷穎unction code
******************************************************/

void MemIf_Init(void)
{
#if EE_STRGOBJ_INDFLASH

	if(1 == MemIf_checkDtValid(EEPROM_MAIN_START_ADDR))
	{
		s_ttbuf_validFlag = 1;
	}

	s_ttbuf_renewFlag = 0;
	s_ccp_validFlag = MemIf_ccp_checksum();
	s_uds_validFlag = MemIf_uds_checksum();

	if(MemDtBkup_Enable == 1)
	{
		if(0 == s_uds_validFlag)
		{//
			MemIf_backupRestore();
		}
	}

	MemIfCfg_recvStorageCb();
#endif
}


void MemIf_mainFunction(void)
{
#if EE_STRGOBJ_INDFLASH

#if 1
	MemIf_ccpDtSave();
#endif

	MemIf_udsDtSave();

	if(1 == s_ttbuf_renewFlag)
	{
		MemIf_BTDtSave();
		s_ttbuf_renewFlag = 0;
	}

	if(MemDtBkup_Enable == 1)
	{
		MemIf_importDtbackup();
	}

#endif
}

void MemIf_udsStoreImmediately(void)
{
#if EE_EXT_WATCHDOG_EN
	mc33907_changeWdWindow(WD_DISABLE);/*close SBC Watchdog*/
#endif
	MemIf_udsDtSave();

	if(MemDtBkup_Enable == 1)
	{
		MemIf_importDtbackup();
	}
#if EE_EXT_WATCHDOG_EN
	mc33907_changeWdWindow(WD_WIN_256);
#endif
}

uint8_t MemIf_WriteEE(uint8_t Le_u_Object,uint8_t* Le_u_Data,uint8_t Le_u_Lng,uint8_t imWrFlag)
{
	uint8_t Le_u_ok = 0U;
	uint8_t Le_u_i = 0U;
	
	switch(CaEepromCfg_Conf[Le_u_Object].Space)
	{
		case EepromCfg_IntEE:
		{
			MemIfCfg_FLASH_Unlock();
			if(CaEepromCfg_Conf[Le_u_Object].Lng == Le_u_Lng)
			{
				for(Le_u_i=0; Le_u_i < Le_u_Lng; Le_u_i++)//
				{
					MemIfCfg_FLASH_ProgramByte((CaEepromCfg_Conf[Le_u_Object].Addr + Le_u_i), Le_u_Data[Le_u_i]);
				}
				Le_u_ok = 1U;
			}
			MemIfCfg_FLASH_Lock();
		}
		break;
		case EepromCfg_ExtEE:
		
		break;
		case EepromCfg_InDflash:
		{
			for(Le_u_i=0; Le_u_i < Le_u_Lng; Le_u_i++)//
			{
				s_ttbuf[CaEepromCfg_Conf[Le_u_Object].Addr + Le_u_i] = Le_u_Data[Le_u_i];
			}

			if(0 == imWrFlag)
			{
				s_ttbuf_renewFlag = 1;
			}
			else
			{
				MemIf_BTDtSave();
				s_ttbuf_renewFlag = 0;
			}
			Le_u_ok = 1U;
		}
		break;
		default:
		break;
	}
	
	return  Le_u_ok;	
}


uint8_t MemIf_ReadEE(uint8_t Le_u_Object,uint8_t* Le_u_Data,uint8_t Le_u_Lng)
{
	uint8_t Le_u_ok = 0U;
	uint8_t Le_u_i = 0U;
	
	if(0 == s_ttbuf_validFlag)
	{
		return Le_u_ok;
	}

	switch(CaEepromCfg_Conf[Le_u_Object].Space)
	{
		case EepromCfg_IntEE:
		{
			if(CaEepromCfg_Conf[Le_u_Object].Lng == Le_u_Lng)
			{
				for(Le_u_i=0; Le_u_i < Le_u_Lng; Le_u_i++)
				{
					Le_u_Data[Le_u_i] = MemIfCfg_FLASH_ReadByte(CaEepromCfg_Conf[Le_u_Object].Addr + Le_u_i);			
				}
				Le_u_ok = 1U;
			}
		}
		break;
		case EepromCfg_ExtEE:
		
		break;
		case EepromCfg_InDflash:
		{
			for(Le_u_i=0; Le_u_i < Le_u_Lng; Le_u_i++)//
			{
				Le_u_Data[Le_u_i] = s_ttbuf[CaEepromCfg_Conf[Le_u_Object].Addr + Le_u_i];
			}
			Le_u_ok = 1U;
		}
		break;
		default:
		break;
	}
	return Le_u_ok;
}


static void MemIf_backupRestore(void)
{
	uint16_t Le_w_i,Le_w_j;
	uint16_t Le_w_len;

	Le_w_len = 4;
	for(Le_w_i = 0;Le_w_i < (EE_BKUP_BUF_LEN>>2);Le_w_i++)
	{
		*((uint32_t*)(&s_bkupBuf[(Le_w_i << 2)])) = MemIfCfg_FLASH_ReadDWord(EEPROM_BKUP_START_ADDR + (Le_w_i << 2));
	}

	if(*(uint32_t*)&s_bkupBuf[0] == MemIf_checkSum(&s_bkupBuf[EE_CHECKSUM_LNG],(EE_BKUP_BUF_LEN-EE_CHECKSUM_LNG)))
	{//backup checksum ok
		for(Le_w_i = 0;Le_w_i < MemDtBkup_Num;Le_w_i++)
		{
			if(NULL == MemCfg_Bkup[Le_w_i].Dt) continue;
			if((Le_w_len + MemCfg_Bkup[Le_w_i].len) >= EE_BKUP_BUF_LEN)
			{
				break;
			}
			for(Le_w_j = 0;Le_w_j < MemCfg_Bkup[Le_w_i].len;Le_w_j++)
			{
				MemCfg_Bkup[Le_w_i].Dt[Le_w_j] = s_bkupBuf[Le_w_len++];
			}
		}
	}
}

static void MemIf_importDtbackup(void)
{
	uint16_t Le_w_i,Le_w_j;
	uint16_t Le_w_len;

	Le_w_len = 4;
	for(Le_w_i = 0;Le_w_i < MemDtBkup_Num;Le_w_i++)
	{
		if(NULL == MemCfg_Bkup[Le_w_i].Dt) continue;

		if((Le_w_len + MemCfg_Bkup[Le_w_i].len) >= EE_BKUP_BUF_LEN)
		{
			break;
		}

		for(Le_w_j = 0;Le_w_j < MemCfg_Bkup[Le_w_i].len;Le_w_j++)
		{
			s_bkupBuf[Le_w_len++] = MemCfg_Bkup[Le_w_i].Dt[Le_w_j];
		}
	}

	*(uint32_t*)&s_bkupBuf[0] = MemIf_checkSum(&s_bkupBuf[EE_CHECKSUM_LNG],(EE_BKUP_BUF_LEN-EE_CHECKSUM_LNG));
	MemIfCfg_FLASH_Unlock();
	MemIfCfg_FLASH_erase(3);
	if(1 == MemIfCfg_FLASH_ProgramDWord(EEPROM_BKUP_START_ADDR,s_bkupBuf,EE_BKUP_BUF_LEN))
	{

	}
	MemIfCfg_FLASH_Lock();
}

static uint32_t MemIf_checkSum(uint8_t *Data_p, uint16_t auwDataLength)
{
		uint32_t aubChecksum = 0;
	    uint16_t auwCnt;

	    for(auwCnt = 0;auwCnt < auwDataLength;auwCnt++)
	    {
	        aubChecksum ^= Data_p[auwCnt];
	    }


	    return aubChecksum;
}

static uint8_t MemIf_checkDtValid(uint32_t s_addr)
{
	uint8_t valid = 0;
	uint16_t Le_w_i = 0U;

	for(Le_w_i = 0;Le_w_i < (EE_OBJECT_TOTALBUF>>2);Le_w_i++)
	{

		*((uint32_t*)(&s_ttbuf[(Le_w_i << 2)])) = MemIfCfg_FLASH_ReadDWord(s_addr + (Le_w_i << 2));
	}

	if(*(uint32_t*)&s_ttbuf[0] == MemIf_checkSum(&s_ttbuf[EE_CHECKSUM_LNG],(EE_OBJECT_TOTALBUF-EE_CHECKSUM_LNG)))
	{
		valid = 1;
	}

	return valid;
}

static uint8_t MemIf_ccp_checksum(void)
{
	uint8_t valid = 0;
	uint16_t Le_w_i = 0U;
	uint32_t blockId;
	uint8_t pjt_appversion[EEPROM_CCP_PRJ_VER_LNG];

	for(Le_w_i = 0;Le_w_i < ((EepromCfg_ccp.dtLen+EEPROM_CCP_DATA_OFFSET)>>2);Le_w_i++)
	{

		*((uint32_t*)(EepromCfg_ccp.ramAddrCpy + (Le_w_i << 2))) = \
									MemIfCfg_FLASH_ReadDWord(EepromCfg_ccp.m_eeAddr + (Le_w_i << 2));
	}

	util_memoryCpy8((uint8_t *)EepromCfg_ccp.ramAddrCpy,pjt_appversion,EEPROM_CCP_PRJ_VER_LNG);
	util_memoryCpy32((uint32_t *)(EepromCfg_ccp.ramAddrCpy+EEPROM_CCP_ID_OFFSET),&blockId,1);

	if(0 == MemIf_varCheck(releasePjtVersion,pjt_appversion,EEPROM_CCP_PRJ_VER_LNG))
	{
		return valid;
	}

	if(blockId == EepromCfg_ccp.id)
	{
		if(*(uint32_t *)(EepromCfg_ccp.ramAddrCpy + EEPROM_CCP_CHECKSUM_OFFSET) == 	\
										util_checksum((uint8_t *)(EepromCfg_ccp.ramAddrCpy + EEPROM_CCP_DATA_OFFSET),EepromCfg_ccp.dtLen))
		{
			util_memoryCpy8((uint8_t *)(EepromCfg_ccp.ramAddrCpy + EEPROM_CCP_DATA_OFFSET),(uint8_t *)EepromCfg_ccp.ramAddr,EepromCfg_ccp.dtLen);
			valid = 1;
		}
	}


	return valid;
}

static uint8_t MemIf_varCheck(uint8_t* sdt,uint8_t* ddt ,uint8_t len)
{
	uint8_t charIndx;
	for(charIndx = 0; charIndx < len; charIndx ++)
	{
		if(ddt[charIndx] != sdt[charIndx])
		{
			break;
		}
	}

	return ((charIndx >= len)?(1):(0));
}

static void MemIf_BTDtSave(void)
{
	*(uint32_t*)&s_ttbuf[0] = MemIf_checkSum(&s_ttbuf[EE_CHECKSUM_LNG],(EE_OBJECT_TOTALBUF-EE_CHECKSUM_LNG));
	MemIfCfg_FLASH_Unlock();
	MemIfCfg_FLASH_erase(0);
	if(1 == MemIfCfg_FLASH_ProgramDWord(EEPROM_MAIN_START_ADDR,s_ttbuf,EE_OBJECT_TOTALBUF))
	{

	}
	MemIfCfg_FLASH_Lock();
}

static void MemIf_ccpDtSave(void)
{
	uint32_t cSum;
	uint32_t Indx;
	uint8_t  Le_u_needsave=0;

	if(0 == s_ccp_validFlag)
	{
		Le_u_needsave=1;
	}
	else
	{
		//Compare the ROM area and the RAM area data
		for(Indx = 0; Indx < EepromCfg_ccp.dtLen; Indx++)
		{
			if(*((uint8_t*)(EepromCfg_ccp.m_eeAddr+EEPROM_CCP_DATA_OFFSET+Indx)) != \
												*((uint8_t *)(EepromCfg_ccp.ramAddr+Indx)))
			{
				break;
			}
		}

		if(Indx < EepromCfg_ccp.dtLen)
		{
			Le_u_needsave=1;
		}
	}

	if(1 == Le_u_needsave)
	{
		MemIfCfg_FLASH_Unlock();

		MemIfCfg_FLASH_erase(2);
		util_memoryCpy8(releasePjtVersion,(uint8_t *)EepromCfg_ccp.ramAddrCpy,EEPROM_CCP_PRJ_VER_LNG);
		util_memoryCpy32((uint32_t *)(&EepromCfg_ccp.id),(uint32_t *)(EepromCfg_ccp.ramAddrCpy + EEPROM_CCP_ID_OFFSET),1);
		cSum = util_checksum((uint8_t *)(EepromCfg_ccp.ramAddr),EepromCfg_ccp.dtLen);
		util_memoryCpy32(&cSum,(uint32_t *)(EepromCfg_ccp.ramAddrCpy + EEPROM_CCP_CHECKSUM_OFFSET),1);
		util_memoryCpy8((uint8_t *)EepromCfg_ccp.ramAddr,(uint8_t *)(EepromCfg_ccp.ramAddrCpy + EEPROM_CCP_DATA_OFFSET),EepromCfg_ccp.dtLen);
		MemIfCfg_FLASH_ProgramDWord(EepromCfg_ccp.m_eeAddr,(uint8_t *)EepromCfg_ccp.ramAddrCpy,(EepromCfg_ccp.dtLen+EEPROM_CCP_DATA_OFFSET));
		MemIfCfg_FLASH_Lock();

		s_ccp_validFlag = 1;
	}
}

static void MemIf_udsDtSave(void)
{
	uint32_t cSum;
	uint32_t Indx;
	uint8_t  Le_u_needsave=0;

	if(0 == s_uds_validFlag)
	{
		Le_u_needsave = 1;
	}
	else
	{
		//Compare the ROM area and the RAM area data
		for(Indx = 0; Indx < EepromCfg_uds.dtLen; Indx++)
		{
			if(*((uint8_t*)(EepromCfg_uds.m_eeAddr+EEPROM_UDS_DATA_OFFSET+Indx)) != \
												*((uint8_t *)(EepromCfg_uds.ramAddr+Indx)))
			{
				break;
			}
		}

		if(Indx < EepromCfg_uds.dtLen)
		{
			Le_u_needsave = 1;
		}
	}

	if(1 == Le_u_needsave)
	{
		MemIfCfg_FLASH_Unlock();

		MemIfCfg_FLASH_erase(1);
		util_memoryCpy8(releasePjtVersion+8,(uint8_t *)EepromCfg_uds.ramAddrCpy,EEPROM_UDS_PRJ_VER_LNG);
		util_memoryCpy32((uint32_t *)(&EepromCfg_uds.id),(uint32_t *)(EepromCfg_uds.ramAddrCpy + EEPROM_UDS_ID_OFFSET),1);
		cSum = util_checksum((uint8_t *)(EepromCfg_uds.ramAddr),EepromCfg_uds.dtLen);
		util_memoryCpy32(&cSum,(uint32_t *)(EepromCfg_uds.ramAddrCpy + EEPROM_UDS_CHECKSUM_OFFSET),1);
		util_memoryCpy8((uint8_t *)EepromCfg_uds.ramAddr,(uint8_t *)(EepromCfg_uds.ramAddrCpy + EEPROM_UDS_DATA_OFFSET),EepromCfg_uds.dtLen);
		MemIfCfg_FLASH_ProgramDWord(EepromCfg_uds.m_eeAddr,(uint8_t *)EepromCfg_uds.ramAddrCpy,(EepromCfg_uds.dtLen+EEPROM_UDS_DATA_OFFSET));
		MemIfCfg_FLASH_Lock();

		s_uds_validFlag = 1;
	}
}

static uint8_t MemIf_uds_checksum(void)
{
	uint8_t valid = 0;
	uint16_t Le_w_i = 0U;
	uint32_t blockId;
	//uint8_t pjt_bswversion[EEPROM_UDS_PRJ_VER_LNG];

	for(Le_w_i = 0;Le_w_i < ((EepromCfg_uds.dtLen+EEPROM_UDS_DATA_OFFSET)>>2);Le_w_i++)
	{

		*((uint32_t*)(EepromCfg_uds.ramAddrCpy + (Le_w_i << 2))) = \
									MemIfCfg_FLASH_ReadDWord(EepromCfg_uds.m_eeAddr + (Le_w_i << 2));
	}

	//util_memoryCpy8((uint8_t *)EepromCfg_uds.ramAddrCpy,pjt_bswversion,EEPROM_UDS_PRJ_VER_LNG);
	util_memoryCpy32((uint32_t *)(EepromCfg_uds.ramAddrCpy+EEPROM_UDS_ID_OFFSET),&blockId,1);

	//if(0 == MemIf_varCheck(releasePjtVersion+8,pjt_bswversion,EEPROM_UDS_PRJ_VER_LNG))
	//{
	//	return 0;
	//}

	if(blockId == EepromCfg_uds.id)
	{
		if(*(uint32_t *)(EepromCfg_uds.ramAddrCpy + EEPROM_UDS_CHECKSUM_OFFSET) == 	\
										util_checksum((uint8_t *)(EepromCfg_uds.ramAddrCpy + EEPROM_UDS_DATA_OFFSET),EepromCfg_uds.dtLen))
		{
			util_memoryCpy8((uint8_t *)(EepromCfg_uds.ramAddrCpy + EEPROM_UDS_DATA_OFFSET),(uint8_t *)EepromCfg_uds.ramAddr,EepromCfg_uds.dtLen);
			valid = 1;
		}
	}


	return valid;
}

uint8_t  MemIf_needRenewSt(void)
{
	return s_ttbuf_renewFlag;
}
