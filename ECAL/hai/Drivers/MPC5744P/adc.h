
#ifndef ADC_H_
#define ADC_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_adc_isValidModule(cpuModu)	(((cpuModu) >=0) && ((cpuModu) < m_adc_cfgModuCnt))
#define m_adc_isValidChan(chan)		(((chan) >= 0) && ((chan) < 16))		
#define m_adc_isValidIsr(isr)			(((isr) >= m_adc_eoc) && ((isr) <= m_adc_eocCtu))
#define m_adc_isValidCovDt(dt)			(((dt) >= 0) && ((dt) <= 0xFFF))
#define m_adc_isValidWd(wd)		(((wd) >= 0) && ((wd) < 16))

#define m_adc_initFunID					0
#define m_adc_selModuFunId					1
#define m_adc_isPowerDownFunId				2
#define m_adc_setPowerDownFunId			3
#define m_adc_clearChanEocFunId			4
#define m_adc_clearEocFunId				5


#define m_adc_idle   			0
#define m_adc_powerDn   		1
#define m_adc_sample   		4
#define m_adc_conversion   	6
#define m_adc_wait				2
#define m_adc_busyCali			3

#define m_adc_eocCtu		4
#define m_adc_jeoc			3
#define m_adc_jech			2
#define m_adc_eoc			1
#define m_adc_ech			0

#define m_adc_imrEocCtuMask	0x00000010ul
#define m_adc_imrJeocMask		0x00000008ul
#define m_adc_imrJechMask		0x00000004ul	
#define m_adc_imrEocMask		0x00000002ul
#define m_adc_imrEchMask		0x00000001ul

#define m_adc_validIsrMask	   (m_adc_imrEocCtuMask | m_adc_imrJeocMask | m_adc_imrJechMask | m_adc_imrEocMask | m_adc_imrEchMask)

/*********************************register***************************************/

/***********control logic register******************/

/***Main configuration Register***/
#define m_adc_dtOverwriteEn  1 /*enable converted data to be overwritten by a new conversion*/
#define m_adc_dtOverwriteDis 0 /*Prevents overwrite of unread converted data;new result is discarded*/

#define m_adc_dtAlignLeft 			   1
#define m_adc_dtAlignRight			   0

#define m_adc_oneShotMode  0/*configures the normal conversion of one chain*/
#define m_adc_scanMode      1/*when the programmed chain conversion is finished it restarts immediately*/


#define m_adc_extTriFallEdge 0
#define m_adc_extTriRiseEdge 1


#define m_adc_clockHalf  0         /*Max:40Mhz*/
#define m_adc_clockFull  1         /*Max:80Mhz*/


/*ADC state*/
#define m_adc_idle    0
#define m_adc_powerDown    1
#define m_adc_waitState    2
#define m_adc_busyInCalibration    3
#define m_adc_sample    4
#define m_adc_conversion    6






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
extern void adc_init(uint8_t module,uint32_t jChanMask, uint32_t norchanMask);
extern bool adc_calibration(uint8_t module);
extern void adc_startNormalConversion(uint8_t module);
extern void adc_setInt(uint8_t module,uint8_t intMask,uint16_t chanMask,functionSt_t funSt);
extern uint8_t adc_getIsrFlag(uint8_t module);
extern uint16_t adc_getChanEoc(uint8_t module);
extern void adc_ClrChanEoc(uint8_t module,uint8_t chan);
extern void adc_clrEoc(uint8_t module,uint32_t mask);
extern bool adc_getDt(uint8_t module,uint16_t adcChan,uint16_t * dt,bool moveCut);

#endif



