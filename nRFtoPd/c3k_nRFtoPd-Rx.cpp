/*
 * c3k_nRFtoPd-Rx.cpp
 * 
 * Copyright 2018  Calvin A. Cherry (calvincherry@gmail.com)
 *                 calcium3000.wordpress.com
 * 
 * UDP socket sections generously provided by binarytides.com
 * (http://www.binarytides.com/programming-udp-sockets-c-linux).
 * 
 * 
 * Compilation:
 *  1.  Download the RF24 library from TMRh20's GitHub repository
 *      (http://tmrh20.github.io).
 *  2.  Navigate to the RF24/examples_linux folder and copy this file
 *      into this directory.
 *  3.  Open the Makefile and append 'c3k_nRFtoPd-Rx' (no quotes) to the
 *      'PROGRAMS' line after 'transfer'.
 *  4.  Open Terminal in this directory and run 'make' (no quotes).  If
 *      all went well there will be no error in the console output and
 *      a new 'c3k_nRFtoPd-Rx' executable will appear.
 * 
 * Use:
 *   This program is designed for use with a Raspberry Pi model 3.  (No
 *   testing has been done on other models, but model 2 should also work.)
 *   An nRF24L01+ transceiver module should be connected as follows:
 * 
 *   Raspberry Pi | nRF24L01+
 *   -------------+----------
 *   +3.3V (17)   | VDD
 *   GND (20)     | GND
 *   BCM 11 (23)  | SCK
 *   BCM 10 (19)  | MOSI
 *   BCM 9 (21)   | MISO
 *   BCM 25 (22)  | CE
 *   BCM 8 (24)   | CSN
 *
 *   To run the program after compilation via the command line:
 *   
 *   sudo ./c3k_nRFtoPd-Rx
 * 
 *   This program is designed to be used in conjunction with the c3k_nRFtoPd-Tx
 *   Arduino sketch.  See that program for details, and ensure that the
 *   PIPE_ADDRESS and data rate match the settings here.
 *
 *   The SERVER is set to localhost, which shouldn't need be messed with
 *   unless you know what you're doing (that'd make one of us), but feel
 *   free to change the PORT number to your liking.
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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include "RF24.h"

#define SERVER          "127.0.0.1"
#define PORT            3001

#define PIPE_ADDRESS    0xbeefbabecafe

using namespace std;


char udp_message[ 4 ];

/* Radio CE = BCM25 (pin 22), CSN = BCM8/CE0 (pin 24) */
RF24 radio( RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ );


/* Wait until a value is received, then return it */
uint8_t get_received_value( void );

/* Print error and exit */
void die( char * );

/* Build UDP message from 8-bit value */
void build_message( uint8_t );


/* Main program */
int main( int argc, char **argv )
{
    struct sockaddr_in si_other;
    int s, slen = sizeof( si_other );
    uint8_t received_value = 0;

    radio.begin();
    
    radio.setDataRate( RF24_1MBPS );
    radio.setPALevel( RF24_PA_LOW );
    /* Set retries to 5x at 250us */
    radio.setRetries( 0, 5 );
    radio.openReadingPipe( 1, PIPE_ADDRESS );
    /* Enable Rx mode */
    radio.startListening();

    /* Create UDP socket */
    if( ( s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) == -1 )
    {
        die( (char *)"socket" );
    }

    /* Zero out the structure si_other */
    memset( (char *)&si_other, 0, sizeof( si_other ) ) ;
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons( PORT );

    /* Convert Internet dot address ('127.0.0.1') to network address */
    if( inet_aton( SERVER, &si_other.sin_addr ) == 0 )
    {
        fprintf( stderr, "inet_aton() failed\n" );
        exit( 1 );
    }

    /* Main loop */
    while( 1 )
    {
        received_value = get_received_value();
        printf( "%d\n", received_value );

        /* Build message and send over UDP */
        build_message( received_value );
        if( sendto( s, udp_message, strlen( udp_message ), 0, (struct sockaddr *)&si_other, slen ) == -1 )
        {
            die( (char *)"sendto" );
        }

        delay( 5 );
    }

    return 0;
}


uint8_t get_received_value( void )
{
    uint8_t received_value = 0;
    
    while( !radio.available() )
    {
        /* Wait (add timeout function here later) */
    }
  
    radio.read( &received_value, sizeof received_value );
    
    return received_value;
}


void die( char *s )
{
    perror( s );
    exit( 1 );
}


void build_message( uint8_t input_value )
{
    /*
     * E.g. input_value = 135 
     * udp_message[0] = 135/100 + 48 = 1 + 48 = 49 = '1'
     * udp_message[1] = (35)/10 + 48 = 3 + 48 = 51 = '3'
     * udp_message[2] = 5 + 48 = 53 = '5'
     */
    udp_message[0] = input_value / 100 + 48;
    udp_message[1] = ( input_value % 100 ) / 10 + 48;
    udp_message[2] = input_value % 10 + 48;
    udp_message[3] = '\n';
}