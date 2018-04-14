/*
 * c3k_i2cbb.h
 * 
 * Copyright 2018  Calvin A. Cherry (calvincherry@gmail.com)
 *                 calcium3000.wordpress.com
 * 
 * Description:
 *   This is the header file for the I2C bit-banging tutorial as seen on
 *   calcium3000.wordpress.com.  Provided are three platforms for common
 *   8-bit and 16-bit microcontrollers: Microchip's PIC8 (using MPLABX)
 *   and AVR (using avr-gcc), and Texas Instruments' MSP430 (using
 *   msp430-gcc).  These functions are basic; more and better features are
 *   recommended for the user to pursue.  However, this should be a decent
 *   starting point.
 *
 * Use:
 *   Comment out the first-encountered #defines below except for the platform
 *   of intent.  Within the respective #ifdef block, adjust the appropriate
 *   hardware parameters -- e.g. SCL and SDA bits, input ports, and data/port
 *   direction slash tristate registers.  More libraries and hardware setups
 *   may be needed -- consult your datasheet.
 *
 * Example:
 * uint8_t data = 0xCC, reg = 0x17, slave_addr = 0x44;
 * 
 * i2c_init();
 * 
 * while( 1 )
 * {
 *     if( i2c_send_byte_data( slave_addr, reg, data ) )
 *     {
 *         PORTA = i2c_receive_byte_data( slave_addr, reg );
 *     }
 *     else
 *     {
 *         PORTA = 0;
 *     }
 * 
 *     _delay_ms( 500 );
 * }
 * 
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */


// #define PIC
// #define MSP430
// #define AVR


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Initializing I2C with PIC
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef PIC

#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

/* Define crystal frequency for delay function */
#define _XTAL_FREQ     8000000

/* I2C setup */
#define SCL            (1 << 0) /* bit 0 */
#define SDA            (1 << 1) /* bit 1 */

#define I2C_ANSEL      ANSELC
#define I2C_TRIS       TRISC
#define I2C_INPORT     PORTC

/* Basic I2C functions */
#define I2C_SET_SCL    I2C_TRIS |= SCL;
#define I2C_CLR_SCL    I2C_TRIS &= ~SCL;
#define I2C_SET_SDA    I2C_TRIS |= SDA;
#define I2C_CLR_SDA    I2C_TRIS &= ~SDA;

/* Compiler-specific function in MPLAB X v3 */
#define I2C_DELAY      __delay_us( 5 );

#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Initializing I2C with MSP430
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef MSP430

#include <stdbool.h>
#include <stdint.h>
#include <msp430g2553.h>

/* I2C setup */
#define SCL            BIT0
#define SDA            BIT1

#define I2C_PSEL       P2SEL
#define I2C_P2SEL      P2SEL2
#define I2C_PDIR       P2DIR
#define I2C_INPORT     P2IN

/* Basic I2C functions */
#define I2C_SET_SCL    I2C_PDIR &= ~SCL;
#define I2C_CLR_SCL    I2C_PDIR |= SCL;
#define I2C_SET_SDA    I2C_PDIR &= ~SDA;
#define I2C_CLR_SDA    I2C_PDIR |= SDA;

/* Compiler-specific function in msp430-gcc
   (10 clock cycles = 5us at 8MHz) */
#define I2C_DELAY      _delay_cycles( 10 );

#endif



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Initializing I2C with AVR
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifdef AVR

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

/* Define clock speed for delay function */
#define F_CPU 8000000UL
#include <avr/delay.h>

/* I2C setup */
#define SCL            _BV( 0 )
#define SDA            _BV( 1 )

#define I2C_DDR        DDRB
#define I2C_PORT       PORTB
#define I2C_INPORT     PINB

/* Basic I2C functions */
#define I2C_SET_SCL    I2C_DDR &= ~SCL;
#define I2C_CLR_SCL    I2C_DDR |= SCL;
#define I2C_SET_SDA    I2C_DDR &= ~SDA;
#define I2C_CLR_SDA    I2C_DDR |= SDA;

/* Compiler-specific function in avr-gcc */
#define I2C_DELAY      _delay_us( 5 );

#endif


/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Initializes pins for I2C operation.

Params:
    none
Returns:
    none

*/
void i2c_init( void );


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Sends slave address plus write bit and one byte of data.

Params:
    address = 7-bit slave address
    data = one byte of data to send following slave address
Returns:
    true if slave has ACK'd both sent bytes

*/
bool i2c_send_byte( uint8_t, uint8_t );


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Sends slave address plus read bit and receives one byte of data.

Params:
    address = 7-bit slave address
Returns:
    byte received from slave

*/
uint8_t i2c_receive_byte( uint8_t );


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Sends slave address plus write bit, then two consecutive data bytes.
(The first byte is typically used as a register address, and the second
as a data byte.)

Params:
    address = 7-bit slave address
    reg = first byte of data after the slave address
    data = second byte of data after the slave address
Returns:
    true if slave has ACK'd all three sent bytes

*/
bool i2c_send_byte_data( uint8_t, uint8_t, uint8_t );


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Sends slave address plus read bit and a data byte, and receives one byte
of data.

Params:
    address = 7-bit slave address
    data = one byte of data to send following slave address
Returns:
    byte received from slave

*/
uint8_t i2c_receive_byte_data( uint8_t, uint8_t );