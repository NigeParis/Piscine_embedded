/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/28 17:52:16 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/eeprom.h>

#include "uart_lib.h"
// #include "led_lib.h"
#include "tools.h"
#include "config.h"
#include "adc_lib.h"
#include "eeprom_lib.h"

/// NOTES: EEPROM in the atmega328P has i k of memory - 1024 bytes - page 29 datasheet
/// write/erase cycles 100,000
/// for scroll in screens cntrl 'a['  and to quit 'q'

typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib

volatile char hex[4];               // global for function toHex()
volatile char nbr_in_a_string[7];   // global variable for function nbr_to_str()


// Definitions - actual memory allocation
// flags for input and output
volatile char buffer[BUFFER];
volatile uint8_t RX_index;
volatile uint8_t inputflag;
volatile uint8_t promptflag;
volatile uint8_t check_inputflag;
volatile uint8_t run_flag;


void resetFlags(void) {

    RX_index = 0;
    inputflag = 0;
    promptflag = 0;
    check_inputflag = 0;
    run_flag = 0;
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



/// NOTE: // USART_RX interupt 19 - page 66
/// ARGS: None
/// RETURNS: None
/// REF: Page 66 - Table 12.1 Vector Nbr: 19
/// USART RX Complete interrupt (fires when a byte arrives on UART).

void __vector_18(void) __attribute__((signal));
void __vector_18(void)
{
    volatile char c;

    c = uart_interupt_rx();
    if (checkChar(c))
        return;
    if (notPrintable(c))
        return;
   
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
        inputflag = 1;
    }
    else {
        if (RX_index < 11){       
            uart_interupt_tx(buffer[RX_index]); 
            RX_index++;      // receive data
        } else {
            c = 0x7F;
            backspace(c);
        }
    }
}


void remove_spaces(char *str, char *dest) {

    int index = 0;
    int dest_index = 0;

    while (str[index] == ' ')
        index++;
    
    while (str && str[index]) {

        while (str[index] == ' ')
            index++;
        dest[dest_index] = str[index];
        index++;
        dest_index++;
    }    
    dest[dest_index] = '\0';
}


void get_hexValue(char *str, char *dest) {

    int index = ft_strlen((uint8_t*)str);
    
    dest[0] = str[index - 2];
    dest[1] = str[index - 1];
    dest[2] = '\0';
}


void empty_str(char *str) {
    str[0] = '\0';
}


bool check_input(char *str){
    
    int len = ft_strlen((uint8_t*)str);
    if (len < 4 || len > 5) {
        return (0);
    }
    return(1);
}


void resetPrompt(char *hexValue, char* hexAddress) {
    uart_printstr("\r\n");
    empty_str(hexValue);
    empty_str(hexAddress);
    resetFlags();
}


int  main( void ) {

    char input[BUFFER] = {0};
    char formatted_input[BUFFER] = {0};
    char hexValue[3] = {'\0', '\0', '\0'};
    char hexAddress[4] = {'\0', '\0', '\0', '\0'};
    
    int i = 0;
    
    uart_init();
    uart_Init_interupts();     
    resetFlags();
    
    while (1) {

        // display prompt
        if (promptflag == 0) {
            uart_printstr("> ");
            promptflag = 1; 
        }

        // get input from buffer
        if (promptflag == 1 && inputflag == 1) {
            i = 0;
            while (buffer[i] != '\0') {
                input[i] = to_lower(buffer[i]);
                i++;        
            }
            input[i] = '\0';
            check_inputflag = 1;
            inputflag = 0;
        }

       
        if (check_inputflag == 1) {

            remove_spaces(input, formatted_input);
            // uart_printstr("\r\n");
            // uart_printstr(formatted_input);
            if (check_input(formatted_input)) { 
                get_hexValue(formatted_input, hexValue);
                if (!is_valid_hex_str((uint8_t*)formatted_input)) {

                    uart_printstr("\r\n");
                    empty_str(hexValue);
                    empty_str(hexAddress);
                    resetFlags();
                    continue ;
                    
                }
                uart_printstr("\r\n");
                uart_printstr(hexValue);
                run_flag = 1;
            } else {
                resetPrompt(hexValue, hexAddress);
                // uart_printstr("\r\n");
                // empty_str(hexValue);
                // empty_str(hexAddress);
                // resetFlags();
            }           
        }


        if (run_flag == 1) {
            uart_printstr("\r\n");
            eeprom_dispay(0, 10);
            resetFlags();
            uart_printstr("\r\n");
        }
    }
}

