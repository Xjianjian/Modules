
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "Lin.h"
#include "LinCfg.h"
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


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/
static lin_rx_isr_cb_t lin_rx_isr_cb[e_lin_max];

/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static uint32_t lin_makeBIDR(uint8_t id,uint8_t dlc,bool tx,bool classic_checksum);
static bool lin_initModule(uint8_t module);
static void lin_changeBaudRate(uint8_t module,uint32_t	baudrate);
static void lin_rxMsg_handle(uint8_t module);
/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void lin_init(void)
{
	uint8_t	indx;
	linCfg_pinsInit();
	for(indx = 0;indx < e_lin_max;indx++)
	{
		if(linCfg_conf[indx].isen)
		{
			lin_initModule(indx);
		}
	}
	linCfg_INTCInterruptHandler();
}


bool lin_master_sendMsg(uint8_t module,uint8_t id, uint8_t * buf, uint8_t dlc,bool wait_over)
{
	bool sendSuccess = FALSE;
	volatile struct LINFlexD_tag * p_lin_module = NULL_PTR;
	uint8_t byte_i = 0;
	uint8_t * p_send_buf[8] = {NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR};
	uint16_t lin_timeout = 0xffff;

	if((NULL_PTR == buf) || (dlc >8))
	{
		return FALSE;
	}


	p_lin_module = linCfg_conf[module].moduleTag;

	if(NULL_PTR != p_lin_module)
	{

		p_send_buf[0] = (uint8_t *)(((uint32_t)&p_lin_module->BDRL.R) + 3);
		p_send_buf[1] = (uint8_t *)(((uint32_t)&p_lin_module->BDRL.R) + 2);
		p_send_buf[2] = (uint8_t *)(((uint32_t)&p_lin_module->BDRL.R) + 1);
		p_send_buf[3] = (uint8_t *)(((uint32_t)&p_lin_module->BDRL.R) + 0);

		p_send_buf[4] = (uint8_t *)(((uint32_t)&p_lin_module->BDRM.R) + 3);
		p_send_buf[5] = (uint8_t *)(((uint32_t)&p_lin_module->BDRM.R) + 2);
		p_send_buf[6] = (uint8_t *)(((uint32_t)&p_lin_module->BDRM.R) + 1);
		p_send_buf[7] = (uint8_t *)(((uint32_t)&p_lin_module->BDRM.R) + 0);

		if(TRUE == linCfg_conf[module].master)
		{
#if 0
			if(0 == p_lin_module->LINSR.B.DTF)
			{
				p_lin_module->LINCR2.B.ABRQ = 1;
				while(1 == p_lin_module->LINCR2.B.ABRQ)
				{
					/*wait until the frame has been aborted successfully*/
				}
			}
#endif
			for(byte_i = 0; byte_i < dlc; byte_i ++)
			{
				*(p_send_buf[byte_i]) = buf[byte_i];
			}
			p_lin_module->BIDR.R = lin_makeBIDR(id, dlc, TRUE, linCfg_conf[module].c_checksum);
			p_lin_module->LINCR2.B.HTRQ = 1;
			while((0 == p_lin_module->LINSR.B.DTF) && (lin_timeout--))
			{

			}
			sendSuccess = TRUE;
		}
	}

	return sendSuccess;
}

void lin_master_sendHeader(uint8_t module,uint8_t id,uint8_t dlc)
{
	volatile struct LINFlexD_tag * p_lin_module = NULL_PTR;

	p_lin_module = linCfg_conf[module].moduleTag;

	if(NULL_PTR != p_lin_module && TRUE == linCfg_conf[module].master)
	{
#if 0
		switch(dlc)
		{												//number of data Bytes will be included into the IDentifier
				case 2 : id = 0x20 | id; break; //0b10_0000 | ID; break;
				case 4 : id = 0x10 | id; break; //0b01_0000 | ID; break;
				case 8 : id = 0x30 | id; break; //0b11_0000 | ID; break;
		}
#endif
		p_lin_module->BIDR.B.ID = id;
		p_lin_module->BIDR.B.DFL = dlc -1;
		p_lin_module->BIDR.B.DIR = 0;
		p_lin_module->LINCR2.B.DDRQ  = 0;
		p_lin_module->LINCR2.B.HTRQ = 1;
	}
}


bool lin_slave_sendMsg(uint8_t module,uint8_t id, uint8_t * buf, uint8_t dlc,bool wait_over)
{
	bool send_success = FALSE;
	volatile struct LINFlexD_tag * p_lin_module = NULL_PTR;
	uint8_t * p_send_buf[8] = {NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR};
	uint8_t byte_i = 0;
	uint16_t lin_timeout = 0xffff;

	if((NULL_PTR == buf) || (dlc >8))
	{
		return FALSE;
	}

	p_lin_module = linCfg_conf[module].moduleTag;

	if(NULL_PTR != p_lin_module && FALSE == linCfg_conf[module].master)
	{
		p_send_buf[0] = (uint8_t *)(((uint32_t)&p_lin_module->BDRL.R) + 3);
		p_send_buf[1] = (uint8_t *)(((uint32_t)&p_lin_module->BDRL.R) + 2);
		p_send_buf[2] = (uint8_t *)(((uint32_t)&p_lin_module->BDRL.R) + 1);
		p_send_buf[3] = (uint8_t *)(((uint32_t)&p_lin_module->BDRL.R) + 0);

		p_send_buf[4] = (uint8_t *)(((uint32_t)&p_lin_module->BDRM.R) + 3);
		p_send_buf[5] = (uint8_t *)(((uint32_t)&p_lin_module->BDRM.R) + 2);
		p_send_buf[6] = (uint8_t *)(((uint32_t)&p_lin_module->BDRM.R) + 1);
		p_send_buf[7] = (uint8_t *)(((uint32_t)&p_lin_module->BDRM.R) + 0);

		for(byte_i = 0; byte_i < dlc; byte_i ++)
		{
			*(p_send_buf[byte_i]) = buf[byte_i];
		}

		p_lin_module->BIDR.R = 0;
		p_lin_module->BIDR.B.DFL = dlc - 1;
		p_lin_module->BIDR.B.DIR = 1;
		p_lin_module->BIDR.B.CCS = linCfg_conf[module].c_checksum;
		p_lin_module->LINCR2.B.DTRQ = 1;
		while((0 == p_lin_module->LINSR.B.DTF) && (lin_timeout--))
		{

		}
		p_lin_module->LINSR.B.DTF=1;
		send_success = TRUE;
	}

	return send_success;
}

void lin_register_rx_msg_cb(uint8_t module,p_lin_rx_msg_isr pFun)
{
	if((NULL_PTR != pFun) &&( module <e_lin_max))
	{
		lin_rx_isr_cb[module].rx_msg_cb = pFun;
	}
}

void lin_register_rx_head_cb(uint8_t module,p_lin_rx_head_isr pFun)
{
	if((NULL_PTR != pFun) &&( module <e_lin_max))
	{
		lin_rx_isr_cb[module].rx_head_cb = pFun;
	}
}


void lin_0_rxMsg_isr(void)
{
	lin_rxMsg_handle(0);
}

void lin_1_rxMsg_isr(void)
{
	lin_rxMsg_handle(1);
}



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void lin_rxMsg_handle(uint8_t module)
{
	volatile struct LINFlexD_tag * p_lin_module = NULL_PTR;
	uint8_t  rec_buf[8] = {0,0,0,0,0,0,0,0};
	uint8_t dlc = 0;
    uint8_t msg_id = 0xFF;

    p_lin_module = linCfg_conf[module].moduleTag;

	if(p_lin_module->LINSR.B.DRF)
	{
		rec_buf[0] = p_lin_module->BDRL.B.DATA0;
		rec_buf[1] = p_lin_module->BDRL.B.DATA1;
		rec_buf[2] = p_lin_module->BDRL.B.DATA2;
		rec_buf[3] = p_lin_module->BDRL.B.DATA3;

		rec_buf[4] = p_lin_module->BDRM.B.DATA4;
		rec_buf[5] = p_lin_module->BDRM.B.DATA5;
		rec_buf[6] = p_lin_module->BDRM.B.DATA6;
		rec_buf[7] = p_lin_module->BDRM.B.DATA7;

		dlc = p_lin_module->LINSR.B.RDC + 1;

        msg_id = p_lin_module->BIDR.B.ID;

		if(NULL_PTR != lin_rx_isr_cb[module].rx_msg_cb)
		{
			lin_rx_isr_cb[module].rx_msg_cb(module, msg_id, rec_buf,dlc);
		}
		/* clear RMB,DRF */
		p_lin_module->LINSR.B.RMB = 1;
		p_lin_module->LINSR.B.DRF = 1;
	}

	else if(p_lin_module->LINSR.B.HRF)
	{
		if(linCfg_conf[module].isen)
		{
			msg_id = p_lin_module->BIDR.B.ID;
			if(1 == linCfg_HeaderIdMatch(msg_id))
			{
				//p_lin_module->LINCR2.B.DDRQ = 1;
				if(NULL_PTR != lin_rx_isr_cb[module].rx_head_cb)
				{
					lin_rx_isr_cb[module].rx_head_cb(module,msg_id);
				}
			}
			else
			{
				p_lin_module->LINCR2.B.DDRQ = 0;
				p_lin_module->BIDR.R = 0;
				p_lin_module->BIDR.B.DFL = 7;
				p_lin_module->BIDR.B.DIR = 0;
				p_lin_module->BIDR.B.CCS = linCfg_conf[module].c_checksum;
			}
		}
		/* clear HRF */
		p_lin_module->LINSR.B.HRF = 1;
	}
}

static bool lin_initModule(uint8_t module)
{
	bool ret_val = TRUE;
	volatile struct LINFlexD_tag * p_lin_module = NULL_PTR;
	uint16_t init_tick = 0;

	p_lin_module = linCfg_conf[module].moduleTag;

	if(TRUE == ret_val && NULL_PTR != p_lin_module)
	{
		/*Enter Init mode*/
		p_lin_module->LINCR1.R = 0x0001;/*No filter,No sleep, Init*/
		/* wait for the INIT mode */
		while((0x1000 != (p_lin_module->LINSR.R & 0xF000)) && (init_tick < 10000))
		{

			init_tick ++;
		}
		if(init_tick < 10000)
		{
			/*configure Baudrate*/
			lin_changeBaudRate(module,linCfg_conf[module].baud);

			p_lin_module->LINCR2.R = 0x40; /* IOBE=1, Bit error resets LIN state machine */

    		p_lin_module->LINTCSR.R = 0; /* LIN timeout mode, no idle on timeout */

			p_lin_module->BIDR.B.CCS = linCfg_conf[module].c_checksum; /* checksum */
			p_lin_module->LINIER.R = 0x5;	// 0x05 enable RX and header interrupt
			if(TRUE == linCfg_conf[module].master)
			{
				/* enter NORMAL mode */
			    /* CCD bit 16 = 0 ... checksum calculation by hardware
			       CFD bit 17 = 0 ... checksum field is sent after the required number
			       of data bytes is sent
			       LASE bit 18 = 0 ... Slave automatic resync disable
			       AWUM bit 19 = 0 ... The sleep mode is exited on software request
			       MBL bits 20:23 = 0b0011 ... 13-bit LIN Master break length
			       BF bit 24 = 1 ... An RX interrupt is generated on identifier not matching any filter
			       SFTM bit 25 = 0 ... Self Test mode disable
			       LBKM bit 26 = 0 ... Loop Back mode disable
			       MME bit 27 = 1 ... Master mode
			       SBDT bit 28 = 0 ... 11-bit Slave Mode break threshold
			       RBLM bit 29 = 0 ... Receive Buffer not locked on overrun
			       SLEEP bit 30 = 0
			       INIT bit 31 = 0 ... entering Normal mode
			       = 0x0390
			       */
			    p_lin_module->LINCR1.R = 0x0390;
			}
			else
			{
				/* enter NORMAL mode */
			    /* CCD bit 16 = 0 ... checksum calculation by hardware
			       CFD bit 17 = 0 ... checksum field is sent after the required number
			       of data bytes is sent
			       LASE bit 18 = 1 ... Slave automatic resync enable
			       AWUM bit 19 = 0 ... The sleep mode is exited on software request
			       MBL bits 20:23 = 0b0011 ... 13-bit LIN Master break length
			       BF bit 24 = 0 ... No IRQ if ID does not match any filter
			       SFTM bit 25 = 0 ... Self Test mode disable
			       LBKM bit 26 = 0 ... Loop Back mode disable
			       MME bit 27 = 0 ... Slave mode
			       SBDT bit 28 = 0 ... 11-bit Slave Mode break threshold
			       RBLM bit 29 = 0 ... Receive Buffer not locked on overrun
			       SLEEP bit 30 = 0
			       INIT bit 31 = 0 ... entering Normal mode
			       =
			       0x2380

			       */
			    p_lin_module->LINCR1.R = 0x2380;
			}
		}
	}
	return ret_val;
}


static uint32_t lin_makeBIDR(uint8_t id, uint8_t dlc, bool tx, bool classic_checksum)
{
	LINFlexD_BIDR_tag  BIDR;

    BIDR.R = 0;
	BIDR.B.DFL = dlc - 1;
	BIDR.B.DIR = tx;
    BIDR.B.CCS = classic_checksum;
    BIDR.B.ID = id;

	return BIDR.R;
}


static void lin_changeBaudRate(uint8_t module,uint32_t	baudrate)
{
	double LFDIVvalue = 0;
	double fractionalPart = 0;
	uint32_t integerPart = 0;
	uint32_t fractionalPartRounded = 0;
	volatile struct LINFlexD_tag * p_lin_module = NULL_PTR;

	p_lin_module = linCfg_conf[module].moduleTag;

	LFDIVvalue = (LIN_CFG_SYS_CLK>>1)/(16.0*baudrate);


	integerPart = floor(LFDIVvalue);										//to get the integer part
	fractionalPart = LFDIVvalue - floor(LFDIVvalue);		//to get only the fractional part
	fractionalPart*=16;
	fractionalPartRounded = round(fractionalPart);

	p_lin_module->LINFBRR.R = fractionalPartRounded;
	p_lin_module->LINIBRR.R = integerPart;

}


