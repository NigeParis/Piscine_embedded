/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 14:26:26 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/30 09:15:15 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>
#include <util/delay.h>
#include "eeprom_lib.h"
#include "uart_lib.h"
#include "tools.h"


/// NOTES: EEPROM in the atmega328P has i k of memory - 1024 bytes - page 29 datasheet
/// write/erase cycles 100,000

typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib

extern volatile char hex[4];                // global variable for function toHex()
extern volatile char nbr_in_a_string[20];    // global variable for function nbr_to_str()


/// NOTE: function reads from an address in the eeprom
/// ARGS: addess in the eeprom 0 to 1023 (1 k) 
/// RETURNS: uint8_t 8 bit char

uint8_t eeprom_read(uint16_t address) {

    while ((EECR & (1 << EERE)))
        ;
    EEAR = address;
    EECR |= (1 << EERE);
    return (EEDR);
    
}


/// NOTE: function reads the eprom line number (0 to  63)
/// Displays the next 16 bytes in hex (ex. line 1 -> 16 to 32)
/// ARGS: uint8_t line number to look for
/// RETURNS: index number of last byte printed

uint16_t print_eeprom_hex_line(uint8_t line) {
    
    if (line > 63) {
        uart_printstr("Error : in function line > 63 \r\n");
        return(0);
    }
    uint16_t position_count = line * 16;
    uint16_t line_end = (line + 1) * 16;    // 96
    uint16_t line_start = (line * 16);  // 80
    position_count = line_start;         // 80
    
    for (uint16_t i = line_start; i < line_end; i++) {
        uint8_t res = eeprom_read(0x000 + position_count);
        toHex(res);
        uart_printstr(hex);
        uart_printstr(" ");
        position_count++;
    }
    return (position_count);
}


/// NOTE: function prints out a segement of the eeprom in the atmega328p
/// ARGS: 3 uint16_t : start pos, end pos, size of the line 16 chars for example
/// RETURNS: Number of chars printed with uarts

uint16_t  print_eeprom_segment(uint16_t line_start, uint16_t line_end, uint16_t line_size) {

    int index = 0;
    if (line_end > 1023 || line_size > 1023){
        uart_printstr("Error: eeprom_segment fn size limit error\r\n");
        return (0) ;
    }
    for(uint16_t i = line_start; i < line_end; i++) {

        uint8_t res  = eeprom_read(0x000 + i);
        if (i %line_size == 0)
            uart_printstr("\r\n");
        if (Printable(res)) {
            uart_tx(res);
        } else {
            uart_tx('.');
        }
        index++;
    }
    return (index);
}


/// NOTE: function does a hexdump line of the eeprom in atmeag328p
/// ARGS: uint8_t 8 bit - line number 0 to 63 max
/// RETURNS: the index number of the last byte printed in the eeprom

uint16_t print_eeprom_hexdump_line(uint8_t lineNbr){
    
    uint16_t position = lineNbr * 16;
    uint16_t charNbr = position;
    
    if (position == 0)       
        uart_printstr("000000");
    else
        uart_printstr("00000");
    toHex_0xFFF(position);
    uart_printstr(hex);
    uart_printstr(" ");
    charNbr = print_eeprom_hex_line(lineNbr);
    uart_printstr(" ");
    print_eeprom_line(lineNbr);
    uart_printstr("\r\n");
    return (charNbr);
}

/// NOTE: function prints the contenu of a line and replaces un printables with '.'
/// ARGS: line number;
/// RETURNS: None

void print_eeprom_line(uint8_t line) {
    
    static volatile uint16_t position_count_ascii = 0;
    uint16_t line_end = (line + 1) * 16;    // 96
    uint16_t line_start = (line * 16);  // 80
    position_count_ascii = line_start;         // 80
    
    uart_tx('|');
    for (uint16_t i = line_start; i < line_end; i++) {               
        uint8_t res = eeprom_read(0x000 + position_count_ascii);
        if (Printable(res))
            uart_tx(res);
        else
            uart_tx('.');
        position_count_ascii++;
    }
    uart_tx('|');
}

