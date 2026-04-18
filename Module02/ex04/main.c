/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/18 20:52:35 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include "tools.h"

#define BAUDRATE 115200                                 // baud Com Speed
#define OSC_SPEED_MHZ 16                                // Oscillation Speed page
#define DBL_SPEED (OSC_SPEED_MHZ/2)                     // p 182 set bit UCSRnA -> U2Xn
#define MYUBRR (F_CPU / (DBL_SPEED * BAUDRATE) -1)      // p 182   UBRR =  16000000
#define BUFFER 50
#define LED2 (1 << PB1)                                 // D2 - with OCR1A
#define PRESCALIER 256
#define TIME_FREQUENCY  (F_CPU / PRESCALIER)            // 16 Mhz / 256 = 62500

#define NAME "Nigel"
#define PASSWORD "1234"


volatile char buffer[BUFFER];
volatile uint8_t RX_index = 0;
volatile uint8_t GetNameStringflag = 0;
volatile uint8_t Stringflag = 0;
volatile uint8_t PassStringflag = 0;
volatile uint8_t Nameflag = 0;
volatile uint8_t Passflag = 2;
volatile uint8_t PassTitleflag = 0;
volatile uint8_t Beginflag = 0;
volatile uint8_t CheckNameflag = 0;
volatile uint8_t CheckPassflag = 0;
volatile uint8_t PassWordflag = 0;
volatile uint8_t notValidChar = 0;
volatile uint8_t getPassword = 0;
volatile uint8_t GetPassStringflag = 0;
volatile uint8_t Gameflag = 0;


void resetFlags(void) {

    RX_index = 0;
    GetNameStringflag = 0;
    Stringflag = 0;
    PassStringflag = 0;
    Nameflag = 2;
    Passflag = 2;
    PassTitleflag = 0;
    Beginflag = 0;
    CheckNameflag = 0;
    CheckPassflag = 0;
    PassWordflag = 0;
    notValidChar = 0;
    getPassword = 0;
    GetPassStringflag = 0;
    Gameflag = 0;
    buffer[0] = '\0';
}



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
    
    c = UDR0;                               // Receive Data char
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



void uart_printchar(volatile char c) {

        while (!(UCSR0A & (1 << UDRE0))) ;  // Set Flag indicates if ready to tramit data 1 = Buf empty
        UDR0 = c;
}



char backspace(char c) {        

    if (c == 0x7F) {
        if (RX_index > 0) {
            uart_printstr("\b");
            uart_printstr(" ");
            uart_printstr("\b");
            RX_index--;
            return c;
        }
    }
    return c;
}


void display_message (char *str) {
    
     if (Nameflag == 1 && Passflag == 1) {
         uart_printstr("\n\rHello ");          
         uart_printstr(str);   
         uart_printstr("!\n\rShall we play a game?");
         Gameflag = 1;
     } else {
         uart_printstr("\n\rBad combination username/password\n\r\n\r");
         resetFlags();
     }
}

void StopBlinkLed(void) {

   TCCR1A &= ~(1 << COM1A0);  // Disable OC1A output
    
}


void __vector_18(void) __attribute__((signal));  // USART_RX interupt 19 - page 66
void __vector_18(void)
{
    volatile char c;

    c = uart_interupt_rx();
    if (checkChar(c))
        return;
    if (notPrintable(c))
        return;
    if (Gameflag == 1) {
        if (c == 'y') {
            uart_printstr("\n\rLet's go!");
        } else {
            uart_printstr("\n\rbye!");
        }
        Gameflag = 2;
        return;
    }    
    if (RX_index < 11) {
        c = backspace(c);

        
        if (c != 0x7F && RX_index < 11) {
            buffer[RX_index] = c;
        } else { 
            return;
        }
    }
    if (c == '\n'|| c == '\r') {
        buffer[RX_index] = '\0';
        RX_index = 0;
        Stringflag = 1;
        if (PassTitleflag == 2)
            PassStringflag = 1;       
    }
    else {
        if (RX_index < 11){       
            if (PassTitleflag != 2)
                uart_interupt_tx(buffer[RX_index]); 
            else 
                uart_interupt_tx('*'); 
            RX_index++;// receive data
        } else {
            c = 0x7F;
            backspace(c);
        }
    }
}


void BlinkLed(void) {
    DDRB |= LED2;                         // LED D1 
    
    TCCR1A |= (1 << COM1A0);

    TCCR1B |= (1 << WGM12);

    TCCR1B |= (1 << CS12);

    OCR1A = (TIME_FREQUENCY / 2); // set Value to 50% - 0.5 sec
}


int  main( void ) {

    uart_Init();
    uart_Init_interupts();     

    char string[BUFFER];
    char password_string[BUFFER];
    string[0] = '\0';
    password_string[0] = '\0';
    
    int i = 0;
    resetFlags();
    
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
                Nameflag = 0;
            } else {
                Nameflag = 1;
            }
            CheckNameflag = 0;
            PassTitleflag = 1;
        }
        
        if (PassTitleflag == 1 && CheckPassflag == 0 && PassWordflag == 0) {
            uart_printstr("                  \n\r   password: ");
            buffer[0] = '\0';
            PassTitleflag = 2;
        }

        if (PassTitleflag == 2 && PassStringflag == 1) {
            i = 0;
            while (buffer[i] != '\0') {
                password_string[i] = buffer[i];
                i++;        
            }
            password_string[i] = '\0';
            getPassword = 1;
            PassStringflag = 0;
            GetPassStringflag = 1;
            PassTitleflag = 0;
        }
        
        
        if (getPassword == 1 && GetPassStringflag == 1 && Gameflag == 0) {
            if (ft_strcmp(password_string, PASSWORD)) {
                Passflag = 0;
            } else {
                Passflag = 1;
            }
            getPassword = 0;
            CheckPassflag = 1;
          
        }

        if (Passflag != 2 && Nameflag != 2 && Gameflag == 0) {

            display_message(string);
            Passflag = 2;
            Nameflag = 2;
            
        }   

        if (Gameflag == 1)
            BlinkLed();
        if (Gameflag == 2) {
            StopBlinkLed();                   // STOP the blinking LED on D2  
            break;
        }
    }
}
