/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/21 15:06:09 by nrobinso         ###   ########.fr       */
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

typedef unsigned char uint8_t;

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
unsigned char hextest[3] = {'a','d','\0'};

void resetFlags(void) {

    RX_index = 0;
    got_hex_input_flag = 0;
    return_key_flag = 0;
    valid_format_flag = 2;
    start_flag = 0;
    check_format_flag = 0;
    notValidChar = 0;
    program_led_flag = 0;
    buffer[0] = '\0';
}


uint8_t convert_from_hex(unsigned char *str) {

    volatile int i = 0;
    volatile uint8_t high_end = 0;
    volatile uint8_t low_end = 0;
    volatile uint8_t res = 0;
    
    if (!str)
        return (0);

    while (str && str[i]) {

        if (i == 0) {
            if (is_digit(str[i])) {
                high_end = (str[i] - '0');      // high nibble × 16
            } else if (str[i] >= 'a' && str[i] <= 'f') {
                high_end = (str[i] - 'a') + 10;
            }
        }

        if  (i == 1) {
                if (is_digit(str[i]))
                    low_end += (str[i] - '0');            // low nibble, no multiplier
                else if (str[i] >= 'a' && str[i] <= 'f')
                    low_end += (str[i] - 'a') + 10;
            }
            i++;
        }
        res = (((high_end) * 16) + low_end);
    return res;
}





char backspace(unsigned char c) {        

    if (c == 0x7F) {
        if (RX_index > 0) {
            uart_printstr("\b \b");
            RX_index--;
            return c;
        }
    }
    return c;
}


void display_message (unsigned char *str) {
    
     if (valid_format_flag == 1) {
         init_rgb();
        
        uart_printstr("\n\rColor is ");          
        uart_printstr((char*)str);   
        uart_printstr(" !\n\rTry another colour ? [y/n]: ");
        DDRD |= LED_RED;    // open for output on PD5
        DDRD |= LED_BLUE;    // open for output on PD5
        DDRD |= LED_GREEN;  // open for output on PD6    
        init_rgb();
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
            if ( c == 'n') {
                uart_printstr("\n\rbye!");
                stop_timers();
            } else {
                return;
            }
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

bool is_valid_input(unsigned char *str) {
    
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

    
    unsigned char string[BUFFER] = {0};         // copy of input from keyboard buffer
    unsigned char hex_red[10] = {0};            // RGB hex strings
    unsigned char hex_blue[10] = {0};
    unsigned char hex_green[10] = {0};

    int red = 0;                // decimal values used in set_rgb function
    int blue = 0;
    int green = 0;

    
    int i = 0;
        
    
    uart_Init();                 // uart set up params - uart lib
    uart_Init_interupts();       // interupt for getting key input
    resetFlags();                // set all flags at start up
    
    
    while (1) {

        // display invitation
        if (start_flag == 0) {
            uart_printstr("Enter color format hex: #RRGGBB\n\r   hex: ");
            start_flag = 1; 
        }

        // get input login name fron buffer
        if (start_flag == 1 && return_key_flag == 1 && got_hex_input_flag == 0) {
            i = 0;
            while (buffer[i] != '\0') {
                string[i] = to_lower(buffer[i]);
                i++;        
            }
            string[i] = '\0';
            got_hex_input_flag = 1;
            check_format_flag = 1;
            return_key_flag = 0;
        }
        
        // check and make PARSING HEX and flag it
        if (check_format_flag == 1 && got_hex_input_flag == 1) {

            if (is_valid_input(string)) { 

                split_hex(string, hex_red, 0);          // split 0xFFFFFF int three rgb hex string
                split_hex(string, hex_green, 2);
                split_hex(string, hex_blue, 4);
          
                red = convert_from_hex(hex_red);        // convert to dec each 0xFF
                green = convert_from_hex(hex_green);
                blue = convert_from_hex(hex_blue);
                
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
        set_rgb(red, green, blue);

        
        if (program_led_flag == 1) {
            
            if (red == 0 || green == 0 || blue == 0) {
            
                if (red == 0)
                    DDRD &= ~(LED_RED);    // close for output on PD5
            
                if (green == 0)
                    DDRD &= ~(LED_GREEN);  // close for output on PD6
            
                if (blue == 0)
                    DDRD &= ~(LED_BLUE);   // close for output on PD5
            } 
        }
        
        // Stop program
        if (program_led_flag == 2) {
            break;
        }
        
    }
}
