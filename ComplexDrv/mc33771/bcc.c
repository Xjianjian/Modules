#include "bcc.h"
#include "SIUL.h"
#include "Defines.h"
#include "platformTypes.h"
#include "stdint.h"
#include "spi_pri.h"
#include "bcc_mc33771.h"
#include "spi.h"
#include "spiCfg.h"
#include "hdio.h"
#include "hdioCfg.h"
#include "global_var.h"
#include "EvtHdl.h"
#include "bcc_spi_dma.h"

#include "MPC5744P.h"

#if (50000000u != m_spi_inClkFreq)
//#error "delay count must be modified"
#endif

/* 引用其它模块的函数 */
extern void spi_init(uint8_t module,spiModuleCfg_t ModcfgInfo);

/* SPI1的配置，SPI1用于正向数据的接收 */
static const uint8_t spiModule1CfgIndxGrp[1] = {0};
static const uint8_t spiModule1CsCmcrGrp[1] = {SIUL_PA5};
static const uint8_t spiModule1CsImcrGrp[1] = {SIUL_ALT1};
static const spiExSlvCfg_t spiExSlvCfg1[1]	=
{
	{
	2000000,		/*mc33664_1*/
	8,
	TRUE,
	FALSE,
	TRUE
	}
};
const spiModuleCfg_t spiModule1Cfg =
{
	FALSE,						/*master*/
	0,						/*intMsk*/
	FALSE,						/*fifoEna*/
	FALSE,						/*dmaEna*/
	1,							/*slvCnt*/
	1,							/*cfgInfoCnt*/
	spiModule1CfgIndxGrp,		/*cfgIndx*/
	spiModule1CsCmcrGrp,		/*csMscr*/
	spiModule1CsImcrGrp,		/*csAltLv*/
	spiExSlvCfg1				/*cfgInfo*/
};


/* 最大IC个数，可以标定 */
uint8_t bcc_icNum = 9;
#if (1 == BCC_TEMP_CHANNEL_HANDLE)
uint16_t bcc_boardTemp[BCC_IC_NUM_MAX][BCC_BOARD_TEMP_NUM_MAX];
#endif
uint16_t bcc_cellVoltBuff[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX] =
{
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE}
};
uint16_t bcc_cellTempBuff[BCC_IC_NUM_MAX][BCC_TEMP_NUM_MAX] =
{
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE}
};

uint32_t bcc_StackVltgBuff[BCC_IC_NUM_MAX] = {0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE};

/*DNTC Variables*/
/* 单体电压、单体温度、和板温，电压精度：1mv，温度精度：0.125， 偏移：-40； 计算公式y=k*x+b */
uint16_t bcc_cellVolt[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX] = 
{
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE}
};
uint16_t bcc_cellTemp[BCC_IC_NUM_MAX][BCC_TEMP_NUM_MAX] = 
{
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE},
	{0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE}
};
uint32_t bcc_StackVltg[BCC_IC_NUM_MAX] = {0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE,0xFFFFFFFE};
uint8_t bcc_CellBalCmd[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX] = 
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
uint8_t bcc_CellBalSta[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX] = 
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
uint8_t bcc_FstSampleFinsh = 0;
uint16_t bcc_ChipTemp[BCC_IC_NUM_MAX] = {0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE,0xFFFE};
bool Bcc_VPWR_OV[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_VPWR_UV[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_CellVltgLine_Open[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX] = 
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
bool Bcc_GPIO_Short[BCC_IC_NUM_MAX][BCC_TEMP_NUM_MAX] = 
{
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0}
};
bool Bcc_GPIO_Open[BCC_IC_NUM_MAX][BCC_TEMP_NUM_MAX] = 
{
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0}
};
bool bcc_BalCir_Short[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX] = 
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
bool bcc_BalCir_Open[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX] = 
{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
bool bcc_VCOM_OV[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool bcc_VCOM_UV[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool bcc_VANA_OV[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool bcc_VANA_UV[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_OSC_ERR[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_ADC_ERR[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_GND_LOSS[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_Thermal_SHD[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_Diag_TimeOut[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_Comm_33664Loss = 0;
bool Bcc_Comm_TAGIDErr[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_Comm_AddrErr[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_Comm_CRCErr[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_Comm_CntErr[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
bool Bcc_Comm_RecvDataNotComplet = 0;


static uint8_t bcc_currentCmd[6];

static bcc_normalStateStep_t bcc_normalStateStep;
static bcc_stateMachine_t bcc_stateMachine = BCC_STATE_INIT;
/* 33771返回指令的结构体 */
bcc_readResData_t bcc_readResData;
static uint8_t bcc_voltFilterCnt[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX];
static uint8_t bcc_tempLimitFilterCnt[BCC_IC_NUM_MAX][BCC_TEMP_NUM_MAX];
static uint8_t bcc_stackVoltDeltaLimitFilterCnt[BCC_IC_NUM_MAX];
static uint8_t bcc_innerTempDeltaLimitFilterCnt[BCC_IC_NUM_MAX];
/* 发送33771启动转换指令时的tag_id */
static uint8_t bcc_conversionTagId = 0;
static bcc_initMachineCtrl_t bcc_initMachineCtrl;
/* CRC算法表 */
static const uint8_t BCC_CRC_TABLE[BCC_CRC_TBL_SIZE];
/* ADC的参考电压 */
static uint16_t bcc_fault1Status[BCC_NUM_CONFIG] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint16_t bcc_fault2Status[BCC_NUM_CONFIG] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint16_t bcc_fault3Status[BCC_NUM_CONFIG] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint16_t bcc_CB_OPEN_FLT[BCC_NUM_CONFIG] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint16_t bcc_CB_SHORT_FLT[BCC_NUM_CONFIG] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

static uint16_t bcc_CTxOpenDiagCellVolt[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX];
static uint8_t bcc_balSendCmdIndex = 0;


#if (9 == BCC_INIT_IC_NUM)
static uint8_t bcc_cellShortCfgTab[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX] =
{
	{1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
};
const uint16_t bcc_faultMask[BCC_NUM_CONFIG] = {0xffcf, 0xf807, 0xff0f, 0xff0f, 0xff0f, 0xff0f, 0xff0f, 0xff8f, 0xffcf};
#else

static uint8_t bcc_cellShortCfgTab[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX] =
{
	{1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
};

const uint16_t bcc_faultMask[BCC_NUM_CONFIG] = {0xffcf, 0xffef, 0xffef, 0xff0f, 0xff0f, 0xff8f, 0xffcf};

#if 0
static uint8_t bcc_cellShortCfgTab[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX] =
{
	{1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
};
const uint16_t bcc_faultMask[BCC_NUM_CONFIG] = {0xffcf, 0xff8f, 0xff0f, 0xff0f, 0xffef, 0xffef, 0xffcf};
#endif

#endif

#if (1 == BCC_TEMP_CHANNEL_HANDLE)
/* 温度通道采样的过滤计数，用于诊断短路和开路故障*/
static uint8_t bcc_tempSampleFilter[BCC_IC_NUM_MAX] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
/* 温度短路到地故障 */
uint8_t bcc_tempShortToGnd[BCC_IC_NUM_MAX] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
/* 温度开路故障 */
uint8_t bcc_tempOpenLoad[BCC_IC_NUM_MAX] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif



static uint8_t bcc_responseDataLenInvalidCnt = 0;

static void bcc_clearSPIData(void);

/* 下拉CS的引脚，默认SPI */
static void bcc_pullDownSpiCS(uint8_t t_pad);
/* 上拉CS的引脚，默认SPI */
static void bcc_pullUpSpiCS(uint8_t t_pad);
/* 读取某个器件的ID，也就是读取初始化寄存器的数据 */
static uint8_t bcc_readDeviceId(uint8_t tDeviceId);
/* 调用SPI驱动，发送一串数据，是复杂驱动和BSW的接口 */
static void bcc_spiSendData(uint8_t t_spiModule, uint8_t t_buff[], uint8_t t_buffLength);
/* 给每一个IC发送均衡指令 */
static void bcc_sendBalCmd(void);
static void bcc_33771LocalWrite(uint16_t t_memoryData, uint8_t t_memoryAddr, uint8_t t_CID, uint8_t t_Command);
/* 全局写指令，可以实现电压同步转换，初始化寄存器同时实现 */
static void bcc_33771GlobalWrite(uint16_t t_memoryData, uint8_t t_memoryAddr);
/* 调用SPI读取函数，读取33771返回的数据流 */
//static uint8_t bcc_readDeviceResponse(uint32_t tBuff[], uint8_t *tBuffLen, uint8_t tBuffSizeMax);
/* 从一条返回指令中读取一个寄存器的数据 */
//static uint8_t bcc_extractOneRegData(uint32_t tBuff[], uint8_t tBuffLen, bcc_readResData_t *pbcc_readResData, uint8_t tIc);
/* 移除返回的数据流中的无效数据，无效数据的产生原因是33664会返回MCU发送的指令，由于读取未使用中断，接收时会
 * 丢帧 */
//static uint8_t bcc_readDataRelocation(uint8_t tAbandonLen, uint8_t tDataLen);
/* 局部写函数，用于写一个IC的数据 */
void bcc_33771LocalWrite(uint16_t t_memoryData, uint8_t t_memoryAddr, uint8_t t_CID, uint8_t t_Command);
/* 设置GPIO口的状态，一般用于SPI_CS引脚 */
static void bcc_setEnablePortState(uint8_t pad, uint8_t tState);
/* 用于实现GPIO口的初始化 */
void bcc_gpioInit(void);
/* 唤醒33664 */
static uint8_t bcc_wakeUp33664(bcc_communicationDir_t tDir);
/* 分配CID，初始化完成之后方能唤醒下一个器件 */
static void bcc_deviceClusterIdAssign(uint8_t tAssignId);
/* Reset (soft reset) BCC to assign CID (in case of CID was already assigned). */
static void bcc_resetAll33771(void);
/* 初始化指定数量IC的寄存器 */
static uint8_t bcc_initReg(uint8_t tIcNum);
/* 唤醒一个33771，用CS唤醒 */
static void bcc_wakeUp33771(bcc_communicationDir_t tDir);
static uint8_t bcc_initDeviceIDWithDir(uint8_t tIDNum, bcc_communicationDir_t tDir);
/* 初始化本模块使用的SPI模块 */
static void bcc_spiInit(void);
/* 初始化数组 */
static void bcc_initArrayData(void);
/* CRC算法函数 */
static uint8_t BCC_CalcCRC(const uint8_t *data, uint8_t dataLen);

static void bcc_resetRunning(void);


uint32_t bcc_version = 100;

/* 使用延迟计数实现的延迟函数 */
static void bcc_delay3us(void);
static void bcc_delay5ms(void);
static void bcc_delay27us(void);
static void bcc_delay640us(void);
static void bcc_delay150us(void);
static uint16_t bcc_abs_uint16(uint16_t t_factorA, uint16_t t_factorB);
static uint16_t bcc_voltFilter(uint8_t tIcIndex, uint8_t tVoltIndex, uint16_t tLastVolt, uint16_t tCurrentVolt);
static uint16_t bcc_tempDeltaLimitFilter(uint8_t tIcIndex, uint8_t tTempIndex, uint16_t tLastVolt, uint16_t tCurrentVolt);
static uint16_t bcc_StackVoltLimitFilter(uint8_t tIcIndex, uint16_t tLastVolt, uint16_t tCurrentVolt);
static uint16_t bcc_innerIcTempDeltaLimitFilter(uint8_t tIcIndex, uint16_t tLastTemp, uint16_t tCurrentTemp);

static void bcc_frameCrcDiag(uint8_t tIcIndex, uint8_t tCrcState);
static void bcc_addrErrDiag(uint8_t tIcIndex, uint8_t tWriteAddr,uint8_t tReadAddr);
static void bcc_diagCBxOpen_step6(void);
static void bcc_diagCBxOpen_restoreRegData(void);
static void bcc_diagCBxOpen_step4(void);
static void bcc_diagCBxOpen_step8(void);
static void bcc_diagCBxOpen_step3(void);
static void bcc_diagCBxOpen_step2(void);
static void bcc_exitDiagMode(void);
static void bcc_startCellBalAndExitBal(void);
static void bcc_enterDiagModeAndPauseBal(void);
static void bcc_adcErrDiag(void);
static void bcc_gndLossDiag(void);
static void bcc_oscErrDiag(void);
static void bcc_VANADiag(void);
static void bcc_VCOMDiag(void);
static void bcc_VPWRDiag(void);
//static void bcc_CTxOpenDiag(void);
static void bcc_clearCBxOpenFaultRaw(void);
static void bcc_updateCBxFaultToRTE(void);
static void bcc_icTempErrAndEixtDiagTimeOutDiag(void);
static void bcc_CBxShortDiag(void);
static void bcc_clearReg_CB_SHORT_FLT(void);
static void bcc_updateSynSampleData(void);
//static void bcc_clrBalStatus(void);
/* 启动所有IC的ADC转换 */
static uint8_t bcc_startConversion(void);
//static uint8_t bcc_echoDataCheck(uint32_t tBuff[], uint8_t tLen);
static void bcc_sysCfg2ForIc8(void);

void bcc_startDmaTransmit(uint8_t tByte);
uint8_t bcc_checkEcho(void);

static void bcc_updateBalSta(void);

uint8_t bcc_echoErrCnt = 0;
uint8_t bcc_1SDiagReadFlg = 0;
uint8_t bcc_300msDiagReadFlg = 0;


/* 该表用于33771的CRC算法 */
static const uint8_t BCC_CRC_TABLE[BCC_CRC_TBL_SIZE] = {
    0x00U, 0x2fU, 0x5eU, 0x71U, 0xbcU, 0x93U, 0xe2U, 0xcdU, 0x57U, 0x78U, 0x09U, 0x26U, 0xebU, 0xc4U, 0xb5U, 0x9aU,
    0xaeU, 0x81U, 0xf0U, 0xdfU, 0x12U, 0x3dU, 0x4cU, 0x63U, 0xf9U, 0xd6U, 0xa7U, 0x88U, 0x45U, 0x6aU, 0x1bU, 0x34U,
    0x73U, 0x5cU, 0x2dU, 0x02U, 0xcfU, 0xe0U, 0x91U, 0xbeU, 0x24U, 0x0bU, 0x7aU, 0x55U, 0x98U, 0xb7U, 0xc6U, 0xe9U,
    0xddU, 0xf2U, 0x83U, 0xacU, 0x61U, 0x4eU, 0x3fU, 0x10U, 0x8aU, 0xa5U, 0xd4U, 0xfbU, 0x36U, 0x19U, 0x68U, 0x47U,
    0xe6U, 0xc9U, 0xb8U, 0x97U, 0x5aU, 0x75U, 0x04U, 0x2bU, 0xb1U, 0x9eU, 0xefU, 0xc0U, 0x0dU, 0x22U, 0x53U, 0x7cU,
    0x48U, 0x67U, 0x16U, 0x39U, 0xf4U, 0xdbU, 0xaaU, 0x85U, 0x1fU, 0x30U, 0x41U, 0x6eU, 0xa3U, 0x8cU, 0xfdU, 0xd2U,
    0x95U, 0xbaU, 0xcbU, 0xe4U, 0x29U, 0x06U, 0x77U, 0x58U, 0xc2U, 0xedU, 0x9cU, 0xb3U, 0x7eU, 0x51U, 0x20U, 0x0fU,
    0x3bU, 0x14U, 0x65U, 0x4aU, 0x87U, 0xa8U, 0xd9U, 0xf6U, 0x6cU, 0x43U, 0x32U, 0x1dU, 0xd0U, 0xffU, 0x8eU, 0xa1U,
    0xe3U, 0xccU, 0xbdU, 0x92U, 0x5fU, 0x70U, 0x01U, 0x2eU, 0xb4U, 0x9bU, 0xeaU, 0xc5U, 0x08U, 0x27U, 0x56U, 0x79U,
    0x4dU, 0x62U, 0x13U, 0x3cU, 0xf1U, 0xdeU, 0xafU, 0x80U, 0x1aU, 0x35U, 0x44U, 0x6bU, 0xa6U, 0x89U, 0xf8U, 0xd7U,
    0x90U, 0xbfU, 0xceU, 0xe1U, 0x2cU, 0x03U, 0x72U, 0x5dU, 0xc7U, 0xe8U, 0x99U, 0xb6U, 0x7bU, 0x54U, 0x25U, 0x0aU,
    0x3eU, 0x11U, 0x60U, 0x4fU, 0x82U, 0xadU, 0xdcU, 0xf3U, 0x69U, 0x46U, 0x37U, 0x18U, 0xd5U, 0xfaU, 0x8bU, 0xa4U,
    0x05U, 0x2aU, 0x5bU, 0x74U, 0xb9U, 0x96U, 0xe7U, 0xc8U, 0x52U, 0x7dU, 0x0cU, 0x23U, 0xeeU, 0xc1U, 0xb0U, 0x9fU,
    0xabU, 0x84U, 0xf5U, 0xdaU, 0x17U, 0x38U, 0x49U, 0x66U, 0xfcU, 0xd3U, 0xa2U, 0x8dU, 0x40U, 0x6fU, 0x1eU, 0x31U,
    0x76U, 0x59U, 0x28U, 0x07U, 0xcaU, 0xe5U, 0x94U, 0xbbU, 0x21U, 0x0eU, 0x7fU, 0x50U, 0x9dU, 0xb2U, 0xc3U, 0xecU,
    0xd8U, 0xf7U, 0x86U, 0xa9U, 0x64U, 0x4bU, 0x3aU, 0x15U, 0x8fU, 0xa0U, 0xd1U, 0xfeU, 0x33U, 0x1cU, 0x6dU, 0x42U
};


/* 电池温度的RT表，不同的项目需要修改 */
const uint32_t Ltc6811_CellTempRtTab[BCC_CELL_TEMP_TAB_LEN] =
{
    221900,                                                                          /*        -40 degree */
    209100, 197100, 185900, 175400, 165600, 156400, 147800, 139800, 132200, 125100,  /* -39 to -30 degree */
    118400, 112000, 106100, 100500, 95300, 90370, 85730, 81370, 77250, 73380,          /* -29 to -20 degree */
    69710, 66260, 62990, 59920, 57010, 54270, 51680, 49230, 46910, 44720,            /* -19 to -10 degree */
    42630, 40660, 38780, 37010, 35330, 33740, 32230, 30800, 29450, 28160,            /* -9  to   0 degree */
    26920, 25750, 24640, 23580, 22580, 21620, 20720, 19850, 19030, 18250,            /* 1-------10 degree */
    17500, 16780, 16100, 15450, 14830, 14240, 13680, 13140, 12630, 12140,            /* 11------20 degree */
    11680, 11230, 10800, 10390, 10000, 9625, 9267, 8924, 8597, 8283,                 /* 21------30 degree */
    7982, 7693, 7417, 7153, 6899, 6656, 6424, 6200, 5986, 5781,                      /* 31------40 degree */
    5583, 5393, 5211, 5035, 4867, 4706, 4551, 4401, 4258, 4120,                      /* 41------50 degree */
    3987, 3860, 3737, 3619, 3505, 3395, 3290, 3188, 3090, 2996,                      /* 51------60 degree */
    2904, 2816, 2731, 2649, 2570, 2493, 2420, 2349, 2280, 2214,                      /* 61------70 degree */
    2150, 2089, 2029, 1971, 1916, 1862, 1810, 1760, 1711, 1665,                      /* 71------80 degree */
    1619, 1575, 1532, 1491, 1451, 1413, 1375, 1339, 1304, 1271,                      /* 81------90 degree */
    1238, 1206, 1175, 1145, 1116, 1088, 1060, 1034, 1008, 983,                        /* 91-----100 degree */
    959, 936, 913, 891, 869, 848, 828, 808, 789, 771,                                /* 101----110 degree */
    753, 735, 718, 702, 685, 670, 655, 640, 625, 611,                                /* 111----120 degree */
    598, 585, 572, 559, 547  /* 121----125 degree0, estimating by yuanchengbao 20150519 */
};


/* 采集板上的RT表，该NTC用于采集均衡的温度 */
const uint32_t Ltc6811_BalTempRtTab[BCC_BOARD_TEMP_RT_NUM] =
{
    188500,                                                                 /*      -40*/
    144300, 111300, 86600, 67800, 53460, 42450, 33930, 27280, 22070, 17960, /*-35 to 10*/
    14700, 12090, 10000, 8310, 6940, 5826, 4911, 4158, 3536, 3019,          /*15 to 60 */
    2588, 2227, 1924, 1668, 1451, 1267, 1110, 975, 860, 760,                /*65 to 110*/
    674, 599, 534                                                           /*115 to 125*/
};




/* 设置GPIO口的状态，一般用于SPI_CS引脚 */
static void bcc_setEnablePortState(uint8_t pad, uint8_t tState)
{
	if (0 == tState)
	{
		SIUL_ClearPad(pad);
	}
	else if (1 == tState)
	{
		SIUL_SetPad(pad);
	}
	else
	{
		/* keep state */
	}
}




static void bcc_delay3us(void)
{
	uint32_t tCnt = 0;

	for (tCnt = 0; tCnt < BCC_DELAY_3US_TICK; tCnt++)
	{
		;
	}
}



#if 0
static void bcc_delay5us(void)
{
	uint32_t tCnt = 0;

	for (tCnt = 0; tCnt < BCC_DELAY_5US_TICK; tCnt++)
	{
		;
	}
}





static void bcc_delay3ms(void)
{
	uint32_t tCnt = 0;

	for (tCnt = 0; tCnt < BCC_DELAY_3MS_TICK; tCnt++)
	{
		;
	}
}

#endif




static void bcc_delay5ms(void)
{
	uint32_t tCnt = 0;

	for (tCnt = 0; tCnt < BCC_DELAY_5MS_TICK; tCnt++)
	{
		;
	}
}




static void bcc_delay27us(void)
{
	uint32_t tCnt = 0;

	for (tCnt = 0; tCnt < BCC_DELAY_27US_TICK; tCnt++)
	{
		;
	}
}





static void bcc_delay640us(void)
{
	uint32_t tCnt = 0;

	for (tCnt = 0; tCnt < BCC_DELAY_640US_TICK; tCnt++)
	{
		;
	}
}

#if 0
static void bcc_delay1200us(void)
{
	uint32_t tCnt = 0;

	for (tCnt = 0; tCnt < BCC_DELAY_1200US_TICK; tCnt++)
	{
		;
	}
}
#endif

static void bcc_delay150us(void)
{
	uint32_t tCnt = 0;

	for (tCnt = 0; tCnt < BCC_DELAY_150US_TICK; tCnt++)
	{
		;
	}
}


/* 此函数将初始化33664的EN/INT引脚、SPI的CS引脚 */
void bcc_gpioInit(void)
{
	/* set first 33664's EN pin to output, but disable 33664 */
	SIUL_DigitalOutput(BCC_ENABLE_PORT_PCR, SIUL_GPIO);
	SIUL_ClearPad(BCC_ENABLE_PORT_PCR);

	/* set first SPI which is master mode to Output, but pull up the CS pin */
	SIUL_DigitalOutput(BCC_MASTER_FIRST_SPI_CS, SIUL_GPIO);
	SIUL_SetPad(BCC_MASTER_FIRST_SPI_CS);

	SIUL_DigitalOutput(BCC_MASTER_SECOND_SPI_CS, SIUL_GPIO);
	SIUL_SetPad(BCC_MASTER_SECOND_SPI_CS);
}


/* 唤醒33664 */
static uint8_t bcc_wakeUp33664(bcc_communicationDir_t tDir)
{
#if 0
	uint32_t  Timeout = BCC_WAKEUP_TIMEOUT;    /* Wake-up timeout. */
#endif
	uint8_t retState = 0;

	if (BCC_COMM_FIRST_33664 == tDir)
	{
		hld_do_MC33664_EN = 0;
		hdio_doUpdtIndx(m_hdo_MC33664_EN);

		bcc_delay150us();

		hld_do_MC33664_EN = 1;
		hdio_doUpdtIndx(m_hdo_MC33664_EN);
	}
	else
	{
		hld_do_MC33664_ENBAK = 0;
		hdio_doUpdtIndx(m_hdo_MC33664_ENBAK);

		bcc_delay150us();

		hld_do_MC33664_ENBAK = 1;
		hdio_doUpdtIndx(m_hdo_MC33664_ENBAK);
	}


#if 0
	while (((bcc_getIntState()) == TRUE) && (Timeout > 0))
	{
	    /* Wait for INTB transition from high to low (max. 100 us). */
	    /* Timeout. */
	    Timeout--;
	 }

	if (Timeout == 0)
	{
		retState = 0;
	}
#endif

	bcc_delay150us();

	return retState;
}


/* 初始化本模块使用的SPI模块 */
static void bcc_spiInit(void)
{
	/* initialize the first SPI which is master mode */

	/* initialize the first SPI which is slave mode */
	spi_init(1,spiModule1Cfg);

}

/* 唤醒一个33771，用CS唤醒 */
static void bcc_wakeUp33771(bcc_communicationDir_t tDir)
{
	uint8_t tPin;

	if (BCC_COMM_FIRST_33664 == tDir)
	{
		tPin = BCC_MASTER_FIRST_SPI_CS;
	}
	else
	{
		tPin = BCC_MASTER_SECOND_SPI_CS;
	}


	bcc_setEnablePortState(tPin, 1);
	bcc_delay640us();

	bcc_setEnablePortState(tPin, 0);
	bcc_delay150us();

	bcc_setEnablePortState(tPin, 1);
	bcc_delay640us();

	bcc_setEnablePortState(tPin, 0);
	//bcc_delay27us();
	bcc_delay150us();

	bcc_setEnablePortState(tPin, 1);
//	bcc_delay640us();
//	bcc_delay1200us();
}


#if 0

/* 移除返回的数据流中的无效数据，无效数据的产生原因是33664会返回MCU发送的指令，由于读取未使用中断，接收时会
 * 丢帧 */
static uint8_t bcc_readDataRelocation(uint8_t tAbandonLen, uint8_t tDataLen)
{
	uint8_t tRetState = 0;
	uint8_t tCnt = 0;


	if (((tAbandonLen + tDataLen) < BCC_READ_BUFF_SZIE) && (3 == tAbandonLen))
	{
		tRetState = 1;

		for (tCnt = 0; tCnt < tDataLen; tCnt++)
		{
			bcc_responseBuff[tCnt] = bcc_responseBuff[tCnt + tAbandonLen];
		}
	}

	return tRetState;
}

#endif


#if 0
static uint8_t bcc_echoDataCheck(uint32_t tBuff[], uint8_t tLen)
{
	uint8_t tRetSta = 0;
	if (tLen >= 3)
	{
		if (((bcc_currentCmd[1]) != (tBuff[0] & 0xFF)) ||
				(bcc_currentCmd[0] != ((tBuff[0] & 0xFF00) >> 8)) ||
				((bcc_currentCmd[3]) != (tBuff[1] & 0xFF)) ||
				(bcc_currentCmd[2] != ((tBuff[1] & 0xFF00) >> 8)) ||
				((bcc_currentCmd[5]) != (tBuff[2] & 0xFF)) ||
				(bcc_currentCmd[4] != ((tBuff[2] & 0xFF00) >> 8)))
		{
			bcc_echoErrCnt++;
			if (bcc_echoErrCnt >= 8)
			{
				bcc_echoErrCnt = 8;
				Bcc_Comm_33664Loss = 1;
			}
		}
		else
		{
			tRetSta = 1;
			if (bcc_echoErrCnt > 0)
			{
				bcc_echoErrCnt--;
			}
			if (bcc_echoErrCnt <= 5)
			{
				Bcc_Comm_33664Loss = 0;
			}
		}
	}
	else
	{
	}
	return tRetSta;
}

#endif


#if 0

/*
 * 读取SPI的数据
 *
 *tBuff：SPI的返回数据，如果没有返回数据，则tBuff的数据是0x55555555；
 *tBuffLen：SPI的返回数据的长度，如果没有返回数据，则数据为0
 *tBuffSizeMax：最大允许返回的数据长度，不能为0
 *
 *返回值：成功=1；失败=0
 * */
static uint8_t bcc_readDeviceResponse(uint32_t tBuff[], uint8_t *tBuffLen, uint8_t tBuffSizeMax)
{

	uint32_t  tCnt = 0;
	uint32_t tTimeOut = 0;
	uint32_t tReadDataCnt = 0;

	uint8_t tReadState = 0;
	uint8_t tSpiReadModule = 1;

	OS_DISABLE_INTERRUPT;
	if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
	{
		tSpiReadModule = 1;
	}
	else
	{
		tSpiReadModule = 2;
	}

	if (((void *)0 != tBuff) && ((void *)0 != tBuffLen) && (tBuffSizeMax > 0))
	{
		/* 初始化返回数据 */
		for (tCnt = 0; tCnt < tBuffSizeMax; tCnt++)
		{
			tBuff[tCnt] = 0x55555555;
		}
		*tBuffLen = 0;


		for (tReadDataCnt = 0; tReadDataCnt < tBuffSizeMax; tReadDataCnt++)
		{
			for (tTimeOut = 0; tTimeOut < BCC_SPI_READ_TIMEOUT_CNT_MAX; tTimeOut++)
			{
				if (0 == spi_readDt(tSpiReadModule, &tBuff[tReadDataCnt]))
				{
					break;
				}
			}

			if (BCC_SPI_READ_TIMEOUT_CNT_MAX == tTimeOut)
			{
				*tBuffLen = tReadDataCnt;
				break;
			}
		}

		if (tBuffSizeMax == tReadDataCnt)
		{
			*tBuffLen = tBuffSizeMax;
		}
		else
		{
			/* do nothing */
		}

		if ((*tBuffLen) == tBuffSizeMax)
		{
			if (1 == bcc_echoDataCheck(tBuff, (*tBuffLen)))
			{
			tReadState = 1;
			}
		}

	//	bcc_receiveDataLenDiag((*tBuffLen), tBuffSizeMax);
	}
	else
	{
		/* do nothing */
	}

	OS_ENABLE_INTERRUPT;

	return tReadState;
}

#endif


static uint8_t bcc_extractOneDataFromDmaBuff(uint8_t tIndex, uint8_t tBuffLen, bcc_readResData_t *pbcc_readResData, uint8_t tIc,
		uint8_t tExpectAddr)
{
	uint8_t tRetState = 0;
	uint8_t tCalcCrc = 0x55;
	uint8_t tBuffByte[6];
	unsigned short int *tBuff;

	if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
	{
		tBuff = &spi1DmaRxBuffer[tIndex];
	}
	else
	{
		tBuff = &spi2DmaRxBuffer[tIndex];
	}


	if ((((void *)0 != tBuff) && ((void *)0 != pbcc_readResData))
			&& (BCC_RESPONSE_DATA_LEN_OF_8BIT == tBuffLen))
	{
#if 1
		tBuffByte[0] = (tBuff[0] >> 8) & 0xFF;
		tBuffByte[1] = tBuff[0] & 0xFF;
		tBuffByte[2] = (tBuff[1] >> 8) & 0xFF;
		tBuffByte[3] = tBuff[1] & 0xFF;
		tBuffByte[4] = (tBuff[2] >> 8) & 0xFF;
		tBuffByte[5] = tBuff[2] & 0xFF;
#endif



		tCalcCrc = BCC_CalcCRC(tBuffByte, 5);
		if (tCalcCrc == tBuffByte[5])
		{
			tRetState = 1;

			if (tIc < BCC_IC_NUM_MAX)
			{
				bcc_frameCrcDiag(tIc, 1);
			}
			pbcc_readResData->mc33771_regData = (((uint16_t)tBuffByte[0]) << 8) + (uint16_t)tBuffByte[1];
			pbcc_readResData->mc33771_regAddr = tBuffByte[2] & 0x7F;

			bcc_addrErrDiag(tIc, tExpectAddr, pbcc_readResData->mc33771_regAddr);

			pbcc_readResData->mc33771_cid = tBuffByte[3] & 0x3F;
			pbcc_readResData->mc33771_msgCnt = ((tBuffByte[4] >> 4) & 0x0F);
		}
		else
		{
			if (tIc < BCC_IC_NUM_MAX)
			{
				bcc_frameCrcDiag(tIc, 0);
			}
		}
	}

	return tRetState;
}


#if 0
/*
 * 从一个返回序列中提取相关数据
 *
 *tBuff：输入的序列
 *tBuffLen：序列的长度
 *pbcc_readResData：所有的相关数据
 *
 *uint8_t：提取成功=1；提取失败=0
 * */
static uint8_t bcc_extractOneRegData(uint32_t tBuff[], uint8_t tBuffLen, bcc_readResData_t *pbcc_readResData, uint8_t tIc)
{
	uint8_t tRetState = 0;
	uint8_t tCalcCrc = 0x55;

	uint8_t tBuffByte[6];


	if ((((void *)0 != tBuff) && ((void *)0 != pbcc_readResData))
			&& (BCC_RESPONSE_DATA_LEN_OF_16BIT == tBuffLen))
	{
		tBuffByte[0] = (tBuff[0] >> 8) & 0xFF;
		tBuffByte[1] = tBuff[0] & 0xFF;
		tBuffByte[2] = (tBuff[1] >> 8) & 0xFF;
		tBuffByte[3] = tBuff[1] & 0xFF;
		tBuffByte[4] = (tBuff[2] >> 8) & 0xFF;
		tBuffByte[5] = tBuff[2] & 0xFF;

		tCalcCrc = BCC_CalcCRC(tBuffByte, 5);
		if (tCalcCrc == tBuffByte[5])
		{
			tRetState = 1;

			if (tIc < BCC_IC_NUM_MAX)
			{
				bcc_frameCrcDiag(tIc, 1);
			}
			pbcc_readResData->mc33771_regData = (((uint16_t)tBuffByte[0]) << 8) + (uint16_t)tBuffByte[1];
			pbcc_readResData->mc33771_regAddr = tBuffByte[2] & 0x7F;
			pbcc_readResData->mc33771_cid = tBuffByte[3] & 0x3F;
			pbcc_readResData->mc33771_msgCnt = ((tBuffByte[4] >> 4) & 0x0F);
		}
		else
		{
			if (tIc < BCC_IC_NUM_MAX)
			{
				bcc_frameCrcDiag(tIc, 0);
			}
		}
	}

	return tRetState;
}

#endif


void bcc_startDmaTransmit(uint8_t tByte)
{
	uint16_t tCounter = 0;

	//bcc_clearDmaData();
	bcc_clearSPIData();

	if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
	{
		for (tCounter = 0; tCounter < (tByte/2); tCounter++)
		{
			spi1DmaRxBuffer[tCounter] = 0xFFFF;
		}

		bccSpi1_isrEnable(1,(tByte/2));
	//	bccSpiDma_startTransfer(e_spi1_dma, tByte);
	}
	else
	{
		for (tCounter = 0; tCounter < (tByte/2); tCounter++)
		{
			spi2DmaRxBuffer[tCounter] = 0xFFFF;
		}

		bccSpi2_isrEnable(1,(tByte/2));
	//	bccSpiDma_startTransfer(e_spi2_dma, tByte);
	}
}



uint8_t bcc_checkEcho(void)
{
	uint8_t tIndex;
	uint8_t tRetSta = 1;
	uint8_t tBuffByte[6];
	uint16_t tEcho[3];

	if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
	{
		tEcho[0] = spi1DmaRxBuffer[0];
		tEcho[1] = spi1DmaRxBuffer[1];
		tEcho[2] = spi1DmaRxBuffer[2];
	}
	else
	{
		tEcho[0] = spi2DmaRxBuffer[0];
		tEcho[1] = spi2DmaRxBuffer[1];
		tEcho[2] = spi2DmaRxBuffer[2];
	}

	tBuffByte[0] = (tEcho[0] >> 8) & 0xFF;
	tBuffByte[1] = tEcho[0] & 0xFF;
	tBuffByte[2] = (tEcho[1] >> 8) & 0xFF;
	tBuffByte[3] = tEcho[1] & 0xFF;
	tBuffByte[4] = (tEcho[2] >> 8) & 0xFF;
	tBuffByte[5] = tEcho[2] & 0xFF;

	for (tIndex = 0; tIndex < 6; tIndex++)
	{
		if (bcc_currentCmd[tIndex] != tBuffByte[tIndex])
		{
			tRetSta = 0;
		}
	}

	if (0 == tRetSta)
	{
		if (bcc_echoErrCnt < 8)
		{
			bcc_echoErrCnt++;
		}
		else
		{
			Bcc_Comm_33664Loss = 1;
		}
	}
	else
	{
		if (bcc_echoErrCnt > 0)
		{
			bcc_echoErrCnt--;
		}
		else
		{
			Bcc_Comm_33664Loss = 0;
		}
	}

	return tRetSta;
}


uint8_t bcc_recvLengthJudge(void)
{
	uint8_t tRetSta = 0;

	if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
	{
		if (1 == bccSpiDma_rxSuccess(e_spi1_dma))
		{
			tRetSta = 1;
		}
	}
	else
	{
		if (1 == bccSpiDma_rxSuccess(e_spi2_dma))
		{
			tRetSta = 1;
		}
	}

	if (0 == tRetSta)
	{
		if (bcc_responseDataLenInvalidCnt < 8)
		{
			bcc_responseDataLenInvalidCnt++;
		}
		else
		{
			Bcc_Comm_RecvDataNotComplet = 1;
		}
	}
	else
	{
		bcc_responseDataLenInvalidCnt = 0;
		Bcc_Comm_RecvDataNotComplet = 0;

#if 0
		if (bcc_responseDataLenInvalidCnt > 0)
		{
			bcc_responseDataLenInvalidCnt--;
		}
		else
		{
			Bcc_Comm_RecvDataNotComplet = 0;
		}
#endif
	}

	return tRetSta;
}


static void bcc_clearSPIData(void)
{
	if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
	{
		bccSpi1_isrEnable(1,(6));
	//	bccSpiDma_startTransfer(e_spi1_dma, tByte);
	}
	else
	{
		bccSpi2_isrEnable(1,(6));
	//	bccSpiDma_startTransfer(e_spi2_dma, tByte);
	}

	bcc_delay27us();
}


volatile uint8_t test_readData = 0;

/* 读取某个器件的ID，也就是读取初始化寄存器的数据 */
static uint8_t bcc_readDeviceId(uint8_t tDeviceId)
{
	uint8_t tRetData = 0xFF;

	bcc_startDmaTransmit(12);
	(void)bcc_33771LocalWrite(0x0001, 0x01, tDeviceId, BCC_CMD_READ);

	bcc_delay150us();

	if (1 == bcc_recvLengthJudge())
	//if (1 == bccSpiDma_rxSuccess(e_spi2_dma))
	{
		bcc_checkEcho();

		if (1 == bcc_extractOneDataFromDmaBuff(3, BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (tDeviceId-1), 0x01))
		{
			if ((tDeviceId == bcc_readResData.mc33771_cid)
						&& (0x01 == bcc_readResData.mc33771_regAddr))
			{
				tRetData = (uint8_t)bcc_readResData.mc33771_regData;
			}
		}
	}
	else
	{

	}


	return tRetData;
}


/* 分配CID，初始化完成之后方能唤醒下一个器件 */
static void bcc_deviceClusterIdAssign(uint8_t tAssignId)
{
	uint8_t t_Command;
	static uint8_t tAssignIdTagCnt = 0;

	t_Command = BCC_CMD_WRITE | (tAssignIdTagCnt << 4);
	bcc_33771LocalWrite(tAssignId, 0x01, 0x00, t_Command);

	tAssignIdTagCnt++;

}





/* 下拉CS的引脚，默认SPI */
static void bcc_pullDownSpiCS(uint8_t t_pad)
{
	SIUL_ClearPad(t_pad);
}

/* 上拉CS的引脚，默认SPI */
static void bcc_pullUpSpiCS(uint8_t t_pad)
{
	SIUL_SetPad(t_pad);
}



static void bcc_clearRxFifo(void)
{
	uint8_t tSpiReadModule = 1;
	uint8_t tCnt = 0;
	uint32_t tData;
	if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
	{
		tSpiReadModule = 1;
	}
	else
	{
		tSpiReadModule = 2;
	}
	for (tCnt = 0; tCnt < 20; tCnt++)
	{
		if (m_spi_readNoErr != spi_readDt(tSpiReadModule, &tData))
		{
			break;
		}
	}
}
/* 调用SPI驱动，发送一串数据，是复杂驱动和BSW的接口 */
static void bcc_spiSendData(uint8_t t_spiModule, uint8_t t_buff[], uint8_t t_buffLength)
{
	uint8_t tCnt = 0;
	uint8_t tPin;

	OS_DISABLE_INTERRUPT;

	switch (t_spiModule)
	{
	case 0:
		if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
		{
			tPin = BCC_MASTER_FIRST_SPI_CS;
		}
		else
		{
			tPin = BCC_MASTER_SECOND_SPI_CS;
		}
		bcc_clearRxFifo();
		bcc_pullDownSpiCS(tPin);
		bcc_delay3us();
		for (tCnt = 0; tCnt < t_buffLength; tCnt++)
		{
			//(void)spi_sendDt(0,0,0x04,(uint16_t)(t_buff[tCnt]),FALSE,TRUE);/*CS2*/

			(void)spi_sendDt(0,0,0x02,(uint16_t)(t_buff[tCnt]),FALSE,TRUE);/*CS1*/
		}
		bcc_delay3us();
		bcc_pullUpSpiCS(tPin);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
	OS_ENABLE_INTERRUPT;
}


/* CRC算法函数 */
static uint8_t BCC_CalcCRC(const uint8_t *data, uint8_t dataLen)
{
    uint8_t crc;      /* Result. */
    uint8_t tableIdx; /* Index to the CRC table. */
    uint8_t dataIdx;  /* Index to the data array (memory). */

    /* Expanding value. */
    crc = 0x42U;

    for (dataIdx = 0U; dataIdx < dataLen; dataIdx++)
    {
#ifdef BCC_MSG_BIGEND
        tableIdx = crc ^ (*(data + dataIdx));
#else
   //     tableIdx = crc ^ (*(data + BCC_MSG_SIZE - 1 - dataIdx));
#endif



        crc = BCC_CRC_TABLE[tableIdx];
    }

    return crc;
}




/*
 *发送一帧指令
 *
 *t_memoryData：指令的数据（如果是读指令，则表示数据的长度，如果是写指令，则表示将要写入的数据）
 *t_memoryAddr：33771寄存器的地址
 *t_CID:cluster ID，是33771的编码
 *t_Command：指令（读、写、全局写、无操作指令）
 *
 *
 *
 * */
void bcc_33771LocalWrite(uint16_t t_memoryData, uint8_t t_memoryAddr, uint8_t t_CID, uint8_t t_Command)
{
	uint8_t tCnt;
	uint8_t tCrcBuff[6];

	/* Memory Data field. */
	tCrcBuff[0] = ((uint8_t)(t_memoryData >> 8));
	tCrcBuff[1] = (uint8_t)t_memoryData;

	/* Memory Address fields. Master/Slave field is always 0 for sending. */
	tCrcBuff[2] = (t_memoryAddr & BCC_MSG_ADDR_MASK);

	/* Physical Address (Cluster ID). */
	tCrcBuff[3] = (t_CID & 0x3FU);

	/* Command field. */
	tCrcBuff[4] = (t_Command & 0xFFU);

	/* CRC field. */
	tCrcBuff[5] = BCC_CalcCRC(&tCrcBuff[0], 5);
	for (tCnt = 0; tCnt < 6; tCnt++)
	{
		bcc_currentCmd[tCnt] = tCrcBuff[tCnt];
	}

	//bcc_spiSendData(0, tSpiSendBuff, 5);
	bcc_spiSendData(BCC_SPI_TRANSMIT_DEFAULT, tCrcBuff, 6);
}


/*
 *全局写：cluster ID默认是0x00
 *
 *t_memoryData：将要写的数据
 *t_memoryAddr：33771的寄存器地址
 * */
static void bcc_33771GlobalWrite(uint16_t t_memoryData, uint8_t t_memoryAddr)
{
	uint8_t tCmd = 0x00;
	static uint8_t stu1_globalWriteCnt = 0;

	tCmd = BCC_CMD_GLOB_WRITE | (stu1_globalWriteCnt << 4);
	bcc_33771LocalWrite(t_memoryData, t_memoryAddr, 0x00, tCmd);
	stu1_globalWriteCnt++;
}


#if 0
/* 比较写入的数据和读取的数据，看它们是否一致 */
static uint8_t bcc_compareDataInGlobalWrite(uint8_t tRegAddr, uint16_t tRegData, uint8_t tIcNum)
{
	uint8_t tIcCnt = 0;
	uint8_t tReadLen;
	bcc_readResData_t tReadCmdData;
	uint8_t tRetState = 0;
	uint8_t tCompareCnt = 0;

	uint8_t tReadIcIndex = 0;

	for (tIcCnt = 0; tIcCnt < tIcNum; tIcCnt++)
	{
		bcc_delay27us();

		if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
		{
			tReadIcIndex = tIcCnt + 1;
		}
		else
		{
			tReadIcIndex = BCC_INIT_IC_NUM - tIcCnt;
		}

		bcc_33771LocalWrite(1, tRegAddr, (tReadIcIndex), BCC_CMD_READ);

		if (1 == bcc_readDeviceResponse(bcc_responseBuff, &tReadLen, BCC_RESPONSE_DATA_LEN_OF_16BIT*2))
		{
			if (tReadLen > BCC_RESPONSE_DATA_LEN_OF_16BIT)
			{
				bcc_getReadBuffLastData(BCC_RESPONSE_DATA_LEN_OF_16BIT, tReadLen);

				if (1 == bcc_extractOneRegData(bcc_responseBuff, BCC_RESPONSE_DATA_LEN_OF_16BIT,
						&tReadCmdData, 0xff))
				{
					if ((tReadCmdData.mc33771_regData == tRegData)
							&& (tReadCmdData.mc33771_regAddr == tRegAddr))
					{
						tCompareCnt++;
					}
				}
			}
		}
	}

	if (tCompareCnt == tIcNum)
	{
		tRetState = 1;
	}

	return tRetState;
}
#endif



#if 0
/* 查表算法，从RT表中获取温度，采样线性插值的方法查找 */
static uint16_t bcc_getTempFromRes(uint32_t tRes, const uint32_t tNTCTabBuff[], uint8_t tTabSize, uint8_t tStep)
{
    uint8_t tSearchIndex;
    uint16_t tuint16_t_Temp;

    if ((const uint32_t *)0 != tNTCTabBuff)
    {
        for (tSearchIndex = 0; tSearchIndex < tTabSize; tSearchIndex++)
        {
            if (tRes >= tNTCTabBuff[tSearchIndex])
            {
                break;
            }
        }

        if ((tSearchIndex > 0) && (tSearchIndex < (tTabSize-1)))
        {
            tuint16_t_Temp = (uint16_t)(((uint32_t)tStep)*8* (tSearchIndex-1) + ((uint32_t)tStep)*8*(tNTCTabBuff[tSearchIndex-1] - tRes)
                /(tNTCTabBuff[tSearchIndex - 1] - tNTCTabBuff[tSearchIndex]));
        }
        else if (0 == tSearchIndex)
        {
            tuint16_t_Temp = 0;
        }
        else
        {
            tuint16_t_Temp = (uint16_t)(((uint32_t)tStep)*8*(tTabSize - 1));
        }
    }

    return tuint16_t_Temp;
}
#endif


#if 0
/* 一阶低筒滤波器，主要用于温度的滤波 */
static uint16_t Ltc6811_DataFilter(uint16_t tCurrentValue, uint16_t tHistoryValue, uint16_t tFilterFactor)
{
    uint16_t retValue;

    if ((0xFFFF != tHistoryValue) && (tCurrentValue != 0xFFFF))
    {
        if (tFilterFactor < BCC_FILTER_FACTOR_MAX)
        {
            retValue = (uint16_t)((((uint32_t)tHistoryValue * tFilterFactor) + ((uint32_t)tCurrentValue *
             (BCC_FILTER_FACTOR_MAX - tFilterFactor))) / BCC_FILTER_FACTOR_MAX);
        }
        else
        {
            retValue = tCurrentValue;
        }
    }
    else
    {
        retValue = tCurrentValue;
    }

    return retValue;
}
#endif


#if (1 == BCC_TEMP_CHANNEL_HANDLE)
/* 温度处理，主要是将从电压转换为温度 */
static void bcc_tempHandle(uint8_t tIcIndex, uint8_t tTempChanIndex, uint16_t tVolt)
{
	uint32_t tRes = 0;
	uint16_t tTemp;

	if (tVolt > BCC_TEMP_SAMPLE_UP_LIMIT)
	{
		if (bcc_tempSampleFilter[tIcIndex] < 10)
		{
			bcc_tempSampleFilter[tIcIndex]++;
		}

		if (10 == bcc_tempSampleFilter[tIcIndex])
		{
			bcc_tempOpenLoad[tIcIndex] |= 0x01 << tTempChanIndex;
		}
	}
	else if (tVolt < BCC_TEMP_SAMPLE_DOWN_LIMIT)
	{
		if (bcc_tempSampleFilter[tIcIndex] < 10)
		{
			bcc_tempSampleFilter[tIcIndex]++;
		}

		if (10 == bcc_tempSampleFilter[tIcIndex])
		{
			bcc_tempShortToGnd[tIcIndex] |= 0x01 << tTempChanIndex;
		}
	}
	else
	{
		bcc_tempSampleFilter[tIcIndex] = 0;
		tRes = (uint32_t)tVolt * BCC_TEMP_SAMPLE_PULL_UP_RES / (BCC_TEMP_SAMPLE_SUPPLY_VOLT - tVolt);
		tTemp = bcc_getTempFromRes(tRes, Ltc6811_CellTempRtTab, BCC_CELL_TEMP_TAB_LEN, 1);

		/* 温度通道 */
		if (6 == tTempChanIndex)
		{
			tTemp = bcc_getTempFromRes(tRes, Ltc6811_BalTempRtTab, BCC_BOARD_TEMP_RT_NUM, 5);
			bcc_boardTemp[tIcIndex][0] = Ltc6811_DataFilter(tTemp,
										bcc_boardTemp[tIcIndex][0], 8);
			//bcc_cellTemp[tIcIndex][6] = tTemp;
			bcc_cellTemp[tIcIndex][6] = bcc_boardTemp[tIcIndex][0];
		}
		else
		{
			tTemp = bcc_getTempFromRes(tRes, Ltc6811_CellTempRtTab, BCC_CELL_TEMP_TAB_LEN, 1);
			//bcc_cellTemp[tIcIndex][tTempChanIndex] = tVolt;
			bcc_cellTemp[tIcIndex][tTempChanIndex] = Ltc6811_DataFilter(tTemp,
					bcc_cellTemp[tIcIndex][tTempChanIndex], 8);
		}
	}
}
#endif



static uint16_t bcc_abs_uint16(uint16_t t_factorA, uint16_t t_factorB)
{
	uint16_t tRetVal;
	if (t_factorA > t_factorB)
	{
		tRetVal = t_factorA - t_factorB;
	}
	else
	{
		tRetVal = t_factorB - t_factorA;
	}
	return tRetVal;
}
static uint16_t bcc_voltFilter(uint8_t tIcIndex, uint8_t tVoltIndex, uint16_t tLastVolt, uint16_t tCurrentVolt)
{
	uint16_t tRetVal;
	if (0xFFFE != bcc_cellVolt[tIcIndex][tVoltIndex])
	{
		if (bcc_abs_uint16(tLastVolt, tCurrentVolt) > BCC_VOLT_FILTER_VOLT_MAX)
		{
			if (bcc_voltFilterCnt[tIcIndex][tVoltIndex] < BCC_VOLT_FILTER_COUNTER_MAX)
			{
				bcc_voltFilterCnt[tIcIndex][tVoltIndex]++;
				tRetVal = tLastVolt;
			}
			else
			{
				bcc_voltFilterCnt[tIcIndex][tVoltIndex] = 0;
				tRetVal = tCurrentVolt;
			}
		}
		else
		{
			bcc_voltFilterCnt[tIcIndex][tVoltIndex] = 0;
			tRetVal = tCurrentVolt;
		}
	}
	else
	{
		tRetVal = tCurrentVolt;
	}

	return tRetVal;
}





static uint16_t bcc_tempDeltaLimitFilter(uint8_t tIcIndex, uint8_t tTempIndex, uint16_t tLastVolt, uint16_t tCurrentVolt)
{
	uint16_t tRetVal;

	if (0xFFFE != bcc_cellTemp[tIcIndex][tTempIndex])
	{
		if (bcc_abs_uint16(tLastVolt, tCurrentVolt) > BCC_TEMP_DELTA_MAX)
		{
			if (bcc_tempLimitFilterCnt[tIcIndex][tTempIndex] < BCC_TEMP_LIMIT_FILTER_COUNTER_MAX)
			{
				bcc_tempLimitFilterCnt[tIcIndex][tTempIndex]++;
				tRetVal = tLastVolt;
			}
			else
			{
				bcc_tempLimitFilterCnt[tIcIndex][tTempIndex] = 0;
				tRetVal = tCurrentVolt;
			}
		}
		else
		{
			bcc_tempLimitFilterCnt[tIcIndex][tTempIndex] = 0;
			tRetVal = tCurrentVolt;
		}
	}
	else
	{
		tRetVal = tCurrentVolt;
	}

	return tRetVal;
}






static uint16_t bcc_StackVoltLimitFilter(uint8_t tIcIndex, uint16_t tLastVolt, uint16_t tCurrentVolt)
{
	uint16_t tRetVal;

	if (0xFFFFFFFE != bcc_StackVltg[tIcIndex])
	{
		if (bcc_abs_uint16(tLastVolt, tCurrentVolt) > BCC_STACK_VOLT_DELTA_LIMIT_MAX)
		{
			if (bcc_stackVoltDeltaLimitFilterCnt[tIcIndex] < BCC_STACK_VOLT_FILTER_COUNTER_MAX)
			{
				bcc_stackVoltDeltaLimitFilterCnt[tIcIndex]++;
				tRetVal = tLastVolt;
			}
			else
			{
				bcc_stackVoltDeltaLimitFilterCnt[tIcIndex] = 0;
				tRetVal = tCurrentVolt;
			}
		}
		else
		{
			bcc_stackVoltDeltaLimitFilterCnt[tIcIndex] = 0;
			tRetVal = tCurrentVolt;
		}
	}
	else
	{
		tRetVal = tCurrentVolt;
	}


	return tRetVal;
}






static uint16_t bcc_innerIcTempDeltaLimitFilter(uint8_t tIcIndex, uint16_t tLastTemp, uint16_t tCurrentTemp)
{
	uint16_t tRetVal;


	if (0xFFFE != bcc_ChipTemp[tIcIndex])
	{
		if (bcc_abs_uint16(tLastTemp, tCurrentTemp) > BCC_INNER_IC_TEMP_DELTA_LIMIT_MAX)
		{
			if (bcc_innerTempDeltaLimitFilterCnt[tIcIndex] < BCC_INNER_IC_TEMP_FILTER_COUNTER_MAX)
			{
				bcc_innerTempDeltaLimitFilterCnt[tIcIndex]++;
				tRetVal = tLastTemp;
			}
			else
			{
				bcc_innerTempDeltaLimitFilterCnt[tIcIndex] = 0;
				tRetVal = tCurrentTemp;
			}
		}
		else
		{
			bcc_innerTempDeltaLimitFilterCnt[tIcIndex] = 0;
			tRetVal = tCurrentTemp;
		}
	}
	else
	{
		tRetVal = tCurrentTemp;
	}


	return tRetVal;
}




/*
 * 配置需要在初始化阶段需要配置的寄存器
 *
 *返回值：配置成功=1；失败=0
 *
 * */
static uint8_t bcc_initReg(uint8_t tIcNum)
{
	uint16_t tRegVal = 0;
	uint8_t tRetState = 0;
	uint8_t tCheckCnt = 0;

	bcc_startDmaTransmit(6);

	//bcc_delay27us();
	tRegVal = 0x0001 << BCC_RW_CB_DRVEN_SHIFT;
	bcc_33771GlobalWrite(tRegVal, BCC_REG_SYS_CFG1_ADDR);
//	bcc_delay27us();


	if (1 == bcc_recvLengthJudge())
	{
		tCheckCnt++;

		bcc_checkEcho();
	}


	if (1 == tCheckCnt)
	{
		tRetState = 1;
	}

	return tRetState;
}

/* Reset (soft reset) BCC to assign CID (in case of CID was already assigned). */
static void bcc_resetAll33771(void)
{
	bcc_33771GlobalWrite(BCC_W_SOFT_RST_MASK, BCC_REG_SYS_CFG1_ADDR);
}


/* 发送ADC启动转换的指令，并赋予ADC_TAG数值，读取时可以用于本次转换的判断 */
static uint8_t bcc_startConversion(void)
{
	uint16_t regVal;     /* Value of ADC_CFG register. */
	uint8_t t_tagId;
	uint8_t tRetSta = 0;


	regVal = BCC_CONF1_ADC_CFG_VALUE;

	bcc_conversionTagId = 0;	/* running in compatibility mode */
	t_tagId = bcc_conversionTagId & 0x0F;
	regVal = BCC_SET_TAG_ID(regVal, t_tagId);
	regVal = BCC_REG_SET_BIT_VALUE(regVal, BCC_W_SOC_MASK);

	bcc_startDmaTransmit(6);

	bcc_33771GlobalWrite(regVal, BCC_REG_ADC_CFG_ADDR);

	if (1 == bcc_checkEcho())
	{
		tRetSta = 1;
	}

	return tRetSta;
}





/* 给每一个IC发送均衡指令 */
static void bcc_sendBalCmd(void)
{
	uint8_t tChannIndex;
	uint16_t tRegVal;
	uint8_t tRegAddr = 0;


	if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
	{
		bcc_balSendCmdIndex++;
		if (bcc_balSendCmdIndex >= bcc_icNum)
		{
			bcc_balSendCmdIndex = 0;
		}
	}
	else
	{
		if (0 == bcc_balSendCmdIndex)
		{
			bcc_balSendCmdIndex = bcc_icNum - 1;
		}
		else
		{
			bcc_balSendCmdIndex--;
		}
	}

	//for (tIcIndex = 0; tIcIndex < bcc_icNum; tIcIndex++)
	{
		for (tChannIndex = 0; tChannIndex < BCC_VOLT_NUM_MAX; tChannIndex++)
		{
		//	bcc_delay27us();
		//	bcc_CellBalSta[bcc_balSendCmdIndex][tChannIndex] = bcc_CellBalCmd[bcc_balSendCmdIndex][tChannIndex];

			if (bcc_CellBalCmd[bcc_balSendCmdIndex][tChannIndex] != bcc_CellBalSta[bcc_balSendCmdIndex][tChannIndex])
			{
				tRegAddr = 0x0C + tChannIndex;
				if (0x00 == bcc_CellBalCmd[bcc_balSendCmdIndex][tChannIndex])
				{
					tRegVal = 0x0000;
					bcc_33771LocalWrite(tRegVal, tRegAddr, (bcc_balSendCmdIndex+1), BCC_CMD_WRITE);
				}
				else if (0x01 == bcc_CellBalCmd[bcc_balSendCmdIndex][tChannIndex])
				{
					tRegVal = 0x0200;
					bcc_33771LocalWrite(tRegVal, tRegAddr, (bcc_balSendCmdIndex+1), BCC_CMD_WRITE);
				}
				else
				{
					/* invalid control cmd, do nothing */
				}
			}
		}
	}
}



/* 初始化数组 */
static void bcc_initArrayData(void)
{
	uint8_t tIcIndex;
	uint8_t tChannelIndex;

	for (tIcIndex = 0; tIcIndex < BCC_IC_NUM_MAX; tIcIndex++)
	{
		bcc_stackVoltDeltaLimitFilterCnt[tIcIndex] = 0;
		bcc_innerTempDeltaLimitFilterCnt[tIcIndex] = 0;

		for (tChannelIndex = 0; tChannelIndex < BCC_VOLT_NUM_MAX; tChannelIndex++)
		{
			//bcc_cellVolt[tIcIndex][tChannelIndex] = 0xFFFF;
			bcc_voltFilterCnt[tIcIndex][tChannelIndex] = 0;
		}

		for (tChannelIndex = 0; tChannelIndex < BCC_TEMP_NUM_MAX; tChannelIndex++)
		{
			//bcc_cellTemp[tIcIndex][tChannelIndex] = 0xFFFF;
			bcc_tempLimitFilterCnt[tIcIndex][tChannelIndex] = 0;
		}
	}
}



#if 0

#define BCC_GPIO_SHORT_VOLT_LIMIT 30
#define BCC_GPIO_OPEN_VOLT_LIMIT 4970

/* 5*500/(500+10000)= 0.2381*/
/* 5*300000/(300000+10000)= 4.8387*/
/* 5*4000000/(4000000+100000)= 4.8780 */
/* 5* 3000/(3000+100000)= 0.1456*/


static void bcc_cellTempDiag(uint8_t tIcIndex, uint8_t tChannIndex, uint16_t tVolt)
{
#if 0

	if ((tIcIndex < BCC_IC_NUM_MAX) && (tChannIndex < BCC_TEMP_NUM_MAX))
	{
		if (tVolt<BCC_GPIO_SHORT_VOLT_LIMIT)
		{
			Bcc_GPIO_Short[tIcIndex][tChannIndex] = 1;
		}
		else
		{
			Bcc_GPIO_Short[tIcIndex][tChannIndex] = 0;
		}


		if (tVolt > BCC_GPIO_OPEN_VOLT_LIMIT)
		{
			Bcc_GPIO_Open[tIcIndex][tChannIndex] = 1;
		}
		else
		{
			Bcc_GPIO_Open[tIcIndex][tChannIndex] = 0;
		}
	}
	else
	{
		/* do nothing */
	}
#endif
}

#endif




uint8_t bcc_crcErrCounter[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
static void bcc_frameCrcDiag(uint8_t tIcIndex, uint8_t tCrcState)
{
	if (BCC_STATE_NORMAL == bcc_stateMachine)
	{
		if (0 == tCrcState)
		{
			bcc_crcErrCounter[tIcIndex]++;

			if (bcc_crcErrCounter[tIcIndex] >= 8)
			{
				bcc_crcErrCounter[tIcIndex] = 8;

				Bcc_Comm_CRCErr[tIcIndex] = 1;
			}
		}
		else
		{
			Bcc_Comm_CRCErr[tIcIndex] = 0;
		}
	}
}


uint8_t bcc_addrErrCounter[BCC_IC_NUM_MAX] = {0,0,0,0,0,0,0,0,0};
static void bcc_addrErrDiag(uint8_t tIcIndex, uint8_t tWriteAddr,uint8_t tReadAddr)
{
	if (tWriteAddr != tReadAddr)
	{
		bcc_addrErrCounter[tIcIndex]++;
		if (bcc_addrErrCounter[tIcIndex] >= 8)
		{
			bcc_addrErrCounter[tIcIndex] = 8;
			Bcc_Comm_AddrErr[tIcIndex] = 1;
		}
	}
	else
	{
		if (bcc_addrErrCounter[tIcIndex] > 0)
		{
			bcc_addrErrCounter[tIcIndex]--;
		}

		if (bcc_addrErrCounter[tIcIndex] < 5)
		{
			Bcc_Comm_AddrErr[tIcIndex] = 0;
		}
	}
}




static uint8_t bcc_pauseBal(void)
{
	uint8_t tReadState = 0;
	SYS_CFG1_tag tCfg1Reg;

	tCfg1Reg.B.CB_AUTO_PAUSE = 0;
	tCfg1Reg.B.CB_DRVEN = 1;
	tCfg1Reg.B.CB_MANUAL_PAUSE = 1;
	tCfg1Reg.B.CYCLIC_TIMER = 0;
	tCfg1Reg.B.DIAG_TIMEOUT = 4;
	tCfg1Reg.B.FAULTWAVE = 0;
	tCfg1Reg.B.GO2DIAG_DIAG_ST = 0;
	tCfg1Reg.B.I_MEAS_EN = 0;
	tCfg1Reg.B.OSCMON = 1;
	tCfg1Reg.B.SOFT_RST_0 = 0;
	tCfg1Reg.B.WAVE_DC_BIT0 = 0;
	tCfg1Reg.B.WAVE_DC_BIT1 = 0;

	bcc_startDmaTransmit(6);

	bcc_33771GlobalWrite((uint16_t)tCfg1Reg.R, BCC_REG_SYS_CFG1_ADDR);

	if (1 == bcc_checkEcho())
	{
		tReadState = 1;
	}

#if 0
	bcc_delay27us();
	if (1 == bcc_compareDataInGlobalWrite(BCC_REG_SYS_CFG1_ADDR, (uint16_t)tCfg1Reg.R, bcc_icNum))
	{
		tReadState = 1;
	}
#endif

	return tReadState;
}

static void bcc_getOneIcData(void);

static void bcc_balCmdRunning(void)
{
	SYS_CFG1_tag tCfg1Reg;

	tCfg1Reg.B.CB_AUTO_PAUSE = 0;
	tCfg1Reg.B.CB_DRVEN = 1;
	tCfg1Reg.B.CB_MANUAL_PAUSE = 0;
	tCfg1Reg.B.CYCLIC_TIMER = 0;
	tCfg1Reg.B.DIAG_TIMEOUT = 4;
	tCfg1Reg.B.FAULTWAVE = 0;
	tCfg1Reg.B.GO2DIAG_DIAG_ST = 0;
	tCfg1Reg.B.I_MEAS_EN = 0;
	tCfg1Reg.B.OSCMON = 1;
	tCfg1Reg.B.SOFT_RST_0 = 0;
	tCfg1Reg.B.WAVE_DC_BIT0 = 0;
	tCfg1Reg.B.WAVE_DC_BIT1 = 0;


	bcc_startDmaTransmit(6);

	bcc_33771GlobalWrite((uint16_t)tCfg1Reg.R, BCC_REG_SYS_CFG1_ADDR);

	if (1 == bcc_checkEcho())
	{
		bcc_sendBalCmd();
	}

	bcc_startDmaTransmit(12);

	(void)bcc_33771LocalWrite(1, BCC_REG_CB_DRV_STS_ADDR, (bcc_balSendCmdIndex+1), BCC_CMD_READ);
}


uint8_t bcc_currentReadDataIc = 1;

static void bcc_getOneIcData(void)
{
	bcc_startDmaTransmit(6 + 6*(0x48-0x23));

	(void)bcc_33771LocalWrite(0x25, 0x24, (bcc_currentReadDataIc), BCC_CMD_READ);
}


static void bcc_getCellVolt(void)
{
	uint8_t tRegIndex = 0;
	uint16_t tVolt = 0;

	for (tRegIndex = 0; tRegIndex < 14; tRegIndex++)
	{
		if (1 == bcc_extractOneDataFromDmaBuff((3 * (0x40 - 0x23 - tRegIndex)), BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_currentReadDataIc-1), (0x40-tRegIndex)))
		{
			if (0x8000 == (bcc_readResData.mc33771_regData & 0x8000))
			{
				bcc_readResData.mc33771_regData &= 0x7FFF;

				tVolt = (uint32_t)bcc_readResData.mc33771_regData * 15259 / 100000;
				bcc_cellVoltBuff[bcc_currentReadDataIc-1][tRegIndex] = bcc_voltFilter((bcc_currentReadDataIc-1), (tRegIndex),
						bcc_cellVoltBuff[bcc_currentReadDataIc-1][tRegIndex], tVolt);

			//	bcc_cellVolt[bcc_currentReadDataIc-1][tRegIndex] = bcc_cellVoltBuff[bcc_currentReadDataIc-1][tRegIndex];
			}
		}
	}
}


static void bcc_updateBalSta(void)
{
	uint8_t tChannelIndex;

	if (1 == bcc_extractOneDataFromDmaBuff(3, BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_currentReadDataIc-1), (BCC_REG_CB_DRV_STS_ADDR)))
	{
		for (tChannelIndex = 0; tChannelIndex < BCC_VOLT_NUM_MAX; tChannelIndex++)
		{
			if (0x00 != ((bcc_readResData.mc33771_regData) & (0x00001 << tChannelIndex)))
			{
				bcc_CellBalSta[bcc_balSendCmdIndex][tChannelIndex] = 1;
			}
			else
			{
				bcc_CellBalSta[bcc_balSendCmdIndex][tChannelIndex] = 0;
			}
		}
	}
}




static void bcc_getTemp(void)
{
	uint8_t tRegIndex = 0;
	uint16_t tVolt = 0;

	for (tRegIndex = 0; tRegIndex < 7; tRegIndex++)
	{
		if (1 == bcc_extractOneDataFromDmaBuff((3 * (0x47 - 0x23 - tRegIndex)), BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_currentReadDataIc-1), (0x47-tRegIndex)))
		{
			if (0x8000 == (bcc_readResData.mc33771_regData & 0x8000))
			{
				bcc_readResData.mc33771_regData &= 0x7FFF;

				tVolt = (uint32_t)bcc_readResData.mc33771_regData * 15259
						/ 100000;

				tVolt = bcc_tempDeltaLimitFilter((bcc_currentReadDataIc-1), (tRegIndex), bcc_cellTempBuff[bcc_currentReadDataIc-1][tRegIndex], tVolt);
				bcc_cellTempBuff[bcc_currentReadDataIc-1][tRegIndex] = tVolt;
				//bcc_tempHandle(tIc, (6 - tRegAddrCnt), tVolt);
			}
		}
	}
}


static void bcc_getStackVolt(void)
{
	uint16_t tVolt = 0;

	if (1 == bcc_extractOneDataFromDmaBuff((3 * (0x32 - 0x23)), BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_currentReadDataIc-1), (0x32)))
	{
		if (0x8000 == (bcc_readResData.mc33771_regData & 0x8000))
		{
			bcc_readResData.mc33771_regData &= 0x7FFF;

			tVolt = (uint32_t)bcc_readResData.mc33771_regData * 244 / 100;

			tVolt = bcc_StackVoltLimitFilter((bcc_currentReadDataIc-1), bcc_StackVltgBuff[bcc_currentReadDataIc-1], tVolt);

			bcc_StackVltgBuff[bcc_currentReadDataIc-1] = tVolt;
		}
	}
}

static void bcc_getIcTemp(void)
{
	uint16_t tTemp = 0;

	if (1 == bcc_extractOneDataFromDmaBuff((3 * (0x48 - 0x23)), BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_currentReadDataIc-1), (0x48)))
	{
		if (0x8000 == (bcc_readResData.mc33771_regData & 0x8000))
		{
			bcc_readResData.mc33771_regData &= 0x7FFF;

			tTemp = (uint32_t)bcc_readResData.mc33771_regData * 32 / 1000;
			if (tTemp < 223)
			{
				tTemp = 0;
			}
			else
			{
				tTemp = (tTemp - 223) * 10;
			}

			//tTemp = (uint32_t)bcc_readResData.mc33771_regData * 32 / 1000 - 273;

			tTemp = bcc_innerIcTempDeltaLimitFilter((bcc_currentReadDataIc-1), bcc_ChipTemp[(bcc_currentReadDataIc-1)], tTemp);

			bcc_ChipTemp[bcc_currentReadDataIc-1] = tTemp;//(tTemp + 0) * 10;
		}
		else
		{
			/* keep value */
		}
	}
}


static void bcc_getFault1_2_3Data(void);
static void bcc_HandleFaultData(void);

static uint8_t bcc_handleOneIcData(void)
{
	uint8_t tRetSta = 0;

	if (1 == bcc_checkEcho())
	{
		bcc_getCellVolt();

		bcc_getTemp();

		bcc_getStackVolt();

		bcc_getIcTemp();

		bcc_getFault1_2_3Data();

		bcc_HandleFaultData();
	}

	bcc_currentReadDataIc++;

	if (bcc_currentReadDataIc > BCC_INIT_IC_NUM)
	{
		tRetSta = 1;

		bcc_currentReadDataIc = 1;
	}

	bcc_getOneIcData();

	return tRetSta;
}



static void bcc_getFault1_2_3Data(void)
{
	if (1 == bcc_extractOneDataFromDmaBuff((3 * (0x24 - 0x23)), BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_currentReadDataIc-1), (0x24)))
	{
		bcc_fault1Status[bcc_currentReadDataIc-1] = bcc_readResData.mc33771_regData;
	}

	if (1 == bcc_extractOneDataFromDmaBuff((3 * (0x25 - 0x23)), BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_currentReadDataIc-1), (0x25)))
	{
		bcc_fault2Status[bcc_currentReadDataIc-1] = bcc_readResData.mc33771_regData;
	}

	if (1 == bcc_extractOneDataFromDmaBuff((3 * (0x26 - 0x23)), BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_currentReadDataIc-1), (0x26)))
	{
		bcc_fault3Status[bcc_currentReadDataIc-1] = bcc_readResData.mc33771_regData;
	}
}

static void bcc_HandleFaultData(void)
{
	bcc_VPWRDiag();
	bcc_VCOMDiag();
	bcc_VANADiag();
	bcc_oscErrDiag();
	bcc_adcErrDiag();
	bcc_gndLossDiag();
	bcc_icTempErrAndEixtDiagTimeOutDiag();
}





uint16_t bcc_diagTimeTick = 0;
#define BCC_DIAG_PERIOD_TICK_MAX 200


static uint8_t bcc_getSampleData(void)
{
	uint8_t tRetState = 0;

	if (1 == bcc_handleOneIcData())
	{
		tRetState = 1;
	}

	return tRetState;

#if 0

	uint8_t tRetState = 0;

	if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
	{
		bcc_OneIcVoltData(bcc_currentReadDataIc);
		bcc_delay640us();
		bcc_OneIcTempData(bcc_currentReadDataIc);
		bcc_delay27us();
		bcc_getOneIcStackVolt(bcc_currentReadDataIc);
		bcc_delay27us();
		bcc_getOneIcInnerTemp(bcc_currentReadDataIc);
		bcc_delay27us();
	}
	else
	{
		bcc_OneIcVoltData(BCC_INIT_IC_NUM - 1 - bcc_currentReadDataIc);
		bcc_OneIcTempData(BCC_INIT_IC_NUM - 1 - bcc_currentReadDataIc);
		bcc_getOneIcStackVolt(BCC_INIT_IC_NUM - 1 - bcc_currentReadDataIc);
		bcc_getOneIcInnerTemp(BCC_INIT_IC_NUM - 1 - bcc_currentReadDataIc);
	}

	bcc_currentReadDataIc++;
	if ((bcc_currentReadDataIc >= bcc_icNum) || (bcc_currentReadDataIc >= BCC_NUM_CONFIG))
	{
		tRetState = 1;
		bcc_currentReadDataIc = 0;
	}

	return tRetState;

#endif
}








typedef struct
{
	uint8_t bcc_commDiagTick;
	uint8_t bcc_commMaxIc;
}bcc_linkerDisConnCtrl_t;

#define BCC_COMM_DIAG_INCREASE_STEP 10
#define BCC_COMM_DIAG_DECREASE_STEP 1
#define BCC_COMM_DIAG_TICK_MAX 100

bcc_linkerDisConnCtrl_t bcc_linkerDisConnCtrl;

uint8_t bcc_commErrIcIndex = 0xFF;

static void bcc_linkerDisConnDiag(uint8_t tIcIndex)
{
	if (BCC_COMM_FIRST_33664 == bcc_initMachineCtrl.bcc_initDir)
	{
		if (bcc_linkerDisConnCtrl.bcc_commMaxIc < tIcIndex)
		{
			bcc_linkerDisConnCtrl.bcc_commMaxIc = tIcIndex;
			bcc_linkerDisConnCtrl.bcc_commDiagTick = 0;
		}
		else if (bcc_linkerDisConnCtrl.bcc_commMaxIc == tIcIndex)
		{
			if (bcc_linkerDisConnCtrl.bcc_commDiagTick < BCC_COMM_DIAG_TICK_MAX)
			{
				bcc_linkerDisConnCtrl.bcc_commDiagTick += BCC_COMM_DIAG_INCREASE_STEP;
			}
			else
			{
				bcc_commErrIcIndex = tIcIndex;
				bcc_linkerDisConnCtrl.bcc_commDiagTick = BCC_COMM_DIAG_TICK_MAX;
			}
		}
		else
		{
			if (bcc_linkerDisConnCtrl.bcc_commDiagTick > 0)
			{
				bcc_linkerDisConnCtrl.bcc_commDiagTick--;
			}
			else
			{
				bcc_linkerDisConnCtrl.bcc_commMaxIc = tIcIndex;
			}
		}
	}
	else
	{
		/* do nothing */
	}
}




static uint8_t bcc_initMachineManagement(void)
{
	uint8_t tRetState = 0;

	if (BCC_INIT_CID == bcc_initMachineCtrl.bcc_initStep)
	{
		bcc_icNum = bcc_initDeviceIDWithDir(BCC_INIT_IC_NUM, bcc_initMachineCtrl.bcc_initDir);
		if (BCC_INIT_IC_NUM == bcc_icNum)
		{
			bcc_initMachineCtrl.bcc_initCnt = 0;

			bcc_initMachineCtrl.bcc_initStep = BCC_INIT_REG;
		}
		else
		{
			bcc_initMachineCtrl.bcc_initCnt++;
			if (bcc_initMachineCtrl.bcc_initCnt > BCC_INIT_CNT_MAX)
			{
				bcc_initMachineCtrl.bcc_initCnt = 0;
				if (bcc_icNum > 0)
				{
					bcc_linkerDisConnDiag(bcc_icNum);
					bcc_initMachineCtrl.bcc_initStep = BCC_INIT_REG;
				}
				else
				{
					bcc_initMachineCtrl.bcc_initStep = BCC_INIT_CID;
				}
			}
			else
			{
				/* wait for next init loop */

			}
		}
	}

	if (BCC_INIT_REG == bcc_initMachineCtrl.bcc_initStep)
	{
		if (1 == bcc_initReg(bcc_icNum))
		{
			bcc_initMachineCtrl.bcc_initCnt = 0;
			bcc_initMachineCtrl.bcc_initStep = BCC_INIT_CID;

			tRetState = 1;
		}
		else
		{
			bcc_initMachineCtrl.bcc_initCnt++;
			if (bcc_initMachineCtrl.bcc_initCnt > BCC_INIT_CNT_MAX)
			{
				bcc_initMachineCtrl.bcc_initCnt = 0;
				bcc_initMachineCtrl.bcc_initStep = BCC_INIT_CID;
			}
		}
	}
	else
	{

	}
	return tRetState;
}

#if 0
static void bcc_goToSleep(void)
{
	bcc_33771GlobalWrite(BCC_GO2SLEEP_ENABLED, BCC_REG_SYS_CFG_GLOBAL_ADDR);
}
#endif

static uint8_t bcc_initDeviceIDWithDir(uint8_t tIDNum, bcc_communicationDir_t tDir)
{
	uint8_t tReadDeviceId;
	uint8_t tInitIcCnt = 0;
	uint8_t tCurrentId = 0;
	uint8_t tInitNum = 0;
	uint8_t tInitTimeOutCnt;
	(void)bcc_wakeUp33664(tDir);

	for (tInitTimeOutCnt = 0; tInitTimeOutCnt < BCC_INIT_ICD_TIMEOUT_MAX; tInitTimeOutCnt++)
	{
		tInitNum = 0;
		bcc_wakeUp33771(tDir);
		bcc_resetAll33771();
		bcc_delay5ms();
		for (tInitIcCnt = 0; tInitIcCnt < tIDNum; tInitIcCnt++)
		{
			if (BCC_COMM_FIRST_33664 == tDir)
			{
				tCurrentId = tInitIcCnt + 1;
			}
			else
			{
				tCurrentId = tIDNum - tInitIcCnt;
			}

//			tCurrentId = tInitIcCnt + 1;

			bcc_wakeUp33771(tDir);

			bcc_deviceClusterIdAssign(tCurrentId);
			bcc_delay27us();
			tReadDeviceId = bcc_readDeviceId(tCurrentId);
			bcc_delay27us();

			if (tReadDeviceId != tCurrentId)
			{
				break;
			}
			else
			{
				tInitNum++;
			}
		}
		if (tInitNum == tIDNum)
		{
			break;
		}
	}
	return tInitNum;
}



static void bcc_updateSynSampleData(void)
{
	uint8_t tIcIndex;
	uint8_t tChannelIndex;

	for (tIcIndex = 0; tIcIndex < BCC_NUM_CONFIG; tIcIndex++)
	{
		for (tChannelIndex = 0; tChannelIndex < BCC_VOLT_NUM_MAX; tChannelIndex++)
		{
			bcc_cellVolt[tIcIndex][tChannelIndex] = bcc_cellVoltBuff[tIcIndex][tChannelIndex];
		}

		for (tChannelIndex = 0; tChannelIndex < BCC_TEMP_NUM_MAX; tChannelIndex++)
		{
			bcc_cellTemp[tIcIndex][tChannelIndex] = bcc_cellTempBuff[tIcIndex][tChannelIndex];
		}

		bcc_StackVltg[tIcIndex] = bcc_StackVltgBuff[tIcIndex];
	}
}


static uint8_t bcc_stateMachine_normal(void)
{
	uint8_t tRetState = 0;

	switch (bcc_normalStateStep)
	{
		case BCC_NORMAL_CLOSE_BAL:
			if (1 == bcc_pauseBal())
			{
				bcc_normalStateStep = BCC_NORMAL_START_CONV;
			}
			break;
		case BCC_NORMAL_START_CONV:
			bcc_startConversion();
			bcc_normalStateStep = BCC_NORMAL_START_BAL;
			break;
		case BCC_NORMAL_START_BAL:
			(void)bcc_balCmdRunning();
			bcc_normalStateStep = BCC_NORMAL_GET_BAL_STA;
			break;
		case BCC_NORMAL_GET_BAL_STA:
			bcc_updateBalSta();
			bcc_getOneIcData();
			bcc_normalStateStep = BCC_NORMAL_GET_DATA;
			break;
		case BCC_NORMAL_GET_DATA:
			if (1 == bcc_recvLengthJudge())
			{
				if (1 == bcc_getSampleData())
				{
					tRetState = 1;
					bcc_updateSynSampleData();
					bcc_normalStateStep = BCC_NORMAL_CLOSE_BAL;
				}
			}
			break;
		default:
			break;
	}

	return tRetState;
}


void bcc_task_init(void)
{
	bcc_gpioInit();
	bcc_spiInit();
	bccSpiDma_Init();
	
	bcc_initMachineCtrl.bcc_initDir = BCC_INIT_DIR_CONFIG;
	bcc_initMachineCtrl.bcc_initCnt = 0;
	bcc_initMachineCtrl.bcc_initStep = BCC_INIT_CID;
	
	
	bcc_startDmaTransmit(12);

	bcc_initArrayData();

	

	bcc_commErrIcIndex = 0xFF;

	bcc_linkerDisConnCtrl.bcc_commDiagTick = 0;
	bcc_linkerDisConnCtrl.bcc_commMaxIc = 0;

	if (1 == bcc_initMachineManagement())
	{
		bcc_stateMachine = BCC_STATE_NORMAL;
	}
}



void bcc_stateMachine_DIAG_CTx_OPEN(void)
{

}


void bcc_stateMachine_DIAG_BALx_OPEN(void)
{

}


#if 0

extern void test_33771Rev(void);

void bcc_task_5ms(void)
{
	//test_3377122reg();

//	test_33771Rev();

	bcc_stateMachine_normal();

#if 0

	if (BCC_STATE_NORMAL == bcc_stateMachine)
	{

		if (1 == bcc_stateMachine_normal())
		{
			bcc_FstSampleFinsh = 1;

			bcc_normalStateCtrl.bcc_normalTimeTick = 0;

			if (BCC_INIT_IC_NUM > bcc_icNum)
			{
			//	bcc_changeInitCommDir();
				bcc_setStateMachine(BCC_STATE_INIT);
			}
		}
		else
		{
			bcc_normalStateCtrl.bcc_normalTimeTick++;

			if (bcc_normalStateCtrl.bcc_normalTimeTick > BCC_NORMAL_TASK_TIMEOUT_MAX)
			{
				bcc_normalStateCtrl.bcc_normalTimeTick = 0;

			//	bcc_changeInitCommDir();
				bcc_setStateMachine(BCC_STATE_INIT);
			}
		}
		if (1 == bcc_reset33771Flg)
		{
			bcc_normalStateCtrl.bcc_normalTimeTick = 0;
			//bcc_stateMachine = BCC_STATE_INIT;
			bcc_setStateMachine(BCC_STATE_INIT);
		}
	}

	bcc_diagModeManagement();

#endif

}

#endif

#if 0
extern void test_3377122reg(void);
void bcc_task_50ms(void)
{
	test_3377122reg();

#if 0

	if (BCC_STATE_NORMAL == bcc_stateMachine)
	{
		//bcc_updateBalStatus();
	}

	if (BCC_STATE_INIT == bcc_stateMachine)
	{
		if (1 == bcc_initMachineManagement())
		{
			bcc_reset33771Flg = 0;
			//bcc_stateMachine = BCC_STATE_NORMAL;
			bcc_setStateMachine(BCC_STATE_NORMAL);
		}
	}
#endif
}

#endif

static void bcc_VPWRDiag(void)
{
	uint8_t tIcIndex = 0;

	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		if (0 != (bcc_fault1Status[tIcIndex] & BCC_RW_VPWR_OV_FLT_MASK))
		{
			Bcc_VPWR_OV[tIcIndex] = 1;
		}
		else
		{
			Bcc_VPWR_OV[tIcIndex] = 0;
		}

		if (0 != (bcc_fault1Status[tIcIndex] & BCC_RW_VPWR_LV_FLT_MASK))
		{
			Bcc_VPWR_UV[tIcIndex] = 1;
		}
		else
		{
			Bcc_VPWR_UV[tIcIndex] = 0;
		}
	}
}



static void bcc_VCOMDiag(void)
{
	uint8_t tIcIndex = 0;

	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		if (0 != (bcc_fault2Status[tIcIndex] & BCC_RW_VCOM_OV_FLT_MASK))
		{
			bcc_VCOM_OV[tIcIndex] = 1;
		}
		else
		{
			bcc_VCOM_OV[tIcIndex] = 0;
		}

		if (0 != (bcc_fault2Status[tIcIndex] & BCC_RW_VCOM_UV_FLT_MASK))
		{
			bcc_VCOM_UV[tIcIndex] = 1;
		}
		else
		{
			bcc_VCOM_UV[tIcIndex] = 0;
		}
	}
}



static void bcc_VANADiag(void)
{
	uint8_t tIcIndex = 0;

	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		if (0 != (bcc_fault2Status[tIcIndex] & BCC_RW_VANA_OV_FLT_MASK))
		{
			bcc_VANA_OV[tIcIndex] = 1;
		}
		else
		{
			bcc_VANA_OV[tIcIndex] = 0;
		}

		if (0 != (bcc_fault2Status[tIcIndex] & BCC_RW_VANA_UV_FLT_MASK))
		{
			bcc_VANA_UV[tIcIndex] = 1;
		}
		else
		{
			bcc_VANA_UV[tIcIndex] = 0;
		}
	}
}


static void bcc_oscErrDiag(void)
{
	uint8_t tIcIndex = 0;

	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		if (0 != (bcc_fault2Status[tIcIndex] & BCC_RW_OSC_ERR_FLT_MASK))
		{
			Bcc_OSC_ERR[tIcIndex] = 1;
		}
		else
		{
			Bcc_OSC_ERR[tIcIndex] = 0;
		}
	}
}


static void bcc_gndLossDiag(void)
{
	uint8_t tIcIndex = 0;

	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		if (0 != (bcc_fault2Status[tIcIndex] & BCC_RW_GND_LOSS_FLT_MASK))
		{
			Bcc_GND_LOSS[tIcIndex] = 1;
		}
		else
		{
			Bcc_GND_LOSS[tIcIndex] = 0;
		}
	}
}






#define BCC_ADC1_A_FLT_MASK 0x0400
#define BCC_ADC1_B_FLT_MASK 0x0800
static void bcc_adcErrDiag(void)
{
	uint8_t tDeviceId;

	for (tDeviceId = 0; tDeviceId < BCC_INIT_IC_NUM; tDeviceId++)
	{
		if (((bcc_fault2Status[tDeviceId] & BCC_ADC1_A_FLT_MASK) != 0)
				|| ((bcc_fault2Status[tDeviceId] & BCC_ADC1_B_FLT_MASK) != 0))

		{
			Bcc_ADC_ERR[tDeviceId] = 1;
		}
		else
		{
			Bcc_ADC_ERR[tDeviceId] = 0;
			/* do nothing */
		}
	}
}




static void bcc_icTempErrAndEixtDiagTimeOutDiag(void)
{
	uint8_t tIcIndex;

	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		if (0x00 != (bcc_fault2Status[tIcIndex] & BCC_RW_IC_TSD_FLT_MASK))
		{
			Bcc_Thermal_SHD[tIcIndex] = 1;
		}
		else
		{
			Bcc_Thermal_SHD[tIcIndex] = 0;
		}
	}



	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		if (0x00 != (bcc_fault3Status[tIcIndex] & BCC_R_DIAG_TO_FLT_MASK))
		{
			Bcc_Diag_TimeOut[tIcIndex] = 1;
		}
		else
		{
			Bcc_Diag_TimeOut[tIcIndex] = 0;
		}
	}
}




void bcc_task_1000ms(void)
{
#if 0
	if (BCC_STATE_DIAG == bcc_stateMachine)
	{
		bcc_readFaultStatus();
		bcc_VPWRDiag();
		bcc_VCOMDiag();
		bcc_VANADiag();
		bcc_oscErrDiag();
		bcc_adcErrDiag();
		bcc_gndLossDiag();
		bcc_icTempErrAndEixtDiagTimeOutDiag();
		bcc_CBxShortDiag();

		test_oldTime = pit_readCnt(0);
		bcc_CTxOpenDiag();
	//	bcc_GPIOxOpenDiag();
	//	bcc_diagCBxOpen();

		bcc_stateMachine = BCC_STATE_NORMAL;
		test_nowTime = pit_readCnt(0);
		if(test_oldTime > test_nowTime)
		{
			test_timeDiff = test_oldTime - test_nowTime;
		}
		else
		{
			test_timeDiff = test_oldTime + 500000000 -  test_nowTime;
		}
		test_timeDiff /= 50;
	}
#endif
}



#if 0
static uint16_t bcc_CB_DRV_STS_Reg[BCC_NUM_CONFIG];
static void bcc_clrBalStatus(void)
{
	uint8_t tIcIndex;

	for (tIcIndex = 0; tIcIndex < BCC_NUM_CONFIG; tIcIndex++)
	{
		bcc_CB_DRV_STS_Reg[tIcIndex] = 0x00;
	}
}
#endif




static void bcc_enterDiagModeAndPauseBal(void)
{
	uint16_t tReg;
	tReg = BCC_DIAG_MODE_ENABLED | BCC_DIAG_TIMEOUT_1S | BCC_CB_MAN_PAUSE_ENABLED | Reserved_BIT_1;
	bcc_33771GlobalWrite(tReg, BCC_REG_SYS_CFG1_ADDR);
}



static void bcc_startCellBalAndExitBal(void)
{
	SYS_CFG1_tag tCfg1Reg;

	tCfg1Reg.B.CB_AUTO_PAUSE = 0;
	tCfg1Reg.B.CB_DRVEN = 1;
	tCfg1Reg.B.CB_MANUAL_PAUSE = 0;
	tCfg1Reg.B.CYCLIC_TIMER = 0;
	tCfg1Reg.B.DIAG_TIMEOUT = 4;
	tCfg1Reg.B.FAULTWAVE = 0;
	tCfg1Reg.B.GO2DIAG_DIAG_ST = 0;
	tCfg1Reg.B.I_MEAS_EN = 0;
	tCfg1Reg.B.OSCMON = 1;
	tCfg1Reg.B.SOFT_RST_0 = 0;
	tCfg1Reg.B.WAVE_DC_BIT0 = 0;
	tCfg1Reg.B.WAVE_DC_BIT1 = 0;

	bcc_33771GlobalWrite((uint16_t)tCfg1Reg.R, BCC_REG_SYS_CFG1_ADDR);
}


static void bcc_exitDiagMode(void)
{
	uint16_t tReg;
	tReg = BCC_DIAG_TIMEOUT_1S | BCC_CB_DRV_ENABLED;
	bcc_33771GlobalWrite(tReg, BCC_REG_SYS_CFG1_ADDR);

}


static void bcc_diagCBxOpen_step2(void)
{
	/* do nothing */
}


static void bcc_diagCBxOpen_step3(void)
{
#if 0
	uint16_t tReg = 0x00;

	tReg = 0x1021;
	bcc_33771GlobalWrite(tReg, BCC_REG_SYS_CFG1_ADDR);
#endif
}

static void bcc_diagCBxOpen_step8(void)
{
	uint16_t tReg = 0x00;

	tReg = BCC_DIAG_TIMEOUT_1S | Reserved_BIT_1;
	bcc_33771GlobalWrite(tReg, BCC_REG_SYS_CFG1_ADDR);
}




static void bcc_diagCBxOpen_step4(void)
{
	uint16_t tReg = 0x00;

	tReg = BCC_RW_CB_OL_ODD_MASK;
	bcc_33771GlobalWrite(tReg, BCC_REG_SYS_DIAG_ADDR);
}

static void bcc_diagCBxOpen_step6(void)
{
	uint16_t tReg = 0x00;

	tReg = BCC_RW_CB_OL_EVEN_MASK;
	bcc_33771GlobalWrite(tReg, BCC_REG_SYS_DIAG_ADDR);
}




static void bcc_diagCBxOpen_restoreRegData(void)
{
	uint16_t tReg = 0x00;
	tReg = 0x0000;
	bcc_33771GlobalWrite(tReg, BCC_REG_SYS_DIAG_ADDR);
}

static void bcc_clearReg_CB_SHORT_FLT(void)
{
	uint8_t tIcIndex;

	for (tIcIndex = 0; tIcIndex < BCC_IC_NUM_MAX; tIcIndex++)
	{
		bcc_CB_SHORT_FLT[tIcIndex] = 0x00;
	}
}




static void bcc_CBxShortDiag(void)
{
	uint8_t tIcIndex;
	uint8_t tChannelIndex;

	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		for (tChannelIndex = 0; tChannelIndex < BCC_VOLT_NUM_MAX; tChannelIndex++)
		{
			if (0 != (bcc_CB_SHORT_FLT[tIcIndex] & (0x0001 << tChannelIndex)))
			{
				bcc_BalCir_Short[tIcIndex][tChannelIndex] = 1;
			}
			else
			{
				bcc_BalCir_Short[tIcIndex][tChannelIndex] = 0;
			}

		}
	}
}


#if 0
static void bcc_diagCBxOpen_readOpenFltReg(void)
{
	uint8_t tReadLen;
	uint8_t tDeviceId;

	for (tDeviceId = 1; tDeviceId <= BCC_INIT_IC_NUM; tDeviceId++)
	{
		(void)bcc_33771LocalWrite(1, BCC_REG_CB_OPEN_FLT_ADDR, tDeviceId, BCC_CMD_READ);

		if (1 == bcc_readDeviceResponse(bcc_responseBuff, &tReadLen, 6))
		{
		if (tReadLen >= 3)
		{
			if (1 == bcc_readDataRelocation((tReadLen - 3), 3))
			{
					if (1 == bcc_extractOneRegData(bcc_responseBuff, 3, &bcc_readResData, (tDeviceId - 1)))
				{
					if ((tDeviceId == bcc_readResData.mc33771_cid)
								&& (BCC_REG_CB_OPEN_FLT_ADDR == bcc_readResData.mc33771_regAddr))
					{
						bcc_CB_OPEN_FLT[tDeviceId-1] |= (uint16_t)bcc_readResData.mc33771_regData & bcc_faultMask[tDeviceId-1];
						}
					}
				}
			}
		}
	}
}
#endif


static void bcc_clearCBxOpenFaultRaw(void)
{
	uint8_t tIcIndex;

	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		bcc_CB_OPEN_FLT[tIcIndex] = 0x00;
	}
}



static void bcc_updateCBxFaultToRTE(void)
{
	uint8_t tIcIndex;
	uint8_t tChannelIndex;

	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		for (tChannelIndex = 0; tChannelIndex < BCC_VOLT_NUM_MAX; tChannelIndex++)
		{
			if (0 != (bcc_CB_OPEN_FLT[tIcIndex] & (0x0001 << tChannelIndex)))
			{
				bcc_BalCir_Open[tIcIndex][tChannelIndex] = 1;
			}
			else
			{
				bcc_BalCir_Open[tIcIndex][tChannelIndex] = 0;
			}

		}
	}
}


bcc_CBx_openDiag_t bcc_CBx_openDiag;


uint8_t bcc_CBx_OpenDiagIndex = 1;

static uint8_t bcc_CBx_diagOpen_readHandle(void)
{
	uint8_t tRetSta = 0;

	if (1 == bcc_recvLengthJudge())
	{
		if (1 == bcc_checkEcho())
		{
			if (1 == bcc_extractOneDataFromDmaBuff(3, BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_CBx_OpenDiagIndex-1),
					BCC_REG_CB_OPEN_FLT_ADDR))
			{
				bcc_CB_OPEN_FLT[bcc_CBx_OpenDiagIndex-1] |=
							(uint16_t)bcc_readResData.mc33771_regData & bcc_faultMask[bcc_CBx_OpenDiagIndex-1];
			}


			if (1 == bcc_extractOneDataFromDmaBuff(6, BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_CBx_OpenDiagIndex-1),
								BCC_REG_CB_OPEN_FLT_ADDR))
			{
				bcc_CB_SHORT_FLT[bcc_CBx_OpenDiagIndex-1] |=
							(uint16_t)bcc_readResData.mc33771_regData & bcc_faultMask[bcc_CBx_OpenDiagIndex-1];
			}
		}
	}

	if (bcc_CBx_OpenDiagIndex < BCC_INIT_IC_NUM)
	{
		/* 7. Read the CB_OPEN_FLT register to determine all CBx_OPEN_ FLT open load faut bits. */

		bcc_CBx_OpenDiagIndex++;

		bcc_startDmaTransmit(18);

		(void)bcc_33771LocalWrite(2, BCC_REG_CB_OPEN_FLT_ADDR, bcc_CBx_OpenDiagIndex, BCC_CMD_READ);
	}
	else
	{
		bcc_CBx_OpenDiagIndex = 1;

		tRetSta = 1;

		bcc_updateCBxFaultToRTE();

		bcc_CBxShortDiag();
	}

	return tRetSta;
}



static uint8_t bcc_CBx_Open_new(void)
{
	uint8_t tRetSta = 0;

	switch (bcc_CBx_openDiag)
	{
		case BCC_DIAG_CBX_OPEN_PREPARE_AND_EVEN_ENABLE:
			/*1. Write SYS_CFG1[GO2DIAG] bit to logic 1 to enter Diagnostic mode.*/
			bcc_enterDiagModeAndPauseBal();

			/* 2. If the number of cells on the cluster is odd, then write the bit SYS_CFG2[NUMB_ODD] to
			logic 1, else write it to logic 0 */
			bcc_diagCBxOpen_step2();

			/* 3. Command the cell balance outputs OFF by setting SYS_CFG1[CB_MANUAL_PAUSE] to
			logic 1. */
			bcc_diagCBxOpen_step3();

			/* 4. Command the SYS_DIAG[CB_OL_ODD,CB_OL_EVEN] field to the 10 configuration. */
			bcc_diagCBxOpen_step4();

			bcc_clearCBxOpenFaultRaw();
			bcc_clearReg_CB_SHORT_FLT();

			bcc_startDmaTransmit(18);

			(void)bcc_33771LocalWrite(2, BCC_REG_CB_OPEN_FLT_ADDR, bcc_CBx_OpenDiagIndex, BCC_CMD_READ);

			bcc_CBx_openDiag = BCC_DIAG_CBX_OPEN_READ_EVEN_DATA;
			break;
		case BCC_DIAG_CBX_OPEN_READ_EVEN_DATA:
			/* 5. Read the CB_OPEN_FLT register to determine all CBx_OPEN_ FLT open load faut bits. */
			if (1 == bcc_CBx_diagOpen_readHandle())
			{
				bcc_CBx_openDiag = BCC_DIAG_CBX_OPEN_ODD_ENABLE;
			}
			break;
		case BCC_DIAG_CBX_OPEN_ODD_ENABLE:
			/* 6. Command the SYS_DIAG[CB_OL_ODD,CB_OL_EVEN] field to the 01 configuration. */
			bcc_diagCBxOpen_step6();

			bcc_startDmaTransmit(18);

			(void)bcc_33771LocalWrite(2, BCC_REG_CB_OPEN_FLT_ADDR, bcc_CBx_OpenDiagIndex, BCC_CMD_READ);

			bcc_CBx_openDiag = BCC_DIAG_CBX_OPEN_ODD_EVEN_DATA;
			break;
		case BCC_DIAG_CBX_OPEN_ODD_EVEN_DATA:
			/* 7. Read the CB_OPEN_FLT register to determine all CBx_OPEN_ FLT open load faut bits. */
			if (1 == bcc_CBx_diagOpen_readHandle())
			{
				tRetSta = 1;

				bcc_CBx_openDiag = BCC_DIAG_CBX_OPEN_PREPARE_AND_EVEN_ENABLE;

				bcc_diagCBxOpen_restoreRegData();

				/* 8. Restore the cell balance outputs by setting SYS_CFG1[CB_MANUAL_PAUSE] to logic 0. */
				bcc_diagCBxOpen_step8();

				/* 9. Exit Diagnostic mode by setting SYS_CFG1[GO2DIAG] bit to logic 0. */
				bcc_startCellBalAndExitBal();
			}
			break;
		default:
			break;
	}

	return tRetSta;
}


#if 0
static void bcc_diagCBxOpen(void)
{
	/*1. Write SYS_CFG1[GO2DIAG] bit to logic 1 to enter Diagnostic mode.*/
	bcc_enterDiagModeAndPauseBal();

	/* 2. If the number of cells on the cluster is odd, then write the bit SYS_CFG2[NUMB_ODD] to
	logic 1, else write it to logic 0 */
	bcc_diagCBxOpen_step2();

	/* 3. Command the cell balance outputs OFF by setting SYS_CFG1[CB_MANUAL_PAUSE] to
	logic 1. */
	bcc_diagCBxOpen_step3();

	/* 4. Command the SYS_DIAG[CB_OL_ODD,CB_OL_EVEN] field to the 10 configuration. */
	bcc_diagCBxOpen_step4();

	bcc_clearCBxOpenFaultRaw();

	bcc_delay150us();
	/* 5. Read the CB_OPEN_FLT register to determine all CBx_OPEN_ FLT open load faut bits. */
	bcc_diagCBxOpen_readOpenFltReg();

	/* 6. Command the SYS_DIAG[CB_OL_ODD,CB_OL_EVEN] field to the 01 configuration. */
	bcc_diagCBxOpen_step6();

	bcc_delay150us();
	/* 7. Read the CB_OPEN_FLT register to determine all CBx_OPEN_ FLT open load faut bits. */
	bcc_diagCBxOpen_readOpenFltReg();
	bcc_diagCBxOpen_restoreRegData();

	bcc_updateCBxFaultToRTE();

	/* 8. Restore the cell balance outputs by setting SYS_CFG1[CB_MANUAL_PAUSE] to logic 0. */
	bcc_diagCBxOpen_step8();

	/* 9. Exit Diagnostic mode by setting SYS_CFG1[GO2DIAG] bit to logic 0. */
	bcc_startCellBalAndExitBal();
}
#endif





static void bcc_CTxOpenDiagStep2_cfgSysDiag(void)
{

	uint16_t tReg = 0x00;

	tReg = BCC_CT_OL_ODD_CLOSED;
	bcc_33771GlobalWrite(tReg, BCC_REG_SYS_DIAG_ADDR);
}


static void bcc_CTxOpenDiagStep8_cfgSysDiag(void)
{

	uint16_t tReg = 0x00;

	tReg = BCC_CT_OL_EVEN_CLOSED;
	bcc_33771GlobalWrite(tReg, BCC_REG_SYS_DIAG_ADDR);
}



static void bcc_CTxOpenDiagStep13_cfgSysDiag(void)
{

	uint16_t tReg = 0x00;

	bcc_33771GlobalWrite(tReg, BCC_REG_SYS_DIAG_ADDR);
}




#if 0
static void bcc_CTxOpenDiag_readVolt(void)
{
	uint8_t tReadLen;
	uint8_t tRegAddrCnt = 0;
	bcc_readResData_t tReadCmdData;
	uint16_t tVolt = 0;
	uint8_t tIc;

	for (tIc = 0; tIc < BCC_INIT_IC_NUM; tIc++)
	{
		(void)bcc_33771LocalWrite(BCC_VOLT_NUM_MAX, 0x33, (tIc + 1), BCC_CMD_READ);

		if (1 == bcc_readDeviceResponse(bcc_responseBuff, &tReadLen,
									(BCC_RESPONSE_DATA_LEN_OF_16BIT*(BCC_VOLT_NUM_MAX+1))))
		{
			//bcc_receiveDataLenDiag(tReadLen, (BCC_RESPONSE_DATA_LEN_OF_16BIT*(BCC_VOLT_NUM_MAX+1)));

			if (1 == bcc_getReadBuffLastData((BCC_RESPONSE_DATA_LEN_OF_16BIT*BCC_VOLT_NUM_MAX), tReadLen))
			{
				for (tRegAddrCnt = 0; tRegAddrCnt < BCC_VOLT_NUM_MAX; tRegAddrCnt++)
				{
					if (1 == bcc_extractOneRegData(&bcc_responseBuff[tRegAddrCnt * BCC_RESPONSE_DATA_LEN_OF_16BIT],
							BCC_RESPONSE_DATA_LEN_OF_16BIT, &tReadCmdData, tIc))
					{
						if (0x8000 == (tReadCmdData.mc33771_regData & 0x8000))
						{
							tReadCmdData.mc33771_regData &= 0x7FFF;

							tVolt = (uint32_t)tReadCmdData.mc33771_regData * 15259 / 100000;
							bcc_CTxOpenDiagCellVolt[tIc][BCC_VOLT_NUM_MAX - tRegAddrCnt - 1] = tVolt;
						}
						else
						{
							/* keep value */
						}
					}
					else
					{
						/* do nothing */
					}
				}
			}
			else
			{

			}
		}
		else
		{

		}
	}
}
#endif




static void bcc_CTxOpenOddChannelDiag(void)
{
	uint8_t tIcIndex;
	uint8_t tChannelInedx;

	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		for (tChannelInedx = 0; tChannelInedx < BCC_VOLT_NUM_MAX; tChannelInedx++)
		{
			if ((1 == tIcIndex) || (2 == tIcIndex) || (5 == tIcIndex))
			{
				/* do nothing */
				if (tChannelInedx > 4)
				{
					if (1 == (tChannelInedx % 2))
					{
						if (
								(30 >= bcc_CTxOpenDiagCellVolt[tIcIndex][tChannelInedx])
									&& (0 != bcc_cellShortCfgTab[tIcIndex][tChannelInedx])
							)
						{
							Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 1;
						}
						else
						{
							Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 0;
						}
					}
					else
					{
					}
				}
				else
				{
					if (0 == (tChannelInedx % 2))
					{
						if (
								(30 >= bcc_CTxOpenDiagCellVolt[tIcIndex][tChannelInedx])
									&& (0 != bcc_cellShortCfgTab[tIcIndex][tChannelInedx])
							)
						{
							Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 1;
						}
						else
						{
							Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 0;
						}
					}
					else
					{
					}
				}
			}
			else
			{
				if (0 == (tChannelInedx % 2))
				{
					if (
							(30 >= bcc_CTxOpenDiagCellVolt[tIcIndex][tChannelInedx])
								&& (0 != bcc_cellShortCfgTab[tIcIndex][tChannelInedx])
						)
					{
						Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 1;
					}
					else
					{
						Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 0;
					}
				}
				else
				{
				/* do nothing */
				}
			}
		}
	}
}
static void bcc_CTxOpenEvenChannelDiag(void)
{
	uint8_t tIcIndex;
	uint8_t tChannelInedx;
	for (tIcIndex = 0; tIcIndex < BCC_INIT_IC_NUM; tIcIndex++)
	{
		for (tChannelInedx = 0; tChannelInedx < BCC_VOLT_NUM_MAX; tChannelInedx++)
		{
			if ((1 == tIcIndex) || (2 == tIcIndex) || (5 == tIcIndex))
			{
				if (tChannelInedx > 4)
				{
					if (0 == (tChannelInedx % 2))
					{
						if (
								(40 >= bcc_CTxOpenDiagCellVolt[tIcIndex][tChannelInedx])
									&& (0 != bcc_cellShortCfgTab[tIcIndex][tChannelInedx])
							)
						{
							Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 1;
						}
						else
						{
							Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 0;
						}
					}
					else
					{
					}
				}
				else
				{
					if (1 == (tChannelInedx % 2))
					{
						if (
								(40 >= bcc_CTxOpenDiagCellVolt[tIcIndex][tChannelInedx])
									&& (0 != bcc_cellShortCfgTab[tIcIndex][tChannelInedx])
							)
						{
							Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 1;
						}
						else
						{
							Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 0;
						}
					}
					else
					{
					}
				}
			}
			else
			{
				if (1 == (tChannelInedx % 2))
				{
					if (
							(40 >= bcc_CTxOpenDiagCellVolt[tIcIndex][tChannelInedx])
								&& (0 != bcc_cellShortCfgTab[tIcIndex][tChannelInedx])
						)
					{
						Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 1;
					}
					else
					{
						Bcc_CellVltgLine_Open[tIcIndex][tChannelInedx] = 0;
					}
				}
				else
				{
				}
			}
		}
	}
}


#if 0
static void bcc_CTxOpenDiag(void)
{
	/* 1. Write SYS_CFG1[GO2DIAG] bit to logic 1 to enter Diagnostic mode. */
	bcc_enterDiagModeAndPauseBal();

	/* 2. Write SYS_DIAG[CT_OL_ODD,CT_OL_EVEN] field to 10 configuration to command fault
			detect switches */
	bcc_CTxOpenDiagStep2_cfgSysDiag();

	/* 3. Wait for K DIAG =5 times the diagnostic time constant tDIAG (see note) */
	bcc_delay3ms();
	bcc_delay3ms();
	bcc_delay3ms();
	bcc_delay3ms();
	bcc_delay3ms();

	/* 4. Write register ADC_CFG[SOC] to initiate a conversion. */
	bcc_startConversion();

	bcc_delay3ms();

	/* 5. Read Conversion Results. */
	bcc_CTxOpenDiag_readVolt();

	/* 6. Determine fault mode according to Table CT Open Detect and Functional Verification. */
	bcc_CTxOpenOddChannelDiag();

	/* 7. Clear CELL_OV and CELL_UV fault registers. */

	/* 8. Write SYS_DIAG[CT_OL_ODD,CT_OL_EVEN] field to 01 configuration to command fault
		detect switches. */
	bcc_CTxOpenDiagStep8_cfgSysDiag();

	/* 9. Wait for K DIAG =5 times the diagnostic time constant tDIAG (see note) */
	bcc_delay3ms();
	bcc_delay3ms();
	bcc_delay3ms();
	bcc_delay3ms();
	bcc_delay3ms();

	/* 10. Write register ADC_CFG[SOC] to initiate a conversion. */
	bcc_startConversion();

	bcc_delay3ms();

	/* 11. Read conversion results. */
	bcc_CTxOpenDiag_readVolt();

	/* 12. Determine fault mode according to Table CT Open Detect and Functional Verification. */
	bcc_CTxOpenEvenChannelDiag();

	/* 13. Write SYS_DIAG[CT_OL_ODD,CT_OL_EVEN] field to 00 to open all switches. */
	bcc_CTxOpenDiagStep13_cfgSysDiag();

	/* 14. Clear CELL_OV and CELL_UV fault registers. */

	/* 15. Wait for K=10 times the measurement time constant t (see note) */
	bcc_delay3ms();

	/* 16. Exit Diagnostic Mode by writing SYS_CFG1[GO2DIAG] bit to logic 0. */
	bcc_exitDiagMode();

}
#endif


static void bcc_sysCfg2ForIc8(void)
{
	uint16_t tReg;
	tReg = BCC_FLT_RST_CFG_COM_OSC | BCC_TIMEOUT_COMM_256MS | BCC_ODD_CELLS;
	bcc_33771LocalWrite(tReg, BCC_REG_SYS_CFG2_ADDR, 0x02, BCC_CMD_WRITE);
	bcc_33771LocalWrite(tReg, BCC_REG_SYS_CFG2_ADDR, 0x03, BCC_CMD_WRITE);
	bcc_33771LocalWrite(tReg, BCC_REG_SYS_CFG2_ADDR, 0x06, BCC_CMD_WRITE);
}


bcc_CTxOpenDiag_step_t bcc_CTxOpenDiag_step = BCC_CTX_OPEN_DIAG_START_ODD;

uint8_t bcc_CTxReadIndex = 1;

static void bcc_readCTxDiagVolt(void)
{
	uint8_t tRegIndex = 0;
	uint16_t tVolt = 0;

	for (tRegIndex = 0; tRegIndex < 14; tRegIndex++)
	{
		//if (1 == bcc_extractOneDataFromDmaBuff(&spi2DmaRxBuffer[3 * (1 + tRegIndex)], BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_CTxReadIndex-1), (0x40-tRegIndex)))
		if (1 == bcc_extractOneDataFromDmaBuff((3 * (1 + tRegIndex)), BCC_RESPONSE_DATA_LEN_OF_8BIT, &bcc_readResData, (bcc_CTxReadIndex-1), (0x33+tRegIndex)))
		{
			if (0x8000 == (bcc_readResData.mc33771_regData & 0x8000))
			{
				bcc_readResData.mc33771_regData &= 0x7FFF;

				tVolt = (uint32_t)bcc_readResData.mc33771_regData * 15259 / 100000;

				//bcc_CTxOpenDiagCellVolt[bcc_CTxReadIndex-1][tRegIndex] = tVolt;
				bcc_CTxOpenDiagCellVolt[bcc_CTxReadIndex-1][13 - tRegIndex] = tVolt;
			}
		}
	}

}






static uint8_t bcc_CTxOpenDiag(void)
{
	uint8_t tRetSta = 0;

	switch (bcc_CTxOpenDiag_step)
	{
		case BCC_CTX_OPEN_DIAG_START_ODD:
			bcc_sysCfg2ForIc8();
			bcc_enterDiagModeAndPauseBal();
			bcc_CTxOpenDiagStep2_cfgSysDiag();

			bcc_CTxOpenDiag_step = BCC_CTX_OPEN_DIAG_START_ODD_CONV;
			break;
		case BCC_CTX_OPEN_DIAG_START_ODD_CONV:
			bcc_startConversion();

			bcc_CTxOpenDiag_step = BCC_CTX_OPEN_DIAG_START_ODD_READ;
			break;
		case BCC_CTX_OPEN_DIAG_START_ODD_READ:
			bcc_startDmaTransmit(6 + 6*(14));
			(void)bcc_33771LocalWrite(BCC_VOLT_NUM_MAX, 0x33, (bcc_CTxReadIndex), BCC_CMD_READ);

			bcc_CTxOpenDiag_step = BCC_CTX_OPEN_DIAG_START_ODD_READ_HANDLE;
			break;
		case BCC_CTX_OPEN_DIAG_START_ODD_READ_HANDLE:
			bcc_readCTxDiagVolt();

			bcc_CTxReadIndex++;
			if (bcc_CTxReadIndex <= BCC_INIT_IC_NUM)
			{
				bcc_startDmaTransmit(6 + 6*(14));
				(void)bcc_33771LocalWrite(BCC_VOLT_NUM_MAX, 0x33, (bcc_CTxReadIndex), BCC_CMD_READ);
			}
			else
			{
				bcc_CTxOpenOddChannelDiag();
				bcc_CTxOpenDiagStep8_cfgSysDiag();

				bcc_CTxReadIndex = 1;
				bcc_CTxOpenDiag_step = BCC_CTX_OPEN_DIAG_START_EVEN_CONV;
			}
			break;
		case BCC_CTX_OPEN_DIAG_START_EVEN_CONV:
			bcc_startConversion();

			bcc_CTxOpenDiag_step = BCC_CTX_OPEN_DIAG_START_EVEN_READ;
			break;
		case BCC_CTX_OPEN_DIAG_START_EVEN_READ:
			bcc_startDmaTransmit(6 + 6*(14));
			(void)bcc_33771LocalWrite(BCC_VOLT_NUM_MAX, 0x33, (bcc_CTxReadIndex), BCC_CMD_READ);

			bcc_CTxOpenDiag_step = BCC_CTX_OPEN_DIAG_START_EVEN_READ_HANDLE;
			break;
		case BCC_CTX_OPEN_DIAG_START_EVEN_READ_HANDLE:
			bcc_readCTxDiagVolt();

			bcc_CTxReadIndex++;
			if (bcc_CTxReadIndex <= BCC_INIT_IC_NUM)
			{
				bcc_startDmaTransmit(6 + 6*(14));
				(void)bcc_33771LocalWrite(BCC_VOLT_NUM_MAX, 0x33, (bcc_CTxReadIndex), BCC_CMD_READ);
			}
			else
			{
				tRetSta = 1;

				bcc_CTxOpenEvenChannelDiag();
				bcc_CTxOpenDiagStep13_cfgSysDiag();

				bcc_exitDiagMode();

				bcc_CTxReadIndex = 1;
				bcc_CTxOpenDiag_step = BCC_CTX_OPEN_DIAG_START_ODD;
			}
			break;
		default:
			break;
	}

	return tRetSta;
}


uint16_t bcc_diagState300msCounter = 0;
uint8_t bcc_diagState300msReadyFlg = 0;
uint16_t bcc_diagState1SCounter = 0;
uint8_t bcc_diagState1SReadyFlg = 0;

static void bcc_stateMachineManagement(void)
{
	bcc_diagState300msCounter++;
	bcc_diagState1SCounter++;

	if (0 == (bcc_diagState300msCounter % 60))
	{
		bcc_diagState300msReadyFlg = 1;
	}

	if (0 == (bcc_diagState1SCounter % 200))
	{
		bcc_diagState1SReadyFlg = 1;
	}
}


uint8_t bcc_resetMonitorFlg = 0;

static void bcc_resetMonitor(void)
{
	uint8_t tCnt = 0;

	for (tCnt = 0; tCnt < BCC_INIT_IC_NUM; tCnt++)
	{
		if (Bcc_Comm_CRCErr[tCnt] > 3)
		{
			bcc_resetMonitorFlg = 1;
			break;
		}
	}

	if (bcc_responseDataLenInvalidCnt > 3)
	{
		bcc_resetMonitorFlg = 1;
	}


	if (1 == bcc_resetMonitorFlg)
	{
		bcc_resetMonitorFlg = 0;

		//bcc_clearDmaData();

		bcc_stateMachine = BCC_STATE_INIT;
		bcc_initMachineCtrl.bcc_initStep = BCC_INIT_CID;

		bcc_normalStateStep = BCC_NORMAL_CLOSE_BAL;

		bcc_CBx_openDiag = BCC_DIAG_CBX_OPEN_PREPARE_AND_EVEN_ENABLE;

		bcc_CTxOpenDiag_step = BCC_CTX_OPEN_DIAG_START_ODD;

		bcc_diagState300msCounter = 0;
		bcc_diagState300msReadyFlg = 0;
		bcc_diagState1SCounter = 0;
		bcc_diagState1SReadyFlg = 0;

		bcc_initDeviceIDWithDir(BCC_INIT_IC_NUM, bcc_initMachineCtrl.bcc_initDir);
	}
}

static void bcc_resetRunning(void)
{
	if (BCC_STATE_INIT == bcc_stateMachine)
	{
		if (1 == bcc_initMachineManagement())
		{
			bcc_stateMachine = BCC_STATE_NORMAL;
		}
	}
}


void bcc_stateMachineRunning(void)
{
	switch (bcc_stateMachine)
	{
		case BCC_STATE_NORMAL:
			if (1 == bcc_stateMachine_normal())
			{
				bcc_FstSampleFinsh = 1;

				if (1 == bcc_diagState1SReadyFlg)
				{
					bcc_diagState1SReadyFlg = 0;

					bcc_stateMachine = BCC_STATE_DIAG_CBX_OPEN;
				}
				else if (1 == bcc_diagState300msReadyFlg)
				{
					bcc_diagState300msReadyFlg = 0;

				//	bcc_stateMachine = BCC_STATE_NORMAL;

					bcc_stateMachine = BCC_STATE_DIAG_CTX_OPEN;
				}
				else
				{
					bcc_stateMachine = BCC_STATE_NORMAL;
				}
			}
			break;
		case BCC_STATE_DIAG_CBX_OPEN:
			if (1 == bcc_CBx_Open_new())
			{
				bcc_stateMachine = BCC_STATE_NORMAL;
			}
			break;
		case BCC_STATE_DIAG_CTX_OPEN:
			if (1 == bcc_CTxOpenDiag())
			{
				bcc_stateMachine = BCC_STATE_NORMAL;
			}
			break;
		default:
			break;
	}
}


void bcc_task_50ms(void)
{
	bcc_resetMonitor();

	bcc_resetRunning();
}

void bcc_task_5ms(void)
{
	bcc_stateMachineManagement();

	bcc_stateMachineRunning();
}

void bcc_task_10ms(void)
{

}

