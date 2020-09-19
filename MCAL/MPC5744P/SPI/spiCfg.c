
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "defines.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "spi.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/
static const spiExSlvCfg_t spiExSlvCfg0	=
{
	5000000,		/*mc33907*/
	16,
	TRUE,
	FALSE,
	TRUE
};

static const spiExSlvCfg_t spiExSlvCfg1[2]	=
{
	{
		2000000,		/*BD8LA700_1*/
		16,
		TRUE,
		FALSE,
		TRUE
	},
	{
		2000000,		/*BD8LA700_2*/
		16,
		TRUE,
		FALSE,
		TRUE
	}
};





static const uint8_t spiModule0CfgIndxGrp = 0;
static const uint8_t spiModule0CsCmcrGrp = SIUL_PF3;
static const uint8_t spiModule0CsImcrGrp = SIUL_ALT1;

static const uint8_t spiModule1CfgIndxGrp[2] = {0,1};
static const uint8_t spiModule1CsCmcrGrp[2] = {SIUL_PA5,	SIUL_PG2};
static const uint8_t spiModule1CsImcrGrp[2] = {SIUL_ALT1,	SIUL_ALT2};




/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/
const spiModuleCfg_t spiModule0Cfg = 
{
	TRUE,						/*master*/
	0,							/*intMsk*/
	FALSE,						/*fifoEna*/
	FALSE,						/*dmaEna*/
	1,							/*slvCnt*/
	1,							/*cfgInfoCnt*/
	&spiModule0CfgIndxGrp,		/*cfgIndx*/
	&spiModule0CsCmcrGrp,		/*csMscr*/
	&spiModule0CsImcrGrp,		/*csAltLv*/
	&spiExSlvCfg0,				/*cfgInfo*/
	0,							/*slvFrameLen*/
	FALSE,						/*slvSckHighValid*/
	FALSE						/*slvCaptOnRiseEdge*/
};

const spiModuleCfg_t spiModule1Cfg = 
{
	TRUE,						/*master*/
	0,							/*intMsk*/
	FALSE,						/*fifoEna*/
	FALSE,						/*dmaEna*/
	2,							/*slvCnt*/
	2,							/*cfgInfoCnt*/
	spiModule1CfgIndxGrp,		/*cfgIndx*/
	spiModule1CsCmcrGrp,		/*csMscr*/
	spiModule1CsImcrGrp,		/*csAltLv*/
	spiExSlvCfg1,				/*cfgInfo*/
	0,							/*slvFrameLen*/
	FALSE,						/*slvSckHighValid*/
	FALSE						/*slvCaptOnRiseEdge*/
};

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/




