/*
* Copyright (c) 2016, SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File: flash.h
* Description: 
* History: 
* 2016-6-5, 1007040, original
*/

#ifndef __FLASH_H__
#define __FLASH_H__

#include "typedefs.h"
#include "platformTypes.h"

/* ---------------------- Flash partitioning on MPC5744P --------------------- */
/*
 * 4x 16 KB in partition 0/1 (2x blocks EEPROM emulation enabled)
 * 2x 32 KB in partition 2/3 (EEPROM emulation enabled)
 * 6x 64 KB in partition 4/5
 * 8x 256 KB in partition 6/7
*/

/* ---------------------- Flash blocks on MPC5744P ---------------------------- */
/* Data flash memory - no overlay, 0x00800000 ~ 0x00817FFF
 * 0x00800000 - 0x00803FFF 16 16KB EEPROM low block 0
 * 0x00804000 - 0x00807FFF 16 16KB EEPROM low block 1
 * 0x00808000 - 0x0080FFFF 32 32KB EEPROM mid block 0
 * 0x00810000 - 0x00817FFF 32 32KB EEPROM mid block 1

 * Program flash memory - no overlay, 0x00F98000 ~ 0x011FFFFF
 * Small flash memory blocks - no overlay:
 * 0x00F98000 - 0x00F9BFFF 16 16KB low flash memory block 2 (boot 0)
 * 0x00F9C000 - 0x00F9FFFF 16 16KB low flash memory block 3 (boot 1)

 * Medium flash memory blocks - no overlay
 * 0x00FA0000 - 0x00FAFFFF 64 64KB high flash memory block 0 (boot 2)
 * 0x00FB0000 - 0x00FBFFFF 64 64KB high flash memory block 1 (boot 3)
 * 0x00FC0000 - 0x00FCFFFF 64 64KB high flash memory block 2
 * 0x00FD0000 - 0x00FDFFFF 64 64KB high flash memory block 3
 * 0x00FE0000 - 0x00FEFFFF 64 64KB high flash memory block 4
 * 0x00FF0000 - 0x00FFFFFF 64 64KB high flash memory block 5

 * Large flash memory blocks - no overlay
 * 0x01000000 - 0x0103FFFF 256 256KB flash memory block 0 (boot 4)
 * 0x01040000 - 0x0107FFFF 256 256KB flash memory block 1 (boot 5)
 * 0x01080000 - 0x010BFFFF 256 256KB flash memory block 2 (boot 6)
 * 0x010C0000 - 0x010FFFFF 256 256KB flash memory block 3 (boot 7)
 * 0x01100000 - 0x0113FFFF 256 256KB flash memory block 4
 * 0x01140000 - 0x0117FFFF 256 256KB flash memory block 5
 * 0x01180000 - 0x011BFFFF 256 256KB flash memory block 6
 * 0x011C0000 - 0x011FFFFF 256 256KB flash memory block 7
*/

#define dFlash_startAddress                 (0x00800000ul)
#define dFlash_endAddress                   (0x00817FFFul)

#define pFlash_startAddress                 (0x00F98000ul)
#define pFlash_endAddress                   (0x011FFFFFul)

#define flashSize_16kByte_startAddr         (0x04000ul)
#define flashSize_32kByte_startAddr         (0x08000ul)
#define flashSize_64kByte_startAddr         (0x01000ul)
#define flashSize_256kByte_startAddr        (0x40000ul)

/* Low D-Flash memory blocks - no overlay */
#define lowDFlash_block0_startAddr          (0x00800000ul)
#define lowDFlash_block1_startAddr          (0x00804000ul)

/* Mid D-Flash memory blocks - no overlay */
#define midDFlash_block0_startAddr          (0x00808000ul)
#define midDFlash_block1_startAddr          (0x00810000ul)

/*Small flash memory blocks - no overlay*/
#define smallPFlash_block2_boot0_startAddr  (0x00F98000ul)
#define smallPFlash_block3_boot1_startAddr  (0x00F9C000ul)

/*Medium flash memory blocks - no overlay*/
#define mediumPFlash_block0_boot2_startAddr (0x00FA0000ul)
#define mediumPFlash_block1_boot3_startAddr (0x00FB0000ul)
#define mediumPFlash_block2_startAddr       (0x00FC0000ul)
#define mediumPFlash_block3_startAddr       (0x00FD0000ul)
#define mediumPFlash_block4_startAddr       (0x00FE0000ul)
#define mediumPFlash_block5_startAddr       (0x00FF0000ul)

/*Large flash memory blocks - no overlay*/
#define largePFlash_block0_boot4_startAddr  (0x01000000ul)
#define largePFlash_block1_boot5_startAddr  (0x01040000ul)
#define largePFlash_block2_boot6_startAddr  (0x01080000ul)
#define largePFlash_block3_boot7_startAddr  (0x010C0000ul)
#define largePFlash_block4_startAddr        (0x01100000ul)
#define largePFlash_block5_startAddr        (0x01140000ul)
#define largePFlash_block6_startAddr        (0x01180000ul)
#define largePFlash_block7_startAddr        (0x011C0000ul)

#define flashBlockEmpty                     (0xFFu)
#define flashBlockTotalCnt                  (20u) /* must equre flashBlock_t item count, and equre low, mid, small, medium large Flash blocks */
#define dFlashLowBlockCnt                   (2u)
#define dFlashMidBlockCnt                   (2u)
#define pFlashSmallBlockCnt                 (2u)
#define pFlashMediumBlockCnt                (6u)
#define pFlashLargeBlockCnt                 (8u)

#define flashProtectBlockCnt                (1u)

typedef enum
{
    lowDFlash_block0,
    lowDFlash_block1,
    midDFlash_block0,
    midDFlash_block1,
    smallPFlash_block2_boot0,    
    smallPFlash_block3_boot1,
    mediumPFlash_block0_boot2,
    mediumPFlash_block1_boot3,
    mediumPFlash_block2,
    mediumPFlash_block3,
    mediumPFlash_block4,
    mediumPFlash_block5,
    largePFlash_block0_boot4,
    largePFlash_block1_boot5,
    largePFlash_block2_boot6,
    largePFlash_block3_boot7,
    largePFlash_block4,
    largePFlash_block5,
    largePFlash_block6,
    largePFlash_block7
}flashBlock_t;

/* flash block information (Include D-Flash and P-Flash) */
typedef struct
{
    uint32_t startAddr;
    uint32_t addrSize;
}flashBlockInf_t;

extern void flash_init(void);
extern void flash_loadDriver(void);
extern uint8_t flash_getBlockIndex(uint32_t  startAddr);
extern bool flash_eraseBlock(uint8_t blockIndex);
extern uint32_t flash_eraseBlockByStartAddr(uint32_t startAddr);
extern bool flash_isInFlashCheck(uint32_t startAddr, uint16_t dataSize);
extern bool flash_notInProtectCheck(uint32_t startAddr, uint16_t dataSize);
extern bool flash_program(uint32_t startAddr, uint8_t progData[], uint16_t dataSize);

extern bool jump2boot_copy_ram_to_ee(uint8_t * p_buf);
#endif /*End of __FLASH_H__ */
/*--------EOF--------*/

