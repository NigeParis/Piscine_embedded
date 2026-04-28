/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/28 15:56:03 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
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



/// NOTE: function display contenu of the eeprom in hex dump format 
/// [address start 0xfff number] [16 bytes in 0xff in 8 bits] [printable unsigned chars visable]
/// ARGS: None
/// RETURNS: None

void eeprom_dispay(void) {

    for (int i = 0; i < 64; i++) {
        print_eeprom_hexdump_line(i);
    }
}



int main(void) {

    uart_init();
    uart_printstr("ATmega328p started...\r\n\r\n");
    eeprom_dispay();
    while(1) {

    }
}

