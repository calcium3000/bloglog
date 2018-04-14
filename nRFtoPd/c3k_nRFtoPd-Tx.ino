/*
 * c3k_nRFtoPd-Tx.ino
 * 
 * Copyright 2018  Calvin A. Cherry (calvincherry@gmail.com)
 *                 calcium3000.wordpress.com
 * 
 * Use:
 *   This program is designed for use with any Arduino platform. An nRF24L01+
 *   transceiver module should be connected as follows:
 * 
 *   Arduino | nRF24L01+
 *   --------+----------
 *   VCC     | VDD
 *   GND     | GND
 *   D17     | SCK
 *   D15     | MOSI
 *   D16     | MISO
 *   D7*     | CE
 *   D8*     | CSN
 *
 *   *Note: any digital pin will work for CE and CSN -- just make sure to
 *    change the RF24 radio object's parameters (line 60) accordingly.
 * 
 *   This program is designed to be used in conjunction with the c3k_nRFtoPd-Rx
 *   C++ program.  See that program for details, and ensure that the
 *   PIPE_ADDRESS and data rate match the settings here.
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

#include <SPI.h>
#include <RF24.h>

#define PIPE_ADDRESS    0xbeefbabecafe


/* Data byte to send */
byte counter = 0;

/* Radio CE = D7, CSN = D8 */
RF24 radio( 7, 8 );


void setup()
{
  radio.begin();
  
  radio.setDataRate( RF24_1MBPS );
  radio.setPALevel( RF24_PA_LOW );
  /* Set retries to 5x at 250us */
  radio.setRetries( 0, 5 );
  radio.openWritingPipe( PIPE_ADDRESS );
  /* Enable Tx mode */
  radio.stopListening();
}


void loop()
{
  radio.write( &counter, sizeof counter );

  delay( 1000 );

  counter += 1;
}