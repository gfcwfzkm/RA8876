/*
 * RA8876.h
 * RA8876 (Low-Level) Library for Atmel (Microchip) AVR Micros (old / new generation, xmega)
 * Probably easy to port to other platforms
 * Uses 16-bit color by default
 * Version 0.8
 * - Mostly feature-complete
 * Created: 17.11.2020
 *  Author: gfcwfzkm
 */ 

#ifndef RA8876_H_
#define RA8876_H_

#include <avr/io.h>
#include <util/delay.h>
#include "io_map.h"
#include "RA8876_reg.h"

/* USER CONFIG FROM HERE */
// Select either 8bit parallel or SPI Interface
#define INTERFACE_PARALLEL_8BIT		1
//#define INTERFACE_SPI				1

// Library-to-MCU-IO
#define DIROUT(PORT,bit)	PORT.DIRSET = (bit)		// Set Pin(s) as Output
#define DIRIN(PORT,bit)		PORT.DIRCLR = (bit)		// Set Pin(s) as Input
#define CLRBIT(PORT,bit)	PORT.OUTCLR = (bit)		// Bit-clear Pin(s)
#define SETBIT(PORT,bit)	PORT.OUTSET = (bit)		// Bit-set Pin(s)
#define TSTBIT(PORT,bit)	(PORT.IN & (bit))		// Bit-read
// Pull-Up/Down on Data Port
#define DATA_PULLRESISTOR_ENABLE(dataport) do { \
		dataport.PIN0CTRL = PORT_OPC_PULLUP_gc;	\
		dataport.PIN1CTRL = PORT_OPC_PULLUP_gc; \
		dataport.PIN2CTRL = PORT_OPC_PULLUP_gc; \
		dataport.PIN3CTRL = PORT_OPC_PULLUP_gc; \
		dataport.PIN4CTRL = PORT_OPC_PULLUP_gc; \
		dataport.PIN5CTRL = PORT_OPC_PULLUP_gc; \
		dataport.PIN6CTRL = PORT_OPC_PULLUP_gc; \
		dataport.PIN7CTRL = PORT_OPC_PULLUP_gc; \
	}while(0)

#if INTERFACE_PARALLEL_8BIT
#define RA8876_CS			LCD_CS
#define RA8876_CS_PORT		MISCPORT
#define	RA8876_EN			DB_EN		// RD
#define RA8876_EN_PORT		SERPORT
#define RA8876_RW			DB_RW
#define RA8876_RW_PORT		MIXPORT
#define RA8876_RS			DB_RS		// A0
#define RA8876_RS_PORT		SERPORT
#define RA8876_WAIT			LCD_WAIT
#define RA8876_WAIT_PORT	LCDPORT
#define RA8876_INT			LCD_IRQ
#define RA8876_INT_PORT		LCDPORT
// If no Hardware-Reset available, uncomment the next three lines
#define RA8876_RST			LCD_RST
#define RA8876_RST_PORT		LCDPORT

#define RA8876_DATA_PORT	DATAPORT
#define RA8876_DATA_OUT		DATAPORT.OUT
#define RA8876_DATA_DIR		DATAPORT.DIR
#define RA8876_DATA_IN		DATAPORT.IN
#define DATA_OUT			0xFF
#define DATA_IN				0x00
#endif

#if INTERFACE_SPI
// Maximal 50MHz laut Datenblatt
#define RA8876_CS_PIN	LCD_CS
#define RA8876_CS_PORT	MISCPORT
#define RA8876_SPI_SLOW()	SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESC_DIV128_gc;
#define RA8876_SPI_FAST()	SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESC_DIV4_gc;
#define RA8876_SPI_WRITE(x)	SPI_exchangeData(x)
#define RA8876_SPI_READ()	SPI_exchangeData(0)
#define RA8876_SPI_CMDWRITE		0x00
#define RA8876_SPI_DATAWRITE	0x80
#define RA8876_SPI_DATAREAD		0xC0
#define RA8876_SPI_STATUSREAD	0x40
#endif 
/* USER CONFIG ENDS HERe */


/* FSTR(s) - Stores a string directly into AVR's flash memory.
 * 
 * Replacement for PSTR to get rid of the old pgmspace.h functions, and use the
 * more modern __flash / __memx equivalent.
 * Pretty much copied from PSTR's define and adapted to __memx.                   */
#define FSTR(s) (__extension__({static __memx const char __c[] = (s); &__c[0];}))

/* Display Setup Parameters - Do not change! Requires Code Adjustmets!
 * Display Code currently only set up to BuyDisplay's 10.1" RA8876 Display!*/
#define W9812G6JH	// 128Mbits Memory (134'217'728 Bits)
#define OSC_FREQ     10	  // crystal clcok
#define DRAM_FREQ    100  // SDRAM clock frequency, unti: MHz
#define CORE_FREQ    100  // Core (system) clock frequency, unit: MHz
#define SCAN_FREQ     50 // Panel Scan clock frequency, unit: MHz
#define TFT_INTERFACE	RA8876_CCR_TFT_24BIT_gc

#define RA8876_WIDTH		1024UL
#define RA8876_HEIGHT		600UL
#define RA8876_COLORBYTES	2		// Library is set up for 16-bit / 2-byte colors!
#define RA8876_CALC_LAYERADDR(x)	(x * RA8876_WIDTH * RA8876_HEIGHT * RA8876_COLORBYTES)

enum RA8876_status {
	RA8876_INT_PIN_STATE	= 0x01,
	RA8876_OPERATION_MODE	= 0x02,
	RA8876_SDRAM_READY		= 0x04,
	RA8876_CORE_BUSY		= 0x08,
	RA8876_MEMR_FIFO_EMPTY	= 0x10,
	RA8876_MEMR_FIFO_FULL	= 0x20,
	RA8876_MEMW_FIFO_EMPTY	= 0x40,
	RA8876_MEMW_FIFO_FULL	= 0x80	
};

/* Enum for the Canvas & Main Start Address of the Display.
 * With the memory installed on the buyDisplay Display, up to
 * 109 Layers at 16-bit colors are possible! So I only made Enum's for 10
 */
enum RA8876_memoryStartAddress {
	LAYER_0	= RA8876_CALC_LAYERADDR(0),
	LAYER_1	= RA8876_CALC_LAYERADDR(1),
	LAYER_2	= RA8876_CALC_LAYERADDR(2),
	LAYER_3	= RA8876_CALC_LAYERADDR(3),
	LAYER_4	= RA8876_CALC_LAYERADDR(4),
	LAYER_5	= RA8876_CALC_LAYERADDR(5),
	LAYER_6	= RA8876_CALC_LAYERADDR(6),
	LAYER_7	= RA8876_CALC_LAYERADDR(7),
	LAYER_8	= RA8876_CALC_LAYERADDR(8),
	LAYER_9	= RA8876_CALC_LAYERADDR(9)
};

enum RA8876_dispMode {
	GRAPHMODE = 0,
	TEXTMODE = 1
};

enum RA8876_selectMem {
	dest_SDRAM=0,
	dest_GC_RAM=RA8876_ICR_MEM_GC_RAM_gc,
	dest_CP_RAM=RA8876_ICR_MEM_CP_RAM_gc
};

enum RA8876_PIP_Select {
	PIP_1 = 0,
	PIP_2 = 1
};

enum RA8876_PIP_ColorDepth {
	PIP_COLOR_DEPTH_8BPP	= 0,
	PIP_COLOR_DEPTH_16BPP	= RA8876_PIPCDEP_PIP2_COLR_DPTH_0,
	PIP_COLOR_DEPTH_24BPP	= RA8876_PIPCDEP_PIP2_COLR_DPTH_1
};

enum RA8876_cursor {
	CURSOR_NOCURSOR = 0,
	CURSOR_IBEAM,
	CURSOR_UNDER,
	CURSOR_BLOCK
};

enum RA8876_intFont {
	ISO8859_1 = RA8876_CCR0_ISO8859_1_gc,
	ISO8859_2 = RA8876_CCR0_ISO8859_2_gc,
	ISO8859_3 = RA8876_CCR0_ISO8859_3_gc,
	ISO8859_4 = RA8876_CCR0_ISO8859_4_gc
};

enum RA8876_TextSize {
	TEXTSIZE_8x16_16x16		= RA8876_CCR0_8X16_16X16_gc,
	TEXTSIZE_12x24_24x24	= RA8876_CCR0_12X24_24X24_gc,
	TEXTSIZE_16x32_32x32	= RA8876_CCR0_16X32_32X32_gc,
};

enum RA8876_GT_Font_ROM {
	GT21L16T1W	= RA8876_GTFNT_GT21L16T1W_gc,
	GT30L16U2W	= RA8876_GTFNT_GT30L16U2W_gc,
	GT30L24T3Y	= RA8876_GTFNT_GT30L24T3Y_gc,
	GT30L24M1Z	= RA8876_GTFNT_GT30L24M1Z_gc,
	GT30L32S4W	= RA8876_GTFNT_GT30L32S4W_gc,
	GT20L24F6Y	= RA8876_GTFNT_GT20L24F6Y_gc,
	GT21L24S1W	= RA8876_GTFNT_GT21L24S1W_gc
};

enum RA8876_GT_Character_Set {
	SINGLE_BYTE_ASCII		= RA8876_GTFNT_CR_CHAR_2,
	SINGLE_BYTE_ISO8859_1	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_0,
	SINGLE_BYTE_ISO8859_2	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_1,
	SINGLE_BYTE_ISO8859_3	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_1 |
			RA8876_GTFNT_CR_CHAR_0,
	SINGLE_BYTE_ISO8859_4	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_2,
	SINGLE_BYTE_ISO8859_5	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_2 | 
			RA8876_GTFNT_CR_CHAR_0,
	SINGLE_BYTE_ISO8859_7	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_2 |
			RA8876_GTFNT_CR_CHAR_1,
	SINGLE_BYTE_ISO8859_8	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_2 | 
			RA8876_GTFNT_CR_CHAR_1 | RA8876_GTFNT_CR_CHAR_0,
	SINGLE_BYTE_ISO8859_9	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_3,
	SINGLE_BYTE_ISO8859_10	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_3 | 
			RA8876_GTFNT_CR_CHAR_0,
	SINGLE_BYTE_ISO8859_11	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_3 | 
			RA8876_GTFNT_CR_CHAR_1,
	SINGLE_BYTE_ISO8859_13	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_3 | 
			RA8876_GTFNT_CR_CHAR_1 | RA8876_GTFNT_CR_CHAR_0,
	SINGLE_BYTE_ISO8859_14	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_3 | 
			RA8876_GTFNT_CR_CHAR_2,
	SINGLE_BYTE_ISO8859_15	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_3 | 
			RA8876_GTFNT_CR_CHAR_2 | RA8876_GTFNT_CR_CHAR_0,
	SINGLE_BYTE_ISO8859_16	= RA8876_GTFNT_CR_CHAR_4 | RA8876_GTFNT_CR_CHAR_3 | 
			RA8876_GTFNT_CR_CHAR_2 | RA8876_GTFNT_CR_CHAR_1,
	DOUBLE_BYTE_GB2312		= 0,
	DOUBLE_BYTE_GB12345		= RA8876_GTFNT_CR_CHAR_0,
	DOUBLE_BYTE_BIG5		= RA8876_GTFNT_CR_CHAR_1,
	DOUBLE_BYTE_UNICODE		= RA8876_GTFNT_CR_CHAR_1 | RA8876_GTFNT_CR_CHAR_0,
	DOUBLE_BYTE_UNIJAPANESE	= RA8876_GTFNT_CR_CHAR_2 | RA8876_GTFNT_CR_CHAR_0,
	DOUBLE_BYTE_JIS0208		= RA8876_GTFNT_CR_CHAR_2 | RA8876_GTFNT_CR_CHAR_1,
	DOUBLE_BYTE_MISC		= RA8876_GTFNT_CR_CHAR_2 | RA8876_GTFNT_CR_CHAR_1 | 
			RA8876_GTFNT_CR_CHAR_0
};

enum RA8876_GT_Character_Width {
	WIDTH_FIXED				= 0,
	WIDTH_VARIABLE_ARIAL	= RA8876_GTFNT_CR_WIDTH_0,
	WIDTH_VARIABLE_ROMAN	= RA8876_GTFNT_CR_WIDTH_1,
	WIDTH_BOLD				= RA8876_GTFNT_CR_WIDTH_0 | RA8876_GTFNT_CR_WIDTH_1
};

enum RA8876_BTE_S0_color {
	BTE_S0_Color_8bpp	= 0,
	BTE_S0_Color_16bpp	= RA8876_BTE_COLR_S0_0,
	BTE_S0_Color_24bpp	= RA8876_BTE_COLR_S0_1
};

enum RA8876_BTE_S1_color {
	BTE_S1_Color_8bpp	= 0,
	BTE_S1_Color_16bpp	= RA8876_BTE_COLR_S1_0,
	BTE_S1_Color_24bpp	= RA8876_BTE_COLR_S1_1,
	BTE_S1_Const_Color	= RA8876_BTE_COLR_S1_1 | RA8876_BTE_COLR_S1_0,
	BTE_S1_8b_Alpha		= RA8876_BTE_COLR_S1_2,
	BTE_S1_16b_Alpha	= RA8876_BTE_COLR_S1_2 | RA8876_BTE_COLR_S1_0
};

enum RA8876_BTE_DEST_color {
	BTE_Dest_Color_8bpp	= 0,
	BTE_Dest_Color_16bpp	= RA8876_BTE_COLR_DEST_0,
	BTE_Dest_Color_24bpp	= RA8876_BTE_COLR_DEST_1
};

enum RA8876_selectCurve {
	CURVE_BOTTOM_LEFT	= 0,
	CURVE_TOP_LEFT		= RA8876_DCR1_PART_SEL_0,
	CURVE_TOP_RIGHT		= RA8876_DCR1_PART_SEL_1,
	CURVE_BOTTOM_RIGHT	= RA8876_DCR1_PART_SEL_1 | RA8876_DCR1_PART_SEL_0
};

enum RA8876_BTE_Operation {
	BTE_MPU_WRITE_w_ROP				= 0,
	BTE_MEM_COPY_MOVE_w_ROP			= RA8876_BTE_CTRL1_CODE_1,
	BTE_MPU_WRITE_w_CHROMA			= RA8876_BTE_CTRL1_CODE_2,
	BTE_MEM_COPY_MOVE_w_CHROMA		= RA8876_BTE_CTRL1_CODE_2 | RA8876_BTE_CTRL1_CODE_0,
	BTE_PATTER_FILL_w_ROP			= RA8876_BTE_CTRL1_CODE_2 | RA8876_BTE_CTRL1_CODE_1,
	BTE_PATTER_FILL_w_CHROMA		= RA8876_BTE_CTRL1_CODE_2 | RA8876_BTE_CTRL1_CODE_1 |
			RA8876_BTE_CTRL1_CODE_0,
	BTE_MPU_WRITE_w_COLOREXP		= RA8876_BTE_CTRL1_CODE_3,
	BTE_MPU_WRITE_w_COLOREXP_CHROMA	= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_0,
	BTE_MEM_COPY_w_OPACITY			= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_1,
	BTE_MPU_WRITE_w_OPACITY			= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_1 | 
			RA8876_BTE_CTRL1_CODE_0,
	BTE_SOLID_FILL					= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_2,
	BTE_MEM_COPY_w_COLOREXP			= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_2 | 
			RA8876_BTE_CTRL1_CODE_1,
	BTE_MEM_COPY_w_COLOREXP_CHROMA	= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_2 | 
			RA8876_BTE_CTRL1_CODE_1 | RA8876_BTE_CTRL1_CODE_0
};

enum RA8876_ROP_Operation {
	ROP_BLACKNESS					= 0,
	ROP_NOTS0_AND_NOTS1				= RA8876_BTE_CTRL1_ROP_0,
	ROP_NOTS0_AND_S1				= RA8876_BTE_CTRL1_ROP_1,
	ROP_NOT_S0						= RA8876_BTE_CTRL1_ROP_1 | RA8876_BTE_CTRL1_ROP_0,
	ROP_S0_AND_NOTS1				= RA8876_BTE_CTRL1_ROP_2,
	ROP_NOT_S1						= RA8876_BTE_CTRL1_ROP_2 | RA8876_BTE_CTRL1_ROP_0,
	ROP_S0_XOR_S1					= RA8876_BTE_CTRL1_ROP_2 | RA8876_BTE_CTRL1_ROP_1,
	ROP_NOTS0_OR_NOTS1				= RA8876_BTE_CTRL1_ROP_2 | RA8876_BTE_CTRL1_ROP_1 | 
			RA8876_BTE_CTRL1_ROP_0,
	ROP_S0_AND_S1					= RA8876_BTE_CTRL1_ROP_3,
	ROP_NOT_S0XORS1					= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_0,
	ROP_S1							= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_1,
	ROP_NOTS0_OR_S1					= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_1 | 
			RA8876_BTE_CTRL1_ROP_0,
	ROP_S0							= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_2,
	ROP_S0_OR_NOTS1					= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_2 | 
			RA8876_BTE_CTRL1_ROP_0,
	ROP_S0_OR_S1					= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_2 | 
			RA8876_BTE_CTRL1_ROP_1,
	ROP_ROP_WHITENESS				= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_2 | 
			RA8876_BTE_CTRL1_ROP_1 | RA8876_BTE_CTRL1_ROP_0
};

enum RA8876_SPI_STATUS {
	SPI_TxFIFO_EMPTY	= 0x80,
	SPI_TxFIFO_FULL		= 0x40,
	SPI_RxFIFO_EMPTY	= 0x20,
	SPI_RxFIFO_FULL		= 0x10,
	SPI_OVF_INTFLAG		= 0x08,
	SPI_IDLE_INTFLAG	= 0x04
};

enum RA8876_SPI_MODE {
	SPI_MODE_0 = 0,
	SPI_MODE_1 = RA8876_SPIMCR2_SPI_MODE_0,
	SPI_MODE_2 = RA8876_SPIMCR2_SPI_MODE_1,
	SPI_MODE_3 = RA8876_SPIMCR2_SPI_MODE_0 | RA8876_SPIMCR2_SPI_MODE_1
};

enum RA8876_FlashAddressBits {
	FLASH_24_BIT_ADDRESS = 0,
	FLASH_32_BIT_ADDRESS = RA8876_SFL_CTRL_ADDRESS
};

enum RA8876_FlashReadMode {
	NORMAL_NO_DUMMY_READ	= 0,
	NORMAL_ONE_DUMMY_READ	= RA8876_SFL_CTRL_BEHAVIOR_2,
	NORMAL_TWO_DUMMY_READ	= RA8876_SFL_CTRL_BEHAVIOR_3,
	DUALMODE_TWO_DUMMY_READ	= RA8876_SFL_CTRL_BEHAVIOR_1,
	DUALMODE_ONE_DUMMY_READ = RA8876_SFL_CTRL_BEHAVIOR_1 |
			RA8876_SFL_CTRL_BEHAVIOR_0
};

enum RA8876_I2C_MODE {
	I2C_MODE_START	= RA8876_IICMCMDR_START,
	I2C_MODE_STOP	= RA8876_IICMCMDR_STOP,
	I2C_MODE_READ	= RA8876_IICMCMDR_READ,
	I2C_MODE_WRITE	= RA8876_IICMCMDR_WRITE,
	I2C_MODE_NACK	= RA8876_IICMCMDR_NACK,
	I2C_MODE_ACK	= 0
};

enum RA8876_I2C_STATUS {
	I2C_STATUS_NO_ACK_RECEIVED	= RA8876_IICMSTUR_NACK_REC,
	I2C_STATUS_BUS_BUSY			= RA8876_IICMSTUR_BUSY,
	I2C_TRANSFERING_DATA		= RA8876_IICMSTUR_TRANS,
	I2C_ARBITRATION_LOST		= RA8876_IICMSTUR_LOST
};

enum RA8876_GPIO_PORT {
	GPIOA = 1,
	GPIOC = 2,
	GPIOD = 3,
	GPIOE = 4,
	GPIOF = 5
};

//-------------- Initialisierung --------------------------------------------------------
void RA8876_init();	// Display und die I/Os initialisieren
//-------------- LOW LEVEL --------------------------------------------------------------
void RA8876_writeCMD(uint8_t _cmd);				// Register/Befehl senden
void RA8876_writeData(uint8_t data);			// Daten senden
uint8_t RA8876_readData();						// Daten lesen
enum RA8876_status RA8876_readStatus(void);		// Statusregister lesen
//-------------- HARDWARE ---------------------------------------------------------------
void RA8876_writeReg(uint8_t _reg, uint8_t _val);
uint8_t RA8876_readReg(uint8_t _reg);
void RA8876_waitReg(uint8_t reg, uint8_t flag);
void RA8876_swReset();
void RA8876_hwReset();
void RA8876_displayOnOff(uint8_t _onoff);
void RA8876_setMode(enum RA8876_dispMode textMode);
enum RA8876_dispMode RA8876_getMode(void);
void RA8876_Memory_Select(enum RA8876_selectMem _selectedMemory);
void RA8876_clearMemory(uint16_t fbg);
void RA8876_brightness(uint8_t val);
//-------------- DISPLAY / CANVAS / AREA ------------------------------------------------
void RA8876_MainWindowStartAddress(enum RA8876_memoryStartAddress _addr);
void RA8876_MainWindowWidth(uint16_t width);
void RA8876_MainWindowStartXY(uint16_t x0, uint16_t y0);
void RA8876_CanvasStartAddr(enum RA8876_memoryStartAddress _addr);
void RA8876_CanvasWidth(uint16_t width);
void RA8876_setActiveArea(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height);
void RA8876_pipSelect(enum RA8876_PIP_Select _pip_sel, enum RA8876_PIP_ColorDepth _clr);
void RA8876_pipImageStartAddress(uint32_t _addr);
void RA8876_pipImageCoords(uint16_t x0, uint16_t y0);
void RA8876_pipImageWidth(uint16_t width);
void RA8876_pipWindowCoords(uint16_t x0, uint16_t y0);
void RA8876_pipWindowSize(uint16_t width, uint8_t height);
void RA8876_pipEnable(enum RA8876_PIP_Select _pip_sel);
void RA8876_pipDisable(enum RA8876_PIP_Select _pip_sel);
//-------------- COLORS -----------------------------------------------------------------
void RA8876_setFColorRGB(uint8_t red, uint8_t green, uint8_t blue);
void RA8876_setBColorRGB(uint8_t red, uint8_t green, uint8_t blue);
void RA8876_setFColor64k(uint16_t fgColor);
void RA8876_setBColor64k(uint16_t bgColor);
void RA8876_transparentOnOff(uint8_t _transOn);
void RA8876_setColor(uint16_t fgColor, uint16_t bgColor, uint16_t Transparent);
//-------------- TEXT -------------------------------------------------------------------
void RA8876_textCursorBlink(uint8_t _blinkRate, enum RA8876_cursor cursorType);
void RA8876_setTextSizeEnlargement(uint8_t mul_width, uint8_t mul_height);
void RA8876_setTextCoords(uint16_t x0, uint16_t y0);
void RA8876_setTextSize(enum RA8876_TextSize txtSize);
void RA8876_FontInternalCGROM(enum RA8876_intFont _font);
void RA8876_FontGTFontROM(void);
void RA8876_selectGTFontChip(enum RA8876_GT_Font_ROM _gt_rom);
void RA8876_setGTFontDecoder(enum RA8876_GT_Character_Set _chr,
	enum RA8876_GT_Character_Width _wdh);
void RA8876_graphicCursorEnable(void);
void RA8876_graphicCursorDisable(void);
void RA8876_graphicCursorColor(uint8_t gcc0, uint8_t gcc1);
void RA8876_graphicCursorSelect(uint8_t _cursor_sel);
void RA8876_graphicCursorLoad(const uint8_t *gcursor);
void RA8876_graphicCursorLoad_f(const __memx uint8_t *gcursor);
void RA8876_graphicCursorCoords(uint16_t x0, uint16_t y0);
void RA8876_setLineGap(uint8_t _linetoline_gap);
void RA8876_setCharGap(uint8_t _chartochar_gap);
void RA8876_printText(const char *text, uint16_t textLength);
void RA8876_char(char c);
void RA8876_print(char *text);
void RA8876_print_f(const __memx char *progmem_s);
#define RA8876_print_PROGMEM(__s)	RA8876_print_f(FSTR(__s))
//-------------- Grafik -----------------------------------------------------------------
void RA8876_setPixelCoords(uint16_t x0, uint16_t y0);	// X und Y Position setzten
void RA8876_drawPixel(uint16_t x0, uint16_t y0, uint16_t color);
void RA8876_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void RA8876_drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void RA8876_fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void RA8876_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void RA8876_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void RA8876_drawTriangle(uint16_t x0, uint16_t y0,
	uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void RA8876_fillTriangle(uint16_t x0, uint16_t y0,
	uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void RA8876_drawEllipse(uint16_t x0, uint16_t y0,
	uint16_t r_w, uint16_t r_h, uint16_t color);
void RA8876_fillEllipse(uint16_t x0, uint16_t y0,
	uint16_t r_w, uint16_t r_h, uint16_t color);
void RA8876_drawRoundedRect(uint16_t x0, uint16_t y0,
	uint16_t x1, uint16_t y1, uint16_t r_w, uint16_t r_h, uint16_t color);
void RA8876_fillRoundedRect(uint16_t x0, uint16_t y0,
	uint16_t x1, uint16_t y1, uint16_t r_w, uint16_t r_h, uint16_t color);
void RA8876_drawCurve(uint16_t x0, uint16_t y0, uint16_t r_w, uint16_t r_h,
	enum RA8876_selectCurve _curve, uint16_t color);
void RA8876_fillCurve(uint16_t x0, uint16_t y0, uint16_t r_w, uint16_t r_h,
	enum RA8876_selectCurve _curve, uint16_t color);
//-------------- BTE AND ROP BASIC FUNCTIONS --------------------------------------------
void RA8876_BTE_ROP_Code(enum RA8876_BTE_Operation bte_code,
	enum RA8876_ROP_Operation rop_code);
void RA8876_BTE_Colors(enum RA8876_BTE_S0_color _s0,
	enum RA8876_BTE_S1_color _s1, enum RA8876_BTE_DEST_color _dest);
void RA8876_BTE_S0_Address(uint32_t _addr);
void RA8876_BTE_S0_Coords(uint16_t x0, uint16_t y0);
void RA8876_BTE_S0_Width(uint16_t width);
void RA8876_BTE_S1_Address(uint32_t _addr);
void RA8876_BTE_S1_Coords(uint16_t x0, uint16_t y0);
void RA8876_BTE_S1_Width(uint16_t width);
void RA8876_BTE_S1_ConstColor64k(uint16_t color);
void RA8876_BTE_S1_ConstColorRGB(uint8_t red, uint8_t green, uint8_t blue);
void RA8876_BTE_Dest_Address(uint32_t _addr);
void RA8876_BTE_Dest_Coords(uint16_t x0, uint16_t y0);
void RA8876_BTE_Dest_Width(uint16_t width);
void RA8876_BTE_WindowSize(uint16_t width, uint16_t height);
void RA8876_BTE_AlphaBlending(uint8_t _opaque);
void RA8876_BTE_enable(void);
void RA8876_BTE_disable(void);
//-------------- Serial Flash / SPI-Master ----------------------------------------------
void RA8876_spi_selectFlash(uint8_t _0_or_1); // 0 is usually Font Rom, 1 is SPI Flash
void RA8876_spi_DMA_mode(void);
void RA8876_spi_Font_mode(void);
void RA8876_spi_FlashAddressMode(enum RA8876_FlashAddressBits _bits);
void RA8876_spi_FlashDummyRead(enum RA8876_FlashReadMode _reads);
void RA8876_spi_clockPeriod(uint8_t _divisor);
void RA8876_spi_enable(void);
void RA8876_spi_disable(void);
void RA8876_spi_DMA_start(void);
uint8_t RA8876_spi_DMA_busy(void);
void RA8876_spi_mode(enum RA8876_SPI_MODE _mode);
enum RA8876_SPI_STATUS RA8876_spi_getStatus();
void RA8876_spi_slaveActive(void);
void RA8876_spi_slaveInactive(void);
void RA8876_spi_send(uint8_t data);
uint8_t RA8876_spi_get(void);
void RA8876_spi_DMA_flashAddress(uint32_t _addr);
void RA8876_spi_DMA_DestCoords(uint16_t x0, uint16_t y0);
void RA8876_spi_DMA_WindowSize(uint16_t width, uint16_t height);
void RA8876_spi_DMA_SrcWidth(uint16_t width);
//-------------- I2C-Master -------------------------------------------------------------
void RA8876_i2c_clockDiv(uint16_t _clkdiv);
void RA8876_i2c_send(uint8_t data);
uint8_t RA8876_i2c_get(void);
void RA8876_i2c_mode(enum RA8876_I2C_MODE _mode);
enum RA8876_I2C_STATUS RA8876_i2c_status(void);
//-------------- GPIO -------------------------------------------------------------------
void RA8876_gpio_setdir(enum RA8876_GPIO_PORT _port, uint8_t direction);
uint8_t RA8876_gpio_getdir(enum RA8876_GPIO_PORT _port);
void RA8876_gpio_write(enum RA8876_GPIO_PORT _port, uint8_t data);
uint8_t RA8876_gpio_read(enum RA8876_GPIO_PORT _port);

#endif /* RA8876_H_ */