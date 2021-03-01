#ifndef __BCC_H__
#define __BCC_H__

#include "stdint.h"
#include "platformTypes.h"
#include "typedefs.h"


#define BCC_REG_INIT_ADDR                   0x01U
#define BCC_REG_SYS_CFG_GLOBAL_ADDR         0x02U
#define BCC_REG_SYS_CFG1_ADDR               0x03U
#define BCC_REG_SYS_CFG2_ADDR               0x04U
#define BCC_REG_SYS_DIAG_ADDR               0x05U
#define BCC_REG_ADC_CFG_ADDR                0x06U
#define BCC_REG_ADC2_OFFSET_COMP_ADDR       0x07U
#define BCC_REG_OV_UV_EN_ADDR               0x08U
#define BCC_REG_CELL_OV_FLT_ADDR            0x09U
#define BCC_REG_CELL_UV_FLT_ADDR            0x0AU
#define BCC_REG_CB1_CFG_ADDR                0x0CU
#define BCC_REG_CB2_CFG_ADDR                0x0DU
#define BCC_REG_CB3_CFG_ADDR                0x0EU
#define BCC_REG_CB4_CFG_ADDR                0x0FU
#define BCC_REG_CB5_CFG_ADDR                0x10U
#define BCC_REG_CB6_CFG_ADDR                0x11U
#define BCC_REG_CB7_CFG_ADDR                0x12U
#define BCC_REG_CB8_CFG_ADDR                0x13U
#define BCC_REG_CB9_CFG_ADDR                0x14U
#define BCC_REG_CB10_CFG_ADDR               0x15U
#define BCC_REG_CB11_CFG_ADDR               0x16U
#define BCC_REG_CB12_CFG_ADDR               0x17U
#define BCC_REG_CB13_CFG_ADDR               0x18U
#define BCC_REG_CB14_CFG_ADDR               0x19U
#define BCC_REG_CB_OPEN_FLT_ADDR            0x1AU
#define BCC_REG_CB_SHORT_FLT_ADDR           0x1BU
#define BCC_REG_CB_DRV_STS_ADDR             0x1CU
#define BCC_REG_GPIO_CFG1_ADDR              0x1DU
#define BCC_REG_GPIO_CFG2_ADDR              0x1EU
#define BCC_REG_GPIO_STS_ADDR               0x1FU
#define BCC_REG_AN_OT_UT_FLT_ADDR           0x20U
#define BCC_REG_GPIO_SHORT_ADDR             0x21U
#define BCC_REG_I_STATUS_ADDR               0x22U
#define BCC_REG_COM_STATUS_ADDR             0x23U
#define BCC_REG_FAULT1_STATUS_ADDR          0x24U
#define BCC_REG_FAULT2_STATUS_ADDR          0x25U
#define BCC_REG_FAULT3_STATUS_ADDR          0x26U
#define BCC_REG_FAULT_MASK1_ADDR            0x27U
#define BCC_REG_FAULT_MASK2_ADDR            0x28U
#define BCC_REG_FAULT_MASK3_ADDR            0x29U
#define BCC_REG_WAKEUP_MASK1_ADDR           0x2AU
#define BCC_REG_WAKEUP_MASK2_ADDR           0x2BU
#define BCC_REG_WAKEUP_MASK3_ADDR           0x2CU
#define BCC_REG_CC_NB_SAMPLES_ADDR          0x2DU
#define BCC_REG_COULOMB_CNT1_ADDR           0x2EU
#define BCC_REG_COULOMB_CNT2_ADDR           0x2FU
#define BCC_REG_MEAS_ISENSE1_ADDR           0x30U
#define BCC_REG_MEAS_ISENSE2_ADDR           0x31U
#define BCC_REG_MEAS_STACK_ADDR             0x32U
#define BCC_REG_MEAS_CELLX_ADDR_START       0x33U
#define BCC_REG_MEAS_ANX_ADDR_START         0x41U
#define BCC_REG_MEAS_IC_TEMP_ADDR           0x48U
#define BCC_REG_MEAS_VBG_DIAG_ADC1A_ADDR    0x49U
#define BCC_REG_MEAS_VBG_DIAG_ADC1B_ADDR    0x4AU
#define BCC_REG_TH_ALL_CT_ADDR              0x4BU
#define BCC_REG_TH_CT14_ADDR                0x4CU
#define BCC_REG_TH_CT13_ADDR                0x4DU
#define BCC_REG_TH_CT12_ADDR                0x4EU
#define BCC_REG_TH_CT11_ADDR                0x4FU
#define BCC_REG_TH_CT10_ADDR                0x50U
#define BCC_REG_TH_CT9_ADDR                 0x51U
#define BCC_REG_TH_CT8_ADDR                 0x52U
#define BCC_REG_TH_CT7_ADDR                 0x53U
#define BCC_REG_TH_CT6_ADDR                 0x54U
#define BCC_REG_TH_CT5_ADDR                 0x55U
#define BCC_REG_TH_CT4_ADDR                 0x56U
#define BCC_REG_TH_CT3_ADDR                 0x57U
#define BCC_REG_TH_CT2_ADDR                 0x58U
#define BCC_REG_TH_CT1_ADDR                 0x59U
#define BCC_REG_TH_AN6_OT_ADDR              0x5AU
#define BCC_REG_TH_AN5_OT_ADDR              0x5BU
#define BCC_REG_TH_AN4_OT_ADDR              0x5CU
#define BCC_REG_TH_AN3_OT_ADDR              0x5DU
#define BCC_REG_TH_AN2_OT_ADDR              0x5EU
#define BCC_REG_TH_AN1_OT_ADDR              0x5FU
#define BCC_REG_TH_AN0_OT_ADDR              0x60U
#define BCC_REG_TH_AN6_UT_ADDR              0x61U
#define BCC_REG_TH_AN5_UT_ADDR              0x62U
#define BCC_REG_TH_AN4_UT_ADDR              0x63U
#define BCC_REG_TH_AN3_UT_ADDR              0x64U
#define BCC_REG_TH_AN2_UT_ADDR              0x65U
#define BCC_REG_TH_AN1_UT_ADDR              0x66U
#define BCC_REG_TH_AN0_UT_ADDR              0x67U
#define BCC_REG_TH_ISENSE_OC_ADDR           0x68U
#define BCC_REG_TH_COULOMB_CNT_MSB_ADDR     0x69U
#define BCC_REG_TH_COULOMB_CNT_LSB_ADDR     0x6AU
#define BCC_REG_SILICON_REV_ADDR            0x6BU
#define BCC_REG_EEPROM_CTRL_ADDR            0x6CU
#define BCC_REG_DED_ENCODE1_ADDR            0x6DU
#define BCC_REG_DED_ENCODE2_ADDR            0x6EU
#define BCC_REG_FUSE_MIRROR_CTRL_ADDR       0x70U


typedef union  ///SYS_CFG1
{
    vuint16_t R;
    struct
    {
        vuint16_t  CYCLIC_TIMER     :3;
        vuint16_t  DIAG_TIMEOUT     :3;
        vuint16_t  I_MEAS_EN        :1;
        vuint16_t  CB_AUTO_PAUSE    :1;
        vuint16_t  CB_DRVEN         :1;
        vuint16_t  GO2DIAG_DIAG_ST  :1;
        vuint16_t  CB_MANUAL_PAUSE  :1;
        vuint16_t  SOFT_RST_0       :1;
        vuint16_t  FAULTWAVE        :1;
        vuint16_t  WAVE_DC_BIT1     :1;
        vuint16_t  WAVE_DC_BIT0     :1;
        vuint16_t  OSCMON           :1;
    } B;
} SYS_CFG1_tag;


/* 指令响应的格式 */
typedef struct
{
	uint16_t mc33771_regData;
	uint8_t mc33771_regAddr;
	uint8_t mc33771_cid;
	uint8_t mc33771_msgCnt;
}bcc_readResData_t;



typedef enum
{
	BCC_MODE_RUNNING = 0,
	BCC_MODE_FINISH  = 1,
	BCC_MODE_RUN_ERR = 2,
}bcc_modeRunState_t;

typedef enum
{
	BCC_WIRE_DIAG_OPEN_ODD_BAL,
	BCC_WIRE_DIAG_START_SMP_IN_ODDBAL,
	BCC_WIRE_READ_DATA_IN_ODDBAL,
	BCC_WIRE_DIAG_OPEN_EVEN_BAL,
	BCC_WIRE_DIAG_START_SMP_IN_EVENBAL,
	BCC_WIRE_READ_DATA_IN_EVENBAL,
}bcc_voltWireDiagStep_t;


typedef enum
{
	BCC_COMM_FIRST_33664,
	BCC_COMM_SECOND33664
}bcc_communicationDir_t;


typedef enum
{
	BCC_INIT_CID,
	BCC_INIT_REG
}bcc_initMachineStep_t;

typedef enum
{
	BCC_NORMAL_CLOSE_BAL,
	BCC_NORMAL_START_CONV,
	BCC_NORMAL_START_BAL,
	BCC_NORMAL_GET_BAL_STA,
	BCC_NORMAL_GET_DATA
}bcc_normalStateStep_t;

typedef enum
{
	BCC_STATE_INIT,
	BCC_STATE_NORMAL,
	BCC_STATE_DIAG_CBX_OPEN,
	BCC_STATE_DIAG_CTX_OPEN
}bcc_stateMachine_t;



typedef struct
{
	uint8_t bcc_initCnt;
	bcc_communicationDir_t bcc_initDir;
	bcc_initMachineStep_t bcc_initStep;
}bcc_initMachineCtrl_t;

typedef struct
{
	uint8_t bcc_normalTimeTick;
}bcc_normalStateCtrl_t;

typedef enum
{
	BCC_CTX_OPEN_DIAG_START_ODD,
	BCC_CTX_OPEN_DIAG_START_ODD_CONV,
	BCC_CTX_OPEN_DIAG_START_ODD_READ,
	BCC_CTX_OPEN_DIAG_START_ODD_READ_HANDLE,
	BCC_CTX_OPEN_DIAG_START_EVEN_CONV,
	BCC_CTX_OPEN_DIAG_START_EVEN_READ,
	BCC_CTX_OPEN_DIAG_START_EVEN_READ_HANDLE
}bcc_CTxOpenDiag_step_t;


typedef enum
{
	BCC_DIAG_CBX_OPEN_PREPARE_AND_EVEN_ENABLE,
	BCC_DIAG_CBX_OPEN_READ_EVEN_DATA,
	BCC_DIAG_CBX_OPEN_ODD_ENABLE,
	BCC_DIAG_CBX_OPEN_ODD_EVEN_DATA
}bcc_CBx_openDiag_t;


typedef enum
{
	BCC_DIAG_1S_READ_FAULT1,
	BCC_DIAG_1S_READ_FAULT2,
	BCC_DIAG_1S_READ_FAULT3,
	BCC_DIAG_1S_DATA_HANDLE1,
	BCC_DIAG_1S_DATA_HANDLE2,
	BCC_DIAG_1S_GPIO_OPEN,
	BCC_DIAG_1S_CBX_OPEN
}bcc_diag1S_t;
typedef enum
{
	BCC_DIAG_300ms_STEP1,
	BCC_DIAG_300ms_DELAY1,
	BCC_DIAG_300ms_DELAY2,
	BCC_DIAG_300ms_START_CONV1,
	BCC_DIAG_300ms_STEP2,
	BCC_DIAG_300ms_DELAY3,
	BCC_DIAG_300ms_DELAY4,
	BCC_DIAG_300ms_START_CONV2,
	BCC_DIAG_300ms_STEP3,
	BCC_DIAG_300ms_EXIT
}bcc_diag300ms_t;
/* 1个返回指令的长度（以16bit衡量） */
#define BCC_RESPONSE_DATA_LEN_OF_16BIT 3
#define BCC_RESPONSE_DATA_LEN_OF_8BIT 6
/* 默认使用的SPI通道 */
#define BCC_SPI_TRANSMIT_DEFAULT 0
/* SPI读取的最大buff */
#define BCC_READ_BUFF_SZIE 100
/* 最大支持的33771数量 */
#define BCC_IC_NUM_MAX 9
/* 最大支持的电压采样通道数 */
#define BCC_VOLT_NUM_MAX 14
/* 最大支持的温度采样通道数 */
#define BCC_TEMP_NUM_MAX 7
/* 板温的最大个数 */
#define BCC_BOARD_TEMP_NUM_MAX 1
/*温度通道采样的上限*/
#define BCC_TEMP_SAMPLE_UP_LIMIT 4970
/* 温度通道采样的下限 */
#define BCC_TEMP_SAMPLE_DOWN_LIMIT 30
/* VCOM的电压，用于温度采样的上拉电源 */
#define BCC_TEMP_SAMPLE_SUPPLY_VOLT 5000
/* 电压采样的拉电阻 */
#define BCC_TEMP_SAMPLE_PULL_UP_RES 6800
/* 电池温度RT表的长度 */
#define BCC_CELL_TEMP_TAB_LEN 166
/* 板温RT表的个数 */
#define BCC_BOARD_TEMP_RT_NUM 34
/* 温度滤波最大的滤波因子 */
#define BCC_FILTER_FACTOR_MAX 10

#define BCC_VOLT_FILTER_VOLT_MAX  50
#define BCC_VOLT_FILTER_COUNTER_MAX  5



/* 延迟函数使用的累加循环，4万次循环->3ms，400次循环57us，20次循环2us */
#define BCC_DELAY_3US_TICK 35
#define BCC_DELAY_5US_TICK 70
#define BCC_DELAY_27US_TICK 400
#define BCC_DELAY_150US_TICK 2200
#define BCC_DELAY_640US_TICK 10000
#define BCC_DELAY_1200US_TICK 18000
#define BCC_DELAY_3MS_TICK 45000
#define BCC_DELAY_5MS_TICK 75000


#define BCC_HW_VERSION HW_V03
/* SPI相关的参数 */
#define BCC_FIRST_33664_EN_PIN SIUL_PC15
#define BCC_SECOND_33664_EN_PIN SIUL_PA0
#define BCC_ENABLE_PORT_PCR BCC_SECOND_33664_EN_PIN//SIUL_PC15
#define BCC_INT_PORT_PCR  SIUL_PB14
#define BCC_MASTER_FIRST_SPI_CS  SIUL_PE15   /* CS1 */
#if (HW_V03 == BCC_HW_VERSION)
#define BCC_MASTER_SECOND_SPI_CS SIUL_PC4   /* CS2 */
#else
#define BCC_MASTER_SECOND_SPI_CS SIUL_PB3   /* CS2 */
#endif


/* 33664唤醒反馈信号的超时时间 */
#define MCU_CLK 50000000
#define BCC_WAKEUP_TIMEOUT  (MCU_CLK / 1000U) /* Timeout used for BCC wake-up. */
/* 调用SPI读取函数的时候，轮询的次数 */
#define BCC_SPI_READ_TIMEOUT_CNT_MAX 300
#define BCC_INIT_COUNT_MAX 50


/* core configuration */
#define BCC_CRC_TBL_SIZE    256U
#define BCC_MSG_BIGEND
#define BCC_MSG_ADDR_MASK   0x7FU


/** BCC Commands. */
/*! @brief No operation command. */
#define BCC_CMD_NOOP              0x00U
/*! @brief Read command. */
#define BCC_CMD_READ              0x01U
/*! @brief Write command. */
#define BCC_CMD_WRITE             0x02U
/*! @brief Global write command. */
#define BCC_CMD_GLOB_WRITE        0x03U



/* Initial value of ADC_CFG register. */
#define BCC_CONF1_ADC_CFG_VALUE ( \
    /* Note: TAG_ID is zero. */ \
    /* Note: SOC is disable (i.e. do not initiate on-demand conversion now). */ \
    BCC_ADC2_PGA_AUTO | \
    /* Note: CC_RST is not set (do not reset CC now). */ \
	BCC_ADC1_A_RES_16BIT | \
    BCC_ADC1_B_RES_16BIT | \
    BCC_ADC2_RES_16BIT \
)


#define BCC_INIT_ICD_TIMEOUT_MAX 1
#define BCC_INIT_CNT_MAX 3
#define BCC_NORMAL_TASK_TIMEOUT_MAX 80
#define BCC_TEMP_DELTA_MAX 200
#define BCC_TEMP_LIMIT_FILTER_COUNTER_MAX 5
#define BCC_STACK_VOLT_DELTA_LIMIT_MAX 2000
#define BCC_STACK_VOLT_FILTER_COUNTER_MAX 5
#define BCC_INNER_IC_TEMP_DELTA_LIMIT_MAX 20
#define BCC_INNER_IC_TEMP_FILTER_COUNTER_MAX 5

#define BCC_ADC_A_B_GAP 14


#define BCC_NUM_CONFIG BCC_IC_NUM_MAX
#if (BCC_NUM_CONFIG != 9)
#error "bcc number is error"
#endif


#define BCC_TEMP_CHANNEL_HANDLE 0

/**********************************注册方向和个数配置***********************************************/
#define BCC_INIT_DIR_CONFIG BCC_COMM_SECOND33664
#define BCC_INIT_IC_NUM 7
/***********************************注册方向和个数配置***********************************************/
/* 最大IC个数，可以标定 */
extern uint8_t bcc_icNum;


/* 单体电压、单体温度、和板温，电压精度：1mv，温度精度：0.125， 偏移：-40； 计算公式y=k*x+b */
extern uint16_t bcc_cellVolt[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX];
extern uint16_t bcc_cellTemp[BCC_IC_NUM_MAX][BCC_TEMP_NUM_MAX];
extern uint32_t bcc_StackVltg[BCC_IC_NUM_MAX];
extern uint8_t bcc_CellBalCmd[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX];
extern uint8_t bcc_CellBalSta[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX];
extern uint8_t bcc_FstSampleFinsh;
extern uint16_t bcc_ChipTemp[BCC_IC_NUM_MAX];
extern bool Bcc_VPWR_OV[BCC_IC_NUM_MAX];
extern bool Bcc_VPWR_UV[BCC_IC_NUM_MAX];
extern bool Bcc_CellVltgLine_Open[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX];
extern bool Bcc_GPIO_Short[BCC_IC_NUM_MAX][BCC_TEMP_NUM_MAX];
extern bool Bcc_GPIO_Open[BCC_IC_NUM_MAX][BCC_TEMP_NUM_MAX];
extern bool bcc_BalCir_Short[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX];
extern bool bcc_BalCir_Open[BCC_IC_NUM_MAX][BCC_VOLT_NUM_MAX];

extern bool bcc_VCOM_OV[BCC_IC_NUM_MAX];
extern bool bcc_VCOM_UV[BCC_IC_NUM_MAX];
extern bool bcc_VANA_OV[BCC_IC_NUM_MAX];
extern bool bcc_VANA_UV[BCC_IC_NUM_MAX];
extern bool Bcc_OSC_ERR[BCC_IC_NUM_MAX];
extern bool Bcc_ADC_ERR[BCC_IC_NUM_MAX];
extern bool Bcc_GND_LOSS[BCC_IC_NUM_MAX];
extern bool Bcc_Thermal_SHD[BCC_IC_NUM_MAX] ;
extern bool Bcc_Diag_TimeOut[BCC_IC_NUM_MAX] ;
extern bool Bcc_Comm_33664Loss;
extern bool Bcc_Comm_TAGIDErr[BCC_IC_NUM_MAX];
extern bool Bcc_Comm_AddrErr[BCC_IC_NUM_MAX];
extern bool Bcc_Comm_CRCErr[BCC_IC_NUM_MAX];
extern bool Bcc_Comm_CntErr[BCC_IC_NUM_MAX];
extern bool Bcc_Comm_RecvDataNotComplet;


extern void bcc_task_init(void);
extern void bcc_task_5ms(void);
extern void bcc_task_10ms(void);
extern void bcc_task_50ms(void);



#endif
