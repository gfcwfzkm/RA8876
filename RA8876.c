/*
 * RA8876.c
 * Version 1.0
 * Created: 17.11.2020 
 *  Author: gfcwfzkm
 */ 

#include "RA8876.h"

#define EN_DELAY()	asm("nop"	"\n\t" \
						"nop"	"\n\t");

enum RA8876_dispMode _textMode;
enum RA8876_TextSize _textSize = TEXTSIZE_8x16_16x16;

static void _setup_PLL_initial()
{
#if (SCAN_FREQ >= 63)
	RA8876_writeReg(RA8876_PPLLC1,RA8876_PPLLC1_DIVK4_gc);    //PLL Divided by 4
	RA8876_writeReg(RA8876_PPLLC2,(SCAN_FREQ*4/OSC_FREQ)-1);
#elif (SCAN_FREQ >= 32)
	RA8876_writeReg(RA8876_PPLLC1,RA8876_PPLLC1_DIVK8_gc);    //PLL Divided by 8
	RA8876_writeReg(RA8876_PPLLC2,(SCAN_FREQ*8/OSC_FREQ)-1);
#elif (SCAN_FREQ >= 16)
	RA8876_writeReg(RA8876_PPLLC1,RA8876_PPLLC1_EXT_DIV2_gc | RA8876_PPLLC1_DIVK8_gc);    //PLL Divided by 16
	RA8876_writeReg(RA8876_PPLLC2,(SCAN_FREQ*16/OSC_FREQ)-1);
#elif (SCAN_FREQ >= 8)
	RA8876_writeReg(RA8876_PPLLC1,RA8876_PPLLC1_EXT_DIV4_gc | RA8876_PPLLC1_DIVK8_gc);    //PLL Divided by 32
	RA8876_writeReg(RA8876_PPLLC2,(SCAN_FREQ*32/OSC_FREQ)-1);
#else
	RA8876_writeReg(RA8876_PPLLC1,RA8876_PPLLC1_EXT_DIV8_gc | RA8876_PPLLC1_DIVK8_gc);    //PLL Divided by 64
	RA8876_writeReg(RA8876_PPLLC2,(SCAN_FREQ*64/OSC_FREQ)-1);
#endif
	
#if (DRAM_FREQ >= 125)
	RA8876_writeReg(RA8876_MPLLC1,RA8876_MPLLC1_DIVK2_gc);    //PLL Divided by 2
	RA8876_writeReg(RA8876_MPLLC2,(DRAM_FREQ*2/OSC_FREQ)-1);
#elif (DRAM_FREQ >= 63)
	RA8876_writeReg(RA8876_MPLLC1,RA8876_MPLLC1_DIVK4_gc);    //PLL Divided by 4
	RA8876_writeReg(RA8876_MPLLC2,(DRAM_FREQ*4/OSC_FREQ)-1);
#elif (DRAM_FREQ >= 31)
	RA8876_writeReg(RA8876_MPLLC1,RA8876_MPLLC1_DIVK8_gc);    //PLL Divided by 8
	RA8876_writeReg(RA8876_MPLLC2,(DRAM_FREQ*8/OSC_FREQ)-1);
#else
	RA8876_writeReg(RA8876_MPLLC1,RA8876_MPLLC1_DIVK8_gc);    //PLL Divided by 8
	RA8876_writeReg(RA8876_MPLLC2,(30*8/OSC_FREQ)-1); //
#endif
	

	// Set Core clock
#if (CORE_FREQ>=125)
	RA8876_writeReg(RA8876_SPLLC1,RA8876_SPLLC1_DIVK2_gc);    //PLL Divided by 2
	RA8876_writeReg(RA8876_SPLLC2,(CORE_FREQ*2/OSC_FREQ)-1);
#elif (CORE_FREQ>=63)
	RA8876_writeReg(RA8876_SPLLC1,RA8876_SPLLC1_DIVK4_gc);    //PLL Divided by 4
	RA8876_writeReg(RA8876_SPLLC2,(CORE_FREQ*4/OSC_FREQ)-1);
#elif (CORE_FREQ>=31)
	RA8876_writeReg(RA8876_SPLLC1,RA8876_SPLLC1_DIVK8_gc);    //PLL Divided by 8
	RA8876_writeReg(RA8876_SPLLC2,(CORE_FREQ*8/OSC_FREQ)-1);
#else
	RA8876_writeReg(RA8876_SPLLC1,RA8876_SPLLC1_DIVK8_gc);    //PLL Divided by 8
	RA8876_writeReg(RA8876_SPLLC2,(30*8/OSC_FREQ)-1); //
#endif
	
	RA8876_writeReg(RA8876_CCR, 0);
	_delay_us(10);
	RA8876_writeReg(RA8876_CCR, RA8876_CCR_CHNG_PLL);
	//Enable_PLL();
	_delay_ms(1);
}

static void _setup_SDRAM_initial()
{
	uint16_t sdram_itv;

#ifdef W9812G6JH

	RA8876_writeReg(RA8876_SDRAR,RA8876_SDRAR_BANKNUM_4 | RA8876_SDRAR_ROW_ADDR_4K_gc | RA8876_SDRAR_COL_ADDR_512_gc);	// 0x29
	RA8876_writeReg(RA8876_SDRMD,RA8876_SDRMD_LATENCY_3CLK_gc);	//CAS:2=0x02?CAS:3=0x03 -0x03-

    sdram_itv = (64000000 / 8192) / (1000/DRAM_FREQ) ;

    sdram_itv-=2;

	RA8876_writeReg(RA8876_SDR_REF_ITVL0,sdram_itv);
	RA8876_writeReg(RA8876_SDR_REF_ITVL1,sdram_itv >>8);

	RA8876_writeReg(RA8876_SDRCR,RA8876_SDRCR_INIT);
#endif

	// Wait until SDRAM is ready
	while((RA8876_readStatus() & RA8876_SDRAM_READY) == 0);
 
	_delay_ms(1);
}

static void _setup_TFT_interface()
{
	uint8_t reg_temp;
	
	reg_temp = RA8876_readReg(RA8876_CCR);
	reg_temp &=~ RA8876_CCR_TFT_PIN_MASK;
	reg_temp |= TFT_INTERFACE;
	RA8876_writeReg(RA8876_CCR, reg_temp);
}

static void _setup_disp_size(uint16_t x, uint16_t y)
{
	uint16_t temp;
	
	temp = (x / 8) - 1;
	RA8876_writeReg(RA8876_HDWR, temp);
	temp = x % 8;
	RA8876_writeReg(RA8876_HDWFTR, temp);
	temp = y - 1;
	RA8876_writeReg(RA8876_VDHR0, temp);
	temp = (y - 1) >> 8;
	RA8876_writeReg(RA8876_VDHR1, temp);	
}

static void _setup_non_disp_size_hor(uint16_t x)
{
	uint8_t temp;
	
	temp = (x / 8) - 1;
	RA8876_writeReg(RA8876_HNDR, temp);
	temp = x % 8;
	RA8876_writeReg(RA8876_HNDFTR, temp);
}

static void _setup_HSYNC_startpos(uint16_t x)
{
	uint8_t temp;
	
	temp = (x / 8) - 1;
	RA8876_writeReg(RA8876_HSTR, temp);
}

static void _setup_HSYNC_pulse(uint16_t x)
{
	uint8_t temp;
	
	temp = (x / 8) - 1;
	RA8876_writeReg(RA8876_HPWR, temp);
}

static void _setup_non_disp_size_ver(uint16_t y)
{
	uint8_t temp;
	
	temp = y - 1;
	RA8876_writeReg(RA8876_VNDR0, temp);
	temp = temp >> 8;
	RA8876_writeReg(RA8876_VNDR1, temp);
}

static void _setup_VSYNC_startpos(uint16_t y)
{
	uint8_t temp;
	
	temp = y - 1;
	RA8876_writeReg(RA8876_VSTR, temp);	
}

static void _setup_VSYNC_pulse(uint16_t y)
{
	uint8_t temp;	
	
	temp = y - 1;
	RA8876_writeReg(RA8876_VPWR, temp);
}

void RA8876_init()
{	
	/* I/O Init */
#if INTERFACE_PARALLEL_8BIT
	CLRBIT(RA8876_WAIT_PORT, RA8876_WAIT);
	CLRBIT(RA8876_INT_PORT, RA8876_INT);
	CLRBIT(RA8876_EN_PORT, RA8876_EN);
	CLRBIT(RA8876_RST_PORT, LCD_RST);
	
	SETBIT(RA8876_CS_PORT, RA8876_CS);
	DIROUT(RA8876_CS_PORT, RA8876_CS);
	DIROUT(RA8876_EN_PORT, RA8876_EN);
	DIROUT(RA8876_RW_PORT, RA8876_RW);
	DIROUT(RA8876_RS_PORT, RA8876_RS);
	DIRIN(RA8876_WAIT_PORT, RA8876_WAIT);
	DIRIN(RA8876_INT_PORT, RA8876_INT);
		
	RA8876_DATA_DIR = DATA_OUT;
	
	DATA_PULLRESISTOR_ENABLE(RA8876_DATA_PORT);
#elif INTERFACE_SPI
	DIROUT(RA8876_CS_PORT, RA8876_CS_PIN);
	SETBIT(RA8876_CS_PORT, RA8876_CS_PIN);
	RA8876_SPI_SLOW();
#endif
	uint8_t reg_temp;
	
	// Überprüfen, ob das Display vollständig aufgestartet ist
	while(RA8876_readStatus() & RA8876_OPERATION_MODE);
	
	RA8876_swReset();	
	_setup_PLL_initial();	
	_setup_SDRAM_initial();
	_setup_TFT_interface();
	
	// 16-bit Color Depth & Mem-write default left-right-top-down
	reg_temp = RA8876_readReg(RA8876_MPWCTR);
	reg_temp |= RA8876_MPWCTR_16BIT_COLOR_gc;
	RA8876_writeReg(RA8876_MPWCTR, reg_temp);
	
	RA8876_setMode(GRAPHMODE);
	RA8876_Memory_Select(dest_SDRAM);
	
	// Set PCLK Falling
	reg_temp = RA8876_readReg(RA8876_DPCR);
	reg_temp |= RA8876_DPCR_PCLK;
	RA8876_writeReg(RA8876_DPCR, reg_temp);
	
	// DE, HSYNC & VSYNC High Active
	reg_temp = RA8876_readReg(RA8876_PCSR);
	reg_temp |= (RA8876_PCSR_XHSYNC_POL | RA8876_PCSR_XVSYNC_POL);
	RA8876_writeReg(RA8876_PCSR, reg_temp);
	
	// Set Display Size
	_setup_disp_size(1024,600);	
	_setup_non_disp_size_hor(160);
	_setup_HSYNC_startpos(160);
	_setup_HSYNC_pulse(70);
	_setup_non_disp_size_ver(23);
	_setup_VSYNC_startpos(12);
	_setup_VSYNC_pulse(10);
	
	// Init Display 16 bit color
	reg_temp = RA8876_readReg(RA8876_MPWCTR);
	reg_temp |= RA8876_MPWCTR_16BIT_COLOR_gc;
	RA8876_writeReg(RA8876_MPWCTR, reg_temp);
	
	// Memory 16 bit color mode
	reg_temp = RA8876_readReg(RA8876_AW_COLOR);
	reg_temp |= RA8876_AW_COLOR_DEPTH_16BIT_gc;
	RA8876_writeReg(RA8876_AW_COLOR, reg_temp);
	
	// Main Window start address
	RA8876_MainWindowStartAddress(LAYER_0);
	
	// Main Window Width
	RA8876_MainWindowWidth(RA8876_WIDTH);
	// Main Window Start Address
	RA8876_MainWindowStartXY(0,0);
	// Canvas start addr?
	RA8876_CanvasStartAddr(LAYER_0);
	// Canvas image width
	RA8876_CanvasWidth(RA8876_WIDTH);
	// Set Active Window to draw in
	RA8876_setActiveArea(0,0,RA8876_WIDTH, RA8876_HEIGHT);
	
#if INTERFACE_SPI
	RA8876_SPI_FAST();
#endif
}

//-------------- LOW LEVEL --------------------------------------------------------------

void RA8876_writeCMD(uint8_t _cmd)
{
#if INTERFACE_PARALLEL_8BIT
	CLRBIT(RA8876_RS_PORT, RA8876_RS);
	CLRBIT(RA8876_RW_PORT, RA8876_RW);
	CLRBIT(RA8876_CS_PORT, RA8876_CS);
	EN_DELAY();
	RA8876_DATA_OUT = _cmd;
	SETBIT(RA8876_EN_PORT, RA8876_EN);
	EN_DELAY();
	CLRBIT(RA8876_EN_PORT, RA8876_EN);
	SETBIT(RA8876_CS_PORT, RA8876_CS);
#elif INTERFACE_SPI
	CLRBIT(RA8876_CS_PORT, RA8876_CS_PIN);
	EN_DELAY();
	RA8876_SPI_WRITE(RA8876_SPI_CMDWRITE);
	RA8876_SPI_WRITE(_cmd);
	EN_DELAY();
	SETBIT(RA8876_CS_PORT, RA8876_CS_PIN);
	EN_DELAY();
#endif
}

void RA8876_writeData(uint8_t data)
{
#if INTERFACE_PARALLEL_8BIT
	CLRBIT(RA8876_CS_PORT, RA8876_CS);
	EN_DELAY();
	while (TSTBIT(RA8876_WAIT_PORT, RA8876_WAIT) == 0);
	SETBIT(RA8876_RS_PORT, RA8876_RS);
	CLRBIT(RA8876_RW_PORT, RA8876_RW);
	RA8876_DATA_OUT = data;
	SETBIT(RA8876_EN_PORT, RA8876_EN);
	EN_DELAY();
	CLRBIT(RA8876_EN_PORT, RA8876_EN);
	SETBIT(RA8876_CS_PORT, RA8876_CS);
#elif INTERFACE_SPI	
	CLRBIT(RA8876_CS_PORT, RA8876_CS_PIN);
	EN_DELAY();
	RA8876_SPI_WRITE(RA8876_SPI_DATAWRITE);
	RA8876_SPI_WRITE(data);
	EN_DELAY();
	SETBIT(RA8876_CS_PORT, RA8876_CS_PIN);
	EN_DELAY();
#endif
}

uint8_t RA8876_readData()
{
	uint8_t temp = 0;
#if INTERFACE_PARALLEL_8BIT
	RA8876_DATA_OUT = 0;
	RA8876_DATA_DIR = DATA_IN;
	SETBIT(RA8876_RW_PORT, RA8876_RW);
	SETBIT(RA8876_RS_PORT, RA8876_RS);
	CLRBIT(RA8876_CS_PORT, RA8876_CS);
	EN_DELAY();
	SETBIT(RA8876_EN_PORT, RA8876_EN);
	EN_DELAY();
	temp = RA8876_DATA_IN;
	CLRBIT(RA8876_EN_PORT, RA8876_EN);
	SETBIT(RA8876_CS_PORT, RA8876_CS);
	RA8876_DATA_DIR = DATA_OUT;
#elif INTERFACE_SPI
	CLRBIT(RA8876_CS_PORT, RA8876_CS_PIN);
	EN_DELAY();
	RA8876_SPI_WRITE(RA8876_SPI_DATAREAD);
	temp = RA8876_SPI_READ();
	EN_DELAY();
	SETBIT(RA8876_CS_PORT, RA8876_CS_PIN);
	EN_DELAY();
#endif
	return temp;
}

enum RA8876_status RA8876_readStatus(void)
{
	uint8_t _status;
#if INTERFACE_PARALLEL_8BIT
	RA8876_DATA_OUT = 0;
	RA8876_DATA_DIR = DATA_IN;
	SETBIT(RA8876_RW_PORT, RA8876_RW);
	CLRBIT(RA8876_RS_PORT, RA8876_RS);
	CLRBIT(RA8876_CS_PORT, RA8876_CS);
	EN_DELAY();
	SETBIT(RA8876_EN_PORT, RA8876_EN);
	EN_DELAY();
	_status = RA8876_DATA_IN;
	CLRBIT(RA8876_EN_PORT, RA8876_EN);
	SETBIT(RA8876_CS_PORT, RA8876_CS);
	RA8876_DATA_DIR = DATA_OUT;
#elif INTERFACE_SPI
	CLRBIT(RA8876_CS_PORT, RA8876_CS_PIN);
	EN_DELAY();
	RA8876_SPI_WRITE(RA8876_SPI_STATUSREAD);
	_status = RA8876_SPI_READ();
	EN_DELAY();
	SETBIT(RA8876_CS_PORT, RA8876_CS_PIN);
	EN_DELAY();
#endif
	return _status;
}

//-------------- HARDWARE ---------------------------------------------------------------

void RA8876_writeReg(uint8_t _reg, uint8_t _val)
{
/*
// Old stuff from RA8875 days
#if INTERFACE_SPI
	for(uint8_t i = 40; i > 0; i--)
	{
		EN_DELAY();
	}
#endif
*/
	RA8876_writeCMD(_reg);
	RA8876_writeData(_val);
}

uint8_t RA8876_readReg(uint8_t _reg)
{
	RA8876_writeCMD(_reg);
	return RA8876_readData();
}

void RA8876_waitReg(uint8_t reg, uint8_t flag)
{
	while (RA8876_readReg(reg) & flag);
}

void RA8876_swReset()
{
	uint8_t temp;
	
	RA8876_writeReg(RA8876_SSR, RA8876_SSR_SWRESET);
	do {
		temp = RA8876_readData(RA8876_SSR);
	} while (temp & RA8876_SSR_SWRESET);
}

void RA8876_hwReset()
{
	SETBIT(RA8876_RST_PORT, RA8876_RST);
	_delay_ms(1);
	CLRBIT(RA8876_RST_PORT, RA8876_RST);
	_delay_ms(100);
}

void RA8876_displayOnOff(uint8_t _onoff)
{
	uint8_t reg_temp = RA8876_readReg(RA8876_DPCR);
	if (_onoff)
	{
		reg_temp |= RA8876_DPCR_DISP_ONOFF;
	}
	else
	{
		reg_temp &=~ RA8876_DPCR_DISP_ONOFF;
	}
	RA8876_writeReg(RA8876_DPCR, reg_temp);
}

void RA8876_setMode(enum RA8876_dispMode textMode)
{
	// Activate Text Mode 
	uint8_t reg_temp;
	
	reg_temp = RA8876_readReg(RA8876_ICR);
	
	if (textMode)
	{
		reg_temp |= RA8876_ICR_TEXT_MODE_EN;
	}
	else
	{
		reg_temp &= ~RA8876_ICR_TEXT_MODE_EN;
	}
	
	RA8876_writeReg(RA8876_ICR, reg_temp);
	_textMode = textMode;
}

enum RA8876_dispMode RA8876_getMode(void)
{
	return _textMode;
}

void RA8876_Memory_Select(enum RA8876_selectMem _selectedMemory)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_ICR);
	reg_temp &= ~RA8876_ICR_MEM_DEST_SEL_MASK;
	reg_temp |= _selectedMemory;
	RA8876_writeReg(RA8876_ICR, reg_temp);
}

void RA8876_clearMemory(uint16_t fbg)
{
	RA8876_fillRect(0,0,RA8876_WIDTH-1, RA8876_HEIGHT-1, fbg);
}

void RA8876_brightness(uint8_t val)
{
	uint8_t reg_temp;
	// Turn on PWM if not already active, map 8-bit to 16-bit PWM0 register
	RA8876_writeReg(RA8876_TCMPB0L, val);
	RA8876_writeReg(RA8876_TCMPB0H, val);
	
	RA8876_writeReg(RA8876_PMUXR, RA8876_PMUXR_CLKDIV_PWM0_0 | RA8876_PMUXR_XPWM0_CTRL_1);
	
	reg_temp = RA8876_readReg(RA8876_PCFGR);
	if (val)
	{
		reg_temp = RA8876_PCFGR_PWM0_DEAD | RA8876_PCFGR_PWM0_RLD | RA8876_PCFGR_PWM0_START;
	}
	else
	{
		reg_temp = 0;
	}
	RA8876_writeReg(RA8876_PCFGR, reg_temp);
}

//-------------- DISPLAY / CANVAS / AREA ------------------------------------------------

void RA8876_MainWindowStartAddress(enum RA8876_memoryStartAddress _addr)
{	
	RA8876_writeReg(RA8876_MISA0,_addr);
	RA8876_writeReg(RA8876_MISA1,_addr>>8);
	RA8876_writeReg(RA8876_MISA2,_addr>>16);
	RA8876_writeReg(RA8876_MISA3,_addr>>24);
}

void RA8876_MainWindowWidth(uint16_t width)
{
	RA8876_writeReg(RA8876_MIW0,width);
	RA8876_writeReg(RA8876_MIW1,width>>8);
}

void RA8876_MainWindowStartXY(uint16_t x0, uint16_t y0)
{
	RA8876_writeReg(RA8876_MWULX0,x0);
	RA8876_writeReg(RA8876_MWULX1,x0>>8);
	RA8876_writeReg(RA8876_MWULY0,y0);
	RA8876_writeReg(RA8876_MWULY1,y0>>8);
}

void RA8876_CanvasStartAddr(enum RA8876_memoryStartAddress _addr)
{
	RA8876_writeReg(RA8876_CVSSA0,_addr);
	RA8876_writeReg(RA8876_CVSSA1,_addr>>8);
	RA8876_writeReg(RA8876_CVSSA2,_addr>>16);
	RA8876_writeReg(RA8876_CVSSA3,_addr>>24);
}

void RA8876_CanvasWidth(uint16_t width)
{
	RA8876_writeReg(RA8876_CVS_IMWTH0,width);
	RA8876_writeReg(RA8876_CVS_IMWTH1,width>>8);
}

void RA8876_setActiveArea(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
{	
	// Set Active Window XY
	RA8876_writeReg(RA8876_AWUL_X0, x0);
	RA8876_writeReg(RA8876_AWUL_X1, x0 >> 8);
	RA8876_writeReg(RA8876_AWUL_Y0, y0);
	RA8876_writeReg(RA8876_AWUL_Y1, y0 >> 8);
	// Set Active Window Size Width & Height
	RA8876_writeReg(RA8876_AW_WTH0, width);
	RA8876_writeReg(RA8876_AW_WTH1, width>>8);
	RA8876_writeReg(RA8876_AW_HT0, height);
	RA8876_writeReg(RA8876_AW_HT1, height>>8);
}

void RA8876_pipSelect(enum RA8876_PIP_Select _pip_sel, enum RA8876_PIP_ColorDepth _clr)
{
	uint8_t reg_temp_color, reg_temp_select;
	
	reg_temp_color = RA8876_readReg(RA8876_PIPCDEP);
	reg_temp_select = RA8876_readReg(RA8876_MPWCTR);
	
	if (_pip_sel)
	{
		// PIP1
		reg_temp_select &=~ RA8876_MPWCTR_PIP_SEL;
		reg_temp_color &=~ RA8876_PIPCDEP_PIP1_COLR_DPTH_MASK;
		reg_temp_color |= (_clr << 2);
	}
	else
	{
		// PIP2
		reg_temp_select |= RA8876_MPWCTR_PIP_SEL;
		reg_temp_color &=~ RA8876_PIPCDEP_PIP2_COLR_DPTH_MASK;
		reg_temp_color |= _clr;
	}
	
	RA8876_writeReg(RA8876_MPWCTR, reg_temp_select);
	RA8876_writeReg(RA8876_PIPCDEP, reg_temp_color);	
}

void RA8876_pipImageStartAddress(uint32_t _addr)
{
	RA8876_writeReg(RA8876_PISA0,_addr);
	RA8876_writeReg(RA8876_PISA1,_addr>>8);
	RA8876_writeReg(RA8876_PISA2,_addr>>16);
	RA8876_writeReg(RA8876_PISA3,_addr>>24);
}

void RA8876_pipImageCoords(uint16_t x0, uint16_t y0)
{
	RA8876_writeReg(RA8876_PWIULX0, x0);
	RA8876_writeReg(RA8876_PWIULX1, x0 >> 8);
	RA8876_writeReg(RA8876_PWIULY0, y0);
	RA8876_writeReg(RA8876_PWIULY1, y0 >> 8);
}

void RA8876_pipImageWidth(uint16_t width)
{
	RA8876_writeReg(RA8876_PIW0, width);
	RA8876_writeReg(RA8876_PIW1, width >> 8);
}

void RA8876_pipWindowCoords(uint16_t x0, uint16_t y0)
{
	RA8876_writeReg(RA8876_PWDULX0, x0);
	RA8876_writeReg(RA8876_PWDULX1, x0 >> 8);
	RA8876_writeReg(RA8876_PWDULY0, y0);
	RA8876_writeReg(RA8876_PWDULY1, y0 >> 8);
}

void RA8876_pipWindowSize(uint16_t width, uint8_t height)
{
	RA8876_writeReg(RA8876_PWW0, width);
	RA8876_writeReg(RA8876_PWW1, width >> 8);
	RA8876_writeReg(RA8876_PWH0, height);
	RA8876_writeReg(RA8876_PWH1, height >> 8);
}

void RA8876_pipEnable(enum RA8876_PIP_Select _pip_sel)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_MPWCTR);
	
	if (_pip_sel)
	{
		// PIP1
		reg_temp |= RA8876_MPWCTR_PIP1_EN;
	}
	else
	{
		// PIP2
		reg_temp |= RA8876_MPWCTR_PIP2_EN;
	}
	
	RA8876_writeReg(RA8876_MPWCTR, reg_temp);
}

void RA8876_pipDisable(enum RA8876_PIP_Select _pip_sel)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_MPWCTR);
	
	if (_pip_sel)
	{
		// PIP1
		reg_temp &=~ RA8876_MPWCTR_PIP1_EN;
	}
	else
	{
		// PIP2
		reg_temp &=~ RA8876_MPWCTR_PIP2_EN;		
	}
	
	RA8876_writeReg(RA8876_MPWCTR, reg_temp);
}


//-------------- COLORS -----------------------------------------------------------------

void RA8876_setFColorRGB(uint8_t red, uint8_t green, uint8_t blue)
{
	RA8876_writeReg(RA8876_FGCR, red);
	RA8876_writeReg(RA8876_FGCG, green);
	RA8876_writeReg(RA8876_FGCB, blue);
}

void RA8876_setBColorRGB(uint8_t red, uint8_t green, uint8_t blue)
{
	RA8876_writeReg(RA8876_BGCR, red);
	RA8876_writeReg(RA8876_BGCG, green);
	RA8876_writeReg(RA8876_BGCB, blue);
}

void RA8876_setFColor64k(uint16_t fgColor)
{	
	RA8876_writeReg(RA8876_FGCR, fgColor >> 8);
	RA8876_writeReg(RA8876_FGCG, fgColor >> 3);
	RA8876_writeReg(RA8876_FGCB, fgColor << 3);
}

void RA8876_setBColor64k(uint16_t bgColor)
{	
	RA8876_writeReg(RA8876_BGCR, bgColor >> 8);
	RA8876_writeReg(RA8876_BGCG, bgColor >> 3);
	RA8876_writeReg(RA8876_BGCB, bgColor << 3);	
}

void RA8876_transparentOnOff(uint8_t _transOn)
{
	uint8_t reg_temp = RA8876_readReg(RA8876_CCR1);
	
	if (_transOn)
	{
		reg_temp |= RA8876_CCR1_CHROMA;
	}
	else
	{
		reg_temp &=~ RA8876_CCR1_CHROMA;
	}
	RA8876_writeReg(RA8876_CCR1, reg_temp);
}

void RA8876_setColor(uint16_t fgColor, uint16_t bgColor, uint16_t Transparent)
{
	RA8876_setFColor64k(fgColor);
	RA8876_setBColor64k(bgColor);
	RA8876_transparentOnOff(Transparent);
}

//-------------- TEXT -------------------------------------------------------------------

void RA8876_textCursorBlink(uint8_t _blinkRate, enum RA8876_cursor cursorType)
{
	uint8_t reg_temp, cW=0, cH=0;
	
	reg_temp = RA8876_readReg(RA8876_GTCCR);
	reg_temp &= ~(RA8876_GTCCR_TXT_CURSOR_EN | RA8876_GTCCR_TXT_CURSOR_BLINK_EN);
	
	
	switch (cursorType)
	{
		case CURSOR_IBEAM:
			switch (_textSize)
			{
				case TEXTSIZE_8x16_16x16:
				cW = 1;
				cH = 16;
				break;
				case TEXTSIZE_12x24_24x24:
				cW = 2;
				cH = 24;
				break;
				case TEXTSIZE_16x32_32x32:
				cW = 3;
				cH = 31;
				break;
			}
			reg_temp |= RA8876_GTCCR_TXT_CURSOR_EN;
			reg_temp &=~ RA8876_GTCCR_GRAPH_CURSOR_EN;
			break;
		case CURSOR_UNDER:
			switch (_textSize)
			{
				case TEXTSIZE_8x16_16x16:
				cW = 8;
				cH = 1;
				break;
				case TEXTSIZE_12x24_24x24:
				cW = 12;
				cH = 2;
				break;
				case TEXTSIZE_16x32_32x32:
				cW = 16;
				cH = 3;
				break;
			}
			reg_temp |= RA8876_GTCCR_TXT_CURSOR_EN;
			reg_temp &=~ RA8876_GTCCR_GRAPH_CURSOR_EN;
			break;
		case CURSOR_BLOCK:
			switch (_textSize)
			{
				case TEXTSIZE_8x16_16x16:
					cW = 8;
					cH = 16;
					break;
				case TEXTSIZE_12x24_24x24:
					cW = 12;
					cH = 24;
					break;
				case TEXTSIZE_16x32_32x32:
					cW = 16;
					cH = 31;
					break;
			}
			reg_temp |= RA8876_GTCCR_TXT_CURSOR_EN;
			reg_temp &=~ RA8876_GTCCR_GRAPH_CURSOR_EN;
			break;
		case CURSOR_NOCURSOR:
		default:
			break;
	}
	
	if (_blinkRate)		reg_temp |= RA8876_GTCCR_TXT_CURSOR_BLINK_EN;
	
	RA8876_writeReg(RA8876_GTCCR, reg_temp);
	RA8876_writeReg(RA8876_BTCR, _blinkRate);
	RA8876_writeReg(RA8876_CURHS, cW);
	RA8876_writeReg(RA8876_CURVS, cH);
}

void RA8876_setTextSizeEnlargement(uint8_t mul_width, uint8_t mul_height)
{
	// Set font size flags 
	uint8_t temp = RA8876_readReg(RA8876_CCR1);
	temp &= ~(RA8876_CCR1_WIDTH_MASK | RA8876_CCR1_HEIGHT_MASK); // Clears bits 0..3
	temp |= ((mul_width & 0x3) << 2) | (mul_height & 0x3);
	RA8876_writeReg(RA8876_CCR1,temp);
}

void RA8876_setTextCoords(uint16_t x0, uint16_t y0)
{
	RA8876_writeReg(RA8876_F_CURX0, x0);
	RA8876_writeReg(RA8876_F_CURX1, x0 >> 8);
	RA8876_writeReg(RA8876_F_CURY0, y0);
	RA8876_writeReg(RA8876_F_CURY1, y0 >> 8);
}

void RA8876_setTextSize(enum RA8876_TextSize txtSize)
{
	// Set font size flags 
	_textSize = txtSize;
	
	uint8_t reg_temp = RA8876_readReg(RA8876_CCR0);
	reg_temp &= ~RA8876_CCR0_CHAR_HEIGHT_MASK;
	reg_temp |= txtSize;
	RA8876_writeReg(RA8876_CCR0,reg_temp);
}

void RA8876_FontInternalCGROM(enum RA8876_intFont _font)
{
	uint8_t reg_temp = RA8876_readReg(RA8876_CCR0);
	reg_temp &= ~(RA8876_CCR0_CGROM_SELECT_MASK | RA8876_CCR0_CHAR_SOURCE_MASK);
	reg_temp |= _font;
	RA8876_writeReg(RA8876_CCR0,reg_temp);
}

void RA8876_FontGTFontROM(void)
{
	uint8_t reg_temp = RA8876_readReg(RA8876_CCR0);
	reg_temp &= ~(RA8876_CCR0_CGROM_SELECT_MASK | RA8876_CCR0_CHAR_SOURCE_MASK);
	reg_temp |= RA8876_CCR0_EXTERNAL_CGROM_gc;
	RA8876_writeReg(RA8876_CCR0,reg_temp);
}

void RA8876_selectGTFontChip(enum RA8876_GT_Font_ROM _gt_rom)
{
	RA8876_writeReg(RA8876_GTFNT_SEL, _gt_rom);
}

void RA8876_setGTFontDecoder(enum RA8876_GT_Character_Set _chr, enum RA8876_GT_Character_Width _wdh)
{
	RA8876_writeReg(RA8876_GTFNT_CR, _chr | _wdh);
}

void RA8876_graphicCursorEnable(void)
{
	uint8_t reg_temp;
	
	reg_temp = RA8876_readReg(RA8876_GTCCR);
	reg_temp &=~ RA8876_GTCCR_TXT_CURSOR_EN;
	reg_temp |= RA8876_GTCCR_GRAPH_CURSOR_EN;
	RA8876_writeReg(RA8876_GTCCR, reg_temp);
}

void RA8876_graphicCursorDisable(void)
{
	uint8_t reg_temp;
	
	reg_temp = RA8876_readReg(RA8876_GTCCR);
	reg_temp &=~ RA8876_GTCCR_GRAPH_CURSOR_EN;
	RA8876_writeReg(RA8876_GTCCR, reg_temp);
}

void RA8876_graphicCursorColor(uint8_t gcc0, uint8_t gcc1)
{
	//Graphic Cursor supports four colors, two of them being 8bpp programmable colors.
	RA8876_writeReg(RA8876_GCC0, gcc0);
	RA8876_writeReg(RA8876_GCC1, gcc1);
}

void RA8876_graphicCursorSelect(uint8_t _cursor_sel)
{
	uint8_t reg_temp = RA8876_readReg(RA8876_GTCCR);
	
	reg_temp &= ~RA8876_GTCCR_GRAPH_CURSOR_SEL_MASK;
	reg_temp |= _cursor_sel << 2;
	
	RA8876_writeReg(RA8876_GTCCR, reg_temp);
}

void RA8876_graphicCursorLoad(const uint8_t *gcursor)
{
	uint8_t byteCounter = 0xFF;
	
	if (_textMode)	RA8876_setMode(GRAPHMODE);
	
	RA8876_Memory_Select(dest_GC_RAM);
	RA8876_writeCMD(RA8876_MRWDP);
	
	do{
		RA8876_writeData(*gcursor++);
	}while(byteCounter--);
	
	RA8876_Memory_Select(dest_SDRAM);
}

void RA8876_graphicCursorLoad_f(const __memx uint8_t *gcursor)
{
	uint8_t byteCounter = 0xFF;
	
	if (_textMode)	RA8876_setMode(GRAPHMODE);
	
	RA8876_Memory_Select(dest_GC_RAM);
	RA8876_writeCMD(RA8876_MRWDP);
	
	do{
		RA8876_writeData(*gcursor++);
	}while(byteCounter--);
	
	RA8876_Memory_Select(dest_SDRAM);
}

void RA8876_graphicCursorCoords(uint16_t x0, uint16_t y0)
{
	RA8876_writeReg(RA8876_GCHP0, x0);
	RA8876_writeReg(RA8876_GCHP1, x0 >> 8);
	RA8876_writeReg(RA8876_GCVP0, y0);
	RA8876_writeReg(RA8876_GCVP1, y0 >> 8);
}

void RA8876_setLineGap(uint8_t _linetoline_gap)
{
	RA8876_writeReg(RA8876_FLDR, _linetoline_gap);
}

void RA8876_setCharGap(uint8_t _chartochar_gap)
{
	RA8876_writeReg(RA8876_F2FSSR, _chartochar_gap);
}

void RA8876_printText(const char *text, uint16_t textLength)
{
	if (!(_textMode))	RA8876_setMode(TEXTMODE);
	
	RA8876_writeCMD(RA8876_MRWDP);
	
	for (uint16_t txtCnt = 0; txtCnt < textLength; txtCnt++)
	{
		RA8876_writeData(text[txtCnt]);		
		while (RA8876_readStatus() & RA8876_MEMW_FIFO_FULL);
	}
	while (!(RA8876_readStatus() & RA8876_MEMW_FIFO_EMPTY));
}

void RA8876_char(char c)
{
	RA8876_printText((const char *)&c, 1);
}

void RA8876_print(char *text)
{
	if (!(_textMode))	RA8876_setMode(TEXTMODE);
	
	RA8876_writeCMD(RA8876_MRWDP);
	
	while(*text)
	{
		RA8876_writeData(*text++);
		while (RA8876_readStatus() & RA8876_MEMW_FIFO_FULL);
	}
	while (!(RA8876_readStatus() & RA8876_MEMW_FIFO_EMPTY));
}

void RA8876_print_f(const __memx char *progmem_s)
{
	char character;
	
	if (!(_textMode))	RA8876_setMode(TEXTMODE);
	
	RA8876_writeCMD(RA8876_MRWDP);
	
	while( (character = *progmem_s++) )	// Print until null-character
	{
		RA8876_writeData(character);
		while (RA8876_readStatus() & RA8876_MEMW_FIFO_FULL);
	}
	while (!(RA8876_readStatus() & RA8876_MEMW_FIFO_EMPTY));
}

//-------------- Grafik -----------------------------------------------------------------

void RA8876_setPixelCoords(uint16_t x0, uint16_t y0)
{
	RA8876_writeReg(RA8876_CURH0, x0);
	RA8876_writeReg(RA8876_CURH1, x0 >> 8);
	RA8876_writeReg(RA8876_CURV0, y0);
	RA8876_writeReg(RA8876_CURV1, y0 >> 8);
}

void RA8876_drawPixel(uint16_t x0, uint16_t y0, uint16_t color)
{
	if (_textMode)	RA8876_setMode(GRAPHMODE);
	RA8876_setPixelCoords(x0,y0);
	RA8876_writeCMD(RA8876_MRWDP);
	RA8876_writeData(color);
	RA8876_writeData(color >> 8);
}

static void _RA8876_Triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint16_t filled)
{
	if (_textMode)	RA8876_setMode(GRAPHMODE);
	
	// (DLHER, DLVER) (DLHSR, DLVSR) (DTPH, DTPV)
	
	RA8876_writeReg(RA8876_DLHER0, x0);
	RA8876_writeReg(RA8876_DLHER1, x0 >> 8);
	
	RA8876_writeReg(RA8876_DLVER0, y0);
	RA8876_writeReg(RA8876_DLVER1, y0 >> 8);
	
	RA8876_writeReg(RA8876_DLHSR0, x1);
	RA8876_writeReg(RA8876_DLHSR1, x1 >> 8);
	
	RA8876_writeReg(RA8876_DLVSR0, y1);
	RA8876_writeReg(RA8876_DLVSR1, y1 >> 8);
	
	RA8876_writeReg(RA8876_DTPH0, x2);
	RA8876_writeReg(RA8876_DTPH1, x2 >> 8);
	
	RA8876_writeReg(RA8876_DTPV0, y2);
	RA8876_writeReg(RA8876_DTPV1, y2 >> 8);
	
	RA8876_setFColor64k(color);
	
	if (filled)
	{
		RA8876_writeReg(RA8876_DCR0, RA8876_DCR0_DRAW_START | RA8876_DCR0_DRAW_TRIANGLE | RA8876_DCR0_FILL_TRGL);
	}
	else
	{
		RA8876_writeReg(RA8876_DCR0, RA8876_DCR0_DRAW_START | RA8876_DCR0_DRAW_TRIANGLE);
	}
	
	// Wait until drawing is done
	while(RA8876_readStatus() & RA8876_CORE_BUSY);
}

static void _RA8876_Rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t r_w, uint16_t r_h, uint16_t color, uint8_t filled, uint8_t rect_type)
{
	if (_textMode)	RA8876_setMode(GRAPHMODE);
	
	if ( (x0 == x1) && (y0 == y1))
	{
		RA8876_drawPixel(x0,y0, color);
		return;
	}
	
	RA8876_writeReg(RA8876_DLHSR0, x0);
	RA8876_writeReg(RA8876_DLHSR1, x0 >> 8);
	
	RA8876_writeReg(RA8876_DLVSR0, y0);
	RA8876_writeReg(RA8876_DLVSR1, y0 >> 8);
	
	RA8876_writeReg(RA8876_DLHER0, x1);
	RA8876_writeReg(RA8876_DLHER1, x1 >> 8);
	
	RA8876_writeReg(RA8876_DLVER0, y1);
	RA8876_writeReg(RA8876_DLVER1, y1 >> 8);
	
	if (rect_type == RA8876_DCR1_DRAW_CIRCLE_SQUARE_gc)
	{
		RA8876_writeReg(RA8876_ELL_A0, r_w);
		RA8876_writeReg(RA8876_ELL_A1, r_w >> 8);
	
		RA8876_writeReg(RA8876_ELL_B0, r_h);
		RA8876_writeReg(RA8876_ELL_B1, r_h >> 8);
	}
	
	RA8876_setFColor64k(color);
	
	if (rect_type == RA8876_DCR0_DRAW_LINE)
	{
		RA8876_writeReg(RA8876_DCR0, RA8876_DCR0_DRAW_START | RA8876_DCR0_DRAW_LINE);
	}
	else if (filled)
	{
		RA8876_writeReg(RA8876_DCR1, RA8876_DCR1_DRAW_START | rect_type | RA8876_DCR1_FILL);
	}
	else
	{
		RA8876_writeReg(RA8876_DCR1, RA8876_DCR1_DRAW_START | rect_type);
	}
	
	// Wait until drawing is done
	while(RA8876_readStatus() & RA8876_CORE_BUSY);
}

static void _RA8876_Curve(uint16_t x0, uint16_t y0, uint16_t r_w, uint16_t r_h, enum RA8876_selectCurve _curve, uint16_t color, uint8_t filled, uint8_t _curve_type)
{
	if (_textMode)	RA8876_setMode(GRAPHMODE);
	
	RA8876_writeReg(RA8876_DEHR0, x0);
	RA8876_writeReg(RA8876_DEHR1, x0 >> 8);
	
	RA8876_writeReg(RA8876_DEVR0, y0);
	RA8876_writeReg(RA8876_DEVR1, y0 >> 8);
		
	RA8876_writeReg(RA8876_ELL_A0, r_w);
	RA8876_writeReg(RA8876_ELL_A1, r_w >> 8);
	
	RA8876_writeReg(RA8876_ELL_B0, r_h);
	RA8876_writeReg(RA8876_ELL_B1, r_h >> 8);
	
	RA8876_setFColor64k(color);
	
	if (filled)
	{
		RA8876_writeReg(RA8876_DCR1, RA8876_DCR1_DRAW_START | _curve_type | _curve | RA8876_DCR1_FILL);
	}
	else
	{
		RA8876_writeReg(RA8876_DCR1, RA8876_DCR1_DRAW_START | _curve_type| _curve);
	}
	
	// Wait until drawing is done
	while(RA8876_readStatus() & RA8876_CORE_BUSY);
}

void RA8876_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	_RA8876_Rect(x0,y0,x1,y1,0,0,color,0,RA8876_DCR0_DRAW_LINE);
}

void RA8876_drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	_RA8876_Rect(x0,y0,x1,y1,0,0,color,0,RA8876_DCR1_DRAW_SQUARE_gc);
}

void RA8876_fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	_RA8876_Rect(x0,y0,x1,y1,0,0,color,1,RA8876_DCR1_DRAW_SQUARE_gc);
}

void RA8876_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
	_RA8876_Curve(x0,y0,r,r,0,color,0,RA8876_DCR1_DRAW_CIRCLE_ELLIPSE_gc);
}

void RA8876_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
	_RA8876_Curve(x0,y0,r,r,0,color,1,RA8876_DCR1_DRAW_CIRCLE_ELLIPSE_gc);
}

void RA8876_drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	_RA8876_Triangle(x0,y0,x1,y1,x2,y2,color,0);
}

void RA8876_fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	_RA8876_Triangle(x0,y0,x1,y1,x2,y2,color,1);
}

void RA8876_drawEllipse(uint16_t x0, uint16_t y0, uint16_t r_w, uint16_t r_h, uint16_t color)
{
	_RA8876_Curve(x0,y0,r_w,r_h,0,color,0,RA8876_DCR1_DRAW_CIRCLE_ELLIPSE_gc);
}

void RA8876_fillEllipse(uint16_t x0, uint16_t y0, uint16_t r_w, uint16_t r_h, uint16_t color)
{
	_RA8876_Curve(x0,y0,r_w,r_h,0,color,1,RA8876_DCR1_DRAW_CIRCLE_ELLIPSE_gc);
}

void RA8876_drawRoundedRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t r_w, uint16_t r_h, uint16_t color)
{
	_RA8876_Rect(x0,y0,x1,y1,r_w,r_h,color,0,RA8876_DCR1_DRAW_CIRCLE_SQUARE_gc);
}

void RA8876_fillRoundedRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t r_w, uint16_t r_h, uint16_t color)
{
	_RA8876_Rect(x0,y0,x1,y1,r_w,r_h,color,1,RA8876_DCR1_DRAW_CIRCLE_SQUARE_gc);
}

void RA8876_drawCurve(uint16_t x0, uint16_t y0, uint16_t r_w, uint16_t r_h, enum RA8876_selectCurve _curve, uint16_t color)
{
	_RA8876_Curve(x0,y0,r_w,r_h,_curve,color,0,RA8876_DCR1_DRAW_CURVE_gc);
}

void RA8876_fillCurve(uint16_t x0, uint16_t y0, uint16_t r_w, uint16_t r_h, enum RA8876_selectCurve _curve, uint16_t color)
{
	_RA8876_Curve(x0,y0,r_w,r_h,_curve,color,1,RA8876_DCR1_DRAW_CURVE_gc);
}

//-------------- BTE AND ROP ------------------------------------------------------------

void RA8876_BTE_ROP_Code(enum RA8876_BTE_Operation bte_code, enum RA8876_ROP_Operation rop_code)
{
	RA8876_writeReg(RA8876_BTE_CTRL1, bte_code | rop_code);
}

void RA8876_BTE_Colors(enum RA8876_BTE_S0_color _s0, enum RA8876_BTE_S1_color _s1, enum RA8876_BTE_DEST_color _dest)
{
	RA8876_writeReg(RA8876_BTE_COLR, _s0 | _s1 | _dest);
}

void RA8876_BTE_S0_Address(uint32_t _addr)
{
	RA8876_writeReg(RA8876_S0_STR0,_addr);
	RA8876_writeReg(RA8876_S0_STR1,_addr>>8);
	RA8876_writeReg(RA8876_S0_STR2,_addr>>16);
	RA8876_writeReg(RA8876_S0_STR3,_addr>>24);
}

void RA8876_BTE_S0_Coords(uint16_t x0, uint16_t y0)
{
	RA8876_writeReg(RA8876_S0_X0,x0);
	RA8876_writeReg(RA8876_S0_X1,x0>>8);
	RA8876_writeReg(RA8876_S0_Y0,y0);
	RA8876_writeReg(RA8876_S0_Y1,y0>>8);
}

void RA8876_BTE_S0_Width(uint16_t width)
{
	RA8876_writeReg(RA8876_S0_WTH0,width);
	RA8876_writeReg(RA8876_S0_WTH1,width>>8);
}

void RA8876_BTE_S1_Address(uint32_t _addr)
{
	RA8876_writeReg(RA8876_S1_STR0,_addr);
	RA8876_writeReg(RA8876_S1_STR1,_addr>>8);
	RA8876_writeReg(RA8876_S1_STR2,_addr>>16);
	RA8876_writeReg(RA8876_S1_STR3,_addr>>24);
}

void RA8876_BTE_S1_Coords(uint16_t x0, uint16_t y0)
{
	RA8876_writeReg(RA8876_S1_X0,x0);
	RA8876_writeReg(RA8876_S1_X1,x0>>8);
	RA8876_writeReg(RA8876_S1_Y0,y0);
	RA8876_writeReg(RA8876_S1_Y1,y0>>8);
}

void RA8876_BTE_S1_Width(uint16_t width)
{
	RA8876_writeReg(RA8876_S1_WTH0,width);
	RA8876_writeReg(RA8876_S1_WTH1,width>>8);
}

void RA8876_BTE_S1_ConstColor64k(uint16_t color)
{
	RA8876_writeReg(RA8876_S1_RED, color >> 8);
	RA8876_writeReg(RA8876_S1_GREEN, color >> 3);
	RA8876_writeReg(RA8876_S1_BLUE, color << 3);
}

void RA8876_BTE_S1_ConstColorRGB(uint8_t red, uint8_t green, uint8_t blue)
{
	RA8876_writeReg(RA8876_S1_RED, red);
	RA8876_writeReg(RA8876_S1_GREEN, green);
	RA8876_writeReg(RA8876_S1_BLUE, blue);
}

void RA8876_BTE_Dest_Address(uint32_t _addr)
{
	RA8876_writeReg(RA8876_DT_STR0,_addr);
	RA8876_writeReg(RA8876_DT_STR1,_addr>>8);
	RA8876_writeReg(RA8876_DT_STR2,_addr>>16);
	RA8876_writeReg(RA8876_DT_STR3,_addr>>24);
}

void RA8876_BTE_Dest_Coords(uint16_t x0, uint16_t y0)
{
	RA8876_writeReg(RA8876_DT_X0,x0);
	RA8876_writeReg(RA8876_DT_X1,x0>>8);
	RA8876_writeReg(RA8876_DT_Y0,y0);
	RA8876_writeReg(RA8876_DT_Y1,y0>>8);
}

void RA8876_BTE_Dest_Width(uint16_t width)
{
	RA8876_writeReg(RA8876_DT_WTH0,width);
	RA8876_writeReg(RA8876_DT_WTH1,width>>8);
}

void RA8876_BTE_WindowSize(uint16_t width, uint16_t height)
{
	RA8876_writeReg(RA8876_BTE_WTH0,width);
	RA8876_writeReg(RA8876_BTE_WTH1,width>>8);
	RA8876_writeReg(RA8876_BTE_HIG0,height);
	RA8876_writeReg(RA8876_BTE_HIG1,height>>8);
}

void RA8876_BTE_AlphaBlending(uint8_t _opaque)
{
	RA8876_writeReg(RA8876_APB_CTRL, _opaque);
}

void RA8876_BTE_enable(void)
{
	RA8876_writeReg(RA8876_BTE_CTRL0, RA8876_BTE_CTRL0_STAT);
}

void RA8876_BTE_disable(void)
{
	RA8876_writeReg(RA8876_BTE_CTRL0, 0);
}

//-------------- Serial Flash / SPI-Master ----------------------------------------------

void _RA8876_accessMode(uint8_t _mode)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_SFL_CTRL);
	if (_mode)
	{
		reg_temp |= RA8876_SFL_CTRL_ACCESS;
	}
	else
	{
		reg_temp &= ~RA8876_SFL_CTRL_ACCESS;
	}
	RA8876_writeReg(RA8876_SFL_CTRL, reg_temp);
}

void RA8876_spi_selectFlash(uint8_t _0_or_1)
{
	uint8_t reg_temp0;
	uint8_t reg_temp1;
	reg_temp0 = RA8876_readReg(RA8876_SFL_CTRL);
	reg_temp1 = RA8876_readReg(RA8876_SPIMCR2);
	if (_0_or_1)
	{
		reg_temp0 |= RA8876_SFL_CTRL_SELECT;
		reg_temp1 |= RA8876_SPIMCR2_SS_PIN;
	}
	else
	{
		reg_temp0 &= ~RA8876_SFL_CTRL_SELECT;
		reg_temp1 &= ~RA8876_SPIMCR2_SS_PIN;
	}
	RA8876_writeReg(RA8876_SFL_CTRL, reg_temp0);
	RA8876_writeReg(RA8876_SPIMCR2, reg_temp1);
}

void RA8876_spi_DMA_mode(void)
{
	_RA8876_accessMode(1);
}

void RA8876_spi_Font_mode(void)
{
	_RA8876_accessMode(0);
}

void RA8876_spi_FlashAddressMode(enum RA8876_FlashAddressBits _bits)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_SFL_CTRL);
	reg_temp &= ~RA8876_SFL_CTRL_ADDRESS;
	reg_temp |= _bits;
	RA8876_writeReg(RA8876_SFL_CTRL, reg_temp);
}

void RA8876_spi_FlashReadMode(enum RA8876_FlashReadMode _reads)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_SFL_CTRL);
	reg_temp &= ~RA8876_SFL_CTRL_BEHAVIOR_MASK;
	reg_temp |= _reads;
	RA8876_writeReg(RA8876_SFL_CTRL, reg_temp);
}

void RA8876_spi_clockPeriod(uint8_t _divisor)
{
	RA8876_writeReg(RA8876_SPI_DIVSOR, _divisor);
}

void RA8876_spi_enable(void)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_CCR);
	reg_temp |= RA8876_CCR_SPI_ENABLE;
	RA8876_writeReg(RA8876_CCR, reg_temp);
}

void RA8876_spi_disable(void)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_CCR);
	reg_temp &= ~RA8876_CCR_SPI_ENABLE;
	RA8876_writeReg(RA8876_CCR, reg_temp);
}

void RA8876_spi_DMA_start(void)
{
	RA8876_writeReg(RA8876_DMA_CTRL, RA8876_DMA_CTRL_START);
}

uint8_t RA8876_spi_DMA_busy(void)
{
	return RA8876_readReg(RA8876_DMA_CTRL);
}

void RA8876_spi_mode(enum RA8876_SPI_MODE _mode)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_SPIMCR2);
	reg_temp &= ~RA8876_SPIMCR2_SPI_MODE_MASK;
	reg_temp |= _mode;
	RA8876_writeReg(RA8876_SPIMCR2, reg_temp);
}

enum RA8876_SPI_STATUS RA8876_spi_getStatus()
{
	return RA8876_readReg(RA8876_SPIMSR);
}

void RA8876_spi_slaveActive(void)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_SPIMCR2);
	reg_temp |= RA8876_SPIMCR2_SS_ACT;
	RA8876_writeReg(RA8876_SPIMCR2, reg_temp);
}

void RA8876_spi_slaveInactive(void)
{
	uint8_t reg_temp;
	reg_temp = RA8876_readReg(RA8876_SPIMCR2);
	reg_temp &=~ RA8876_SPIMCR2_SS_ACT;
	RA8876_writeReg(RA8876_SPIMCR2, reg_temp);
}

void RA8876_spi_send(uint8_t data)
{
	/* Puts data on the 16-entries deep Tx FIFO
	 * Check the SPI Status regarding Tx FIFO full / empty */
	RA8876_writeReg(RA8876_SPIDR, data);
}

uint8_t RA8876_spi_transfer(uint8_t data)
{
	uint8_t readedData;
	RA8876_writeReg(RA8876_SPIDR, data);
	
	// Wait until data has been send, thus a byte has been received
	while((RA8876_spi_getStatus() & SPI_TxFIFO_EMPTY) == 0);
	
	readedData = RA8876_readReg(RA8876_SPIDR);
	
	return readedData;
}

uint8_t RA8876_spi_get()
{
	/* Reads data from the 16-entries deep Rx FIFO
	 * Check the SPI Status regarding Rx FIFO full / empty */
	return RA8876_readReg(RA8876_SPIDR);
}
void RA8876_spi_DMA_flashAddress(uint32_t _addr)
{
	RA8876_writeReg(RA8876_DMA_SSTR0, _addr);
	RA8876_writeReg(RA8876_DMA_SSTR1, _addr >> 8);
	RA8876_writeReg(RA8876_DMA_SSTR2, _addr >> 16);
	RA8876_writeReg(RA8876_DMA_SSTR3, _addr >> 24);
}

void RA8876_spi_DMA_Dest_Coords(uint16_t x0, uint16_t y0)
{
	RA8876_writeReg(RA8876_DMA_DX0, x0);
	RA8876_writeReg(RA8876_DMA_DX1, x0 >> 8);
	RA8876_writeReg(RA8876_DMA_DY0, y0);
	RA8876_writeReg(RA8876_DMA_DY1, y0 >> 8);
}

void RA8876_spi_DMA_WindowSize(uint16_t width, uint16_t height)
{
	RA8876_writeReg(RA8876_DMAW_WTH0, width);
	RA8876_writeReg(RA8876_DMAW_WTH1, width >> 8);
	RA8876_writeReg(RA8876_DMAW_HIGH0, height);
	RA8876_writeReg(RA8876_DMAW_HIGH1, height >> 8);
}

void RA8876_spi_DMA_SrcWidth(uint16_t width)
{
	RA8876_writeReg(RA8876_DMA_SWTH0, width);
	RA8876_writeReg(RA8876_DMA_SWTH1, width >> 8);
}

// - SPI DRIVER INTERFACE FUNCTION FOR EXTERNAL LIBRARIES -
uint8_t RA8876_spi_InterfacePrepare(void *RASPI)
{
	RA8876_SPI_t *_spi = (RA8876_SPI_t*)RASPI;
	RA8876_spi_selectFlash(_spi->slaveSel);
	RA8876_spi_slaveActive();
	return 0;
}

uint8_t RA8876_spi_InterfaceSendBytes(void *RASPI, uint8_t addr,
					const uint8_t *buf_ptr,	uint32_t buf_len)
{
	uint32_t byteCnt;
	
	if (addr != 0)	RA8876_spi_send(addr);
	
	for(byteCnt = 0; byteCnt < buf_len; byteCnt++)
	{
		RA8876_spi_send(buf_ptr[byteCnt]);
	}
	
	return 0;
}

uint8_t RA8876_spi_InterfaceTransceiveBytes(void *RASPI, uint8_t addr,
						uint8_t *buf_ptr, uint32_t buf_len)
{
	uint32_t byteCnt;
	
	if (addr != 0)	RA8876_spi_send(addr);
	
	for(byteCnt = 0; byteCnt < buf_len; byteCnt++)
	{
		buf_ptr[byteCnt] = RA8876_spi_transfer(buf_ptr[byteCnt]);
	}
	
	return 0;
}

uint8_t RA8876_spi_InterfaceGetBytes(void *RASPI, uint8_t addr,
						uint8_t *buf_ptr, uint32_t buf_len)
{
	uint32_t byteCnt;
	
	if (addr != 0)	RA8876_spi_send(addr);
	
	for(byteCnt = 0; byteCnt < buf_len; byteCnt++)
	{
		buf_ptr[byteCnt] = RA8876_spi_get();
	}
	
	return 0;
}

uint8_t RA8876_spi_InterfaceFinish(void *RASPI)
{
	RA8876_spi_slaveInactive();
	return 0;
}

//-------------- I2C-Master -------------------------------------------------------------

void RA8876_i2c_clockDiv(uint16_t _clkdiv)
{
	RA8876_writeReg(RA8876_IICMCPR0, _clkdiv);
	RA8876_writeReg(RA8876_IICMCPR0, _clkdiv >> 8);
}

void RA8876_i2c_send(uint8_t data)
{
	RA8876_writeReg(RA8876_IICMTXR, data); 
}

uint8_t RA8876_i2c_get(void)
{
	return RA8876_readReg(RA8876_IICMRXR);
}

void RA8876_i2c_mode(enum RA8876_I2C_MODE _mode)
{
	RA8876_writeReg(RA8876_IICMCMDR, _mode);
}

enum RA8876_I2C_STATUS RA8876_i2c_status(void)
{
	return RA8876_readReg(RA8876_IICMSTUR);
}

//-------------- GPIO -------------------------------------------------------------------

void RA8876_gpio_setdir(enum RA8876_GPIO_PORT _port, uint8_t direction)
{
	switch (_port)
	{
		case RAGPIOA:
			RA8876_writeReg(RA8876_GPIOAD, direction);
			break;
		case RAGPIOC:
			RA8876_writeReg(RA8876_GPIOCD, direction);
			break;
		case RAGPIOD:
			RA8876_writeReg(RA8876_GPIODD, direction);
			break;
		case RAGPIOE:
			RA8876_writeReg(RA8876_GPIOED, direction);
			break;
		case RAGPIOF:
			RA8876_writeReg(RA8876_GPIOFD, direction);
			break;
		default:
			break;
	}
}

uint8_t RA8876_gpio_getdir(enum RA8876_GPIO_PORT _port)
{
	uint8_t reg_temp = 0;
	
	switch (_port)
	{
		case RAGPIOA:
			reg_temp = RA8876_readReg(RA8876_GPIOAD);
			break;
		case RAGPIOC:
			reg_temp = RA8876_readReg(RA8876_GPIOCD);
			break;
		case RAGPIOD:
			reg_temp = RA8876_readReg(RA8876_GPIODD);
			break;
		case RAGPIOE:
			reg_temp = RA8876_readReg(RA8876_GPIOED);
			break;
		case RAGPIOF:
			reg_temp = RA8876_readReg(RA8876_GPIOFD);
			break;
		default:
			break;
	}
	
	return reg_temp;
}

void RA8876_gpio_write(enum RA8876_GPIO_PORT _port, uint8_t data)
{
	switch (_port)
	{
		case RAGPIOA:
			RA8876_writeReg(RA8876_GPIOA, data);
			break;
		case RAGPIOC:
			RA8876_writeReg(RA8876_GPIOC, data);
			break;
		case RAGPIOD:
			RA8876_writeReg(RA8876_GPIOD, data);
			break;
		case RAGPIOE:
			RA8876_writeReg(RA8876_GPIOE, data);
			break;
		case RAGPIOF:
			RA8876_writeReg(RA8876_GPIOF, data);
			break;
		default:
			break;
	}
}

uint8_t RA8876_gpio_read(enum RA8876_GPIO_PORT _port)
{
	uint8_t reg_temp = 0;
	
	switch (_port)
	{
		case RAGPIOA:
			reg_temp = RA8876_readReg(RA8876_GPIOA);
			break;
		case RAGPIOC:
			reg_temp = RA8876_readReg(RA8876_GPIOC);
			break;
		case RAGPIOD:
			reg_temp = RA8876_readReg(RA8876_GPIOD);
			break;
		case RAGPIOE:
			reg_temp = RA8876_readReg(RA8876_GPIOE);
			break;
		case RAGPIOF:
			reg_temp = RA8876_readReg(RA8876_GPIOF);
			break;
		default:
			break;
	}
	
	return reg_temp;
}
