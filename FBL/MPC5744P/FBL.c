/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "fbl_infor.h"
#include "sysClock.h"
#include "can.h"
#include "spi.h"
#include "spiCfg.h"
#include "interruptCtrl.h"
#include "pitCfg.h"
#include "exEE25LC256.h"
#include "mc33907.h"
#include "tja1145Cfg.h"
#include "nt_export.h"
#include "uds.h"
#include "watchdog.h"
#include "uds.h"
#include "obd.h"
#include "hdio.h"
#include "MemIf.h"

//#define resetProgramRequestGap 30 /* [tick] */
//#define updtAppReqStayInBLGap  5000/* [tick] */

extern void xcptn_xmpl(void);
extern void jumpToApp(uint32_t entryPoint);
extern bool nt_pdu_transmit(uint8_t nt_pdu_buf[], uint8_t buf_len);

uint8_t hash_andReqFlgarray[48] = {0};
uint8_t ee2ramBuff[EE2RAM_DLC] = {0};
bool ee2ramCopy_diag = FALSE;
bool ee2ramCopy_hash = FALSE;
uint8_t i = 0,hash_i = 0;

__attribute__ ((section(".text")))

int main(void)
{
	uint8_t nrspBuf[8];
    uint32_t tickCnt = 0;
	uint32_t tickStartTime = 0;
	uint32_t blReqSt1 = 0;
	uint32_t blReqSt2 = 0;


	uint8_t readUpdtReqFlag[8] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
	uint8_t writeUpdtReqFlag[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

	/* Disable global interrupts */
    xcptn_xmpl (); /* Configure Interrupts */
	disableInterrupts;
    
	/* Initial Bus Clock */
	sysClk_init();

    periClk_init();

    hdio_init();
	/* Initial CAN module */
    can_initModule();
    spi_init(0,spiModule0Cfg);
    mc33907_init();

    //tja1145_init();

#if FBL_EX_EEPROM
    spi_init(2,spiModule2Cfg);
#endif

	/* Initial task timer */
    sysSchClk_init();



    /* initial flash */
    flash_init();
    /* Copy datas or code to RAM */
    fblNvm_load();

#if FBL_EX_EEPROM
    ee256_init();
#endif

	/* UDS initial */
    uds_init();
    obd_init();/*for NDS*/

#if FBL_EX_EEPROM
	ee256_read(0x7FD0-diagInfo_DLC,(uint32_t)ee2ramBuff,EE2RAM_DLC);
	while(m_ee256_idle != ee256_getSt())
	{
		ee256_cyclic();
	}
#else
	 MemIf_ReadEE(EepromCfg_obdDiag,ee2ramBuff,diagInfo_DLC);
	 MemIf_ReadEE(EepromCfg_appHashRet,ee2ramBuff+diagInfo_DLC,hashInfo_DLC/2);
	 MemIf_ReadEE(EepromCfg_hashInfo,ee2ramBuff+diagInfo_DLC+hashInfo_DLC/2,hashInfo_DLC/2);
	 MemIf_ReadEE(EepromCfg_wrUpdtFlag,ee2ramBuff+diagInfo_DLC+hashInfo_DLC,appJump2BootInfo_DLC);
#endif
/* if all of data from EEPROM is 0xFF, DO NOT copy data 2 ram */
	for(i = 0; i < EE2RAM_DLC; i++)
	{
		 if(0xFF != ee2ramBuff[i] && diagInfo_DLC > i)
		 {
			 ee2ramCopy_diag = TRUE;
		 }
		 if(0xFF != ee2ramBuff[i] && diagInfo_DLC <= i)
		 {
			 ee2ramCopy_hash = TRUE;
		 }
	}

	if(TRUE == ee2ramCopy_diag)
	{
		for(i = 0; i < diagInfo_DLC; i++)
		{
			 *((uint8_t*)(obdDiagDtGrp)+i) = (uint8_t)ee2ramBuff[i];
		}
	}

	if(TRUE == ee2ramCopy_hash)
	{
		for(hash_i = i; hash_i < 20+i; hash_i++)
		{
			appHashResult[hash_i-i] = ee2ramBuff[hash_i];
			hash_sha1[(hash_i-i) / 4] |= (uint32_t)(ee2ramBuff[hash_i+20] << ((3- (hash_i-i) % 4)*8));
		}

		for(i = 0; i < 8; i++)
		{
			readUpdtReqFlag[i] = ee2ramBuff[i + diagInfo_DLC + hashInfo_DLC];
		}
	}


	if(m_ee256_jobOk == ee256_getJobRslt())
	{
		blReqSt1 = ((uint32_t)readUpdtReqFlag[0] << 24) |((uint32_t)readUpdtReqFlag[1] << 16) |\
					((uint32_t)readUpdtReqFlag[2] << 8) | readUpdtReqFlag[3];
		blReqSt2 = ((uint32_t)readUpdtReqFlag[4] << 24) |((uint32_t)readUpdtReqFlag[5] << 16) |\
					((uint32_t)readUpdtReqFlag[6] << 8) | readUpdtReqFlag[7];

		if(0xDEADBEEF ==  blReqSt1 && 0xBEEFDEAD ==  blReqSt2)
		{
			jump2AppDely = updtAppReqStayInBLGap;
			jump2AppAllowed = FALSE;
			uds_enterProgSession();
			nrspBuf[0] = 0x06;/*for NDS*/
			nrspBuf[1] = 0x50;
			nrspBuf[2] = 0x02;
			nrspBuf[3] = 0x00;
			nrspBuf[4] = 0x32;
			nrspBuf[5] = 0x01;
			nrspBuf[6] = 0xF4;
			nt_pdu_transmit(nrspBuf, 8);
		}
#if FBL_EX_EEPROM
		ee256_write(0x7FF8,(uint32_t)writeUpdtReqFlag,8);

		while(m_ee256_idle != ee256_getSt())
		{
			ee256_cyclic();
		}
#else
	MemIf_WriteEE(EepromCfg_wrUpdtFlag,writeUpdtReqFlag,8);
#endif
	}
    
	/* Check Update flag */

	/* Clear update flag */

	

    /* initial watchdog */
    swd_init(100);

    /* Log start time */
    tickStartTime = sysClk_CurCnt;

    /* Running task */
	for(;;)
	{
	   	can_fastTask();
        if (sysClk_TimePass(tickStartTime) >= sysClk_1ms)
        {
            /* 1ms task */
            uds_task();
            /*feed watchdog*/
            swd_service();
            /* reset program request gap timeout check */
            if (tickCnt == jump2AppDely)
            {
                if(
                	TRUE == rtSpt_blockDtcCheck() &&
                	TRUE == uds_allowedJumpToApp()  && \
					//TRUE == rtSpt_hashCheck()     &&
					TRUE == rtSpt_appIDcheck()
				 )
                {
                    /* Jump to Application */
                	//jumpToApp(*(uint32_t *)(fblNvmDts_ram.appFingerprint.startAddr + appAddrOffset_entryPoint));
                	jumpToApp(*(uint32_t *)(0x01000014));
                }
            }

            /* if tick counter to max value, not return to zero */
            if (tickCnt < 0xFFFFFFFFul)
            {
                tickCnt++;
            }
            
            tickStartTime = tickStartTime + sysClk_1ms;
            if (sysClk_TimePass(tickStartTime) >= sysClk_1ms)
            {
                tickStartTime = sysClk_CurCnt;
            }
        }
	}
	
	return 0;
}
