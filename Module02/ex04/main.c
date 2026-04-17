/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/17 19:03:47 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include "tools.h"

#define BAUDRATE 115200                                 // baud Com Speed
#define OSC_SPEED_MHZ 16                                // Oscillation Speed page
#define DBL_SPEED (OSC_SPEED_MHZ/2)                     // p 182 set bit UCSRnA -> U2Xn
#define MYUBRR (F_CPU / (DBL_SPEED * BAUDRATE) -1)      // p 182   UBRR =  16000000
#define BUFFER 50
#define NAME "Nige"

volatile char buffer[BUFFER];
volatile uint8_t RX_index = 0;
volatile uint8_t GetNameStringflag = 0;
volatile uint8_t Stringflag = 0;
volatile uint8_t Nameflag = 0;
volatile uint8_t Passflag = 0;
volatile uint8_t Beginflag = 0;
volatile uint8_t CheckNameflag = 0;

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
    
    c = UDR0;
    return (c);                             // return c
}


void uart_printstr(volatile char* str) {

    int i = 0;
                                            // UDRE0 - page 200 indicates Data Register Empty
    while (str[i] != '\0') {                // UCSR0A - page 200 USART Control and status register
        while (!(UCSR0A & (1 << UDRE0))) ;  // Set Flag indicates if ready to tramit data 1 = Buf empty
        UDR0 = str[i];
                              // add to output buffer
        i++; 
    }

    
}



void __vector_18(void) __attribute__((signal));  // USART_RX interupt 19 - page 66
void __vector_18(void)
{
    char c;
    c = uart_interupt_rx();
    // (void)c;
    buffer[RX_index] = c;
    
    if (c == '\n'|| c == '\r') {
        buffer[RX_index] = '\0';
        RX_index = 0;
        Stringflag = 1;
        
    }
    else {
        uart_interupt_tx(buffer[RX_index]); 
        RX_index++;// receive data

    }

    
}


bool ft_strcmp(char *str, char *name) {

    int i = 0;
    
    if (str[0] == '\0') 
        return (1);

    while (str[i] != '\0') {

        if (str[i] != name[i])
            return (1);
        i++;
    } 
    
    return (0);
}




int  main( void ) {
    RX_index = 0;
    uart_Init();
    uart_Init_interupts();     

    char string[BUFFER];
    string[0] = '\0';
    int i = 0;

    
    (void)string;
    
    
    while (1) {
        
        if (Beginflag == 0) {
            uart_printstr("Enter your login: \n\r   username: ");
            Beginflag = 1; 
        }

        if (Beginflag == 1 && Stringflag == 1 && GetNameStringflag == 0) {
            i = 0;
            while (buffer[i] != '\0') {
                string[i] = buffer[i];
                i++;        
            }
            string[i] = '\0';
            GetNameStringflag = 1;
            CheckNameflag = 1;
            Stringflag = 0;
        }
        
        if (CheckNameflag == 1 && GetNameStringflag == 1) {
            if (ft_strcmp(string, NAME)) {
                uart_printstr(" - False");
                Nameflag = 0;
            } else {
                uart_printstr(" - True");
                Nameflag = 1;
            }
            CheckNameflag = 0;
        }




        
    };
}
