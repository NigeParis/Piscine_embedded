/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/20 14:18:01 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/delay.h>                         // delay for the debounce
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tools.h"
#include "uart_lib.h"
#include "config.h"
#include "led_lib.h"
#include "timers.h"



#define LED_RED (1 << PD5)    // define RED led mask
#define LED_GREEN (1 << PD6)  // define GREEN led mask
#define LED_BLUE (1 << PD3)   // define BLUE led mask

// Definitions - actual memory allocation
// flags for ordering the name and password input and output
volatile char buffer[BUFFER];
volatile uint8_t RX_index;
volatile uint8_t GetNameStringflag;
volatile uint8_t Stringflag;
volatile uint8_t PassStringflag;
volatile uint8_t Nameflag;
volatile uint8_t Passflag;
volatile uint8_t PassTitleflag;
volatile uint8_t Beginflag;
volatile uint8_t CheckNameflag;
volatile uint8_t CheckPassflag;
volatile uint8_t PassWordflag;
volatile uint8_t notValidChar;
volatile uint8_t getPassword;
volatile uint8_t GetPassStringflag;
volatile uint8_t Gameflag;


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


char backspace(char c) {        

    if (c == 0x7F) {
        if (RX_index > 0) {
            uart_printstr("\b \b");
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
         set_rgb(255,23,2);
         Gameflag = 1;
     } else {
         uart_printstr("\n\rBad combination username/password\n\r\n\r");
         resetFlags();
     }
}


// void timer_init_timer0(void) {

//     TCCR0A= (1 << COM0B1)   // 8 bit Timer 0 after Schema works on PD6 and PD5 - page 113
//             | (1 << COM0A1) 
//             | (1 << WGM00)  // Mode 3 for timer 0 - WGM00 + WGM01 - page 115 datasheet
//             | (1 << WGM01); // Mode 3 WGM00 + WGM01 --> SAME as WG10 + WGM11

//     TCCR0B= (1 << CS00)     // prescaler 64 - also works with others prescaler setting
//             | (1 << CS01);  // CS01 + C00  - page 117 datasheet
//  }

// /// NOTE: Timer 2 after Schema and page 162 in the datasheet

// void timer_init_timer2(void) {

//     TCCR2A=                 // 8 Bit Timer 2 - page 164 datasheet
//             (1 << COM2B1)   // Only use COM2B1 this works with PD3 - see schema
//             | (1 << WGM21)  // Mode 3 Fast PWM - TOP 0xFF Upd OCRx: BOTTOM Timer Over Flow Flag "TOV" MAX
//             | (1 << WGM20); // PWM mode 3 - page 164 datasheet
            
//     TCCR2B=                
//             (1 << CS22);    // prescaler 64 - also works with others
// }
    
// void  init_rgb(void) {

//     timer_init_timer0();    // timer 0 controls led PD5 and PD6 (OCR0A and OCR0B)
//     timer_init_timer2();    // timer 2 controls led PD3 (OCR2B)
// }


void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}


void __vector_18(void) __attribute__((signal));
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


int main(void) {
        
    DDRD |= LED_RED;    // open for output on PD5  
    DDRD |= LED_GREEN;  // open for output on PD6
    DDRD |= LED_BLUE;   // open for output on PD3

    init_rgb();
    
    char string[BUFFER] = {0};
    char password_string[BUFFER] = {0};
    int i = 0;
    
    uart_Init();
    uart_Init_interupts();     
    resetFlags();
    
    while (1) {

        // display login invitation
        if (Beginflag == 0) {
            uart_printstr("Enter your login: \n\r   username: ");
            Beginflag = 1; 
        }

        // get input login name fron buffer
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

        // check login name and flag it
        if (CheckNameflag == 1 && GetNameStringflag == 1) {
            if (ft_strcmp(string, NAME)) {
                Nameflag = 0;
            } else {
                Nameflag = 1;
            }
            CheckNameflag = 0;
            PassTitleflag = 1;
        }

        // display password invitation
        if (PassTitleflag == 1 && CheckPassflag == 0 && PassWordflag == 0) {
            uart_printstr("                  \n\r   password: ");
            buffer[0] = '\0';
            PassTitleflag = 2;
        }

        // get input password fron buffer
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

        // check password and flag it        
        if (getPassword == 1 && GetPassStringflag == 1 && Gameflag == 0) {
            if (ft_strcmp(password_string, PASSWORD)) {
                Passflag = 0;
            } else {
                Passflag = 1;
            }
            getPassword = 0;
            CheckPassflag = 1;
        }

        // display result of login and password
        if (Passflag != 2 && Nameflag != 2 && Gameflag == 0) {

            display_message(string);
            Passflag = 2;
            Nameflag = 2; 
        }   

        // blink led D2 if password and login match in display_message()
        if (Gameflag == 1)
            Blink(LED2);        // Start blinking led D2
  
        // Stop Blinking led D2 and exit main while loop
        if (Gameflag == 2) {
            StopBlink(LED2);    // STOP the blinking LED on D2  
            break;
        }
    }
}
