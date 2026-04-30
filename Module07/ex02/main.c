/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/30 12:11:52 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>                         // delay for the debounce

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

#define WRITE_ERROR 200  // eeprom status define


struct divice_config {

    uint16_t magic;
    uint32_t id;
    int16_t  priority;    
    char tag[33];
    uint16_t check;
};




volatile char hex[4];               // global for function toHex()
volatile char nbr_in_a_string[33];  // global variable for function nbr_to_str()
volatile uint8_t status;            // global status eeprom   

// Definitions - actual memory allocation
// flags for input and output
volatile char buffer[BUFFER];
volatile uint8_t RX_index;
volatile uint8_t inputflag;
volatile uint8_t promptflag;
volatile uint8_t check_inputflag;
volatile uint8_t epprom_write_flag;
char cmd[BUFFER] = {0};
char value[BUFFER] = {0};




void resetFlags(void);
char backspace(char c);
void remove_spaces(char *str, char *dest);
void resetPrompt(char *hexValue, char* hexAddress);
bool check_input(char *str);
void empty_str(char *str);
void remove_spaces(char *str, char *dest);
void get_hexValue(char *str, char *dest);
void  get_hexAddress(char *str, char *dest);
void ft_strcpy(char *str, char *dest);



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

        // receive data
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
            RX_index++;      
        } else {
            c = 0x7F;
            backspace(c);
        }
    }
}


void eeprom_write_str(uint16_t start_Address, volatile char *string, uint8_t len) {

    uint8_t index = 0;
    uint8_t count = 0;

    while (string[index] != '\0') {

        count += eeprom_update((start_Address), string[index]);
        pause_in_milliseconds(20);
        start_Address++;
        index++;
    }
    count += eeprom_update((start_Address), '\0');
    start_Address++;
    index++;
    while (index < len) {
        string[index] = '\0';
        count += eeprom_update((start_Address), 0);
        start_Address++;
        index++;
    }
    // uart_printstr("\r\nDEBUG eep write_str : ");
    // uart_printstr(string);
    // uart_printstr("\r\nDEBUG eep write_str END: \r\n");
    uart_printstr("\r\nupdated chars: ");
    putnbr(count);
    uart_printstr("\r\n");

    
}




void eeprom_clear(void) {

    uint16_t index = 0;
    
    for (uint16_t i = 0; i < 1024; i++) {
        eeprom_update(index, 255);
        index++;        
    }
}



uint8_t eeprom_write_structure(uint16_t address, struct divice_config *setup) {

    (void)address;
    uint8_t len  = 0;
    uint16_t tempNbr = 0;


    nbr_to_str(setup->magic);    
    // uart_printstr(nbr_in_a_string);
    eeprom_write_str(address, nbr_in_a_string, 16);
    // len = ft_strlen((unsigned char*)nbr_in_a_string) + 1;
    len += 16;
    nbr_in_a_string[0] = '\0';
    // putnbr_32t(setup->id);
    nbr_to_str(setup->id); 
    // uart_printstr("DEBUG: Nbr in string set_id : ");
    // uart_printstr(nbr_in_a_string);
    // uart_printstr("\r\n");
    // uart_printstr("DEBUG: Nbr in string END\r\n");
    eeprom_write_str(address + len, nbr_in_a_string, 32);
    // len += ft_strlen((unsigned char*)nbr_in_a_string) + 1;
    len += 32;

    if (setup->priority < 0) {
        tempNbr = setup->priority * -1;
        nbr_in_a_string[0] = '-';
    }
    nbr_to_str(tempNbr);
    // uart_printstr("\r\nDEBUG: set_priority: ");
    // uart_printstr(nbr_in_a_string);
    // uart_printstr("\r\nDEBUG: set_priority END\r\n");
    eeprom_write_str(address + len, nbr_in_a_string, 16);
    // len += ft_strlen((unsigned char*)nbr_in_a_string) + 1;
    len += 16;
    nbr_in_a_string[0] = '\0';

    eeprom_write_str(address + len, setup->tag, 33);
    len += 33;
    
    nbr_to_str(setup->check);
    // uart_printstr("\r\nDEBUG: set_check: ");
    // uart_printstr(nbr_in_a_string);
    // uart_printstr("\r\nDEBUG: set_check END\r\n");

    eeprom_write_str(address + len, nbr_in_a_string, 16);
    // len = ft_strlen((unsigned char*)nbr_in_a_string) + 1;
    len += 16;
   

    return (status);   
}



void ft_strcpy(char *str, char *dest) {

    uint8_t index = 0;

    while (str && str[index]) {
        dest[index] = str[index];
        index++;
    }
    dest[index] = '\0';
}

// struct divice_config {

//     uint16_t magic;
//     uint32_t id;
//     int16_t  priority;    
//     char tag[33];
//     uint16_t check;
// };

#define SLOT1 0
#define SLOT2 96
#define SLOT3 192
#define SLOT4 288



void get_cmd (char *str, char *cmd, char *value) {

    uint8_t index = 0;
    uint8_t cmd_index = 0;
    uint8_t value_index = 0;
        
    while (str[index] != '\0') {

        while (str && str[index] != ' ') {

            cmd[cmd_index] = str[index];
            cmd_index++;
            index++;
        }
        cmd[cmd_index] = '\0';   
        index++;
        while (str && str[index]) {
            value[value_index] = str[index];
            value_index++;
            index++;
        }
        value[value_index] = '\0';
    }

    // uart_printstr("\r\n");
    // uart_printstr(cmd);
    // uart_printstr(" ");
    // uart_printstr(value);
    // uart_printstr("\r\n");
}


void set_id (char *cmd_arg) {

    if (!Printable(cmd_arg[0]))
        return;

    
}



int  main( void ) {

    status = 0;     // global status of eeprom

    char input[BUFFER] = {0};
    char formatted_input[BUFFER] = {0};
    char hexValue[3] = {'\0', '\0', '\0'};
    char hexAddress[4] = {'\0', '\0', '\0', '\0'};
    nbr_in_a_string[0] = '\0';

    struct divice_config setup;
    setup.magic = 56535;
    setup.id = 42949;
    setup.priority = -126;
    ft_strcpy("0123456789", setup.tag);
    setup.check = 22222;
    (void)setup;
    
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

        // paring check
        if (check_inputflag == 1) {

            remove_spaces(input, formatted_input);
            get_cmd(formatted_input, cmd, value);

            if (ft_strcmp((unsigned char*)formatted_input , (unsigned char*)"delete")) {
                eeprom_clear();
                resetPrompt(hexValue, hexAddress);
                continue;
            }
            if (ft_strcmp((unsigned char*)formatted_input , (unsigned char*)"set_id")) {
                uart_printstr("\r\nSET_ID called\r\n");
                set_id(value);
                resetPrompt(hexValue, hexAddress);
                continue;
            }

            if (ft_strcmp((unsigned char*)cmd , (unsigned char*)"display")) {
                uart_printstr("\r\nDISPLAY called\r\n");
                eeprom_dispay(0, nbrStr_to_dec(value), 0, 0);            
                resetPrompt(hexValue, hexAddress);
                continue;
            }
            if (check_input(formatted_input)) { 
                // uart_printstr("\r\n");
                // uart_printstr(formatted_input);
                // uart_printstr("\r\n");


                
                epprom_write_flag = 1;
            } else {

                // uart_printstr(formatted_input);
                resetPrompt(hexValue, hexAddress);
            }           
        }

        // check if value is already in the address - write it if not and display
        if (epprom_write_flag == 1) {

            

            eeprom_write_structure(SLOT1, &setup);

            
            pause_in_milliseconds(500);

            // uart_printstr("\n\r DEBUG status\r\n");
           
            putnbr_32t(status);    
            if (status == WRITE_ERROR) {uart_printstr("Corruption detected\n\r");}
            uart_printstr("\r\n");
            // eeprom_dispay(0, 26, 0, 0);            
            resetFlags();
            uart_printstr("\r\n");
        }
    }
}



void resetFlags(void) {

    RX_index = 0;
    inputflag = 0;
    promptflag = 0;
    check_inputflag = 0;
    epprom_write_flag = 0;
    status = 0;
    buffer[0] = '\0';
    cmd[0] = '\0';
    value[0] = '\0';
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


void remove_spaces(char *str, char *dest) {

    int index = 0;
    int dest_index = 0;

    while (str[index] == ' ')
        index++;
    
    while (str && str[index]) {
        
        if (str[index] == ' ') {
        dest[dest_index] = str[index];
            index++;
            dest_index++;
        }
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




void  get_hexAddress(char *str, char *dest) {

    int len = ft_strlen((uint8_t*)str);
    if (len == 4) {
        dest[0] = str[0];
        dest[1] = str[1];
        dest[2] = '\0';   
    }
    if (len == 5) {
        dest[0] = str[0];
        dest[1] = str[1];
        dest[2] = str[2];
        dest[3] = '\0';
    }
}

