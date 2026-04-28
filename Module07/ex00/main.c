/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/28 14:56:08 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "tools.h"
#include "uart_lib.h"
#include "adc_lib.h"
#include "eeprom_lib.h"


/// NOTES: EEPROM in the atmega328P has i k of memory - 1024 bytes - page 29 datasheet
/// write/erase cycles 100,000

typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib

volatile char hex[4];               // global for function toHex()
volatile char nbr_in_a_string[7];   // global variable for function nbr_to_str()
volatile char pot_reading[5];       // global variable for function nbr_to_str()

// uint8_t eeprom_read(uint16_t address) {

//     while ((EECR & (1 << EERE)))
//         ;
//     EEAR = address;
//     EECR |= (1 << EERE);
//     return (EEDR);
    
// }

// void print_eeprom_hex_line(uint8_t line) {
    
//     uint16_t position_count = line * 16;
//     uint16_t line_end = (line + 1) * 16;    // 96
//     uint16_t line_start = (line * 16);  // 80
//     position_count = line_start;         // 80
    
//  for (uint16_t i = line_start; i < line_end; i++) {
//         uint8_t res = eeprom_read(0x000 + position_count);
//         toHex(res);
//         uart_printstr(hex);
//         uart_printstr(" ");
//         position_count++;
//         _delay_ms(80);
//     }
// }




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
        _delay_ms(80);
    }
    uart_tx('|');
}


void print_eeprom(uint8_t lineNbr){
        uint8_t position = lineNbr * 16;
        if ( position == 0)       
            uart_printstr("000000");
        else
            uart_printstr("00000");
        toHex_0xFFF(position);
        uart_printstr(hex);
        uart_printstr(" ");
        print_eeprom_hex_line(lineNbr);
        uart_printstr(" ");
        print_eeprom_line(lineNbr);
        uart_printstr("\r\n");
}





int main(void) {

    uart_init();
    uart_printstr("ATmega328p started...\r\n\r\n");
    

    // print_eeprom(0);
    // print_eeprom(4);
    // print_eeprom(6);

    print_eeprom_segment(1022, 1024, 64);
    // putnbr(nbr);
    while(1) {
        


        
    }
}

