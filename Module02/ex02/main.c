/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/17 13:24:21 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

#define BAUDRATE 115200                                 // baud Com Speed
#define OSC_SPEED_MHZ 16                                // Oscillation Speed page
#define DBL_SPEED (OSC_SPEED_MHZ/2)                     // p 182 set bit UCSRnA -> U2Xn
#define MYUBRR (F_CPU / (DBL_SPEED * BAUDRATE) -1)      // p 182   UBRR =  16000000

void uart_Init(void){
    
    UBRR0H = (MYUBRR>>8);                               // Set BAUD RATE REGISTER HIGH side  -  page 204
    UBRR0L = MYUBRR;                                    // Set BAUD RATE REGISTER LOW side  -  page 204
    UCSR0A = (1<<U2X0);                                 // Set Double Speed Mode p182  - p200
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);               // Enable transmitter and receiver - page 201
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);                   // Set frame format: 8 data bits, 1 stop bit, no parity - page 203 
}


void uart_tx(char c)
{
                                                        // Wait for empty transmit buffer
    while ((UCSR0A & (1 << UDRE0)) == 0) ;              // Set Flag indicates if ready to send data 1 = Buf empty - page 200
    UDR0 = c;                                           // Put data into buffer, sends the data
}

char uart_rx(void) {
    
    char c;
    
    while ((UCSR0A & (1 << RXC0)) == 0) ;               // Recieve data Flag - page 200
    c = UDR0;                                           // receive data
    return (c);                                         // return c
}

int  main( void )
{
    uart_Init();
    
    while (1) {
        char c;
        
        c = uart_rx();   
        uart_tx(c);
    }
}
