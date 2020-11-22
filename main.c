/*
 * hw_test_01.c
 *
 * Created: 22.10.2020 08:16:04
 * Author : gfcwfzkm
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "io_map.h"
#include "RA8876.h"

void mcu_init()
{
	/* CPU-Takt auf 32 MHz einstellen */
	OSC.CTRL |= (OSC_RC32MEN_bm);						// 32MHz Oszillator aktivieren
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));				// Warten bis der 32MHz Oszillator stabil ist
	_PROTECTED_WRITE(CLK.CTRL, CLK_SCLKSEL_RC32M_gc);	// 32MHz als Haupttakt w�hlen
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

int main(void)
{
	mcu_init();
	disp_init();
	
	// Transparent Background for Text
	RA8876_transparentOnOff(1);
	
	// Draw some fancy geometrics
	RA8876_drawPixel(1,1,0xf800);
	RA8876_drawLine(10,15,600,400,0x001f);
	RA8876_drawRect(700,200,900,550,0xFFFF);
	RA8876_fillRect(500,400,600,500,0x07e0);
	RA8876_drawCircle(300,200,40,0xABCD);	
	RA8876_fillCircle(300,400,40,0x66CD);
	RA8876_drawTriangle(600,10,700,60,650,100,0xAB34);
	RA8876_fillTriangle(700,10,800,60,750,100,0xAB34);
	RA8876_drawRoundedRect(500,200,700,400,20,30,0x00FF);
	RA8876_fillRoundedRect(800,200,1000,400,20,30,0xF00F);	
	RA8876_drawCurve(200,450,150,100,CURVE_BOTTOM_LEFT, 0xFF00);
	RA8876_fillCurve(190,440,150,150,CURVE_TOP_LEFT, 0x0FF0);
	RA8876_drawCurve(210,440,150,100,CURVE_TOP_RIGHT, 0xF0F0);
	RA8876_fillCurve(210,450,100,50,CURVE_BOTTOM_RIGHT, 0x00FF);
	
	// Draw some internal-font text
	RA8876_setFColor64k(0);
	RA8876_setTextXY(0,0);
	RA8876_FontInternalCGROM(ISO8859_1);
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_print_PROGMEM("Internal Font ISO / IEC 8859-1:");
	RA8876_setTextXY(0,32);
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	for(uint16_t i = 0; i < 256; i++)
	{
		RA8876_char((char)i);
	}
	
	RA8876_setTextXY(0,128);
	RA8876_setTextSize(TEXTSIZE_8x16_16x16);
	RA8876_print_PROGMEM("8x16 / 16x16 Font ");
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_print_PROGMEM("12x24 / 24x24 Font ");
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_print_PROGMEM("16x32 / 32x32 Font");
	
	// Draw some external font text (GT30L32S4W Font Chip on SS 0)
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_setTextXY(0,176);
	RA8876_print_PROGMEM("External Font-Chip GT30L32S4W:");
	RA8876_fillRect(0, 208, 480, 210, 0);
	
	RA8876_FontGTFontROM();
	RA8876_SelectGTFontChip(GT30L32S4W);
	RA8876_setTextSize(TEXTSIZE_8x16_16x16);
	RA8876_spi_selectFlash(0);
	RA8876_spi_Font_mode();
	RA8876_spi_FlashAddressMode(FLASH_24_BIT_ADDRESS);
	RA8876_spi_clockPeriod(4);
	RA8876_spi_enable();
	
	RA8876_SetGTFontDecoder(SINGLE_BYTE_ASCII,WIDTH_FIXED);	
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_setTextXY(0,216);
	RA8876_print_PROGMEM("Fixed Width ASCII:");
	RA8876_setTextXY(0,240);
	RA8876_setTextSize(TEXTSIZE_8x16_16x16);
	RA8876_print_PROGMEM("8x16 / 16x16 Font ");
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_print_PROGMEM("12x24 / 24x24 Font ");
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_print_PROGMEM("16x32 / 32x32 Font");
	
	RA8876_SetGTFontDecoder(SINGLE_BYTE_ASCII,WIDTH_VARIABLE_ARIAL);	
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_setTextXY(0,288);
	RA8876_print_PROGMEM("Variable Width Arial:");
	RA8876_setTextXY(0,312);
	RA8876_setTextSize(TEXTSIZE_8x16_16x16);
	RA8876_print_PROGMEM("8x16 / 16x16 Font ");
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_print_PROGMEM("12x24 / 24x24 Font ");
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_print_PROGMEM("16x32 / 32x32 Font");
	
	RA8876_SetGTFontDecoder(SINGLE_BYTE_ASCII,WIDTH_VARIABLE_ROMAN);
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_setTextXY(0,350);
	RA8876_print_PROGMEM("Variable Width NewTimesRoman:");
	RA8876_setTextXY(0,384);
	RA8876_setTextSize(TEXTSIZE_8x16_16x16);
	RA8876_print_PROGMEM("8x16 / 16x16 Font ");
	RA8876_setTextSize(TEXTSIZE_12x24_24x24);
	RA8876_print_PROGMEM("12x24 / 24x24 Font ");
	RA8876_setTextSize(TEXTSIZE_16x32_32x32);
	RA8876_print_PROGMEM("16x32 / 32x32 Font");
	
	RA8876_spi_disable();
	RA8876_FontInternalCGROM(ISO8859_1);
	
	while (1)
    {
		
    }
}