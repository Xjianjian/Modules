
#include "derivative.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "flash.h"
#include "swt.h"

const flashBlockInf_t flashBlocksInf[flashBlockTotalCnt] =
{
    /* Low D-Flash, each 16kByte */
    {lowDFlash_block0_startAddr          , flashSize_16kByte_startAddr},
    {lowDFlash_block1_startAddr          , flashSize_16kByte_startAddr},
    /* Mid D-Flash, each 32kByte */
    {midDFlash_block0_startAddr          , flashSize_32kByte_startAddr},
    {midDFlash_block1_startAddr          , flashSize_32kByte_startAddr},
    /* small P-Flash, each 16kByte */
    {smallPFlash_block2_boot0_startAddr  , flashSize_16kByte_startAddr},
    {smallPFlash_block3_boot1_startAddr  , flashSize_16kByte_startAddr},
    /* medium P-Flash, each 64kByte */
    {mediumPFlash_block0_boot2_startAddr , flashSize_64kByte_startAddr},
    {mediumPFlash_block1_boot3_startAddr , flashSize_64kByte_startAddr},
    {mediumPFlash_block2_startAddr       , flashSize_64kByte_startAddr},
    {mediumPFlash_block3_startAddr       , flashSize_64kByte_startAddr},
    {mediumPFlash_block4_startAddr       , flashSize_64kByte_startAddr},
    {mediumPFlash_block5_startAddr       , flashSize_64kByte_startAddr},
    /* large P-Flash each 256kByte */
    {largePFlash_block0_boot4_startAddr  , flashSize_256kByte_startAddr},
    {largePFlash_block1_boot5_startAddr  , flashSize_256kByte_startAddr},
    {largePFlash_block2_boot6_startAddr  , flashSize_256kByte_startAddr},
    {largePFlash_block3_boot7_startAddr  , flashSize_256kByte_startAddr},
    {largePFlash_block4_startAddr        , flashSize_256kByte_startAddr},
    {largePFlash_block5_startAddr        , flashSize_256kByte_startAddr},
    {largePFlash_block6_startAddr        , flashSize_256kByte_startAddr},
    {largePFlash_block7_startAddr        , flashSize_256kByte_startAddr}
};

static uint8_t flashProtectBlocks[flashProtectBlockCnt] = {mediumPFlash_block0_boot2};


void flash_init(void)
{
	/* unlock all blocks (for simplicity) */
	C55FMC.LOCK0.R = 0;
	C55FMC.LOCK1.R = 0;
	C55FMC.LOCK2.R = 0;
	C55FMC.LOCK3.R = 0;
}

void flash_loadDriver(void)
{
}

/*
 * Function:flash_getBlockNum
 * Param<uint32_t  startAddr>:
 * Return<flashBlock_t>: block index in flashBlocksInf, also flashBlock_t item name expect 0xFF. 0xFF (macro: flashBlockEmpty) means not find.
 * Discription: get the flash block number by start address
 * Note: 
*/
uint8_t flash_getBlockIndex(uint32_t  startAddr)
{
    uint8_t flashBlock = 0;

    for(flashBlock = 0; flashBlock < flashBlockTotalCnt; flashBlock ++)
    {
        /*note: mta0 must start from the start addr of a flash sector.*/
        if(startAddr == flashBlocksInf[flashBlock].startAddr)
        {
            break;
        }
    }

    if (flashBlock >= flashBlockTotalCnt)
    {
        flashBlock = flashBlockEmpty;
    }
    
    return(flashBlock);
}

bool flash_eraseBlock(uint8_t blockIndex)
{
    bool eraseAllowed = TRUE;
    bool eraseSuccess = FALSE;
    uint8_t protectBlock_i = 0;

    /* erase block index check */
    if (blockIndex >= flashBlockTotalCnt)
    {
        /* above max block index, forbid erase */
        eraseAllowed = FALSE;
    }
    else
    {
        /* protect block search */
        for (; protectBlock_i < flashProtectBlockCnt; protectBlock_i++)
        {
            if (blockIndex == flashProtectBlocks[protectBlock_i])
            {
                /* erase block index is protect block, forbid erase. */
                eraseAllowed = FALSE;
                break;
            }
        }
    }

    if (eraseAllowed)
    {
    	/* clr ecc event error if have*/
    	if (1 == C55FMC.MCR.B.EER)
    	{
    		C55FMC.MCR.B.EER = 1;
    	}
    	/* clr read-while-write error if have*/
    	if (1 == C55FMC.MCR.B.RWE)
    	{
    		C55FMC.MCR.B.RWE = 1;
    	}

        /*wait for last programming or erasing completed*/
        while(C55FMC.MCR.B.PGM)
        {
        	/*feed watchdog*/
        	swt_feedDog();
        }
        while(C55FMC.MCR.B.ERS)
        {
        	/*feed watchdog*/
        	swt_feedDog();
        }

    	/*step1. unlock all blocks */
    	C55FMC.LOCK0.R  = 0;
        C55FMC.LOCK1.R = 0;
        C55FMC.LOCK2.R = 0;
        C55FMC.LOCK3.R = 0;

    	/*step2. change the vaule in the MCR.ERS bit from 0 to 1.*/
        C55FMC.MCR.B.ERS = 1;

        /*step3. Select the block to be erasered*/
        /*the mid Flash is for EEPROM*/
        if (blockIndex < dFlashLowBlockCnt)
        {
        	/* low D-Flash*/
        	C55FMC.SEL0.B.LOWSEL = 0x01u << blockIndex;
        }
        else if (blockIndex < (dFlashLowBlockCnt + dFlashMidBlockCnt))
        {
            /* mid D-Flash */
            C55FMC.SEL0.B.MIDSEL = 0x01u << (blockIndex - dFlashLowBlockCnt);
        }
        else if (blockIndex < (dFlashLowBlockCnt + dFlashMidBlockCnt + pFlashSmallBlockCnt))
        {
            /* small P-Flash */
            C55FMC.SEL0.B.LOWSEL =  0x01u << (blockIndex - dFlashLowBlockCnt - dFlashMidBlockCnt + 2);
        }
        else if (blockIndex < (dFlashLowBlockCnt + dFlashMidBlockCnt + pFlashSmallBlockCnt + pFlashMediumBlockCnt))
        {
        	/* medium P-Flash*/
        	C55FMC.SEL1.B.HIGHSEL = (1 << (blockIndex - dFlashLowBlockCnt - dFlashMidBlockCnt - pFlashSmallBlockCnt));
        }
        else /* here can make sure blockIndex is less than ( < ) flashBlockTotalCnt becouse above block index detect */
        {
        	/* large P-Flash*/
        	C55FMC.SEL2.B.A256KSEL = (1 << (blockIndex - dFlashLowBlockCnt - dFlashMidBlockCnt - pFlashSmallBlockCnt - pFlashMediumBlockCnt));
        }

        /*step4	. write to any address in flash block: interlock write*/
        *(unsigned int*)flashBlocksInf[blockIndex].startAddr = 0xFFFFFFFF;
        /* step 5: start erase*/
        C55FMC.MCR.B.EHV = 1;
        /* step 6: wait erase finish*/
        while(C55FMC.MCR.B.DONE == 0)
        {
            /* Here need set an timeout tick and service watchdog */
        	/*feed watchdog*/
        	swt_feedDog();
        }

        C55FMC.MCR.B.EHV = 0; /* clr EHV bit*/
        C55FMC.MCR.B.ERS = 0; /* terminate erase operation*/

        eraseSuccess = TRUE;
    }
    
    return eraseSuccess;
}

/*
 * Function:flash_eraseBlockByStartAddr
 * Param<uint32_t startAddr>: Erase block start address
 * Return<uint32_t>: Erase flash size. 0: for erase fail or block start address invalid.
 * Discription:
 * Note: 
*/
uint32_t flash_eraseBlockByStartAddr(uint32_t startAddr)
{
    uint32_t eraseSize = 0;
    uint8_t blockIndex = flash_getBlockIndex(startAddr); /* Get block index by start address */
    
    if (blockIndex < flashBlockTotalCnt) /* check block index */
    {
        /* block index valid */
        /* erase block */
        if (TRUE == flash_eraseBlock(blockIndex))
        {
            /* erase success */
            /* record erase size */
            eraseSize = flashBlocksInf[blockIndex].addrSize;
        }
    }

    return eraseSize; /* return erase size */
}

/*
 * Function:flash_isInFlashCheck
 * Param<uint32_t startAddr>: check start address
 * Param<uint16_t dataSize>: check address size
 * Return<bool>: TRUE: is in Flash (D-Flash or P-Flash). FALSE: one or more address is not in FLASH.
 * Discription:
 * Note: 
*/
bool flash_isInFlashCheck(uint32_t startAddr, uint16_t dataSize)
{
    bool isInFlash = FALSE;

    /* in D-Flash check */
    if ((startAddr >= dFlash_startAddress) /* check start address is valid */
        && ((startAddr + dataSize - 1) <= dFlash_endAddress) /* check end address(startAddr plus dataSize) is valid */
        && (startAddr < (startAddr + dataSize))) /* address range not roll back */
    {
        isInFlash = TRUE;
    }

    /* in P-Flash check */
    if ((startAddr >= pFlash_startAddress) /* check start address is valid */
        && ((startAddr + dataSize - 1) <= pFlash_endAddress) /* check end address(startAddr plus dataSize) is valid */
        && (startAddr < (startAddr + dataSize))) /* address range not roll back */
    {
        isInFlash = TRUE;
    }

    return isInFlash;
}

/*
 * Function:flash_notInProtectCheck
 * Param<uint32_t startAddr>: flash start address
 * Param<uint16_t dataSize>: flash address size
 * Return<bool>: TRUE, the address range is not in FLASH protect area. FALSE, have one or more address is in FLASH protect area.
 * Discription:
 *              check logic:  startAddress(check) <= endAddress(protect block) and endAddress(check) >= startAddress(protect block)
 *              protect block:                   |<- start address,                       end address  ->|
 *              case 1:        |<-  startAddress(check)       endAddress(check) -> |
 *              case 2:                               |<- startAddress(check)   endAddress(check) ->|
 *              case 3:                                                       |<- startAddress(check)       endAddress(check) ->|
 * Note: 
*/
bool flash_notInProtectCheck(uint32_t startAddr, uint16_t dataSize)
{
    bool notInProtect = TRUE; /* Pre-set result to TRUE */
    uint8_t protectFlash_i = 0; /* index in flashProtectBlocks array */
    uint8_t blockIndex = 0; /* flash block index in flashBlocksInf array */

    /* address range roll back check */
    if (startAddr < (startAddr + dataSize))
    {
        /* search all of protect block */
        for (; protectFlash_i < flashProtectBlockCnt; protectFlash_i++)
        {
            /* get block index */
            blockIndex = flashProtectBlocks[protectFlash_i];
            /* in protect check */
            if ((startAddr <= (flashBlocksInf[blockIndex].startAddr + flashBlocksInf[blockIndex].addrSize))
                && ((startAddr + dataSize) >= flashBlocksInf[blockIndex].startAddr))
            {
                /* have one or more address in current protect block */
                notInProtect = FALSE;
                break;
            }
        }
    }
    else
    {
        notInProtect = FALSE;
    }

    return notInProtect;
}

/*
 * Function:flash_program
 * Param<uint32_t startAddr>: program target address
 * Param<uint8_t progData[]>: program source data
 * Param<uint16_t dataSize>:  program source data size
 * Return<bool>: TRUE: program accept and success. FALSE: program fail or reject.
 * Discription:
 * Note: 
*/
bool flash_program(uint32_t startAddr, uint8_t progData[], uint16_t dataSize)
{
    bool progSuccess = TRUE; /* program result, pre-set to TRUE */
    bool progAccept = TRUE;   /* Flash program accept flag, pre-set to TRUE */
    uint16_t progDt_i = 0;    /* program data index of progData[] */
    uint32_t *targetProgData = (uint32_t *)(startAddr); /* target program address data point */
    uint32_t *sourceProgData = (uint32_t *)(progData);

    /* program area is in Flash check */
    progAccept = flash_isInFlashCheck(startAddr, dataSize);
    
    /* Protect memory check */
    progAccept = progAccept & flash_notInProtectCheck(startAddr, dataSize);
    
    if ((TRUE == progAccept) /* in Flash area and not in protect flash area */
        && ((startAddr & 0x07ul) == 0)  /* and aline with 4 bytes (startAddr) */
        && (0 != dataSize) /* and data size not zero(0) */
        && ((dataSize & 0x07ul) == 0))  /* and aline with 4 bytes (dataSize) */
    {
        dataSize = dataSize >> 2; /* change progData from byte array to word(4 byte) array, so set dataSize to quarter(1/4) */
        /* Program flash */
        while ((progDt_i < dataSize) && (TRUE == progSuccess))
        {
            /* program 4 bytes (DWORD) */
            /* pre-check, the current program 4 source byts, if not equre(!=) target data, program it, else jump over to next DWORD */
            if ((targetProgData[progDt_i] != sourceProgData[progDt_i])
                || (targetProgData[progDt_i + 1] != sourceProgData[progDt_i + 1]))
            {
                /* program or erase pending check */
                while(C55FMC.MCR.B.PGM)
                {
                    /* feed watchdog */
                	swt_feedDog();
                }

                while(C55FMC.MCR.B.ERS)
                {
                	/* feed watchdog */
                	swt_feedDog();
                }


                /* set program */
                C55FMC.MCR.B.PGM = 1;
                /* write program data to target address */
                targetProgData[progDt_i] = sourceProgData[progDt_i];
                targetProgData[progDt_i + 1] = sourceProgData[progDt_i + 1];
                /* start program */
                C55FMC.MCR.B.EHV = 1;
                /* wait program compelete */
                while(!C55FMC.MCR.B.DONE ) /* wait DONE*/
                {
                	/* feed watchdog */
                	swt_feedDog();
                }
                /* comfirm program compelete */
                while(!C55FMC.MCR.B.PEG)  /*  confirm PEG*/
                {
                	/* feed watchdog */
                	swt_feedDog();
                }
                /* set Flash high voltage shut down */
                C55FMC.MCR.B.EHV = 0;
                /* set flash program idle */
                C55FMC.MCR.B.PGM = 0;       /* also terminate program*/
                /* check program result */
                if ((targetProgData[progDt_i] != sourceProgData[progDt_i])
                    || (targetProgData[progDt_i + 1] != sourceProgData[progDt_i + 1]))
                {
                    /* program fail (target program address data not equre source data after program) */
                    /* set program fail */
                    progSuccess = FALSE;
                    break; /* stop program */
                }
            }

            progDt_i += 2; /* progDT_i pointer to next 8 bytes (2 * 4 bytes) */
        }
    }
    else
    {
        /* Not allowed program, becouse not in flash area, contain protect flash area or not aline with 4. */
        /* set program result to FALSE */
        progSuccess = FALSE;
    }

    return progSuccess;
}

bool jump2boot_copy_ram_to_ee(uint8_t * p_buf)
{

	//uint8_t * p_buf = (uint8_t *)m_dtc_dt_ram_addr;
	uint32_t check_sum = 0;
	bool retval1 = FALSE;
	bool retval2 = FALSE;
	uint8_t write_remain_time = 3;
#if 0
	check_sum = util_checksum((uint8_t *)m_dtc_dt_ram_addr,m_dtc_dt_ram_dlc - 4);
	*(uint8_t *)(m_dtc_dt_ram_addr + m_dtc_dt_ram_dlc - 4) = (uint8_t)((check_sum >> 24) & 0xFF);
	*(uint8_t *)(m_dtc_dt_ram_addr + m_dtc_dt_ram_dlc - 3) = (uint8_t)((check_sum >> 16) & 0xFF);
	*(uint8_t *)(m_dtc_dt_ram_addr + m_dtc_dt_ram_dlc - 2) = (uint8_t)((check_sum >> 8) & 0xFF);
	*(uint8_t *)(m_dtc_dt_ram_addr + m_dtc_dt_ram_dlc - 1) = (uint8_t)(check_sum  & 0xFF);
#endif
	while(write_remain_time > 0 && FALSE == retval1)
	{
		if(flashSize_32kByte_startAddr == flash_eraseBlockByStartAddr(midDFlash_block0_startAddr))
		{
			if(TRUE == flash_program(midDFlash_block0_startAddr,p_buf,flashSize_32kByte_startAddr))
			{
				retval1 = TRUE;
			}
		}
		else
		{
			write_remain_time --;
		}

	}

	return 0;
}
