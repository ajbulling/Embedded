/*
 * "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
 * All rights reserved.
 * (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
 * (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
 * (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */

#ifndef ESOS_PIC24_LCD_H
#define ESOS_PIC24_LCD_H

/**
 * \addtogroup ESOS_Task_LCD_Service
 * @{
 */
 
 /** \file
 *  \brief This file contains routines which configure and
 *  use an LCD interface on the Microchip PIC24 MCUs.
 */

/* I N C L U D E S **********************************************************/
#include <esos_lcd44780.h>

/* PIC24-SPECIFIC MACROS *******************************************************/
#define  __esos_lcd44780_pic24_config		__esos_lcd44780_pic24_config

#define  __esos_lcd44780_pic24_setDataPins \
    __esos_lcd44780_pic24_setDataPins
#define  __esos_lcd44780_pic24_getDataPins \
    __esos_lcd44780_pic24_getDataPins
#define  __esos_lcd44780_pic24_configDataPinsAsInput \
    __esos_lcd44780_pic24_configDataPinsAsInput
#define  __esos_lcd44780_pic24_configDataPinsAsOutput \
    __esos_lcd44780_pic24_configDataPinsAsOutput

#define  __ESOS_LCD44780_PIC24_SET_E_HIGH          LCD_E = 1
#define  __ESOS_LCD44780_PIC24_SET_E_LOW           LCD_E = 0
#define  __ESOS_LCD44780_PIC24_SET_RW_READ         LCD_RW = 1
#define  __ESOS_LCD44780_PIC24_SET_RW_WRITE        LCD_RW = 0
#define  __ESOS_LCD44780_PIC24_SET_RS_REGISTERS    LCD_RS = 0
#define  __ESOS_LCD44780_PIC24_SET_RS_DATA         LCD_RS = 1

#define  __ESOS_LCD44780_PIC24_SET_D0              LCD_D0 = 1
#define  __ESOS_LCD44780_PIC24_CLEAR_D0            LCD_D0 = 0
#define  __ESOS_LCD44780_PIC24_GET_D0              LCD_D0
#define  __ESOS_LCD44780_PIC24_SET_D1              LCD_D1 = 1
#define  __ESOS_LCD44780_PIC24_CLEAR_D1            LCD_D1 = 0
#define  __ESOS_LCD44780_PIC24_GET_D1              LCD_D1
#define  __ESOS_LCD44780_PIC24_SET_D2              LCD_D2 = 1
#define  __ESOS_LCD44780_PIC24_CLEAR_D2            LCD_D2 = 0
#define  __ESOS_LCD44780_PIC24_GET_D2              LCD_D2
#define  __ESOS_LCD44780_PIC24_SET_D3              LCD_D3 = 1
#define  __ESOS_LCD44780_PIC24_CLEAR_D3            LCD_D3 = 0
#define  __ESOS_LCD44780_PIC24_GET_D3              LCD_D3
#define  __ESOS_LCD44780_PIC24_SET_D4              LCD_D4 = 1
#define  __ESOS_LCD44780_PIC24_CLEAR_D4            LCD_D4 = 0
#define  __ESOS_LCD44780_PIC24_GET_D4              LCD_D4
#define  __ESOS_LCD44780_PIC24_SET_D5              LCD_D5 = 1
#define  __ESOS_LCD44780_PIC24_CLEAR_D5            LCD_D5 = 0
#define  __ESOS_LCD44780_PIC24_GET_D5              LCD_D5
#define  __ESOS_LCD44780_PIC24_SET_D6              LCD_D6 = 1
#define  __ESOS_LCD44780_PIC24_CLEAR_D6            LCD_D6 = 0
#define  __ESOS_LCD44780_PIC24_GET_D6              LCD_D6
#define  __ESOS_LCD44780_PIC24_SET_D7              LCD_D7 = 1
#define  __ESOS_LCD44780_PIC24_CLEAR_D7            LCD_D7 = 0
#define  __ESOS_LCD44780_PIC24_GET_D7              LCD_D7

/* P R I V A T E   P R O T O T Y P E S *****************************************/
void __esos_lcd44780_pic24_config ( void );
void __esos_lcd44780_pic24_setDataPins( uint8_t u8_data);
uint8_t __esos_lcd44780_pic24_getDataPins( void );
void __esos_lcd44780_pic24_configDataPinsAsInput( void );
void __esos_lcd44780_pic24_configDataPinsAsOutput( void );

#endif
