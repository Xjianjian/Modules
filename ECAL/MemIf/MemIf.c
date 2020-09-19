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
static uint8_t s_ttbuf[EE_OBJECT_TOTALBUF];
static uint8_t s_ttbuf_validFlag;
static uint8_t s_ttbuf_renewFlag;

/*******************************************************
description闁挎冻鎷穎unction declaration
*******************************************************/
/*Global function declaration*/

/*Static function declaration*/
static uint32_t MemIf_checkSum(uint8_t *Data_p, uint16_t auwDataLength);
static uint8_t MemIf_checkDtValid(uint32_t s_addr);
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
	else
	{
		if(1 == MemIf_checkDtValid(EEPROM_BKUP_START_ADDR))
		{
			s_ttbuf_validFlag = 1;
		}
	}

	s_ttbuf_renewFlag = 0;
#endif
}


void MemIf_mainFunction(void)
{
#if EE_STRGOBJ_INDFLASH

	if(1 == s_ttbuf_renewFlag)
	{
		s_ttbuf_renewFlag = 0;
		*(uint32_t*)&s_ttbuf[0] = MemIf_checkSum(&s_ttbuf[EE_CHECKSUM_LNG],(EE_OBJECT_TOTALBUF-EE_CHECKSUM_LNG));
		MemIfCfg_FLASH_Unlock();
		MemIfCfg_FLASH_erase(0);
		if(1 == MemIfCfg_FLASH_ProgramDWord(EEPROM_MAIN_START_ADDR,s_ttbuf,EE_OBJECT_TOTALBUF))
		{

		}

		MemIfCfg_FLASH_erase(1);
		if(1 == MemIfCfg_FLASH_ProgramDWord(EEPROM_BKUP_START_ADDR,s_ttbuf,EE_OBJECT_TOTALBUF))
		{

		}
		MemIfCfg_FLASH_Lock();
	}
#endif
}

uint8_t MemIf_WriteEE(uint8_t Le_u_Object,uint8_t* Le_u_Data,uint8_t Le_u_Lng)
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

			s_ttbuf_renewFlag = 1;
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

uint8_t  MemIf_needRenewSt(void)
{
	return s_ttbuf_renewFlag;
}


