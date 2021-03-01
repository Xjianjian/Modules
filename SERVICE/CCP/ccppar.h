/*----------------------------------------------------------------------------
| File:
|   ccppar.h
|
| Project:
|   CCP driver example
|   CANape CAN Calibration Tool
|
| Description:
|   Customization header for ccp.c
|   See CCP.DOC for a complete description of all options
|
 ----------------------------------------------------------------------------*/

#ifndef __CCPPAR_H__
#define __CCPPAR_H__


/*----------------------------------------------------------------------------*/
/* Platform independant types */

/*
#define CCP_MOTOROLA  or #define CCP_INTEL
#define CCP_BYTE    unsigned char
#define CCP_WORD    unsigned short
#define CCP_DWORD   unsigned long
#define CCP_BYTEPTR unsigned char*
#define CCP_ROM
#define CCP_RAM
#define CCP_DAQBYTEPTR CCP_BYTEPTR
#define CCP_MTABYTEPTR CCP_BYTEPTR
*/

/*----------------------------------------------------------------------------*/
/* Disable/Enable Interrupts */

/* Has to be defined if ccpSendCallBack may interrupt ccpDaq */
#define CCP_DISABLE_INTERRUPT	 __asm__(" wrteei 0");
#define CCP_ENABLE_INTERRUPT	 __asm__(" wrteei 1");

/*----------------------------------------------------------------------------*/
/* CCP parameters */

/* CCP Identifiers and Address */
#define CCP_STATION_ADDR  0x1F2E           /* Define CCP_STATION_ADDR in Intel Format */
                                      /* High/Low byte swapped on motorola machines !!! (0x3900) */

#define CCP_STATION_ID    "ECU00001"  /* Plug&Play station identification */

#define CCP_DTO_ID        0x729           /* CAN identifier ECU -> Master */
#define CCP_CRO_ID        0x739           /* CAN identifier Master -> ECU */


/*----------------------------------------------------------------------------*/
/* CCP Data Acuisition Parameters */

#define CCP_DAQ                   /* Enable synchronous data acquisition in ccpDaq() */
#define CCP_MAX_ODT 41             /* Number of ODTs in each DAQ lists */
#define CCP_MAX_DAQ 3             /* Number of DAQ lists */


/*----------------------------------------------------------------------------*/
/* CCP Options */

/* Use the transmit queue in CCP.C */
/* Complete sampling is done in ccpDaq(x) and the messages are written into the queue */
#define CCP_SEND_QUEUE

/* Indicate queue overruns in the msb of pid */
/* Will be displayed in CANape's status bar if CANAPE.INI: [asap1a] check_overflow=1 */
#define CCP_SEND_QUEUE_OVERRUN_INDICATION

/* Transmit only one message in one call to ccpDaq() */
 /*#define CCP_SEND_SINGLE*/

/* Allow an ODT entry size >1 */
/* Not recommended for CANape, this will only need additional RAM space */
 #define CCP_ODT_ENTRY_SIZE

/* Use GET_SEED and UNLOCK */
/* This is usually user dependent, just a skeleton here */
//#define CCP_SEED_KEY

/* Implement the flash programming feature in the ECU*/
/* This is usually user dependent, just a skeleton here */
#define CCP_PROGRAM

/* Activate the flash programming kernel download feature */
/* This is a CCP extension for CANape */
/*#define CCP_BOOTLOADER_DOWNLOAD*/

/* Implement the memory checksum feature */
/* The checksum will be calculated in ccpBackground() */
/* This may be implementation specific */
#define CCP_CHECKSUM
#define CCP_CHECKSUM_TYPE CCP_WORD

#define CCP_SET_SESSION_STATUS

/* Use a 16 bit CRC algorithm */
/* Note:
   This will need additional 512 byte of ROM
   CCP_CHECKSUM_TYPE has to be WORD !
*/
/* #define CCP_CHECKSUM_CCITT*/

/* Check for pending CCP commands in ccpBackground() */
/* #ifdef CCP_CMD_NOT_IN_INTERRUPT */

/* Enable Memory Write Protection */
#define CCP_WRITE_PROTECTION

/* Enable EEPROM Read/Write Access */
/* #define CCP_WRITE_EEPROM */
/* #define CCP_READ_EEPROM */



/*----------------------------------------------------------------------------*/
/* Special values for the CCPsim WIN32 Application */
#if defined(__WIN32__) || defined(WIN32)

  #define CCP_TESTMODE /* Turn on screen output via printf */
  #define CCPPRINT printf

  #define CCP_INTEL

  extern unsigned long gDtoId;
  extern unsigned long gCroId;
  extern unsigned short gStationAddr;
  #undef CCP_CRO_ID
  #define CCP_CRO_ID gCroId
  #undef CCP_DTO_ID
  #define CCP_DTO_ID gDtoId
  #undef CCP_STATION_ADDR
  #define CCP_STATION_ADDR gStationAddr
  #undef CCP_STATION_ID
  #define CCP_STATION_ID "CCPSIM"

  #undef CCP_MAX_ODT
  #undef CCP_MAX_DAQ
  #define CCP_MAX_ODT 10
  #define CCP_MAX_DAQ 5
  #define CCP_ODT_ENTRY_SIZE
  #undef  CCP_SEND_SINGLE
  #undef  CCP_SEND_QUEUE
  #define CCP_SEND_QUEUE_OVERRUN_INDICATION
  #define CCP_SEND_QUEUE_SIZE (CCP_MAX_ODT*CCP_MAX_DAQ)
  #define CCP_SEED_KEY
  #undef  CCP_BOOTLOADER_DOWNLOAD
  #define CCP_PROGRAM
  #define CCP_WRITE_PROTECTION
  #define CCP_CHECKSUM
  #define CCP_CHECKSUM_TYPE CCP_WORD
  #define CCP_CHECKSUM_CCITT

#endif

#endif
