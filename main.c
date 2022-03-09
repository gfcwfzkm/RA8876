/*
 * hw_test_01.c
 *
 * Created: 22.10.2020 08:16:04
 * Author : gfcwfzkm
 */

#ifdef DEMO_EXAMPLE
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "io_map.h"
#include "RA8876.h"
#include "image_data_logo_8bpp_rle.h"
#include "image_data_pic1_rle.h"
#include "image_data_pic2_rle.h"
#include "image_data_pic3_rle.h"
#include "image_data_pic4_rle.h"
#include "graph_cursor.h"

void prepareSend(uint32_t _startAddr,uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
{
	while(RA8876_readStatus() & RA8876_CORE_BUSY);
	RA8876_setMode(GRAPHMODE);
	
	RA8876_BTE_Colors(BTE_S0_Color_8bpp, BTE_S1_Color_16bpp, BTE_Dest_Color_16bpp);
	RA8876_BTE_S0_Address(_startAddr);
	RA8876_BTE_S0_Coords(x0,y0);
	RA8876_BTE_S0_Width(RA8876_WIDTH);
	RA8876_BTE_Dest_Address(_startAddr);
	RA8876_BTE_Dest_Coords(x0,y0);
	RA8876_BTE_Dest_Width(RA8876_WIDTH);
	RA8876_BTE_WindowSize(width, height);
	RA8876_BTE_ROP_Code(BTE_MPU_WRITE_w_ROP, ROP_S0);
	RA8876_BTE_enable();
	
	RA8876_writeCMD(RA8876_MRWDP);
}

void send_8bpp_RLE(uint16_t img_length, const __memx uint8_t *img_progmem)
{
	uint16_t imgdata_cnt;
	uint8_t img_data;
	int8_t rle_inst = 0;
	
	for (imgdata_cnt = 0; imgdata_cnt < img_length; imgdata_cnt++)
	{
		// load next rle-compressed image data from program memory
		//img_data = pgm_read_byte(img_progmem + imgdata_cnt);
		img_data = img_progmem[imgdata_cnt];
		
		if (rle_inst > 0)
		{
			// If rle_inst is a positive number, repeat the image data rle_inst -times.
			for (; rle_inst > 0; rle_inst--)
			{
				RA8876_writeData(img_data);
			}
		}
		else if (rle_inst < 0)
		{
			// if rle_inst is negative, the next -1*(rle_inst) amount of bytes are uncompressed, so just send those.
			RA8876_writeData(img_data);
			rle_inst++;
		}
		else
		{
			// if rle_inst is 0, it's time to load the next instruction
			rle_inst = (int8_t)img_data;
		}
	}
}

void send_monochrome_RLE(uint16_t img_length, const __memx uint8_t *img_progmem, uint8_t fgColor, uint8_t bgColor)
{
	uint16_t imgdata_cnt;
	uint8_t img_data;
	int8_t rle_inst = 0;
	int8_t bit_cnt = 0;
	
	for (imgdata_cnt = 0; imgdata_cnt < img_length; imgdata_cnt++)
	{
		// load next rle-compressed image data from program memory
		//img_data = pgm_read_byte(img_progmem + imgdata_cnt);
		
		img_data = img_progmem[imgdata_cnt];
		
		if (rle_inst > 0)
		{
			// If rle_inst is a positive number, repeat the image data rle_inst -times.
			for (; rle_inst > 0; rle_inst--)
			{
				for (bit_cnt = 7; bit_cnt >= 0; bit_cnt--)
				{
					if (img_data & (1<<bit_cnt))
					{
						RA8876_writeData(bgColor);
					}
					else
					{
						RA8876_writeData(fgColor);
					}
				}
			}
		}
		else if (rle_inst < 0)
		{
			// if rle_inst is negative, the next -1*(rle_inst) amount of bytes are uncompressed, so just send those.
			for (bit_cnt = 7; bit_cnt >= 0; bit_cnt--)
			{
				if (img_data & (1<<bit_cnt))
				{
					RA8876_writeData(bgColor);
				}
				else
				{
					RA8876_writeData(fgColor);
				}
			}
			rle_inst++;
		}
		else
		{
			// if rle_inst is 0, it's time to load the next instruction
			rle_inst = (int8_t)img_data;
		}
	}
}

void bte_copy(uint32_t src_addr, uint16_t src_x, uint16_t src_y, uint32_t dest_addr, uint16_t dest_x, uint16_t dest_y, uint16_t img_width, uint16_t img_height)
{
	while(RA8876_readStatus() & RA8876_CORE_BUSY);
	
	RA8876_BTE_S0_Address(src_addr);
	RA8876_BTE_S0_Coords(src_x,src_y);
	RA8876_BTE_S0_Width(RA8876_WIDTH);
		
	RA8876_BTE_Dest_Address(dest_addr);
	RA8876_BTE_Dest_Coords(dest_x,dest_y);
	RA8876_BTE_Dest_Width(RA8876_WIDTH);
	RA8876_BTE_WindowSize(img_width,img_height);
	
	RA8876_BTE_Colors(BTE_S0_Color_16bpp, BTE_S1_Color_16bpp, BTE_Dest_Color_16bpp);
	RA8876_BTE_ROP_Code(BTE_MEM_COPY_MOVE_w_ROP, ROP_S0);
	RA8876_BTE_enable();
	
	while(RA8876_readStatus() & RA8876_CORE_BUSY);
}

void mcu_init()
{
	/* CPU-Takt auf 32 MHz einstellen */
	OSC.CTRL |= (OSC_RC32MEN_bm);						// 32MHz Oszillator aktivieren
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));				// Warten bis der 32MHz Oszillator stabil ist
	_PROTECTED_WRITE(CLK.CTRL, CLK_SCLKSEL_RC32M_gc);	// 32MHz als Haupttakt wählen
	OSC.CTRL &= ~OSC_RC2MEN_bm;							// 2MHz Takt deaktivieren
	
	/* IO Setup */
	PORTCFG.MPCMASK = PIN7_bm | PIN6_bm | PIN5_bm | PIN4_bm | PIN3_bm | PIN2_bm | PIN1_bm | PIN0_bm;
	DATAPORT.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
	
	ADDRPORT.DIRSET = ADDR_MCS | ADDR_BD3 | ADDR_BD2 | ADDR_BD1 | ADDR_BD0 | ADDR_IC2 | ADDR_IC1 | ADDR_IC0;
	
	SERPORT.DIRSET = SPI_SCK | SPI_MOSI | DB_RS | UART_TX |UART_XCK | DB_EN;
	SERPORT.PIN2CTRL = PORT_OPC_PULLDOWN_gc;	// UART_RX
	SERPORT.PIN6CTRL = PORT_OPC_PULLDOWN_gc;	// SPI_MISO
	
	MIXPORT.DIRSET = LCD_CS | MEM_CS | IMOSI | ISCK | DB_RW;
	MIXPORT.PIN2CTRL = PORT_OPC_PULLDOWN_gc;	// IMISO
	
	MISCPORT.DIRSET = DBG_TX;
	PORTCFG.MPCMASK = SDA | SCL;
	MISCPORT.PIN0CTRL = PORT_OPC_PULLUP_gc;
	PORTCFG.MPCMASK = DBG_RX | UNUSED | TOUCH_IRQ | PE_IRQ | RTC_SIG;
	MISCPORT.PIN7CTRL = PORT_OPC_PULLDOWN_gc;
	
	LCDPORT.DIRSET = LCD_RST;
	PORTCFG.MPCMASK = RE_SW | RE_A | RE_B;
	MISCPORT.PIN3CTRL = PORT_OPC_PULLDOWN_gc;
	PORTCFG.MPCMASK = LCD_WAIT | LCD_IRQ | IRQH | IRQM;
	MISCPORT.PIN1CTRL = PORT_OPC_PULLUP_gc;
	
	STATPORT.DIRSET = STATLED;
	STATPORT.PIN0CTRL = PORT_OPC_PULLUP_gc;
}

void disp_init()
{
	RA8876_hwReset();
	RA8876_init();	
	RA8876_brightness(1);
	RA8876_clearMemory(0xFFFF);
	RA8876_displayOnOff(1);
	//GT30L32S4W Font Chip used
}

struct testst {
	uint32_t a;
	uint8_t c;
	uint32_t b;
} tsts;

int main(void)
{
	mcu_init();
	disp_init();
	
	RA8876_setFColor64k(0);
	RA8876_setBColor64k(0xFFFF);
	RA8876_setTextCoords(0,0);
	RA8876_print_PROGMEM("Lade Bilder auf das Display...");
	
	prepareSend(LAYER_1,0,0,1024,442);
	send_8bpp_RLE(31332, image_data_logo_8bpp_rle);
	prepareSend(LAYER_1,45,430,200,140);
	send_8bpp_RLE(18795, image_data_pic1_rle);
	prepareSend(LAYER_1,288,430,200,140);
	send_8bpp_RLE(17785, image_data_pic2_rle);
	prepareSend(LAYER_1,532,430,200,140);
	send_8bpp_RLE(19453, image_data_pic3_rle);
	prepareSend(LAYER_1,780,430,200,140);
	send_8bpp_RLE(18995, image_data_pic4_rle);
	
	bte_copy(LAYER_1, 0, 0, LAYER_0, 0, 0, 1024, 442);
	
	// Transparent Background for Text
	RA8876_transparentOnOff(1);
	
	// Draw some fancy geometrics
	RA8876_drawPixel(1,1,0xf800);
	RA8876_drawLine(10,15,600,400,0x001f);
	RA8876_drawRect(700,200,900,550,0xF00F);
	RA8876_fillRect(500,400,600,500,0x07e0);
	RA8876_drawCircle(300,200,40,0xABCD);	
	RA8876_fillCircle(300,400,40,0x66CD);
	RA8876_drawTriangle(600,10,700,60,650,100,0xAB34);
	RA8876_fillTriangle(700,10,800,60,750,100,0xAB34);
	RA8876_drawRoundedRect(500,200,700,400,20,30,0x00FF);
	RA8876_fillRoundedRect(800,150,1000,350,20,30,0xF00F);	
	RA8876_drawCurve(200,450,150,100,CURVE_BOTTOM_LEFT, 0xFF00);
	RA8876_fillCurve(190,440,150,150,CURVE_TOP_LEFT, 0x0FF0);
	RA8876_drawCurve(210,440,150,100,CURVE_TOP_RIGHT, 0xF0F0);
	RA8876_fillCurve(210,450,100,50,CURVE_BOTTOM_RIGHT, 0x00FF);
	
	// Draw some internal-font text
	RA8876_setFColor64k(0);
	RA8876_setTextCoords(0,0);
	RA8876_FontInternalCGROM(ISO8859_1);
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_print_PROGMEM("Internal Font ISO / IEC 8859-1:");
	RA8876_setTextCoords(0,32);
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	for(uint16_t i = 0; i < 256; i++)
	{
		RA8876_char((char)i);
	}
	
	RA8876_setTextCoords(0,128);
	RA8876_setTextSize(TEXTSIZE_8x16_16x16);
	RA8876_print_PROGMEM("8x16 / 16x16 Font ");
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_print_PROGMEM("12x24 / 24x24 Font ");
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_print_PROGMEM("16x32 / 32x32 Font");
	
	// Draw some external font text (GT30L32S4W Font Chip on SS 0)
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_setTextCoords(0,176);
	RA8876_print_PROGMEM("External Font-Chip GT30L32S4W:");
	RA8876_fillRect(0, 208, 480, 210, 0);
	
	RA8876_FontGTFontROM();
	RA8876_selectGTFontChip(GT30L32S4W);
	RA8876_setTextSize(TEXTSIZE_8x16_16x16);
	RA8876_spi_selectFlash(0);
	RA8876_spi_Font_mode();
	RA8876_spi_FlashAddressMode(FLASH_24_BIT_ADDRESS);
	RA8876_spi_clockPeriod(4);
	RA8876_spi_enable();
	
	RA8876_setGTFontDecoder(SINGLE_BYTE_ASCII,WIDTH_FIXED);	
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_setTextCoords(0,216);
	RA8876_print_PROGMEM("Fixed Width ASCII:");
	RA8876_setTextCoords(0,240);
	RA8876_setTextSize(TEXTSIZE_8x16_16x16);
	RA8876_print_PROGMEM("8x16 / 16x16 Font ");
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_print_PROGMEM("12x24 / 24x24 Font ");
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_print_PROGMEM("16x32 / 32x32 Font");
	
	RA8876_setGTFontDecoder(SINGLE_BYTE_ASCII,WIDTH_VARIABLE_ARIAL);	
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_setTextCoords(0,288);
	RA8876_print_PROGMEM("Variable Width Arial:");
	RA8876_setTextCoords(0,312);
	RA8876_setTextSize(TEXTSIZE_8x16_16x16);
	RA8876_print_PROGMEM("8x16 / 16x16 Font ");
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_print_PROGMEM("12x24 / 24x24 Font ");
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_print_PROGMEM("16x32 / 32x32 Font");
	
	RA8876_setGTFontDecoder(SINGLE_BYTE_ASCII,WIDTH_VARIABLE_ROMAN);
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_setTextCoords(0,350);
	RA8876_print_PROGMEM("Variable Width NewTimesRoman:");
	RA8876_setTextCoords(0,384);
	RA8876_setTextSize(TEXTSIZE_8x16_16x16);
	RA8876_print_PROGMEM("8x16 / 16x16 Font ");
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_print_PROGMEM("12x24 / 24x24 Font ");
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_print_PROGMEM("16x32 / 32x32 Font");
	
	RA8876_spi_disable();
	RA8876_FontInternalCGROM(ISO8859_1);
	// https://lcd-image-converter.riuson.com/en/about/
	// https://www.microchip.com//wwwAppNotes/AppNotes.aspx?appnote=en591568
		
	bte_copy(LAYER_1, 45, 430, LAYER_0, 45, 430, 200, 140);
	bte_copy(LAYER_1, 288, 430, LAYER_0, 290, 430, 200, 140);
	bte_copy(LAYER_1, 532, 430, LAYER_0, 535, 430, 200, 140);
	bte_copy(LAYER_1, 780, 430, LAYER_0, 780, 430, 200, 140);
	
	RA8876_graphicCursorColor(0xFF, 0x00);
	
	RA8876_graphicCursorSelect(0);
	RA8876_graphicCursorLoad_f(gImage_pen_il);
	RA8876_graphicCursorSelect(1);
	RA8876_graphicCursorLoad_f(gImage_arrow_il);
	RA8876_graphicCursorSelect(2);
	RA8876_graphicCursorLoad_f(gImage_busy_im);
	RA8876_graphicCursorSelect(3);
	RA8876_graphicCursorLoad_f(gImage_no_im);
	
	RA8876_graphicCursorSelect(0);
	RA8876_graphicCursorCoords(300, 500);
	RA8876_graphicCursorEnable();
	
	RA8876_pipSelect(PIP_1, PIP_COLOR_DEPTH_16BPP);
	RA8876_pipImageStartAddress(LAYER_1);
	RA8876_pipImageCoords(288, 430);
	RA8876_pipImageWidth(RA8876_WIDTH);
	RA8876_pipWindowCoords(0,0);
	RA8876_pipWindowSize(200,140);
	RA8876_pipEnable(PIP_1);
	
	RA8876_pipSelect(PIP_2, PIP_COLOR_DEPTH_16BPP);
	RA8876_pipImageStartAddress(LAYER_1);
	RA8876_pipImageCoords(532, 430);
	RA8876_pipImageWidth(RA8876_WIDTH);
	RA8876_pipWindowCoords(600,400);
	RA8876_pipWindowSize(200,140);
	RA8876_pipEnable(PIP_2);
	
	uint16_t x0 = 0, y0 = 0, x1 = 600, y1 = 400;
	int8_t mulX0 = 1, mulY0 = 1, mulX1 = -1, mulY1 = 1;
	
	uint16_t xcur = 300, ycur = 500, curCnt = 0;
	int8_t mulXCur = 1, mulYCur = -1;
	uint8_t curSel = 0;
	while (1)
    {
		_delay_ms(5);
		
		if ((y0 >= (RA8876_HEIGHT-140)) && (mulY0 == 1)) mulY0 = -1;
		if ((y0 == 0) && (mulY0 == -1)) mulY0 = 1;		
		if ((x0 >= (RA8876_WIDTH-200)) && (mulX0 == 1)) mulX0 = -1;
		if ((x0 == 0) && (mulX0 == -1)) mulX0 = 1;
		
		x0 += mulX0;
		y0 += mulY0;
		
		if ((y1 >= (RA8876_HEIGHT-140)) && (mulY1 == 1)) mulY1 = -1;
		if ((y1 == 0) && (mulY1 == -1)) mulY1 = 1;
		if ((x1 >= (RA8876_WIDTH-200)) && (mulX1 == 1)) mulX1 = -1;
		if ((x1 == 0) && (mulX1 == -1)) mulX1 = 1;
		
		x1 += mulX1;
		y1 += mulY1;
		
		if ((ycur >= (RA8876_HEIGHT-32)) && (mulYCur == 1)) mulYCur = -1;
		if ((ycur == 0) && (mulYCur == -1)) mulYCur = 1;
		if ((xcur >= (RA8876_WIDTH-32)) && (mulXCur == 1)) mulXCur = -1;
		if ((xcur == 0) && (mulXCur == -1)) mulXCur = 1;
		
		if ((curCnt % 2) == 0)	xcur += mulXCur;
		if ((curCnt % 4) == 0)	ycur += mulYCur;
		
		if (++curCnt >= 400){
			curCnt = 0;
			if(++curSel >= 4)	curSel = 0;
			RA8876_graphicCursorSelect(curSel);
		}
		
		RA8876_pipSelect(PIP_1, PIP_COLOR_DEPTH_16BPP);
		RA8876_pipWindowCoords(x0, y0);
		
		RA8876_pipSelect(PIP_2, PIP_COLOR_DEPTH_16BPP);
		RA8876_pipWindowCoords(x1, y1);
		
		RA8876_graphicCursorCoords(xcur, ycur);
    }
}
#endif