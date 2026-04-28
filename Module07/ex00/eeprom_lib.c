/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 14:26:26 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/28 14:39:03 by nrobinso         ###   ########.fr       */
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
extern volatile char nbr_in_a_string[7];    // global variable for function nbr_to_str()


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
/// RETURNS: None

void print_eeprom_hex_line(uint8_t line) {
    
    if (line > 63) {
        uart_printstr("Error : in function line > 63");
        return;
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
        _delay_ms(80);
    }
}


