
#ifndef HDIO_CFG_H_
#define HDIO_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_hdio_stLow 					0
#define m_hdio_stHigh 					1
#define m_hdio_stUnknown 				2


typedef enum
{
	e_di_ign_sw = 0,
	e_di_brake1_sw,
	e_di_brake2_sw,
	e_di_fs0b,
	e_di_can0_err,
	e_di_can1_err,
	e_di_04,//04-12 is reserve
	e_di_05,
	e_di_06,
	e_di_07,
	e_di_08,
	e_di_09,
	e_di_10,
	e_di_11,
	e_di_12,
	e_di_max
}hdioCfg_di_obj;

typedef enum
{
	e_do_12v_digital_pwr = 0,
	e_do_ps_en,
	e_do_open_load_en,
	e_do_three_way_pwr,
	e_do_four_way_pwr,
	e_do_ari_cooling_pwr,
	e_do_compressor_pwr,
	e_do_diagnose_en,
	e_do_curr1_fb_chose0,
	e_do_curr1_fb_chose1,
	e_do_dcdc_en,
	e_do_bat_cooling_pwr,
	e_do_main_intake_grid_pwr,
	e_do_bat_ptc_pwr,
	e_do_curr2_fb_chose0,
	e_do_curr2_fb_chose1,
	e_do_bd8l700_reset1,
	e_do_bd8l700_reset2,
	e_do_body_can_standby,
	e_do_can_en,
	e_do_inter_can_standby,
	e_do_lin_mode,
	e_do_lin_wake,
	e_do_max
}hdioCfg_do_obj;


/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	uint8_t  chanId;
	uint8_t  dir;
	uint8_t  pullType;
	bool     safeMode;
	bool     ioStd;	/*TRUE:normal IO, FALSE:inverse IO*/
	bool 	 slewFast;
	bool     openDrain;
	bool	 readDeb;//input filter
}hdioCfg_di_t;

typedef struct
{
	uint8_t  chanId;
	uint8_t  dir;
	uint8_t  pullType;
	bool     safeMode;
	bool     ioStd;	/*TRUE:normal IO, FALSE:inverse IO*/
	bool 	 slewFast;
	bool     openDrain;
	bool 	 writeCheck;
	bool  	 initVal;
}hdioCfg_do_t;


/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const hdioCfg_di_t  diCfgGrp[e_di_max];
extern const hdioCfg_do_t  doCfgGrp[e_do_max];
/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


#endif




