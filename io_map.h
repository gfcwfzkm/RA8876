/*
 * io_map.h
 *
 * Created: 22.10.2020 08:25:23
 *  Author: gfcwfzkm
 */ 


#ifndef IO_MAP_H_
#define IO_MAP_H_

/* Port A
 * Paralleler 8-bit Datenbus, geht an das Display & Bustreiber 
 */
#define DATAPORT	PORTA
#define DB0			PIN0_bm
#define DB1			PIN1_bm
#define DB2			PIN2_bm
#define DB3			PIN3_bm
#define DB4			PIN4_bm
#define DB5			PIN5_bm
#define DB6			PIN6_bm
#define DB7			PIN7_bm

/* Port C
 * Serielle Schnittstellen, gehen zum Bus (SPI & UART), mit zwi Parallel-Port Steuerleitungen
 */
#define DB_EN		PIN0_bm
#define DB_RS		PIN4_bm

/* Port D
 * Interner SPI Bus, LCD-Steuerleitungen und USB-Datenleitungen
 */
#define DB_RW		PIN0_bm

#endif /* IO_MAP_H_ */
