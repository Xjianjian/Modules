

/* ---- Source switch on ------------------------------------------------------*/
#ifndef MC33907_H_
#define MC33907_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "PwSBC.h"
#include "typedefs.h"
#include "platformTypes.h"
/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
/*configure region*/
#define m_33907_spiModule		m_spi_module0
#define m_33907_spiCtas			m_spi0_mc33907Ctas
#define m_33907_spiCsMask		m_spi0_mc33907CsMask

/*error code region*/
#define m_33907_noErr			0x00000000
#define m_33907_waitRspTo		0x00000001	/*wait response timeout*/
#define m_33907_spiErr			0x00000002
#define m_33907_RecdataErr		0x00000004


#define m_33907_spiSecErrMask	0x00000001	/*secure bit error*/
#define m_33907_spiClkErrMask	0x00000002	/*Clk Error,clock cycle not equal to 16*/
#define m_33907_spiReqErr		0x00000004	/*Req error, wrong addr, write init registers in normal mode or wrong read or write*/
#define m_33907_spiPErr		0x00000008	/*spi parity bit err*/

/*vreg1 error*/
#define m_33907_vbatUVErrMask				0x00000001
#define m_33907_vsupUVErrMask				0x00000002
#define m_33907_vpreTherShutDownErrMask	0x00000004
#define m_33907_vpreOVErrMask				0x00000008
#define m_33907_vpreUVErrMask				0x00000010
#define m_33907_vcoreTherShutDownErrMask	0x00000020
#define m_33907_vcoreOVErrMask				0x00000040
#define m_33907_vcoreUVErrMask				0x00000080

/*vreg2 error*/
#define m_33907_vcanTherSdErrMask			0x00000001
#define m_33907_vcanOVErrMask				0x00000002
#define m_33907_vcanUVErrMask				0x00000004
#define m_33907_vauxTherSdErrMask			0x00000008
#define m_33907_vauxCurrLimitErrMask		0x00000010
#define m_33907_vauxOVErrMask				0x00000020
#define m_33907_vauxUVErrMask				0x00000040


/*vreg3 error*/
#define m_33907_vccaTherSdErrMask			0x00000001
#define m_33907_vccaCurrLimitErrMask		0x00000002
#define m_33907_vccaOVErrMask				0x00000004
#define m_33907_vccaUVErrMask				0x00000008


/*CAN1 error*/
#define m_33907_canH2Bat				0x00000001
#define m_33907_canH2Gnd				0x00000002
#define m_33907_canL2Bat				0x00000004
#define m_33907_canL2Gnd				0x00000008
#define m_33907_canDominant			0x00000010
#define m_33907_canRxRecessive			0x00000020
#define m_33907_canTxDominant			0x00000040

/*CAN2 error*/
#define m_33907_canOV					0x00000001
#define m_33907_canOC					0x00000002



/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern bool mc33907_enVcore(void);
extern bool mc33907_disVcore(void);
extern bool mc33907_changeWdWindow(uint8_t duration);
extern bool mc33907_disVcan(void);
extern bool mc33907_enVcca(void);
extern bool mc33907_releaseFSout(void);
extern void mc33907_getMode(void);
extern void mc33907_getHwCfg(void);
extern void mc33907_getLfsr(void);
extern bool mc33907_sendWdSeed(uint8_t seed); 
extern bool mc33907_refreshWd(void);
extern bool mc33907_disVcca(void);
extern void mc33907_getInitSupervisor1(void);
extern bool mc33907_setInitSupervisor1(uint8_t cmd);
extern bool mc33907_setInitInt(uint32_t cmd);
extern bool mc33907_enableVcan(void);
extern bool mc33907_setLpOff(void);
extern void mc33907_getWUSoc(void);
extern bool mc33907_setIntReq(void);
extern void mc33907_init(void);
extern void mc33907_spiFaultDiag(void);
extern void mc33907_diagVreg1(void);
extern void mc33907_diagVreg2(void);
extern void mc33907_diagVreg3(void);
extern void mc33907_diagCan1(void);
extern void mc33907_diagCan2(void);
extern void mc33907_dispose(void);
extern void mc33907_get_io_state(void);
extern void mc33907_enterSleep(void);
extern bool mc33907_setInitSupervisor2(uint8_t cmd);
extern bool mc33907_setInitSupervisor3(uint8_t cmd);
extern bool mc33907_setInitFssm1(uint8_t cmd);
extern bool mc33907_setInitFssm2(uint8_t cmd);
extern bool mc33907_setWu1(uint32_t cmd);
extern bool mc33907_setWu2(uint32_t cmd);
extern bool mc33907_setAmuxChannel(uint8_t channel);
extern bool mc33907_setCanMode(uint8_t mode);
extern bool mc33907_SetLINmode(uint32_t mode);
extern void mc33907_getcanlinmode(void);
#endif


