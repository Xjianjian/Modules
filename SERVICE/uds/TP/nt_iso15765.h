/*
* Copyright (c) 2015,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.:ACEC
* File:nt_iso15765.h
* Description:
* REQ IDs:
* History:
* 2015-4-14, 1007040, original
*/

#ifndef __NT_ISO15765_H__
#define __NT_ISO15765_H__

#include "nt_types.h"

/* PDU (N_PCI + N_Data, not include N_AI) informations */
#define NT_PDU_BUFFER_SIZE   8      /* PDU buffer size define (for CAN, must full 8 bytes) */
#define NT_NPCI_POS          nt_npci_start_pos /* N_PCI start position in PDU buffer */
#define NT_NPCI_MASK         0xF0u  /* PDU type mask invert mask */
#define NT_NPCI_INV_MASK     0x0Fu
/* N_PCI macros */
#define NT_NPCI_SF           0x00u  /* PDU N_PCI Single Frame */
#define NT_NPCI_FF           0x10u  /* PDU N_PCI First Frame */
#define NT_NPCI_CF           0x20u  /* PDU N_PCI Consecutive Frame */
#define NT_NPCI_FC           0x30u  /* PDU N_PCI Flow Control */
/* SF macros */
#define NT_SF_MAX_DL         (NT_PDU_BUFFER_SIZE - NT_NPCI_POS - 1)
/* FF macros */
#define NT_FF_MIN_DL         (8 - NT_NPCI_POS)
#define NT_FF_DT_START_POS   (NT_NPCI_POS + 2)
#define NT_FF_DT_LEN         (6 - NT_NPCI_POS)
/* FC macros */
#define NT_FC_FS_CTS         0 /* ContinueToSend */
#define NT_FC_FS_WT          1 /* Wait */
#define NT_FC_FS_OVFLW       2 /* Overflow */
#define NT_FC_FS_COUNT       3 /* FS types count */
/* CF macros */
#define NT_CF_DT_START_POS   (NT_NPCI_POS + 1)
#define NT_CF_MAX_DL         (NT_PDU_BUFFER_SIZE - NT_NPCI_POS - 1)

#endif /*End of __NT_ISO15765_H__ */
/*--------EOF--------*/


