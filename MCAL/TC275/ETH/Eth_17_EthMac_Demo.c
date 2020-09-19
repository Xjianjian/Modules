/******************************************************************************
**                                                                           **
** Copyright (C) Infineon Technologies (2013)                                **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
** This document contains proprietary information belonging to Infineon      **
** Technologies. Passing on and copying of this document, and communication  **
** of its contents is not permitted without prior written authorization.     **
**                                                                           **
*******************************************************************************
**                                                                           **
**  $FILENAME   : Eth_17_EthMac_Demo.c $                                     **
**                                                                           **
**  $CC VERSION : \main\33 $                                                 **
**                                                                           **
**  $DATE       : 2015-11-05 $                                               **
**                                                                           **
**  AUTHOR      : DL-AUTOSAR-Engineering                                     **
**                                                                           **
**  VENDOR      : Infineon Technologies                                      **
**                                                                           **
**  DESCRIPTION : This file contains                                         **
**                Demo application for Ethernet driver                       **
**                                                                           **
**  MAY BE CHANGED BY USER [yes/no]: Yes                                     **
**                                                                           **
******************************************************************************/
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Ifx_reg.h"

#include "Eth_17_EthMac.h"
#include "EthIf_Cbk.h"

#include "Test_Setup.h"
#include "Test_Print.h"
#include "Test_Time.h"
#include "Test_Main.h"
/* Inclusion of  Mcal header file */
#include "Mcal.h"
#include "IfxCpu_reg.h"
#include "Eth_17_EthMac.h"

#include "Eth_17_EthMac_Demo.h"
#include "Platform_Types.h"
#include "IfxEth_Phy_Pef7071.h"
#include "EthIf_Cbk.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define CPU_PSW_USER_MODE_MASK (0xFFFFF3FFU)
#define SET_USER_MODE_MASK ((1U  <<  10U))
#define ETH_IP_PROTOCOL                         (0X0800U)
#define ETH_ARP_PROTOCOL                        (0X0806U)
                                                
#define PHY_CTRL_REG                            (0x00) 
#define PHY_STAT_REG                            (0x01) 
#define PHY_PHYCTL1_REG                         (0x13) 
#define PHY_PHYCTL2_REG                         (0x14) 
#define PHY_MIICTRL_REG                         (0x17)
                                                
#define PHY_100MBPS_FULLDPLX_REG_VAL            (0x2100) 
                                                
#define PHY_PHYCTL1_REG_VAL                     (0x0001) 
#define PHY_PHYCTL2_REG_VAL                     (0x8006) 
                                                
#define PHY_MIICTRL_RMII_REG_VAL                (0xF702)
                                                
#define ETH_PHY_REG_COUNT                       (32U)
                                                
#define ETH_PHY_INIT_DURATION                   (2000U)
                                                
#define ETH_FRAME_TYPE_1                        (0xABCDU)
                                                
#define MMC_TX_GB_REG_BASE_OFFSET               (0x124U)
#define MMC_RX_GB_REG_BASE_OFFSET               (0x1ACU)
                                                
#define ETH_ESC_CHAR_ASCII_VAL                  (27U)
                                                
#define ETH_OFFSET_4_BYTE                       (4U)
#define ETH_OFFSET_8_BYTE                       (8U)
#define ETH_OFFSET_12_BYTE                      (12U)
#define ETH_OFFSET_16_BYTE                      (16U)
#define ETH_OFFSET_20_BYTE                      (20U)
                                                
#define ETH_IPADDRESS_LENGTH                    (4U)
                                                
#define ETH_ARPFRAME_DEST_IPADD_OFFSET          (24U)
                                                
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
uint32 JBFrameCount;
uint32 Src_IPaddress = 0xa9fe35ab;/*169.254.53.171*/
//uint32 Src_IPaddress = 0xC0A8000A;/*192.168.0.10*/
static uint16 PhyReg[32];
static uint8 Temp_Macaddress[6];
static uint8 MacDestArpaddress[6]={0xff,0xff,0xff,0xff,0xff,0xff}; /*Broadcast*/
//static uint8 MacDestArpaddress[6]={0x00,0x03,0x19,0x00,0x00,0x01}; /*Broadcast*/
//{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; /*Broadcast*/

static uint8 TmpBuffIdx=0;//赋初值
static uint8 *TmpBuffPtr[5];
static volatile BufReq_ReturnType Global_TempUSage1;
volatile Std_ReturnType EthDemoRetVal;

uint8 ArpReply;

uint8 ArpData[28]=
{ 
//0xff,0xff,0xff,0xff,0xff,0xff,0x11,0x22,0x33,0x44,0x55,0x66,0x08,0x06,
0x00,0x01,0x08,0x00,0x06,0x04,0x00,0x01,0x11,0x22,0x33,0x44,0x55,0x66,
0xa9,0xfe,0x35,0xab,0x00,0x00,0x00,0x00,0x00,0x00,0xa9,0xfe,0x35,0x3e
};
/*{
0x00,0x01,0x08,0x00,0x06,0x04,0x00,0x01,0x00,0x03,0x19,0x00,0x00,0x01,
0xa9,0xfe,0xfe,0xab,0x00,0x00,0x00,0x00,0x00,0x00,0xa9,0xfe,0xab,0xa8
};*/

//static uint8 MacDestBrodaddress[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static uint8 MacDestBrodaddress[6] = {0x11,0x22,0x33,0x44,0x55,0x66};






/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

/*******************************************************************************
**    Fill Tx Buffer           **
*******************************************************************************/

void TxBufferFill(uint8 *pTxBuff,uint16 DataLen,uint8 *Framedatax )
{
  uint8 i=0;
  for(;i<DataLen ;i++)
    {
      *(pTxBuff + i)= Framedatax[i];
    }
 return;
}

/*******************************************************************************
**    Transmit Arp Frame           **
*******************************************************************************/
void TransmitArpFrame(void)
{
  boolean Txstatus1=0; 
  uint16 Tmp_Txlenghth1 = 28;
  //MacDestBrodaddress[] = {0x11,0x22,0x33,0x44,055,0x66};
  /*Provides access to reserved transmit buffer of indexed controller*/
  Global_TempUSage1 = Eth_17_EthMac_ProvideTxBuffer(0,&TmpBuffIdx,TmpBuffPtr,&Tmp_Txlenghth1);//question1:TmpBuffIdx no value  EB->Transmit frame1-1

  TxBufferFill(TmpBuffPtr[0],Tmp_Txlenghth1,ArpData);
  Txstatus1 = 1;
  EthDemoRetVal = Eth_17_EthMac_Transmit(0,TmpBuffIdx,ETH_ARP_PROTOCOL,Txstatus1,Tmp_Txlenghth1,&MacDestBrodaddress[0]);//EB->Transmit frame2-1
}

/*******************************************************************************
**    Initialise Eth Driver including PHY        **
*******************************************************************************/
uint16  readphy = 0;
uint16  readphy1 = 0;
uint16  readphy2 = 0;
uint16  readphy3 = 0;
uint16  readphy4 = 0;
uint16  readphy5 = 0;
uint16  readphy6 = 0;
uint16  readphy7 = 0;
uint16  readphy8 = 0;
uint16  readphy9 = 0;
uint16  readphy10 = 0;
uint16  readphy11 = 0;
uint16  phyadd = 0;
uint16 valueread=0;
uint32 value22 = 0;
void TransmitInit (void)
{
  uint32 i;

  /* RMII Mode,Twisted pair MAC,3.5ns timing skew for Tx and Rx,RClk ON */
  //Eth_17_EthMac_WriteMii(0,0,PHY_MIICTRL_REG,PHY_MIICTRL_RMII_REG_VAL);//0x17
#if 1
#if 1
   Eth_17_EthMac_WriteMii(0,0,0x11,0x9804);//0x11    step1

   Eth_17_EthMac_WriteMii(0,0,0x12,0xCE01);//0x12    step2  master=0xCE01 slave = 0x4E01

   Eth_17_EthMac_WriteMii(0,0,0x13,0x244);//0x13    step3

   Eth_17_EthMac_WriteMii(0,0,0x1C,0x0001);//0x1C      step4

    Eth_17_EthMac_WriteMii(0,0,0x16,0x8000);//0x16      step5

   Eth_17_EthMac_WriteMii(0,0,0x1B,0x0000);//0x1B       step6
   Eth_17_EthMac_WriteMii(0,0,0,0x2100);     //00     step7    00x2100不需要修改   loop mode
#else
   /**************loopback mode************************/
   //Set the PHY to normal mode and disable LINK_CONTROL
  //Select loopback mode in Extended Control register
  Eth_17_EthMac_WriteMii(0,6,0x11,0x1804);//0x11    step1  interloop
   // Eth_17_EthMac_WriteMii(0,6,0x11,0x180c);//0x11    step1  exterloop
 //  Eth_17_EthMac_WriteMii(0,6,0x11,0x181c);//0x11    step1  remotloop   16388
   //Enable loopback operation in Basic Control register
   Eth_17_EthMac_WriteMii(0,6,0,0x6100);     //00     step7    00x2100不需要修改   loop mode

   Eth_17_EthMac_WriteMii(0,6,0x12,0xCE01);//0x12    step2   slave = 4E01

   Eth_17_EthMac_WriteMii(0,6,0x13,0x244);//0x13    step3

   Eth_17_EthMac_WriteMii(0,6,0x1C,0x0001);//0x1C      step4

    Eth_17_EthMac_WriteMii(0,6,0x16,0x8000);//0x16      step5

   Eth_17_EthMac_WriteMii(0,6,0x1B,0x0000);//0x1B       step6
  // Enable LINK_CONTROL
   Eth_17_EthMac_WriteMii(0,6,0x11,0x9804);//0x11    step1  interloop
  // Eth_17_EthMac_WriteMii(0,6,0x11,0x980c);//0x11    step1  exterloop.
  // Eth_17_EthMac_WriteMii(0,6,0x11,0x981c);//0x11    step1  remotloop
#endif
#endif

#if 0
  /* Read PHY Registers*/
///  for(i=0;i < ETH_PHY_REG_COUNT;i++)
 /// {
	  phyadd = 2000;
  //  Eth_17_EthMac_ReadMii((uint8)0,(uint8)0,(uint8)i,&PhyReg[i]);
	//  while(1)
	//  {
  Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)0x11, &valueread);
	//  }
  ///  if(PhyReg[i] < 30000)
  ///  {
    ///	readphy = PhyReg[i];
    readphy = valueread;


  //  IfxEth_Phy_Pef7071_read_mdio_reg((uint32)0, (uint32)0, &value22);
    readphy = value22;
    ///	phyadd = i;
  ///  }
 /// }
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)23, &readphy11);//24580
#if 1
 while (1 == 1)
{
  Eth_17_EthMac_ReadMii((uint8)0,(uint8)6,(uint8)1,&PhyReg[0]);//不需要修改
  if ((PhyReg[0] & (1 << 2)) != 0)//不需要修改
     break;/* Link Established *///不需要修改
}
#endif

  //print_f("\n ETH Driver Initialised Successfully ");
 // print_f("\n Ethernet LINK UP ");
//                                                               auto mode value
#endif

	do
	{
		Eth_17_EthMac_ReadMii((uint8)0,(uint8)6,(uint8)1,&PhyReg[0]);//不需要修改
	}
	while((PhyReg[0] & (1 << 2)) == 0);

    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)0, &readphy1);//8448     0x2100
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)17, &readphy2);//38912    0x9800
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)18, &readphy3);//56837  slaveauto=24069
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)19, &readphy4);//581
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)22, &readphy5);//32768
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)27, &readphy6);//32800
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)28, &readphy7);//1
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)1, &readphy8);//481
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)2, &readphy9);//384
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)3, &readphy10);//56577
    Eth_17_EthMac_ReadMii((uint8)0,(uint8)6, (uint8)23, &readphy11);//24580  16388  32768
    Timer_DelayMs(ETH_PHY_INIT_DURATION);

}

/*******************************************************************************
**    Ping Application                                            **
*******************************************************************************/
char *lpt;
void Ping_App(void)
{
//	 *lpt=';';
//  char c;
  uint8 i;
//  char param[20];
//  char szString1[20];
//  char *pszString1;
//  uint32 paramArray[6];
 // uint8 paramCount=0,charCount=0;
 // print_f("\n\n Enter IP Address to Ping(Your PC IP) : ");
//  getline(szString1, sizeof szString1 - 1);
 // pszString1 = &szString1[0];

  ///do
  ///{
    /* Look for the '.' delimiter */
 //   if (*pszString1 != '.')
  //  {
 //       param[charCount++]= (char) *pszString1;
  //  }
  ////  else
  //  {
  //      param[charCount] = 0;
  //      paramArray[paramCount] = (uint32)atoi(param);
   ///     paramCount++;
  //      charCount = 0;
  //  }
  //  pszString1++;
  ///}while (*pszString1 != '\0');

  //param[charCount] = 0;
  //paramArray[paramCount] = (uint32) atoi(param);
/*  ETH_OPERATION_MODE.B.DFF = 0U; /* Flush the Rx FIFO */
 /* for(i=0;i<ETH_IPADDRESS_LENGTH;i++)
  {
    ArpData[ETH_ARPFRAME_DEST_IPADD_OFFSET+i] = (uint8)paramArray[i];
  }
  Src_IPaddress = (((paramArray[0]&0xFF)<<ETH_SHIFT_24BIT)|((paramArray[1]&0xFF)<<ETH_SHIFT_16BIT)|((paramArray[2]&0xFF)<<ETH_SHIFT_8BIT)|(paramArray[3]&0xFF));*/
  ETH_OPERATION_MODE.B.DFF = 1U; /* Disable Flush the Rx FIFO */
  TransmitArpFrame();

   /// do
  ///  {
      Eth_17_EthMac_Receive(0);//EB->Receive frame1-1
   /// }while(( ArpReply2 != 1)&&((c=(char)get_ch()) != (char)ETH_ESC_CHAR_ASCII_VAL));

  /*  if(ArpReply == 1)
    {
      ArpReply = 0;
    }
    else
    {*/
    //  print_f("\n Destination Host Unreachable\n  ");
    //  print_f("Reason 1: Ethernet Cross Wire is NOT connected between AURIX Tri-Board and Your PC\n ");
    //  print_f("Reason 2: IP Address Entered is Wrong\n ");
    //}
 }

/*******************************************************************************
**    Promiscuous mode Receive all Frames                                     **
*******************************************************************************/
void Receive_Poll (void)
{
  char c;

///  do
///  {
    Eth_17_EthMac_Receive(0);//EB->Receive frame1-2
 /// }while( (c=(char)get_ch()) != (char)ETH_ESC_CHAR_ASCII_VAL);
 /* Wait Till Esc Key is pressed */   
 }

/*******************************************************************************
**    Transmit ETH Frame of User Defined Length                               **
*******************************************************************************/
void Tx_EthFrame (void)
{
  char szString[80];
  uint16 TmpTxlength,i;
  uint8 *TmpEthTxBuffPtr;  
  
  TxFrameCount = 0;     
///  print_f("\n\n Enter ETH Frame Payload Length(42...1500) : ");
  getline(szString, sizeof szString - 1);                                   
     
  TmpTxlength = (uint16)atoi(szString);

  Global_TempUSage1 = Eth_17_EthMac_ProvideTxBuffer(0,&TmpBuffIdx,&TmpEthTxBuffPtr,&TmpTxlength);//EB->Transmit frame1-2

  for (i=0; i < TmpTxlength ; i++)
  {
     *TmpEthTxBuffPtr = i+1;
     TmpEthTxBuffPtr+=1;
  }

  EthDemoRetVal = Eth_17_EthMac_Transmit(0,TmpBuffIdx,ETH_FRAME_TYPE_1,1,TmpTxlength,&MacDestArpaddress[0]);//EB->Transmit frame2-2
  Timer_DelayMs(300);
  #if (!((ETH_17_ENA_TX_INT == STD_ON) && (ETH_17_ENA_RX_INT == STD_ON)))
 /// do
  ///{
    Eth_17_EthMac_TxConfirmation(0); //EB->Transmit frame3-2
///  }
  #endif
 /// while(!TxFrameCount);

  //print_f("\n Transmitted ETH Frame of FrameType 0xABCD");
 // print_f("\n Payload Length %d bytes and Total Frame Length %d bytes\n",TmpTxlength,(TmpTxlength+ETH_FRAMEHEADER_LENGTH+ETH_FRAMECHKSUM_LENGTH));
  TxFrameCount = 0;
}

/*******************************************************************************
**    Get Tx Frame Count for Different Frame Lengths                          **
*******************************************************************************/
void Get_TxFrameCount (void)
{
  uint32 MMC_CtrVal;
  
  Eth_17_EthMac_GetCounterState(0, MMC_TX_GB_REG_BASE_OFFSET, &MMC_CtrVal);              

 // print_f("\n No. of Good and Bad Frame Transmitted with Length 64 bytes: %d\n ",MMC_CtrVal);

  Eth_17_EthMac_GetCounterState(0, MMC_TX_GB_REG_BASE_OFFSET + ETH_OFFSET_4_BYTE, &MMC_CtrVal);              

  //print_f("No. of Good and Bad Frame Transmitted with Length between 65 and 127 bytes: %d\n ",MMC_CtrVal);

  Eth_17_EthMac_GetCounterState(0, MMC_TX_GB_REG_BASE_OFFSET + ETH_OFFSET_8_BYTE, &MMC_CtrVal);              

 // print_f("No. of Good and Bad Frame Transmitted with Length between 128 and 255 bytes: %d\n ",MMC_CtrVal);

  Eth_17_EthMac_GetCounterState(0, MMC_TX_GB_REG_BASE_OFFSET + ETH_OFFSET_12_BYTE, &MMC_CtrVal);              

 // print_f("No. of Good and Bad Frame Transmitted with Length between 256 and 511 bytes: %d\n ",MMC_CtrVal);

  Eth_17_EthMac_GetCounterState(0, MMC_TX_GB_REG_BASE_OFFSET + ETH_OFFSET_16_BYTE, &MMC_CtrVal);              

 // print_f("No. of Good and Bad Frame Transmitted with Length between 512 and 1023 bytes: %d\n ",MMC_CtrVal);

  Eth_17_EthMac_GetCounterState(0, MMC_TX_GB_REG_BASE_OFFSET + ETH_OFFSET_20_BYTE, &MMC_CtrVal);              

 // print_f("No. of Good and Bad Frame Transmitted with Length 1024 and 1522 bytes: %d\n ",MMC_CtrVal);
}

/*******************************************************************************
**    Get Rx Frame Count for Different Frame Lengths                          **
*******************************************************************************/
void Get_RxFrameCount (void)
{
  uint32 MMC_CtrVal;
  
  Eth_17_EthMac_GetCounterState(0, MMC_RX_GB_REG_BASE_OFFSET, &MMC_CtrVal);              

 // print_f("\n No. of Good and Bad Frame Received with Length 64 bytes: %d\n ",MMC_CtrVal);

  Eth_17_EthMac_GetCounterState(0, MMC_RX_GB_REG_BASE_OFFSET + ETH_OFFSET_4_BYTE, &MMC_CtrVal);              

 // print_f("No. of Good and Bad Frame Received with Length between 65 and 127 bytes: %d\n ",MMC_CtrVal);

  Eth_17_EthMac_GetCounterState(0, MMC_RX_GB_REG_BASE_OFFSET + ETH_OFFSET_8_BYTE, &MMC_CtrVal);              

 // print_f("No. of Good and Bad Frame Received with Length between 128 and 255 bytes: %d\n ",MMC_CtrVal);

  Eth_17_EthMac_GetCounterState(0, MMC_RX_GB_REG_BASE_OFFSET + ETH_OFFSET_12_BYTE, &MMC_CtrVal);              

 // print_f("No. of Good and Bad Frame Received with Length between 256 and 511 bytes: %d\n ",MMC_CtrVal);

  Eth_17_EthMac_GetCounterState(0, MMC_RX_GB_REG_BASE_OFFSET + ETH_OFFSET_16_BYTE, &MMC_CtrVal);              

 // print_f("No. of Good and Bad Frame Received with Length between 512 and 1023 bytes: %d\n ",MMC_CtrVal);

  Eth_17_EthMac_GetCounterState(0, MMC_RX_GB_REG_BASE_OFFSET + ETH_OFFSET_20_BYTE, &MMC_CtrVal);              

  //print_f("No. of Good and Bad Frame Received with Length 1024 and 1522 bytes: %d\n ",MMC_CtrVal);
}

/*******************************************************************************
**    Read PHY Registers                          **
*******************************************************************************/
void Read_PhyReg (void)
{
  uint16 Ret_RegVal;
  
  Eth_17_EthMac_ReadMii(0, 0, PHY_CTRL_REG, &Ret_RegVal);

 // print_f("\n Reg Value Read from Ethernet PHY RegIdx 0 (PHY_CTRL_REG): 0x%x \n ",Ret_RegVal);

  Eth_17_EthMac_ReadMii(0, 0, PHY_STAT_REG, &Ret_RegVal);

 // print_f("Reg Value Read from Ethernet PHY RegIdx 1 (PHY_STAT_REG): 0x%x \n ",Ret_RegVal);

  Eth_17_EthMac_ReadMii(0, 0, PHY_PHYCTL1_REG, &Ret_RegVal);

 // print_f("Reg Value Read from Ethernet PHY RegIdx 19 (PHY_PHYCTL1_REG): 0x%x \n ",Ret_RegVal);

  Eth_17_EthMac_ReadMii(0, 0, PHY_PHYCTL2_REG, &Ret_RegVal);

 // print_f("Reg Value Read from Ethernet PHY RegIdx 20 (PHY_PHYCTL2_REG): 0x%x \n ",Ret_RegVal);

  Eth_17_EthMac_ReadMii(0, 0, PHY_MIICTRL_REG, &Ret_RegVal);

 // print_f("Reg Value Read from Ethernet PHY RegIdx 23 (PHY_MIICTRL_REG): 0x%x \n ",Ret_RegVal);

}

/*******************************************************************************
** Syntax : void Eth_17_Demo(void)                                            **
**                                                                            **
** Service ID:   : NA                                                         **
**                                                                            **
** Sync/Async:   : Synchronous                                                **
**                                                                            **
** Reentrancy:   : Non Reentrant                                              **
**                                                                            **
** Parameters (in): none                                                      **
**                                                                            **
** Parameters (out): none                                                     **
**                                                                            **
** Return value: none                                                         **
**                                                                            **
** Description : Ethernet Demo Application                                   **
*******************************************************************************/
void Eth_17_Demo(void)
{
  #ifdef IFX_MCAL_RUN_MODE_DEFINE
  /* IFX_MCAL_RUN_MODE_DEFINE = 0 indicates MCAL,
  runs in Supervisor mode*/
  #if(IFX_MCAL_RUN_MODE_DEFINE != 0U)
  uint32 Cpu_PSW;
  #endif 
  #endif 
  char szString[10];
  volatile uint8 loopvar = 0;
  volatile uint8 EN1Val, EN1SelVal, PortPC12;
  EN1Val = (uint8)SCU_EXTCON.B.EN1;
  EN1SelVal = (uint8)SCU_EXTCON.B.SEL1;
  PortPC12 = (uint8)P11_IOCR12.B.PC12;

  Mcal_ResetSafetyENDINIT_Timed(150000U);
  P11_IOCR12.B.PC12 = 0x16U;
  SCU_EXTCON.B.SEL1 = 5U;
  SCU_EXTCON.B.EN1 = 1U;
  Mcal_SetSafetyENDINIT_Timed();

  TransmitInit(); /* Initialise Eth Driver including PHY */

  Mcal_ResetSafetyENDINIT_Timed(150000U);
  SCU_EXTCON.B.EN1 = EN1Val;
  SCU_EXTCON.B.SEL1 = EN1SelVal;
  P11_IOCR12.B.PC12 = PortPC12;
  Mcal_SetSafetyENDINIT_Timed();
  
  #ifdef IFX_MCAL_RUN_MODE_DEFINE
  /* IFX_MCAL_RUN_MODE_DEFINE = 0 indicates MCAL,
    runs in Supervisor mode*/
  #if(IFX_MCAL_RUN_MODE_DEFINE != 0U)
      /* Set the user-1 mode on*/
  Cpu_PSW = (uint32)MFCR(CPU_PSW);
  Cpu_PSW = (Cpu_PSW & CPU_PSW_USER_MODE_MASK);
  Cpu_PSW = (Cpu_PSW | SET_USER_MODE_MASK);
  MTCR (CPU_PSW, Cpu_PSW); /* Set mode to user-1 mode. */
  #endif /* IFX_MCAL_RUN_MODE_DEFINE */
  #endif /* IFX_MCAL_RUN_MODE_DEFINE */

  while (loopvar == 0)
  {

     
     getline(szString, sizeof szString - 1);                                   
    
     switch (*szString)
     {
        case '1':   
          Receive_Poll();
          break;

        case '2':   
          Ping_App();
          break;          

        case '3':   
          Tx_EthFrame();
          break;          

        case '4':   
          Get_TxFrameCount();
          break;          

         case '5':   
          Get_RxFrameCount();
          break;          

         case '6':   
          Read_PhyReg();
          break;          

         case 'x':   
          loopvar = 1;
          break;          

         default:
        // print_f("\n\n  INVALID OPTION");
          break;
    }/* End of Case */
  }/* End of While loop */
}


void Eth_17_Run(void)
{
  static uint8 loopvar = 1;
  volatile uint8 EN1Val, EN1SelVal, PortPC12;
  EN1Val = (uint8)SCU_EXTCON.B.EN1;
  EN1SelVal = (uint8)SCU_EXTCON.B.SEL1;
  PortPC12 = (uint8)P11_IOCR12.B.PC12;

  Mcal_ResetSafetyENDINIT_Timed(150000U);
  P11_IOCR12.B.PC12 = 0x16U;
  SCU_EXTCON.B.SEL1 = 5U;
  SCU_EXTCON.B.EN1 = 1U;
  Mcal_SetSafetyENDINIT_Timed();

  TransmitInit(); /* Initialise Eth Driver including PHY */

  Mcal_ResetSafetyENDINIT_Timed(150000U);
  SCU_EXTCON.B.EN1 = EN1Val;
  SCU_EXTCON.B.SEL1 = EN1SelVal;
  P11_IOCR12.B.PC12 = PortPC12;
  Mcal_SetSafetyENDINIT_Timed();



    switch (loopvar)
     {
        case '1':
          Receive_Poll();
          loopvar = 2;
          break;

        case '2':
          Ping_App();
          loopvar = 3;
          break;

        case '3':
          Tx_EthFrame();
          loopvar = 4;
          break;

        case '4':
          Get_TxFrameCount();
          loopvar = 5;
          break;

         case '5':
          Get_RxFrameCount();
          loopvar = 6;
          break;

         case '6':
          Read_PhyReg();
          loopvar = 7;

          break;

         default:
        	 loopvar = 1;
          break;
    }/* End of Case */
}

uint8 ethstep1 = 0;
uint8 ethstep2 = 0;
uint8 ethstep3 = 0;
uint8 ethstep4 = 0;
uint8 ethstep5 = 0;
uint8 ethstep6 = 0;
uint8 ethstep7 = 0;
uint8 ethstep8 = 0;
void Eth_17_Run2(void)
{
  static uint8 loopvar = 1;
  ethstep1 = 1;
    switch (loopvar)
     {
        case 1:
          Receive_Poll();
          loopvar = 2;
          ethstep2 = 2;
          break;

        case 2:
          Ping_App();//reset
          loopvar = 3;
          ethstep3 = 3;
          break;

        case 3:
        //  Tx_EthFrame();//reset
          loopvar = 4;
          ethstep4 = 4;
          break;

        case 4:
          Get_TxFrameCount();
          loopvar = 5;
          ethstep5 = 5;
          break;

         case 5:
          Get_RxFrameCount();
          loopvar = 6;
          ethstep6 = 6;
          break;

         case 6:
        //  Read_PhyReg();
          loopvar = 7;
          ethstep7 = 7;
          break;

         default:
        	 loopvar = 1;
        	 ethstep8 = 8;
          break;
    }/* End of Case */
}



void Eth_int(void)
{
	  volatile uint8 EN1Val, EN1SelVal, PortPC12;
	  EN1Val = (uint8)SCU_EXTCON.B.EN1;
	  EN1SelVal = (uint8)SCU_EXTCON.B.SEL1;
	  PortPC12 = (uint8)P11_IOCR12.B.PC12;

	  Mcal_ResetSafetyENDINIT_Timed(150000U);
	  P11_IOCR12.B.PC12 = 0x16U;
	  SCU_EXTCON.B.SEL1 = 5U;//0101B fETH is selected for the external clock signal
	  SCU_EXTCON.B.EN1 = 1U;//0B No external clock is provided  1B The configured external clock is provided
	  Mcal_SetSafetyENDINIT_Timed();

	  Eth_17_EthMac_ControllerInit(0, 0);//EB->Controller Initialization1
	  Eth_17_EthMac_SetControllerMode(0,ETH_MODE_ACTIVE);//EB->Controller Initialization2
	  Eth_17_EthMac_GetPhysAddr((uint8)0,&Temp_Macaddress[0]);
	  TransmitInit(); /* Initialise Eth PHY */

	  Mcal_ResetSafetyENDINIT_Timed(150000U);
	  SCU_EXTCON.B.EN1 = EN1Val;
	  SCU_EXTCON.B.SEL1 = EN1SelVal;
	  P11_IOCR12.B.PC12 = PortPC12;
	  Mcal_SetSafetyENDINIT_Timed();
	  //TransmitInit(); /* Initialise Eth Driver including PHY */
}

/*******************************************************************************
** Transmit ETH Frame of User Defined Length **
*******************************************************************************/
void Tx1_EthFrame (void)
{

char szString[80];
uint16 TmpTxlength,i;
uint8 *TmpEthTxBuffPtr;
TxFrameCount = 0;
TmpTxlength = 8;
Global_TempUSage1 = Eth_17_EthMac_ProvideTxBuffer(0,&TmpBuffIdx,&TmpEthTxBuffPtr,&TmpTxlength);

for (i=0; i < 8 ; i++)
{
*TmpEthTxBuffPtr = i+1;
TmpEthTxBuffPtr+=1;
}

EthDemoRetVal = Eth_17_EthMac_Transmit(0,TmpBuffIdx,ETH_FRAME_TYPE_1,1,TmpTxlength,&MacDestArpaddress[0]);
//DISABLE();
Timer_DelayMs(300);
//ENABLE();
#if (!((ETH_17_ENA_TX_INT == STD_ON) && (ETH_17_ENA_RX_INT == STD_ON)))
do
{
Eth_17_EthMac_TxConfirmation(0);
}
#endif
while(!TxFrameCount);
TxFrameCount = 0;

//Timer_DelayMs(500);
//Eth_17_EthMac_Receive(0);
}

uint8 ethtxStep1;
uint8 ethtxStep2;
uint8 ethtxStep3;
uint8 ethtxStep4;
uint8 ethtxStep5;
uint8 ethtxStep6;
uint8 ethtxStep7;
uint8 ethtxStep8;
void Tx2_EthFrame (void)
{

uint16 TmpTxlength = 28;
uint8 *TmpEthTxBuffPtr[28];
TxFrameCount = 0;
ethtxStep1 = 1;
#if 1
Global_TempUSage1 = Eth_17_EthMac_ProvideTxBuffer(0,&TmpBuffIdx,&TmpEthTxBuffPtr,&TmpTxlength);
ethtxStep2 =1;
TxBufferFill(TmpEthTxBuffPtr[0],TmpTxlength,ArpData);
ethtxStep3=1;
EthDemoRetVal = Eth_17_EthMac_Transmit(0,TmpBuffIdx,0x0806,1,TmpTxlength,&MacDestArpaddress[0]);
ethtxStep4=1;
Timer_DelayMs(300);
ethtxStep5=1;
#if (!((ETH_17_ENA_TX_INT == STD_ON) && (ETH_17_ENA_RX_INT == STD_ON)))
do
{
	ethtxStep6=1;
Eth_17_EthMac_TxConfirmation(0);
ethtxStep7=1;
}
#endif
while(!TxFrameCount);
{
	ethtxStep8 = 1;
    TxFrameCount = 0;
}
ethtxStep8=2;
#endif
}


void Rx_loopback(void)
{
	Eth_17_EthMac_Receive(0);
}



