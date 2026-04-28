/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/28 15:17:10 by nrobinso         ###   ########.fr       */
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




int main(void) {

    uart_init();
    uart_printstr("ATmega328p started...\r\n\r\n");
    

    // print_eeprom(0);
    // print_eeprom(4);
    putnbr(print_eeprom_hexdump_line(63));

    print_eeprom_segment(0, 1023, 64);
    // putnbr(nbr);
    while(1) {
        


        
    }
}

