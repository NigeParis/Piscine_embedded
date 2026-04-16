/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/16 21:25:58 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUDRATE 115200                                 // baud Com Speed
#define OSC_SPEED_MHZ 16                                // Oscillation Speed page
#define DBL_SPEED (OSC_SPEED_MHZ/2)                     // p 182 set bit UCSRnA -> U2Xn

#define MYUBRR (F_CPU / (DBL_SPEED * BAUDRATE) -1)      // p 182   UBRR =  16000000
                                                        //         -----------------  -1 = 16.36
                                                        //            8 x 115200
void uart_Init(void)
{
    /*Set baud rate */
    UBRR0H = (MYUBRR>>8);                               // Set BAUD RATE REGISTER HIGH side  -  page 204
    UBRR0L = MYUBRR;                                    // Set BAUD RATE REGISTER LOW side  -  page 204
    UCSR0A = (1<<U2X0);                                 // Set Double Speed Mode p182  - p200
    UCSR0B = (1<<TXEN0);                                // Enable transmitter - page 201
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00); 
}


#define PRESCALER 1024                           // SYNIC with CS12 p142 ref: DataSheet
#define TIME_FREQUENCY (F_CPU / PRESCALER) / 0.5     // MAX Value to be stored in OCR1A

void timer_init() {

    // SET Fast PWM mode 14 - . page 142 
    TCCR1A |= ((1 << COM1A1) | (1 << WGM11)); 
    
    // SET Fast PWM mode 14 NB - TCCR1B timer counter regiister B setting - page 142
    TCCR1B |= ((1 << WGM12) | (1 << WGM13));  
    
    // Set Clk prescaler to F_CPU/256 = 16MHz/256 = 62500 per tick - page 139 & 143 
    TCCR1B |= ((1 << CS12) | (1 << CS10));
    
    // ICR1 TOP Value 100% on - 1 sec. - page 129 
    ICR1 = TIME_FREQUENCY - 1;

    TIMSK1 |= (1 << OCIE1A);  // ENABLE interupts on CTC  page - 144

    SREG |= (1 << 7);  // ENABLE Global interupts page 20
    
}



void uart_printstr(const char* str) {

    int i = 0;
                                            // UDRE0 - page 200 indicates Data Register Empty
    while (str[i] != '\0') {                // UCSR0A - page 200 USART Control and status register
        while (!(UCSR0A & (1 << UDRE0))) ;  // Set Flag indicates if ready to tramit data 1 = Buf empty
        UDR0 = str[i];                      // add to output buffer
        i++; 
    }

    
}

void __vector_11(void) __attribute__((signal, used, externally_visible)); // TIMER1_COMPA interupt 11 - page 66
void __vector_11(void)
{
    uart_printstr("Hello World!\r\n\0");   
}


int  main( void )
{
    timer_init();
    uart_Init();
    
    while (1) {}
    
}


