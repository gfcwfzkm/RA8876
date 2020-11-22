/*
 * RA8876_reg.h
 * Register & Bit Definition
 * Created: 10.11.2020 16:51:14
 *  Author: gfcwfzkm
 */ 


#ifndef RA8876_REG_H_
#define RA8876_REG_H_

/*
 * 19.2 Chip Configuration Registers
 */
#define RA8876_SSR				0x00	// Software Reset Register
#define RA8876_SSR_SWRESET		0x01	// 0 = Normal Op; 1 = Software Reset

#define RA8876_CCR				0x01	// Chip Configuration Register
#define RA8876_CCR_CHNG_PLL		0x80	// Reconfigure PLL frequency
#define RA8876_CCR_MASK_XnWAIT	0x40
#define RA8876_CCR_KEYSCAN_EN	0x20	// Key-Scan Enable / Disable
#define RA8876_CCR_TFT_PIN_1	0x10	// TFT Panel I/F Output Pin Setting Bit 1
#define RA8876_CCR_TFT_PIN_0	0x08	// TFT Panel I/F Output Pin Setting Bit 0
#define RA8876_CCR_TFT_PIN_MASK		(RA8876_CCR_TFT_PIN_1 | RA8876_CCR_TFT_PIN_0)
#define RA8876_CCR_TFT_24BIT_gc		0x00
#define RA8876_CCR_TFT_18BIT_gc		(RA8876_CCR_TFT_PIN_0)
#define RA8876_CCR_TFT_16BIT_gc		(RA8876_CCR_TFT_PIN_1)
#define RA8876_CCR_TFT_NOTFT_gc		(RA8876_CCR_TFT_PIN_1 | RA8876_CCR_TFT_PIN_0)
#define RA8876_CCR_I2C_ENABLE	0x04	// I2C Master Interface Enable / Disable
#define RA8876_CCR_SPI_ENABLE	0x02	// Serial Flash or SPI Interface Enable / Disable
#define RA8876_CCR_DB_WIDTH		0x01	// Host Data Bus Width Selection

#define RA8876_MACR				0x02	// Memory Access Control Register
#define RA8876_MACR_FORMAT_1	0x80	// Host Read/Write image Data Format	
#define RA8876_MACR_FORMAT_0	0x40
#define RA8876_MACR_FORMAT_MASK		(RA8876_MACR_FORMAT_1 | RA8876_MACR_FORMAT_0)
#define RA8876_MACR_DIRECT_gc	0x00
#define RA8876_MACR_MASK_EACH_gc	(RA8876_MACR_FORMAT_1)
#define RA8876_MACR_MASK_EVEN_gc	(RA8876_MACR_FORMAT_1 | RA8876_MACR_FORMAT_0)
#define RA8876_MACR_RD_DIR_1	0x20	// Host Read Memory Direction (Only for Graphic Mode)
#define RA8876_MACR_RD_DIR_0	0x10
#define RA8876_MACR_RD_DIR_MASK		(RA8876_MACR_RD_DIR_1 | RA8876_MACR_RD_DIR_0)
#define RA8876_MACR_RD_L_TO_R_gc	0x00
#define RA8876_MACR_RD_R_TO_L_gc	(RA8876_MACR_RD_DIR_0)
#define RA8876_MACR_RD_T_TO_B_gc	(RA8876_MACR_RD_DIR_1)
#define RA8876_MACR_RD_B_TO_T_gc	(RA8876_MACR_RD_DIR_1 | RA8876_MACR_RD_DIR_0)
#define RA8876_MACR_WR_DIR_1	0x04	// Host Write Memory Direction (Only for Graphic Mode)
#define RA8876_MACR_WR_DIR_0	0x02
#define RA8876_MACR_WR_DIR_MASK		(RA8876_MACR_WR_DIR_1 | RA8876_MACR_WR_DIR_0)
#define RA8876_MACR_WR_L_TO_R_gc	0x00
#define RA8876_MACR_WR_R_TO_L_gc	(RA8876_MACR_WR_DIR_0)
#define RA8876_MACR_WR_T_TO_B_gc	(RA8876_MACR_WR_DIR_1)
#define RA8876_MACR_WR_B_TO_T_gc	(RA8876_MACR_WR_DIR_1 | RA8876_MACR_WR_DIR_0)

#define RA8876_ICR				0x03	// Input Control Register
#define RA8876_ICR_IRQ_LEVEL	0x80	// Output to MPU Interrupt Pin's active level
#define RA8876_ICR_EXT_IRQ_DBC	0x40	// External Interrupt input (XPS[0] pin) de-bounce
#define RA8876_ICR_EXT_IRQ_TT_1	0x20	// External Interrupt input (XPS[0] pin) trigger type
#define RA8876_ICR_EXT_IRQ_TT_0	0x10
#define RA8876_ICR_EXT_IRQ_TT_MASK		(RA8876_ICR_EXT_IRQ_TT_1 | RA8876_ICR_EXT_IRQ_TT_0)
#define RA8876_ICR_EXT_IRQ_LO_TRG_gc	0x00
#define RA8876_ICR_EXT_IRQ_FAL_TRG_gc	(RA8876_ICR_EXT_IRQ_TT_0)
#define RA8876_ICR_EXT_IRQ_HI_TRG_gc	(RA8876_ICR_EXT_IRQ_TT_1)
#define RA8876_ICR_EXT_IRQ_RIS_TRG_gc	(RA8876_ICR_EXT_IRQ_TT_1 | RA8876_ICR_EXT_IRQ_TT_0)
#define RA8876_ICR_TEXT_MODE_EN	0x04	// Text Mode Enable
#define RA8876_ICR_MEM_DEST_SEL_1	0x02	// Memory port Read/Write Destination Selection
#define RA8876_ICR_MEM_DEST_SEL_0	0x01
#define RA8876_ICR_MEM_DEST_SEL_MASK	(RA8876_ICR_MEM_DEST_SEL_1 | RA8876_ICR_MEM_DEST_SEL_0)
#define RA8876_ICR_MEM_SDRAM_gc			0x00
#define RA8876_ICR_MEM_GAMMA_TABLE_gc	(RA8876_ICR_MEM_DEST_SEL_0)
#define RA8876_ICR_MEM_GC_RAM_gc		(RA8876_ICR_MEM_DEST_SEL_1)
#define RA8876_ICR_MEM_CP_RAM_gc		(RA8876_ICR_MEM_DEST_SEL_1 | RA8876_ICR_MEM_DEST_SEL_0)

#define RA8876_MRWDP			0x04	// Memory Data Read/Write Port

/*
 * 19.3 Chip Configuration Registers
 */
#define RA8876_PPLLC1			0x05	// SCLK PLL Control Register 1
#define RA8876_PPLLC1_EXT_DIV_2	0x20	// SCLK extra divider
#define RA8876_PPLLC1_EXT_DIV_1	0x10
#define RA8876_PPLLC1_EXT_DIV_0	0x08
#define RA8876_PPLLC1_EXT_DIV_MASK	(RA8876_PPLLC1_EXT_DIV_2 | RA8876_PPLLC1_EXT_DIV_1 | RA8876_PPLLC1_EXT_DIV_0)
#define RA8876_PPLLC1_EXT_DIV16_gc	(RA8876_PPLLC1_EXT_DIV_0)
#define RA8876_PPLLC1_EXT_DIV8_gc	(RA8876_PPLLC1_EXT_DIV_2 | RA8876_PPLLC1_EXT_DIV_1)
#define RA8876_PPLLC1_EXT_DIV4_gc	(RA8876_PPLLC1_EXT_DIV_2)
#define RA8876_PPLLC1_EXT_DIV2_gc	(RA8876_PPLLC1_EXT_DIV_1)
#define RA8876_PPLLC1_EXT_DIV1_gc	0x00
#define RA8876_PPLLC1_DIVK_1	0x04	// SCLK PLLDIVK
#define RA8876_PPLLC1_DIVK_0	0x02
#define RA8876_PPLLC1_DIVK_MASK	(RA8876_PPLLC1_DIVK_1 | RA8876_PPLLC1_DIVK_0)
#define RA8876_PPLLC1_DIVK0_gc	0x00
#define RA8876_PPLLC1_DIVK2_gc	(RA8876_PPLLC1_DIVK_0)
#define RA8876_PPLLC1_DIVK4_gc	(RA8876_PPLLC1_DIVK_1)
#define RA8876_PPLLC1_DIVK8_gc	(RA8876_PPLLC1_DIVK_1 | RA8876_PPLLC1_DIVK_0)
#define RA8876_PPLLC1_DIVM2		0x01	// SCLK PLLDIVM

#define RA8876_PPLLC2			0x06	// SCLK PLL Control Register 2
#define RA8876_PPLLC2_DIVN_5	0x20	// SCLK PLLDIVN
#define RA8876_PPLLC2_DIVN_4	0x10
#define RA8876_PPLLC2_DIVN_3	0x08
#define RA8876_PPLLC2_DIVN_2	0x04
#define RA8876_PPLLC2_DIVN_1	0x02
#define RA8876_PPLLC2_DIVN_0	0x01
#define RA8876_PPLLC2_DIVN_MASK	0x3F

#define RA8876_MPLLC1			0x07	// MCLK PLL Control Register 1
#define RA8876_MPLLC1_DIVK_1	0x04	// MCLK PLLDIVK
#define RA8876_MPLLC1_DIVK_0	0x02
#define RA8876_MPLLC1_DIVK_MASK	(RA8876_MPLLC1_DIVK_1 | RA8876_MPLLC1_DIVK_0)
#define RA8876_MPLLC1_DIVK1_gc	0x00
#define RA8876_MPLLC1_DIVK2_gc	(RA8876_MPLLC1_DIVK_0)
#define RA8876_MPLLC1_DIVK4_gc	(RA8876_MPLLC1_DIVK_1)
#define RA8876_MPLLC1_DIVK8_gc	(RA8876_MPLLC1_DIVK_1 | RA8876_MPLLC1_DIVK_0)
#define RA8876_MPLLC1_DIVM		0x01	// MCLK PLLDIVM
#define RA8876_MPLLC1_DIVM1_gc	0x00
#define RA8876_MPLLC1_DIVM2_gc	(RA8876_MPLLC1_DIVM))

#define RA8876_MPLLC2			0x08	// MCLK PLL Control Register 2
#define RA8876_MPLLC2_DIVN_5	0x20	// MCLK PLLDIVN
#define RA8876_MPLLC2_DIVN_4	0x10
#define RA8876_MPLLC2_DIVN_3	0x08
#define RA8876_MPLLC2_DIVN_2	0x04
#define RA8876_MPLLC2_DIVN_1	0x02
#define RA8876_MPLLC2_DIVN_0	0x01
#define RA8876_MPLLC2_DIVN_MASK	0x3F

#define RA8876_SPLLC1			0x09	// CCLK PLL	Control Register 1
#define RA8876_SPLLC1_DIVK_1	0x04	// CCLK PLLDIVK
#define RA8876_SPLLC1_DIVK_0	0x02
#define RA8876_SPLLC1_DIVK_MASK	0x06
#define RA8876_SPLLC1_DIVK1_gc	0x00
#define RA8876_SPLLC1_DIVK2_gc	(RA8876_SPLLC1_DIVK_0)
#define RA8876_SPLLC1_DIVK4_gc	(RA8876_SPLLC1_DIVK_1)
#define RA8876_SPLLC1_DIVK8_gc	(RA8876_SPLLC1_DIVK_1 | RA8876_SPLLC1_DIVK_0)
#define RA8876_SPLLC1_DIVM_		0x01	// CCLK PLLDIVM

#define RA8876_SPLLC2			0xA0	// CCLK PLL	Control Register 2
#define RA8876_SPLLC2_DIVN_5	0x20	// MCLK PLLDIVN
#define RA8876_SPLLC2_DIVN_4	0x10
#define RA8876_SPLLC2_DIVN_3	0x08
#define RA8876_SPLLC2_DIVN_2	0x04
#define RA8876_SPLLC2_DIVN_1	0x02
#define RA8876_SPLLC2_DIVN_0	0x01
#define RA8876_SPLLC2_DIVN_MASK	0x3F

/*
 * 19.4 Interrupt Control Register
 */
#define RA8876_INTEN			0x0B	// Interrupt Enable Register
#define RA8876_INTEN_WAKEINT_EN	0x80	// Wakeup/resume Interrupt Enable
#define RA8876_INTEN_EXTINT_EN	0x40	// External Interrupt input (XPS[0] pin) Enable
#define RA8876_INTEN_I2CINT_EN	0x20	// I2C Master Interrupt Enable
#define RA8876_INTEN_VSINT_EN	0x10	// Vsync time based interrupt Enable
#define RA8876_INTEN_KSINT_EN	0x08	// Key Scan Interrupt Enable
#define RA8876_INTEN_MISKINT_EN	0x04	// Serial Flash DMA Complete | Draw task finished | BTE Process Complete etc.
#define RA8876_INTEN_PWM1INT_EN	0x02	// PWM timer 1 Interrupt Enable
#define RA8876_INTEN_PWM0INT_EN	0x01	// PWM timer 0 Interrupt Enable

#define RA8876_INTF				0x0C	// Interrupt Event Flag Register
#define RA8876_INTF_WAKEINT_FLG	0x80	// Wakeup/resume Interrupt flag
#define RA8876_INTF_EXTINT_FLG	0x40	// External Interrupt input (XPS[0] pin) flag
#define RA8876_INTF_I2CINT_FLG	0x20	// I2C Master Interrupt flag
#define RA8876_INTF_VSINT_FLG	0x10	// Vsync time based interrupt flag
#define RA8876_INTF_KSINT_FLG	0x08	// Key Scan Interrupt flag
#define RA8876_INTF_MISKINT_FLG	0x04	// Serial Flash DMA Complete | Draw task finished | BTE Process Complete etc.
#define RA8876_INTF_PWM1INT_FLG	0x02	// PWM timer 1 Interrupt flag
#define RA8876_INTF_PWM0INT_FLG	0x01	// PWM timer 0 Interrupt flag

#define RA8876_MINTFR				0x0C	// Mask Interrupt Event Flag Register
#define RA8876_INTF_MASK_WAKEINT_FLG	0x80	// Mask Wakeup/resume Interrupt flag
#define RA8876_INTF_MASK_EXTINT_FLG	0x40	// Mask External Interrupt input (XPS[0] pin) flag
#define RA8876_INTF_MASK_I2CINT_FLG	0x20	// Mask I2C Master Interrupt flag
#define RA8876_INTF_MASK_VSINT_FLG	0x10	// Mask Vsync time based interrupt flag
#define RA8876_INTF_MASK_KSINT_FLG	0x08	// Mask Key Scan Interrupt flag
#define RA8876_INTF_MASK_MISKINT_FLG	0x04	// Mask Serial Flash DMA Complete | Draw task finished | BTE Process Complete etc.
#define RA8876_INTF_MASK_PWM1INT_FLG	0x02	// Mask PWM timer 1 Interrupt flag
#define RA8876_INTF_MASK_PWM0INT_FLG	0x01	// Mask PWM timer 0 Interrupt flag

#define RA8876_PUENR			0x0E	// Pull-up control Register
#define RA8876_PUENR_GPIO_F		0x20	// GPIO-F[7:0] Pull-up Enable (XPDAT[23:19, 15:13])
#define RA8876_PUENR_GPIO_E		0x10	// GPIO-E[7:0] Pull-up Enable (XPDAT[12:10, 7:3])
#define RA8876_PUENR_GPIO_D		0x08	// GPIO-D[7:0] Pull-up Enable (XPDAT[18, 2, 17, 16, 9, 8, 1, 0])
#define RA8876_PUENR_GPIO_C		0x04	// GPIO-C[7:0] Pull-up Enable (XnSFCS1, XnSFCS0, XMISO, XMOSI, XSCK)
#define RA8876_PUENR_XDB_UPPER	0x02	// XDB[15:8] Pull-up Enable
#define RA8876_PUENR_XDB_LOWER	0x01	// XDB[7:0] Pull-up Enable

#define RA8876_PSFSR			0x0F	// PDAT for PIO/Key Function Select Register
#define RA8876_PSFSR_XPDAT_18	0x80	// XPDAT[18] - GPIO or Key function select
#define RA8876_PSFSR_XPDAT_17	0x40	// XPDAT[17] - GPIO or Key function select
#define RA8876_PSFSR_XPDAT_16	0x20	// XPDAT[16] - GPIO or Key function select
#define RA8876_PSFSR_XPDAT_9	0x10	// XPDAT[9] - GPIO or Key function select
#define RA8876_PSFSR_XPDAT_8	0x08	// XPDAT[8] - GPIO or Key function select
#define RA8876_PSFSR_XPDAT_2	0x04	// XPDAT[2] - GPIO or Key function select
#define RA8876_PSFSR_XPDAT_1	0x02	// XPDAT[1] - GPIO or Key function select
#define RA8876_PSFSR_XPDAT_0	0x01	// XPDAT[0] - GPIO or Key function select

/*
 * 19.5 LCD Display Control Register
 */
#define RA8876_MPWCTR			0x10	// Main/PIP Window Control Register
#define RA8876_MPWCTR_PIP1_EN	0x80	// PIP 1 window Enable/Disable
#define RA8876_MPWCTR_PIP2_EN	0x40	// PIP 2 window Enable/Disable
#define RA8876_MPWCTR_PIP_SEL	0x10	// Select Configure PIP 1 or PIP 2 Window's parameters
#define RA8876_MPWCTR_DPTH_1	0x08	// Main Image Color Depth Setting
#define RA8876_MPWCTR_DPTH_0	0x04
#define RA8876_MPWCTR_DPTH_MASK	0x0C
#define RA8876_MPWCTR_8BIT_COLOR_gc		0
#define RA8876_MPWCTR_16BIT_COLOR_gc	(RA8876_MPWCTR_DPTH_0)
#define RA8876_MPWCTR_24BIT_COLOR_gc	(RA8876_MPWCTR_DPTH_1)
#define RA8876_MPWCTR_CTRPNL_SIG	0x01	// To Control Panel's synchronous signals

#define RA8876_PIPCDEP			0x11	// PIP Window Color Depth Setting
#define RA8876_PIPCDEP_PIP1_COLR_DPTH_1		0x08	// PIP 1 Window Color Depth Setting
#define RA8876_PIPCDEP_PIP1_COLR_DPTH_0		0x04
#define RA8876_PIPCDEP_PIP1_COLR_DPTH_MASK	0x0C
#define RA8876_PIPCDEP_PIP2_COLR_DPTH_1		0x02	// PIP 2 Window Color Depth Setting
#define RA8876_PIPCDEP_PIP2_COLR_DPTH_0		0x01
#define RA8876_PIPCDEP_PIP2_COLR_DPTH_MASK	0x03

#define RA8876_DPCR				0x12	// Display Configuration Register
#define RA8876_DPCR_PCLK		0x80	// PCLK Inversion
#define RA8876_DPCR_DISP_ONOFF	0x40	// Display ON/OFF
#define RA8876_DPCR_DISP_TEST	0x20	// Display Test Color Bar
#define RA8876_DPCR_VDIR		0x08	// VDIR
#define RA8876_DPCR_XPDAT_OUTSEQ_2	0x04	// Parallel XPDAT[23:0] Output Sequence
#define RA8876_DPCR_XPDAT_OUTSEQ_1	0x02
#define RA8876_DPCR_XPDAT_OUTSEQ_0	0x01
#define RA8876_DPCR_XPDAT_OUTSEQ_MASK	0x07

#define RA8876_PCSR				0x13	// Panel scan Clock and Data Setting Register
#define RA8876_PCSR_XHSYNC_POL	0x80	// XHSYNC Polarity
#define RA8876_PCSR_XVSYNC_POL	0x40	// XVSYNC Polarity
#define RA8876_PCSR_XDE_POL		0x20	// XDE Polarity
#define RA8876_PCSR_XDE_IDLE	0x10	// XDE IDLE STATE
#define RA8876_PCSR_XPCLK_IDLE	0x08	// XPCLK IDLE STATE
#define RA8876_PCSR_XPDAT_IDLE	0x04	// XPDAT IDLE STATE
#define RA8876_PCSR_XHSYNC_IDLE	0x02	// XHSYNC IDLE STATE
#define RA8876_PCSR_HVSYNC_IDLE	0x01	// XVSYNC IDLE STATE

#define RA8876_HDWR				0x14	// Horizontal Display Width Register

#define RA8876_HDWFTR			0x15	// Horizontal Display Width Fine Tune Register

#define RA8876_HNDR				0x16	// Horizontal Non-Display Period Register

#define RA8876_HNDFTR			0x17	// Horizontal Non-Display Period Fine tune Register

#define RA8876_HSTR				0x18	// HSYNC Start Position Register

#define RA8876_HPWR				0x19	// HSYNC Pulse Width Register

#define RA8876_VDHR0			0x1A	// Vertical Display Height Register 0

#define RA8876_VDHR1			0x1B	// Vertical Display Height Register 1

#define RA8876_VNDR0			0x1C	// Vertical Non-Display Period Register 0

#define RA8876_VNDR1			0x1D	// Vertical Non-Display Period Register 1

#define RA8876_VSTR				0x1E	// VSYNC Sart Position Register

#define RA8876_VPWR				0x1F	// VSYNC Pulse Width Register

#define RA8876_MISA0			0x20	// Main Image Start Address 0

#define RA8876_MISA1			0x21	// Main Image Start Address 1

#define RA8876_MISA2			0x22	// Main Image Start Address 2

#define RA8876_MISA3			0x23	// Main Image Start Address 3

#define RA8876_MIW0				0x24	// Main Image Width 0

#define RA8876_MIW1				0x25	// Main Image Width 1

#define RA8876_MWULX0			0x26	// Main Window Upper-Left corner X-coordinates 0

#define RA8876_MWULX1			0x27	// Main Window Upper-Left corner X-coordinates 1

#define RA8876_MWULY0			0x28	// Main Window Upper-Left corner Y-coordinates 0

#define RA8876_MWULY1			0x29	// Main Window Upper-Left corner Y-coordinates 1

#define RA8876_PWDULX0			0x2A	// PIP 1 or 2 Window Display Upper-Left X-coordinates 0

#define RA8876_PWDULX1			0x2B	// PIP 1 or 2 Window Display Upper-Left X-coordinates 0

#define RA8876_PWDULY0			0x2C	// PIP 1 or 2 Window Display Upper-Left Y-coordinates 0

#define RA8876_PWDULY1			0x2D	// PIP 1 or 2 Window Display Upper-Left Y-coordinates 0

#define RA8876_PISA0			0x2E	// PIP 1 or 2 Image Start Address 0

#define RA8876_PISA1			0x2F	// PIP 1 or 2 Image Start Address 1

#define RA8876_PISA2			0x30	// PIP 1 or 2 Image Start Address 2

#define RA8876_PISA3			0x31	// PIP 1 or 2 Image Start Address 3

#define RA8876_PIW0				0x32	// PIP 1 or 2 Image Width 0

#define RA8876_PIW1				0x33	// PIP 1 or 2 Image Width 1

#define RA8876_PWIULX0			0x34	// PIP 1 or 2 Window Image Upper-Left corner X-coordinates 0

#define RA8876_PWIULX1			0x35	// PIP 1 or 2 Window Image Upper-Left corner X-coordinates 1

#define RA8876_PWIULY0			0x36	// PIP 1 or 2 Window Image Upper-Left corner Y-coordinates 0

#define RA8876_PWIULY1			0x37	// PIP 1 or 2 Window Image Upper-Left corner Y-coordinates 1

#define RA8876_PWW0				0x38	// PIP 1 or 2 Window Width 0

#define RA8876_PWW1				0x39	// PIP 1 or 2 Window Width 1

#define RA8876_PWH0				0x3A	// PIP 1 or 2 Window Height 0

#define RA8876_PWH1				0x3B	// PIP 1 or 2 Window Height 1

#define RA8876_GTCCR			0x3C	// Graphic / Text Cursor Control Register
#define RA8876_GTCCR_GM_CORR	0x80	// Gamma Correction Enable
#define RA8876_GTCCR_GM_TBL_1	0x40	// Gamma table select for MPU write gamma data
#define RA8876_GTCCR_GM_TBL_0	0x20
#define RA8876_GTCCR_GM_TBL_MASK	0x60
#define RA8876_GTCCR_GRAPH_CURSOR_EN	0x10	// Graphic Cursor Enable
#define RA8876_GTCCR_GRAPH_CURSOR_SEL_1	0x08	// Graphic Cursor Selection Bit
#define RA8876_GTCCR_GRAPH_CURSOR_SEL_0	0x04
#define RA8876_GTCCR_GRAPH_CURSOR_SEL_MASK	0x0C
#define RA8876_GTCCR_TXT_CURSOR_EN	0x02	// Text Cursor Enable
#define RA8876_GTCCR_TXT_CURSOR_BLINK_EN	0x01	// Text Cursor Blinking Enable

#define RA8876_BTCR				0x3D	// Blink Time Control Register

#define RA8876_CURHS			0x3E	// Text Cursor Hoizontal Size Register

#define RA8876_CURVS			0x3F	// Text Cursor Vertical Size Register

#define RA8876_GCHP0			0x40	// Graphic Cursor Horizontal Position Register 0

#define RA8876_GCHP1			0x41	// Graphic Cursor Horizontal Position Register 1

#define RA8876_GCVP0			0x42	// Graphic Cursor Vertical Position Register 0

#define RA8876_GCVP1			0x43	// Graphic Cursor Vertical Position Register 1

#define RA8876_GCC0				0x44	// Graphic Cursor Color 0

#define RA8876_GCC1				0x45	// Graphic Cursor Color 1

/*
 * 19.6 Geomatric Engine Control Register
 */
#define RA8876_CVSSA0			0x50	// Canvas Start address 0

#define RA8876_CVSSA1			0x51	// Canvas Start address 1

#define RA8876_CVSSA2			0x52	// Canvas Start address 2

#define RA8876_CVSSA3			0x53	// Canvas Start address 3

#define RA8876_CVS_IMWTH0		0x54	// Canvas image width 0

#define RA8876_CVS_IMWTH1		0x55	// Canvas image width 1

#define RA8876_AWUL_X0			0x56	// Active Window Upper-Left corner X-coordinates 0

#define RA8876_AWUL_X1			0x57	// Active Window Upper-Left corner X-coordinates 1

#define RA8876_AWUL_Y0			0x58	// Active Window Upper-Left corner Y-coordinates 0

#define RA8876_AWUL_Y1			0x59	// Active Window Upper-Left corner Y-coordinates 1

#define RA8876_AW_WTH0			0x5A	// Active Window Width 0

#define RA8876_AW_WTH1			0x5B	// Active Window Width 1

#define RA8876_AW_HT0			0x5C	// Active Window Height 0

#define RA8876_AW_HT1			0x5D	// Active Window Height 1

#define RA8876_AW_COLOR			0x5E	// Color Depth of Canvas & Active Window
#define RA8876_AW_COLOR_ADDRMODE	0x04	// Canvas addressing mode
#define RA8876_AW_COLOR_WIDTH_1	0x02	// Canvas image's color depth & memory R/W data width
#define RA8876_AW_COLOR_WIDTH_0	0x01
#define RA8876_AW_COLOR_WIDTH_MASK	0x03
#define RA8876_AW_COLOR_DEPTH_8BIT_gc	0x00
#define RA8876_AW_COLOR_DEPTH_16BIT_gc	(RA8876_AW_COLOR_WIDTH_0)
#define RA8876_AW_COLOR_DEPTH_24BIT_gc	(RA8876_AW_COLOR_WIDTH_1)

#define RA8876_CURH0			0x5F	// Graphic Read/Write position Horizontal Position Register 0

#define RA8876_CURH1			0x60	// Graphic Read/Write position Horizontal Position Register 1

#define RA8876_CURV0			0x61	// Graphic Read/Write position Vertical Position Register 0

#define RA8876_CURV1			0x62	// Graphic Read/Write position Vertical Position Register 1

#define RA8876_F_CURX0			0x63	// Text Write X-coordinates Register 0

#define RA8876_F_CURX1			0x64	// Text Write X-coordinates Register 1

#define RA8876_F_CURY0			0x65	// Text Write Y-coordinates Register 0

#define RA8876_F_CURY1			0x66	// Text Write Y-coordinates Register 1

#define RA8876_DCR0				0x67	// Draw Line / Triangle Control Register 0
#define RA8876_DCR0_DRAW_START	0x80	// Draw Line / Triangle Start
#define RA8876_DCR0_FILL_TRGL	0x20	// Fill function for Triangle Signal
#define RA8876_DCR0_DRAW_SEL	0x02	// Draw Triangle or Line Select Signal
#define RA8876_DCR0_DRAW_LINE		0x00	// Extras to make it more clear in the code
#define RA8876_DCR0_DRAW_TRIANGLE	RA8876_DCR0_DRAW_SEL	

#define RA8876_DLHSR0			0x68	// Draw Line / Square / Triangle Point 1 X-coordinates Register 0

#define RA8876_DLHSR1			0x69	// Draw Line / Square / Triangle Point 1 X-coordinates Register 1

#define RA8876_DLVSR0			0x6A	// Draw Line / Square / Triangle Point 1 Y-coordinates Register 0

#define RA8876_DLVSR1			0x6B	// Draw Line / Square / Triangle Point 1 Y-coordinates Register 1

#define RA8876_DLHER0			0x6C	// Draw Line / Square / Triangle Point 2 X-coordinates Register 0

#define RA8876_DLHER1			0x6D	// Draw Line / Square / Triangle Point 2 X-coordinates Register 1

#define RA8876_DLVER0			0x6E	// Draw Line / Square / Triangle Point 2 Y-coordinates Register 0

#define RA8876_DLVER1			0x6F	// Draw Line / Square / Triangle Point 2 Y-coordinates Register 1

#define RA8876_DTPH0			0x70	// Draw Line / Square / Triangle Point 3 X-coordinates Register 0

#define RA8876_DTPH1			0x71	// Draw Line / Square / Triangle Point 3 X-coordinates Register 1

#define RA8876_DTPV0			0x72	// Draw Line / Square / Triangle Point 3 Y-coordinates Register 0

#define RA8876_DTPV1			0x73	// Draw Line / Square / Triangle Point 3 Y-coordinates Register 1

#define RA8876_DCR1				0x76	// Draw Circle / Ellipse / Ellipse Curve / Circle Square Control Register 1
#define RA8876_DCR1_DRAW_START	0x80	// Draw Circle / Ellipse / Square / Circle Square Start Signal
#define RA8876_DCR1_FILL		0x40	// Fill the Circle / Ellipse / Square / Circle Square Signal
#define RA8876_DCR1_DRAW_SEL_1	0x20	// Draw Circle / Ellipse / Square / Ellipse Curve / Circle Square Select
#define RA8876_DCR1_DRAW_SEL_0	0x10
#define RA8876_DCR1_DRAW_SEL_MASK	0x30
#define RA8876_DCR1_DRAW_CIRCLE_ELLIPSE_gc	0x00
#define RA8876_DCR1_DRAW_CURVE_gc	(RA8876_DCR1_DRAW_SEL_0)
#define RA8876_DCR1_DRAW_SQUARE_gc	(RA8876_DCR1_DRAW_SEL_1)
#define RA8876_DCR1_DRAW_CIRCLE_SQUARE_gc	(RA8876_DCR1_DRAW_SEL_1 | RA8876_DCR1_DRAW_SEL_0)
#define RA8876_DCR1_PART_SEL_1	0x02	// Draw Circle / Ellipse Curve Part Select (DECP)
#define RA8876_DCR1_PART_SEL_0	0x01
#define RA8876_DCR1_PART_SEL_MASK	0x03

#define RA8876_ELL_A0			0x77	// Draw Circle / Ellipse / Circle Square Major radius Setting Register

#define RA8876_ELL_A1			0x78	// Draw Circle / Ellipse / Circle Square Major radius Setting Register

#define RA8876_ELL_B0			0x79	// Draw Circle / Ellipse / Circle Square Minor radius Setting Register

#define RA8876_ELL_B1			0x7A	// Draw Circle / Ellipse / Circle Square Minor radius Setting Register

#define RA8876_DEHR0			0x7B	// Draw Circle / Ellipse / Circle Square Center X-coordinates Register 0

#define RA8876_DEHR1			0x7C	// Draw Circle / Ellipse / Circle Square Center X-coordinates Register 1

#define RA8876_DEVR0			0x7D	// Draw Circle / Ellipse / Circle Square Center X-coordinates Register 0

#define RA8876_DEVR1			0x7E	// Draw Circle / Ellipse / Circle Square Center X-coordinates Register 1

/*
 * 19.7 PWM Timer Control Register
 */
#define RA8876_PSCLR			0x84	// PWM Prescaler Register

#define RA8876_PMUXR			0x85	// PWM Clock MUX Register
#define RA8876_PMUXR_CLKDIV_PWM1_1	0x80	// Select 2nd clock divider's MUX input for PWM Timer 1
#define RA8876_PMUXR_CLKDIV_PWM1_0	0x40
#define RA8876_PMUXR_CLKDIV_PWM1_MASK	0xC0
#define RA8876_PMUXR_CLKDIV_PWM0_1	0x20	// Select 2nd clock divider's MUX input for PWM Timer 0
#define RA8876_PMUXR_CLKDIV_PWM0_0	0x10
#define RA8876_PMUXR_CLKDIV_PWM0_MASK	0x30
#define RA8876_PMUXR_XPWM1_CTRL_1	0x08	// XPWM[1] pin function control
#define RA8876_PMUXR_XPWM1_CTRL_0	0x04
#define RA8876_PMUXR_XPWM1_CTRL_MASK	0x0C
#define RA8876_PMUXR_XPWM0_CTRL_1	0x02	// XPWM[0] pin function control
#define RA8876_PMUXR_XPWM0_CTRL_0	0x01
#define RA8876_PMUXR_XPWM0_CTRL_MASK	0x03

#define RA8876_PCFGR			0x86	// PWM Configuration Register
#define RA8876_PCFGR_PWM1_INV	0x40	// PWM Timer 1 output inverter on/off
#define RA8876_PCFGR_PWM1_RLD	0x20	// PWM Timer 1 auto reload on/off
#define RA8876_PCFGR_PWM1_START	0x10	// PWM Timer 1 start/stop
#define RA8876_PCFGR_PWM0_DEAD	0x08	// PWM Timer 0 Dead zone enable
#define RA8876_PCFGR_PWM0_INV	0x04	// PWM Timer 0 output inverter on/off
#define RA8876_PCFGR_PWM0_RLD	0x02	// PWM Timer 0 auto reload on/off
#define RA8876_PCFGR_PWM0_START	0x01	// PWM Timer 0 start/stop

#define RA8876_DZ_LENGTH		0x87	// Timer 0 Dead zone length register

#define RA8876_TCMPB0L			0x88	// Timer 0 compare buffer register

#define RA8876_TCMPB0H			0x89	// Timer 0 compare buffer register

#define RA8876_TCNTB0L			0x8A	// Timer 0 count buffer register

#define RA8876_TCNTB0H			0x8B	// Timer 0 count buffer register

#define RA8876_TCMPB1L			0x8C	// Timer 1 compare buffer register

#define RA8876_TCMPB1H			0x8D	// Timer 1 compare buffer register

#define RA8876_TCNTB1L			0x8E	// Timer 1 count buffer register

#define RA8876_TCNTB1H			0x8F	// Timer 1 count buffer register

/*
 * 19.8 Block Transfer Engine (BTE) Control Register
 */
#define RA8876_BTE_CTRL0		0x90	// BTE Function Control Register 0
#define RA8876_BTE_CTRL0_STAT	0x10	// BTE Function Enable / Status
#define RA8876_BTE_CTRL0_PATRN	0x01	// PATTERN Format

#define RA8876_BTE_CTRL1		0x91	// BTE Function Control Register 1
#define RA8876_BTE_CTRL1_ROP_3	0x80	// BTE ROP Code Bit[3:0] or Color expansion starting bit
#define RA8876_BTE_CTRL1_ROP_2	0x40
#define RA8876_BTE_CTRL1_ROP_1	0x20
#define RA8876_BTE_CTRL1_ROP_0	0x10
#define RA8876_BTE_CTRL1_ROP_MASK	0xF0
#define RA8876_BTE_CTRL1_CODE_3	0x08	// BTE Operation Code Bit[3:0]
#define RA8876_BTE_CTRL1_CODE_2	0x04
#define RA8876_BTE_CTRL1_CODE_1 0x02
#define RA8876_BTE_CTRL1_CODE_0	0x01
#define RA8876_BTE_CTRL1_CODE_MASK	0x0F

#define RA8876_BTE_COLR			0x92	// Source 0/1 & Destination Color Depth
#define RA8876_BTE_COLR_S0_1	0x40	// S0 Color Depth
#define RA8876_BTE_COLR_S0_0	0x20
#define RA8876_BTE_COLR_S0_MASK	0x60
#define RA8876_BTE_S0_256_gc	0x00
#define RA8876_BTE_S0_64k_gc	RA8876_BTE_COLR_S0_0
#define RA8876_BTE_S0_16M_gc	RA8876_BTE_COLR_S0_1
#define RA8876_BTE_COLR_S1_2	0x10	// S1 Color Depth
#define RA8876_BTE_COLR_S1_1	0x08
#define RA8876_BTE_COLR_S1_0	0x04
#define RA8876_BTE_COLR_S1_MASK	0x1C
#define RA8876_BTE_S1_256_gc	0x00
#define RA8876_BTE_S1_64k_gc	RA8876_BTE_COLR_S1_0
#define RA8876_BTE_S1_16M_gc	RA8876_BTE_COLR_S1_1
#define RA8876_BTE_S1_CONST_COLOR_gc	(RA8876_BTE_COLR_S1_1 | RA8876_BTE_COLR_S1_0)
#define RA8876_BTE_S1_8BIT_ALPHA_gc		RA8876_BTE_COLR_S1_2
#define RA8876_BTE_S1_16BIT_ALPHA_gc	(RA8876_BTE_COLR_S1_2 | RA8876_BTE_COLR_S1_0)
#define RA8876_BTE_COLR_DEST_1	0x02	// Destination Color Depth
#define RA8876_BTE_COLR_DEST_0	0x01	
#define RA8876_BTE_COLR_DEST_MASK	0x03

#define RA8876_S0_STR0			0x93	// Source 0 memory start address 0

#define RA8876_S0_STR1			0x94	// Source 0 memory start address 1

#define RA8876_S0_STR2			0x95	// Source 0 memory start address 2

#define RA8876_S0_STR3			0x96	// Source 0 memory start address 3

#define RA8876_S0_WTH0			0x97	// Source 0 image width 0

#define RA8876_S0_WTH1			0x98	// Source 0 image width 1

#define RA8876_S0_X0			0x99	// Source 0 Window Upper-Left corner X-coordinates 0

#define RA8876_S0_X1			0x9A	// Source 0 Window Upper-Left corner X-coordinates 1

#define RA8876_S0_Y0			0x9B	// Source 0 Window Upper-Left corner Y-coordinates 0

#define RA8876_S0_Y1			0x9C	// Source 0 Window Upper-Left corner Y-coordinates 1

#define RA8876_S1_STR0			0x9D	// Source 1 memory start address 0 OR
#define RA8876_S1_RED			0x9D	// S1 constant color - Red Element

#define RA8876_S1_STR1			0x9E	// Source 1 memory start address 1 OR
#define RA8876_S1_GREEN			0x9E	// S1 constant color - Green Element

#define RA8876_S1_STR2			0x9F	// Source 1 memory start address 2 OR
#define RA8876_S1_BLUE			0x9F	// S1 constant color - Blue Element

#define RA8876_S1_STR3			0xA0	// Source 1 memory start address 3

#define RA8876_S1_WTH0			0xA1	// Source 1 image width 0

#define RA8876_S1_WTH1			0xA2	// Source 1 image width 1

#define RA8876_S1_X0			0xA3	// Source 1 Window Upper-Left corner X-coordinates 0

#define RA8876_S1_X1			0xA4	// Source 1 Window Upper-Left corner X-coordinates 1

#define RA8876_S1_Y0			0xA5	// Source 1 Window Upper-Left corner Y-coordinates 0

#define RA8876_S1_Y1			0xA6	// Source 1 Window Upper-Left corner Y-coordinates 1

#define RA8876_DT_STR0			0xA7	// Destination memory start address 0

#define RA8876_DT_STR1			0xA8	// Destination memory start address 1

#define RA8876_DT_STR2			0xA9	// Destination memory start address 2

#define RA8876_DT_STR3			0xAA	// Destination memory start address 3

#define RA8876_DT_WTH0			0xAB	// Destination image width 0

#define RA8876_DT_WTH1			0xAC	// Destination image width 1

#define RA8876_DT_X0			0xAD	// Destination Window Upper-Left corner X-coordinates 0

#define RA8876_DT_X1			0xAE	// Destination Window Upper-Left corner X-coordinates 1

#define RA8876_DT_Y0			0xAF	// Destination Window Upper-Left corner Y-coordinates 0

#define RA8876_DT_Y1			0xB0	// Destination Window Upper-Left corner Y-coordinates 1

#define RA8876_BTE_WTH0			0xB1	// BTE Window Width 0

#define RA8876_BTE_WTH1			0xB2	// BTE Window Width 1

#define RA8876_BTE_HIG0			0xB3	// BTE Window Height 0

#define RA8876_BTE_HIG1			0xB4	// BTE Window Height 1

#define RA8876_APB_CTRL			0xB5	// Alpha Blending

/*
 * 19.9 Serial Flash & SPI Master Control Register
 */
#define RA8876_DMA_CTRL			0xB6	// Serial Flash DMA Controller REG

#define RA8876_SFL_CTRL			0xB7	// Serial Flash/ROM Controller Register
#define RA8876_SFL_CTRL_SELECT	0x80	// Serial Flash/ROM I/F # Select
#define RA8876_SFL_CTRL_ACCESS	0x40	// Serial Flash/ROM Access Mode
#define RA8876_SFL_CTRL_ADDRESS	0x20	// Serial Flash/ROM Address Mode
#define RA8876_SFL_CTRL_COMPATB	0x10	// RA8875 compatible mode
#define RA8876_SFL_CTRL_BEHAVIOR_3	0x08	// Read Command code & behavior selection
#define RA8876_SFL_CTRL_BEHAVIOR_2	0x04
#define RA8876_SFL_CTRL_BEHAVIOR_1	0x02
#define RA8876_SFL_CTRL_BEHAVIOR_0	0x01
#define RA8876_SFL_CTRL_BEHAVIOR_MASK	0x0F

#define RA8876_SPIDR			0xB8	// SPI master Tx / Rx FIFO Data Register

#define RA8876_SPIMCR2			0xB9	// SPI master Control Register
#define RA8876_SPIMCR2_SPI_INT	0x40	// SPI Master Interrupt enable
#define RA8876_SPIMCR2_SS_PIN	0x20	// Control Slave Select drive on which xnsfcs
#define RA8876_SPIMCR2_SS_ACT	0x10	// Slave Select signal active
#define RA8876_SPIMCR2_FIFO_ERR	0x08	// Mask interrupt for FIFO overflow error
#define RA8876_SPIMCR2_EMPTYINT	0x04	// Mask interrupt for while Tx FIFO empty & SPI engine / FSM idle
#define RA8876_SPIMCR2_SPI_MODE_1	0x02	// SPI operation mode
#define RA8876_SPIMCR2_SPI_MODE_0	0x01
#define RA8876_SPIMCR2_SPI_MODE_MASK	0x03

#define RA8876_SPIMSR			0xBA	// SPI master Status Register
#define RA8876_SPIMSR_TX_FIFO_EMPTY	0x80	// Tx FIFO empty flag
#define RA8876_SPIMSR_TX_FIFO_FULL	0x40	// Tx FIFO full flag
#define RA8876_SPIMSR_RX_FIFO_EMPTY	0x20	// Rx FIFO empty flag
#define RA8876_SPIMSR_RX_FIFO_FULL	0x10	// Rx FIFO full flag
#define RA8876_SPIMSR_OVERFLOW		0x08	// Overflow interrupt flag
#define RA8876_SPIMSR_IDLE			0x04	// Tx FIFO empty & SPI engine / FSM idle interrupt flag

#define RA8876_SPI_DIVSOR		0xBB	// SPI Clock period

#define RA8876_DMA_SSTR0		0xBC	// Serial flash DMA Source Starting Address 0

#define RA8876_DMA_SSTR1		0xBD	// Serial flash DMA Source Starting Address 1

#define RA8876_DMA_SSTR2		0xBE	// Serial flash DMA Source Starting Address 2

#define RA8876_DMA_SSTR3		0xBF	// Serial flash DMA Source Starting Address 3

#define RA8876_DMA_DX0			0xC0	// DMA Destination Window Upper-Left corner X-coordinates 0

#define RA8876_DMA_DX1			0xC1	// DMA Destination Window Upper-Left corner X-coordinates 1

#define RA8876_DMA_DY0			0xC2	// DMA Destination Window Upper-Left corner Y-coordinates 0

#define RA8876_DMA_DY1			0xC3	// DMA Destination Window Upper-Left corner Y-coordinates 1

#define RA8876_DMAW_WTH0		0xC6	// DMA Block Width 0

#define RA8876_DMAW_WTH1		0xC7	// DMA Block Width 1

#define RA8876_DMAW_HIGH0		0xC8	// DMA Block Height 0

#define RA8876_DMAW_HIGH1		0xC9	// DMA Block Height 1

#define RA8876_DMA_SWTH0		0xCA	// DMA Source Picture Width 0

#define RA8876_DMA_SWTH1		0xCB	// DMA Source Picture Width 1

/*
 * 19.10 Text Engine
 */
#define RA8876_CCR0				0xCC	// Character Control Register 0
#define RA8876_CCR0_CHAR_SOURCE_1	0x80	// Character source selection
#define RA8876_CCR0_CHAR_SOURCE_0	0x40
#define RA8876_CCR0_CHAR_SOURCE_MASK	(RA8876_CCR0_CHAR_SOURCE_1 | RA8876_CCR0_CHAR_SOURCE_0)
#define RA8876_CCR0_INTERNAL_CGROM_gc	0x00
#define RA8876_CCR0_EXTERNAL_CGROM_gc	(RA8876_CCR0_CHAR_SOURCE_0)
#define RA8876_CCR0_USERDEFINED_gc		(RA8876_CCR0_CHAR_SOURCE_1)
#define RA8876_CCR0_CHAR_HEIGHT_1	0x20	// Character Height Setting
#define RA8876_CCR0_CHAR_HEIGHT_0	0x10
#define RA8876_CCR0_CHAR_HEIGHT_MASK	(RA8876_CCR0_CHAR_HEIGHT_1 | RA8876_CCR0_CHAR_HEIGHT_0)
#define RA8876_CCR0_8X16_16X16_gc		0x00
#define RA8876_CCR0_12X24_24X24_gc		(RA8876_CCR0_CHAR_HEIGHT_0)
#define RA8876_CCR0_16X32_32X32_gc		(RA8876_CCR0_CHAR_HEIGHT_1)
#define RA8876_CCR0_CGROM_SELECT_1	0x02	// Character Selection for internal CGROM
#define RA8876_CCR0_CGROM_SELECT_0	0x01
#define RA8876_CCR0_CGROM_SELECT_MASK	(RA8876_CCR0_CGROM_SELECT_1 | RA8876_CCR0_CGROM_SELECT_0)
#define RA8876_CCR0_ISO8859_1_gc	0x00
#define RA8876_CCR0_ISO8859_2_gc	(RA8876_CCR0_CGROM_SELECT_0)
#define RA8876_CCR0_ISO8859_3_gc	(RA8876_CCR0_CGROM_SELECT_1)
#define RA8876_CCR0_ISO8859_4_gc	(RA8876_CCR0_CGROM_SELECT_1 | RA8876_CCR0_CGROM_SELECT_0)

#define RA8876_CCR1				0xCD	// Character Control Register 1
#define RA8876_CCR1_ALIG_BIT	0x80	// Full Alignment Selection Bit
#define RA8876_CCR1_CHROMA		0x40	// Chroma keying enable on Text input
#define RA8876_CCR1_CHAR_ROT	0x10	// Character Rotation
#define RA8876_CCR1_WIDTH_1		0x08	// Character width enlargement factor
#define RA8876_CCR1_WIDTH_0		0x04
#define RA8876_CCR1_WIDTH_MASK	0x0C
#define RA8876_CCR1_HEIGHT_1	0x02	// Character Height enlargement factor
#define RA8876_CCR1_HEIGHT_0	0x01
#define RA8876_CCR1_HEIGHT_MASK	0x03

#define RA8876_GTFNT_SEL		0xCE	// GT Character ROM Select
#define RA8876_GTFNT_SEL_0		0x20
#define RA8876_GTFNT_SEL_1		0x40
#define RA8876_GTFNT_SEL_2		0x80
#define RA8876_GTFNT_SEL_MASK	(RA8876_GTFNT_SEL_2 | RA8876_GTFNT_SEL_1 | RA8876_GTFNT_SEL_0)
#define RA8876_GTFNT_GT21L16T1W_gc	0
#define RA8876_GTFNT_GT30L16U2W_gc	(RA8876_GTFNT_SEL_0)
#define RA8876_GTFNT_GT30L24T3Y_gc	(RA8876_GTFNT_SEL_1)
#define RA8876_GTFNT_GT30L24M1Z_gc	(RA8876_GTFNT_SEL_1 | RA8876_GTFNT_SEL_0)
#define RA8876_GTFNT_GT30L32S4W_gc	(RA8876_GTFNT_SEL_2)
#define RA8876_GTFNT_GT20L24F6Y_gc	(RA8876_GTFNT_SEL_2 | RA8876_GTFNT_SEL_0)
#define RA8876_GTFNT_GT21L24S1W_gc	(RA8876_GTFNT_SEL_2 | RA8876_GTFNT_SEL_1)

#define RA8876_GTFNT_CR			0xCF	// GT Character ROM Control register
#define RA8876_GTFNT_CR_CHAR_4	0x80
#define RA8876_GTFNT_CR_CHAR_3	0x40
#define RA8876_GTFNT_CR_CHAR_2	0x20
#define RA8876_GTFNT_CR_CHAR_1	0x10
#define RA8876_GTFNT_CR_CHAR_0	0x08
#define RA8876_GTFNT_CR_WIDTH_1	0x02
#define RA8876_GTFNT_CR_WIDTH_0	0x01

#define RA8876_FLDR				0xD0	// Character Line gap Setting Register

#define RA8876_F2FSSR			0xD1	// Character to Character Space Setting Register

#define RA8876_FGCR				0xD2	// Foreground Color Register - Red

#define RA8876_FGCG				0xD3	// Foreground Color Register - Green

#define RA8876_FGCB				0xD4	// Foreground Color Register - Blue

#define RA8876_BGCR				0xD5	// Background Color Register - Red

#define RA8876_BGCG				0xD6	// Background Color Register - Green

#define RA8876_BGCB				0xD7	// Background Color Register - Blue

#define RA8876_CGRAM_STR0		0xDB	// CGRAM Start Address 0

#define RA8876_CGRAM_STR1		0xDB	// CGRAM Start Address 1

#define RA8876_CGRAM_STR2		0xDB	// CGRAM Start Address 2

#define RA8876_CGRAM_STR3		0xDB	// CGRAM Start Address 3

/*
 * 19.11 Power Management Control Register
 */
#define RA8876_PMU				0xDF	// Power Management Register
#define RA8876_PMU_POWERDOWN	0x80	// Enter Power saving state
#define RA8876_PMU_STANDBY_gc	0x01	// Power saving Mode definition - Standby Mode
#define RA8876_PMU_SUSPEND_gc	0x02	// Power saving Mode definition - Suspend Mode
#define RA8876_PMU_SLEEP_gc		0x03	// Power saving Mode definition - Sleep Mode

/*
 * 19.12 SDRAM Control Register
 */
#define RA8876_SDRAR			0xE0	// SDRAM attribute register
#define RA8876_SDRAR_POWERTYPE	0x80	// SDRAM Power Saving type
#define RA8876_SDRAR_MEMTYPE	0x40	// SDRAM memory type
#define RA8876_SDRAR_BANKNUM_4	0x20	// SDRAM Bank number
#define RA8876_SDRAR_ROW_ADDR_1	0x10	// SDRAM Row addressing
#define RA8876_SDRAR_ROW_ADDR_0	0x08
#define RA8876_SDRAR_ROW_ADDR_MASK	0x18
#define RA8876_SDRAR_ROW_ADDR_2K_gc	0x00
#define RA8876_SDRAR_ROW_ADDR_4K_gc	(RA8876_SDRAR_ROW_ADDR_0)
#define RA8876_SDRAR_ROW_ADDR_8K_gc	(RA8876_SDRAR_ROW_ADDR_1)
#define RA8876_SDRAR_COL_ADDR_2	0x04	// SDRAM Column addressing
#define RA8876_SDRAR_COL_ADDR_1	0x02
#define RA8876_SDRAR_COL_ADDR_0	0x01
#define RA8876_SDRAR_COL_ADDR_MASK	0x07
#define RA8876_SDRAR_COL_ADDR_256_gc	0x00
#define RA8876_SDRAR_COL_ADDR_512_gc	(RA8876_SDRAR_COL_ADDR_0)
#define RA8876_SDRAR_COL_ADDR_1024_gc	(RA8876_SDRAR_COL_ADDR_1)
#define RA8876_SDRAR_COL_ADDR_2048_gc	(RA8876_SDRAR_COL_ADDR_1 | RA8876_SDRAR_COL_ADDR_0)
#define RA8876_SDRAR_COL_ADDR_4096_gc	(RA8876_SDRAR_COL_ADDR_2)

#define RA8876_SDRMD			0xE1	// SDRAM mode register & extended mode register
#define RA8876_SDRMD_REFRESH_2	0x80	// Partial-Array Self Refresh
#define RA8876_SDRMD_REFRESH_1	0x40
#define RA8876_SDRMD_REFRESH_0	0x20
#define RA8876_SDRMD_REFRESH_MASK	0xE0
#define RA8876_SDRMD_DRIVER_1	0x10	// To select the driver strength of the DQ outputs
#define RA8876_SDRMD_DRIVER_0	0x08
#define RA8876_SDRMD_DRIVER_MASK	0x18
#define RA8876_SDRMD_LATENCY_2	0x04	// SDRAM CAS latency
#define RA8876_SDRMD_LATENCY_1	0x02
#define RA8876_SDRMD_LATENCY_0	0x01
#define RA8876_SDRMD_LATENCY_MASK	0x07
#define RA8876_SDRMD_LATENCY_2CLK_gc	(RA8876_SDRMD_LATENCY_1)
#define RA8876_SDRMD_LATENCY_3CLK_gc	(RA8876_SDRMD_LATENCY_1 | RA8876_SDRMD_LATENCY_0)

#define RA8876_SDR_REF_ITVL0	0xE2	// SDRAM auto refresh interval

#define RA8876_SDR_REF_ITVL1	0xE3	// SDRAM auto refresh interval

#define RA8876_SDRCR			0xE4	// SDRAM Control register
#define RA8876_SDRCR_BURSTLEN_1	0x80	// Length to break a burst transfer
#define RA8876_SDRCR_BURSTLEN_0	0x40
#define RA8876_SDRCR_BURSTLEN_MASK	0xC0
#define RA8876_SDRCR_XMCKE		0x10	// XMCKE pin state
#define RA8876_SDRCR_WARNING	0x08	// Report warning condition
#define RA8876_SDRCR_TIMING_EN	0x04	// SDRAM timing parameter register enable
#define RA8876_SDRCR_PWRDWN_EN	0x02	// SDRAM enter power saving mode
#define RA8876_SDRCR_INIT		0x01	// Start SDRAM initialization procedure

/*
 * 19.13 I2C Master Register
 */
#define RA8876_IICMCPR0			0xE5	// I2C Master Clock Pre-scale Register 0

#define RA8876_IICMCPR1			0xE6	// I2C Master Clock Pre-scale Register 1

#define RA8876_IICMTXR			0xE7	// I2C Master Transmit Register

#define RA8876_IICMRXR			0xE8	// I2C Master Receive Register

#define RA8876_IICMCMDR			0xE9	// I2C Master Command Register
#define RA8876_IICMCMDR_START	0x80	// START
#define RA8876_IICMCMDR_STOP	0x40	// STOP
#define RA8876_IICMCMDR_READ	0x20	// READ
#define RA8876_IICMCMDR_WRITE	0x10	// WRITE
#define RA8876_IICMCMDR_ACK		0x08	// ACKNOWLEDGE
#define RA8876_IICMCMDR_NOISEF	0x01	// Noise Filter

#define RA8876_IICMSTUR			0xEA	// I2C Master Status Register
#define RA8876_IICMSTUR_ACK_REC	0x80	// Received acknowledge from slave
#define RA8876_IICMSTUR_BUSY	0x40	// I2C Bus is Busy
#define RA8876_IICMSTUR_TRANS	0x02	// Transfer in progress
#define RA8876_IICMSTUR_LOST	0x01	// Abritration lost

/*
 * 19.14 GPI & GPO Register
 */
#define RA8876_GPIOAD			0xF0	// GPIO-A direction

#define RA8876_GPIOA			0xF1	// GPIO-A

#define RA8876_GPIOB			0xF2	// GPIO-B

#define RA8876_GPIOCD			0xF3	// GPIO-C direction

#define RA8876_GPIOC			0xF4	// GPIO-C

#define RA8876_GPIODD			0xF5	// GPIO-D direction

#define RA8876_GPIOD			0xF6	// GPIO-D

#define RA8876_GPIOED			0xF7	// GPIO-E direction

#define RA8876_GPIOE			0xF8	// GPIO-E

#define RA8876_GPIOFD			0xF9	// GPIO-F direction

#define RA8876_GPIOF			0xFA	// GPIO-F

/*
 * 19.15 Key-Scan Control Registers
 */
#define RA8876_KSCR1			0xFB	// Key-Scan Control Register 1
#define RA8876_KSCR1_LONG		0x40	// Long Key Enable Bit
#define RA8876_KSCR1_DEBOUNCE_1	0x20	// Short Key de-bounce Times
#define RA8876_KSCR1_DEBOUNCE_0	0x10
#define RA8876_KSCR1_DEBOUNCE_MASK	0x30
#define RA8876_KSCR1_REPEATABLE	0x08	// Repeatable Key enable
#define RA8876_KSCR1_PERIOD_DIV_2	0x04	// Period of Key scan controller to scan one row
#define RA8876_KSCR1_PERIOD_DIV_1	0x02
#define RA8876_KSCR1_PERIOD_DIV_0	0x01
#define RA8876_KSCR1_PERIOD_DIV_MASK	0x07

#define RA8876_KSCR2			0xFC	// Key-Scan Control Register 2
#define RA8876_KSCR2_WAKEUP		0x80	// Key-Scan Wakeup Function Enable bit
#define RA8876_KSCR2_REL_IRQ	0x40	// Key released interrupt enable
#define RA8876_KSCR2_LONGKEY_MASK	0x1C	// Long Key Recognition Factor
#define RA8876_KSCR2_HITKEY_MASK	0x03	// Numbers of Key Hit

#define RA8876_KSDR0			0xFD	// Key-Scan Data Register

#define RA8876_KSDR1			0xFE	// Key-Scan Data Register

#define RA8876_KSDR2			0xFF	// Key-Scan Data Register

#endif /* RA8876_REG_H_ */