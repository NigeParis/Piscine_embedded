/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/17 14:06:47 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include "utils.h"

#define BAUDRATE 115200                                 // baud Com Speed
#define OSC_SPEED_MHZ 16                                // Oscillation Speed page
#define DBL_SPEED (OSC_SPEED_MHZ/2)                     // p 182 set bit UCSRnA -> U2Xn
#define MYUBRR (F_CPU / (DBL_SPEED * BAUDRATE) -1)      // p 182   UBRR =  16000000

void uart_Init(void) {
    
    UBRR0H = (MYUBRR>>8);                   // Set BAUD RATE REGISTER HIGH side  -  page 204
    UBRR0L = MYUBRR;                        // Set BAUD RATE REGISTER LOW side  -  page 204
    UCSR0A = (1<<U2X0);                     // Set Double Speed Mode p182  - p200
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);       // Set frame format: 8 data bits, 1 stop bit, no parity - page 203 
    UCSR0B |= (1 << RXEN0);                 // Enable receiver USART - page 202
    UCSR0B |= (1 << TXEN0);                 // Enable transmitter and receiver - page 201
}

void uart_Init_interupts(void) {

    UCSR0B |= (1 << RXCIE0);                // Enable Complete Interupt Enable receiver - page 201
    SREG |= (1 << 7);                       // ENABLE Global interupts page 20
}

void uart_interupt_tx(char c) {

    UDR0 = c;                               // Put data into buffer, sends the data
}

char uart_interupt_rx(void) {
    
    char c;
    
    c = UDR0;                               // receive data
    return (c);                             // return c
}


void __vector_18(void) __attribute__((signal));  // USART_RX interupt 19 - page 66
void __vector_18(void)
{
    char c;

    c = uart_interupt_rx();   
    c = inverse_char(c);
    uart_interupt_tx(c);
}

int  main( void )
{
    uart_Init();
    uart_Init_interupts();     
    while (1) ;
}
