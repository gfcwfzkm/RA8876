/*
 * RA8876.h
 * RA8876 Library for Atmel (Microchip) AVR Micros, probably
 * easy to port to other micros
 * Version 0.1
 * Todo:
 * - SPI Interface
 * Created: 17.11.2020
 *  Author: gfcwfzkm
 */ 

#ifndef RA8876_H_
#define RA8876_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "io_map.h"	// IO Definition of my mcu test setup
#include "RA8876_reg.h"

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

#define INTERFACE_PARALLEL_8BIT		1
//#define INTERFACE_SPI				1

// Driver-to-MCU-IO
#define DIROUT(PORT,bit)	PORT.DIRSET = (bit)
#define DIRIN(PORT,bit)	PORT.DIRCLR = (bit)
#define CLRBIT(PORT,bit)	PORT.OUTCLR = (bit)		// Bit-l�schen
#define SETBIT(PORT,bit)	PORT.OUTSET = (bit)		// Bit-setzten
#define TSTBIT(PORT,bit)	(PORT.IN & (bit))		// Bit-lesen

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

enum RA8876_dispMode {
	GRAPHMODE = 0,
	TEXTMODE = 1
};

enum RA8876_selectMem {
	dest_SDRAM=0,
	dest_GC_RAM=RA8876_ICR_MEM_GC_RAM_gc,
	dest_CP_RAM=RA8876_ICR_MEM_CP_RAM_gc
};

enum RA8876_TextSize {
	TEXTSIZE_8x16_16x16		= RA8876_CCR0_8X16_16X16_gc,
	TEXTSIZE_12x24_24x24	= RA8876_CCR0_12X24_24X24_gc,
	TEXTSIZE_16x32_32x32	= RA8876_CCR0_16X32_32X32_gc,
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
	MPU_WRITE_w_ROP				= 0,
	MEM_COPY_MOVE_w_ROP			= RA8876_BTE_CTRL1_CODE_1,
	MPU_WRITE_w_CHROMA			= RA8876_BTE_CTRL1_CODE_2,
	MEM_COPY_MOVE_w_CHROMA		= RA8876_BTE_CTRL1_CODE_2 | RA8876_BTE_CTRL1_CODE_0,
	PATTER_FILL_w_ROP			= RA8876_BTE_CTRL1_CODE_2 | RA8876_BTE_CTRL1_CODE_1,
	PATTER_FILL_w_CHROMA		= RA8876_BTE_CTRL1_CODE_2 | RA8876_BTE_CTRL1_CODE_1 |
			RA8876_BTE_CTRL1_CODE_0,
	MPU_WRITE_w_COLOREXP		= RA8876_BTE_CTRL1_CODE_3,
	MPU_WRITE_w_COLOREXP_CHROMA	= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_0,
	MEM_COPY_w_OPACITY			= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_1,
	MPU_WRITE_w_OPACITY			= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_1 | 
			RA8876_BTE_CTRL1_CODE_0,
	SOLID_FILL					= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_2,
	MEM_COPY_w_COLOREXP			= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_2 | 
			RA8876_BTE_CTRL1_CODE_1,
	MEM_COPY_w_COLOREXP_CHROMA	= RA8876_BTE_CTRL1_CODE_3 | RA8876_BTE_CTRL1_CODE_2 | 
			RA8876_BTE_CTRL1_CODE_1 | RA8876_BTE_CTRL1_CODE_0
};

enum RA8876_ROP_Operation {
	ROP_BLACKNESS				= 0,
	NOTS0_AND_NOTS1				= RA8876_BTE_CTRL1_ROP_0,
	NOTS0_AND_S1				= RA8876_BTE_CTRL1_ROP_1,
	NOT_S0						= RA8876_BTE_CTRL1_ROP_1 | RA8876_BTE_CTRL1_ROP_0,
	S0_AND_NOTS1				= RA8876_BTE_CTRL1_ROP_2,
	NOT_S1						= RA8876_BTE_CTRL1_ROP_2 | RA8876_BTE_CTRL1_ROP_0,
	S0_XOR_S1					= RA8876_BTE_CTRL1_ROP_2 | RA8876_BTE_CTRL1_ROP_1,
	NOTS0_OR_NOTS1				= RA8876_BTE_CTRL1_ROP_2 | RA8876_BTE_CTRL1_ROP_1 | 
			RA8876_BTE_CTRL1_ROP_0,
	S0_AND_S1					= RA8876_BTE_CTRL1_ROP_3,
	NOT_S0XORS1					= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_0,
	S1							= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_1,
	NOTS0_OR_S1					= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_1 | 
			RA8876_BTE_CTRL1_ROP_0,
	S0							= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_2,
	S0_OR_NOTS1					= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_2 | 
			RA8876_BTE_CTRL1_ROP_0,
	S0_OR_S1					= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_2 | 
			RA8876_BTE_CTRL1_ROP_1,
	ROP_WHITENESS				= RA8876_BTE_CTRL1_ROP_3 | RA8876_BTE_CTRL1_ROP_2 | 
			RA8876_BTE_CTRL1_ROP_1 | RA8876_BTE_CTRL1_ROP_0
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
uint16_t RA8876_getActiveAreaX(void);
uint16_t RA8876_getActiveAreaY(void);
uint16_t RA8876_getActiveAreaWidth(void);
uint16_t RA8876_getActiveAreaHeight(void);
void RA8876_setActiveArea(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height);
//-------------- COLORS -----------------------------------------------------------------
void RA8876_setFColorRGB(uint8_t red, uint8_t green, uint8_t blue);
void RA8876_setBColorRGB(uint8_t red, uint8_t green, uint8_t blue);
void RA8876_setFColor64k(uint16_t fgColor);
void RA8876_setBColor64k(uint16_t bgColor);
void RA8876_transparentOnOff(uint8_t _transOn);
void RA8876_setColor(uint16_t fgColor, uint16_t bgColor, uint16_t Transparent);
//-------------- TEXT -------------------------------------------------------------------
void RA8876_cursorBlink(uint8_t _blinkRate, enum RA8876_cursor cursorType);
void RA8876_setTextSizeEnlargement(uint8_t mul_width, uint8_t mul_height);
void RA8876_setTextXY(uint16_t x0, uint16_t y0);
void RA8876_setTextSize(enum RA8876_TextSize txtSize);
void RA8876_FontInternalCGROM(enum RA8876_intFont _font);
void RA8876_FontGTFontROM(void);
void RA8876_SelectGTFontChip(enum RA8876_GT_Font_ROM _gt_rom);
void RA8876_SetGTFontDecoder(enum RA8876_GT_Character_Set _chr,
	enum RA8876_GT_Character_Width _wdh);
void RA8876_printText(const char *text, uint16_t textLength);
void RA8876_char(char c);
void RA8876_print(char *text);
void RA8876_print_p(const char *progmem_s);
#define RA8876_print_PROGMEM(__s)	RA8876_print_p(PSTR(__s))
//-------------- Grafik -----------------------------------------------------------------
void RA8876_setPixelPos(uint16_t x0, uint16_t y0);	// X und Y Position setzten
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
//-------------- BTE AND ROP ------------------------------------------------------------
// WIP
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

//-------------- Serial Flash / SPI-Master ----------------------------------------------
void RA8876_spi_selectFlash(uint8_t _0_or_1);
void RA8876_spi_DMA_mode(void);
void RA8876_spi_Font_mode(void);
void RA8876_spi_FlashAddressMode(enum RA8876_FlashAddressBits _bits);
void RA8876_spi_FlashDummyRead(enum RA8876_FlashReadMode _reads);
void RA8876_spi_clockPeriod(uint8_t _divisor);
void RA8876_spi_enable(void);
void RA8876_spi_disable(void);

//-------------- I2C-Master -------------------------------------------------------------

#endif /* RA8876_H_ */