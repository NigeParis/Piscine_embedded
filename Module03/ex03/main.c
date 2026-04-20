/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/20 17:16:59 by nrobinso         ###   ########.fr       */
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

#define INPUT_SIZE_MAX 7

// Definitions - actual memory allocation
// flags for ordering the name and password input and output
volatile char buffer[BUFFER];
volatile uint8_t RX_index;
volatile uint8_t got_hex_input_flag;
volatile uint8_t return_key_flag;
volatile uint8_t valid_format_flag;
volatile uint8_t start_flag;
volatile uint8_t check_format_flag;
volatile uint8_t CheckPassflag;
volatile uint8_t notValidChar;
volatile uint8_t program_led_flag;
volatile uint8_t test = 0;

void resetFlags(void) {

    RX_index = 0;
    got_hex_input_flag = 0;
    return_key_flag = 0;
    valid_format_flag = 2;
    start_flag = 0;
    check_format_flag = 0;
    notValidChar = 0;
    program_led_flag = 0;
    test = 0;
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
    
     if (valid_format_flag == 1) {
         init_rgb();
        
         uart_printstr("\n\rColor is #");          
         uart_printstr(str);   
         uart_printstr("!\n\rTry another colour ? [y/n]: ");
         program_led_flag = 1;
     } else {
         uart_printstr("\n\rFormat error - #RRGGBB\n\r\n\r");
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
    if (program_led_flag == 1) {
        if (c == 'y') {
            uart_printstr("\n\r\n\r");
            resetFlags();
            stop_timers();
            return;
        } else {
            uart_printstr("\n\rbye!");
            stop_timers();
        }
        program_led_flag = 2;
        return;
    }    
    if (RX_index < INPUT_SIZE_MAX) {
        c = backspace(c);
        if (c != 0x7F && RX_index < INPUT_SIZE_MAX) {
            buffer[RX_index] = c;
        } else { 
            return;
        }
    }
    if (c == '\n'|| c == '\r') {
        buffer[RX_index] = '\0';
        RX_index = 0;
        return_key_flag = 1;
    }
    else {
        if (RX_index < INPUT_SIZE_MAX){       
                uart_interupt_tx(buffer[RX_index]); 
            RX_index++;// receive data
        } else {
            c = 0x7F;
            backspace(c);
        }
    }
}

bool is_valid_input(char *str) {
    
    valid_format_flag = 1;        
    if (!str)
        return (0);    

    if (!is_len_size(str, INPUT_SIZE_MAX)) {  
        valid_format_flag = 0;
        return (0);
    }
    if (!is_first_char(str, '#')) {
        valid_format_flag = 0;
        return (0);         
    }
    if (!is_valid_hex_str(str)) {
        valid_format_flag = 0;
        return (0);         
    }                      
    return (1);
}

int main(void) {
        
    DDRD |= LED_RED;    // open for output on PD5  
    DDRD |= LED_GREEN;  // open for output on PD6
    DDRD |= LED_BLUE;   // open for output on PD3

    
    char string[BUFFER] = {0};
    char hex_string[BUFFER] = {0};
    int i = 0;
    
    (void)hex_string;
    
    uart_Init();
    uart_Init_interupts();     
    resetFlags();
    
    while (1) {

        // display login invitation
        if (start_flag == 0) {
            uart_printstr("Enter color format hex: #RRGGBB\n\r   hex: ");
            start_flag = 1; 
        }

        // get input login name fron buffer
        if (start_flag == 1 && return_key_flag == 1 && got_hex_input_flag == 0) {
            i = 0;
            while (buffer[i] != '\0') {
                string[i] = buffer[i];
                i++;        
            }
            string[i] = '\0';
            got_hex_input_flag = 1;
            check_format_flag = 1;
            return_key_flag = 0;
        }

        // check PARSING HEX login name and flag it
        if (check_format_flag == 1 && got_hex_input_flag == 1) {

            if (is_valid_input(string)) {                
                valid_format_flag = 1;        
            } else {
                valid_format_flag = 0;
            }
            check_format_flag = 0;
        }


        if (valid_format_flag != 2 && program_led_flag == 0) {

            display_message(string);
            valid_format_flag = 2; 
        }   

        // Set led PD3,5,6 to color
        if (program_led_flag == 1) {
        
            test++;
            if (test>2)
                test = 1;
            if (test == 1)
                set_rgb(255,0,0);  // set colour
            if (test == 2)
                set_rgb(0,0,255);  // set colour
        }
        // Stop program
        if (program_led_flag == 2) {
            break;
        }
    }
}
