/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/16 18:16:26 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/delay.h>
#include <avr/io.h>
#define BAUDRATE 115200                                 // baud Com Speed
#define OSC_SPEED_MHZ 16                                // Oscillation Speed page
#define DBL_SPEED (OSC_SPEED_MHZ/2)                     // p 182 set bit UCSRnA -> U2Xn

#define MYUBRR (F_CPU / (DBL_SPEED * BAUDRATE) -1)      // p 182   UBRR =  16000000
                                                        //         -----------------  -1 = 16.36
                                                        //            8 x 115200
void uart_Init(void)
{
    /*Set baud rate */
    UBRR0H = (MYUBRR>>8); `                             // Set BAUD RATE REGISTER HIGH side  -  page 204
    UBRR0L = MYUBRR;                                    // Set BAUD RATE REGISTER LOW side  -  page 204
    UCSR0A = (1<<U2X0);                                 // Set Double Speed Mode p182  - p200

    UCSR0B = (1<<TXEN0);                                // Enable transmitter - page 201

    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);                   // SET number Data bits to 8 bits - Page 203
}


void uart_tx(char c)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0))) ;                  // Set Flag indicates if ready to tramit data 1 = Buf empty
    UDR0 = c;                                           // Put data into buffer, sends the data
}

int  main( void )
{

    uart_Init();
    while (1) {
        uart_tx('Z');
        _delay_ms(1000);
    }
}


