/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/25 11:45:51 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "tools.h"
#include "uart_lib.h"
#include "adc_lib.h"

typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib

volatile char hex[3];               // global for function toHex()
volatile char nbr_in_a_string[7];   // global variable for function nbr_to_str()
volatile char pot_reading[5];   // global variable for function nbr_to_str()
volatile char light_reading[5];   // global variable for function nbr_to_str()
volatile char temp_reading[5];   // global variable for function nbr_to_str()



void adc_getReading(void(*function)()) {
    function();    
}




void ft_transfer(volatile char *nbr_in_a_string, volatile char *reading) {

    int i = 0;
    while (nbr_in_a_string[i] != '\0') {

        reading[i] = nbr_in_a_string[i];
        i++;
    }
    reading[i] = '\0';

}



int main(void) {

    uint16_t buffer;
    // volatile char result[19];
    
    uart_Init();                     // initiate uarts connection
    adc_init_10_bit(); 
    uart_printstr("connected to the AtMega328P Board ...\r\n");        

    while (1) { 
        
        adc_getReading(adc_init_pot);
        buffer = adc_10_bit_rx();
        nbr_to_str(buffer);
        ft_transfer(nbr_in_a_string, pot_reading);        
        
        adc_getReading(adc_init_ldr);
        buffer = adc_10_bit_rx();
        nbr_to_str(buffer);
        ft_transfer(nbr_in_a_string, light_reading);        

        adc_getReading(adc_init_ntc);
        buffer = adc_10_bit_rx();
        nbr_to_str(buffer);
        ft_transfer(nbr_in_a_string, temp_reading);        

        _delay_ms(20);
        
        uart_printstr(pot_reading);
        uart_printstr(", ");
        uart_printstr(light_reading);
        uart_printstr(", ");
        uart_printstr(temp_reading);

        uart_printstr("\r\n");

    }

}
    
