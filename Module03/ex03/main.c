/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/20 14:46:47 by nrobinso         ###   ########.fr       */
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
    
    (void)password_string;
    
    uart_Init();
    uart_Init_interupts();     
    resetFlags();
    
    while (1) {

        // display login invitation
        if (Beginflag == 0) {
            uart_printstr("Enter color format : #RRGGBB\n\r   hex: ");
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

        // check PARSING HEX login name and flag it
        if (CheckNameflag == 1 && GetNameStringflag == 1) {
            if (ft_strcmp(string, NAME)) {
                Nameflag = 0;
            } else {
                Nameflag = 1;
            }
            CheckNameflag = 0;
            PassTitleflag = 1;
        }


        if (Nameflag != 2 && Gameflag == 0) {

            display_message(string);
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
