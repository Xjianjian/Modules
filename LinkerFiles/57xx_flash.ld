/* Entry Point */
ENTRY(_start)

/* define heap and stack size */
__HEAP_SIZE            = 0 ;
__STACK_SIZE           = 0x8000 ;

SRAM_SIZE =  384K;    
/* Define SRAM Base Address */
SRAM_BASE_ADDR = 0x40000000;

/* Define CPU0 Local Data SRAM Allocation */
LOCALDMEM_SIZE = 64K;
/* Define CPU0 Local Data SRAM Base Address */
LOCALDMEM_BASE_ADDR = 0x50800000;
    
MEMORY
{	
    flash_rchw        : org = 0x01000000,   len = 0x4
    cpu0_reset_vec    : org = 0x01000004,   len = 0x4 
    appStartReserved   : org = 0x01000008,   len = 0x8
    
    appStartId		  : org = 0x01000010,   len = 0x4
    appEntryPointInfo : org = 0x01000014,   len = 0x4
    appVersionInfo    : org = 0x01000018,	len = 0x8 
    m_cal_flash		  : org = 0x01000020,	len = 32k
    m_text            : org = 0x01008020,   len = 0x171FE0

    appEndId            : org = 0x011FFFFC,   len = 0x4

    
    m_cal_ram : 		org = 0x40000000, len = 32k
    m_eeBkAddr : 		org = 0x40008000, len = 32k
    m_eeDiagDtAddr :	org = 0x40010000, len = 4k
    
    m_data :	org = 0x40011000,len = 322K
    int_dram  : org = 0x50800000,   len = 64K    
}


SECTIONS
{
    .rchw   : 
    {
        KEEP(*(.rchw))
    } > flash_rchw

    .cpu0_reset_vector  : 
    {
        KEEP(*(.cpu0_reset_vector))
    } > cpu0_reset_vec

    .startup : ALIGN(0x400)
    {
    __start = . ;
    	*(.startup)
    } > m_text
    
    .core_exceptions_table   : ALIGN(4096)
    {
      __IVPR_VALUE = . ;
      *(.core_exceptions_table)
    } > m_text

    .intc_vector_table   : ALIGN(4096)
    {
      *(.intc_vector_table)
    } > m_text
      
    .text : 
    {
      *(.text.startup)
      *(.text)
      *(.init)
      *(.text.*)
      . = ALIGN(16);
    } > m_text
          
    .rodata :
    {
      *(.rodata)
      *(.rodata.*)
    } > m_text
    
    .eh_frame_hdr : { *(.eh_frame_hdr) } > m_text
    .eh_frame     : { KEEP (*(.eh_frame)) } > m_text
    
	.data   : 
	{
	  *(.data)
	  *(.data.*)
	}  > m_data AT>m_text
	
    .cal_ram :
    {
    	*(.cal_ram)
    } > m_cal_ram AT>m_cal_flash
    
    .sdata2  :
	{  
	  *(.sdata2)
	  *(.sdata2.*)
	} > m_data AT>m_text

	.sbss2    (NOLOAD)   :
	{
      /* _SDA2_BASE_ = .; */ 
	  *(.sbss2)
	  *(.sbss2.*)
	} > m_data
    
    .sdata  :
	{
	  *(.sdata)
	  *(.sdata.*)
	} > m_data AT>m_text
    
	.bss   (NOLOAD)  :
	{
	  __BSS_START = .;
	  *(.sbss)
	  *(.sbss.*)	
      *(.bss)
      *(.bss.*)
      *(COMMON)
      __BSS_END = .;
    } > m_data
    

    
    .eeBkAddr :
    {
    	*(.eeBkAddr)
        *(.eeBkAddr.*)
    } > m_eeBkAddr
    
    .eeDiagDtAddr :
    {
    	*(.eeDiagDtAddr)
        *(.eeDiagDtAddr.*)
    } > m_eeDiagDtAddr AT>m_text
    
    
    .eeDiagDtAddr :
    {
    	*(.eeDiagDtAddr)
        *(.eeDiagDtAddr.*)
    } > m_eeDiagDtAddr AT>m_text
    
    .startID :
    {
    	KEEP(*(.startID))
    } > appStartId
    
    .endID :
    {
    	KEEP(*(.endID))
    } > appEndId
    
    .entryPointInf :
    {
    	KEEP(*(.entryPointInf))
    } > appEntryPointInfo
    
    .versionInf :
    {
    	KEEP(*(.versionInf))
    } > appVersionInfo
    
    
    .stack (NOLOAD) : ALIGN(16) 
    {
      __HEAP = . ;
      PROVIDE (_end = . );
      PROVIDE (end = . );      
      . += __HEAP_SIZE ;
      __HEAP_END = . ;
      _stack_end = . ;
      . +=  __STACK_SIZE ;
      _stack_addr = . ;
      __SP_INIT = . ;
      . += 4;
    } > m_data
 
/*-------- LABELS USED IN CODE -------------------------------*/
        
/* Labels for Copying Initialised Data from Flash to RAM */
__DATA_SRAM_ADDR  = ADDR(.data);
__SDATA_SRAM_ADDR = ADDR(.sdata);
__CALI_RAM_ADDR  = ADDR(.cal_ram);
__DIAG_RAM_ADDR  = ADDR(.eeDiagDtAddr);

__DATA_SIZE   = SIZEOF(.data);
__SDATA_SIZE  = SIZEOF(.sdata);
__CALI_RAM_SIZE  = SIZEOF(.cal_ram);
__DIAG_RAM_SIZE  = SIZEOF(.eeDiagDtAddr);


__DATA_ROM_ADDR  = LOADADDR(.data);
__SDATA_ROM_ADDR = LOADADDR(.sdata);
__CALI_ROM_ADDR  = LOADADDR(.cal_ram);
__DIAG_ROM_ADDR  = LOADADDR(.eeDiagDtAddr);
   
/* Labels Used for Initialising SRAM ECC */
__SRAM_SIZE = SRAM_SIZE;
__SRAM_BASE_ADDR = SRAM_BASE_ADDR;

__LOCAL_DMEM_SIZE = LOCALDMEM_SIZE;
__LOCAL_DMEM_BASE_ADDR = LOCALDMEM_BASE_ADDR;

__BSS_SIZE    = __BSS_END - __BSS_START;    
      
}




