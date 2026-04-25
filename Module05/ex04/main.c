/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/25 12:48:07 by nrobinso         ###   ########.fr       */
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

/// NOTE: function calls adc reading function 
/// ARGS: function to be called
/// RETURNS: None

void adc_getReading(void(*function)()) {
    function();    
}


/// NOTE: function transfers the string number collected to the correct reading string
/// ARGS: pointer to string, pointer to target
/// RETURNS: None

void ft_transfer(volatile char *nbr_in_a_string, volatile char *reading) {

    int i = 0;
    while (nbr_in_a_string[i] != '\0') {

        reading[i] = nbr_in_a_string[i];
        i++;
    }
    reading[i] = '\0';

}



int main(void) {

    uint16_t buffer;                                            // reception buffer for Raw output
    
    uart_Init();                                                // initiate uarts connection
    adc_init_internal_temp_sensor();                            // initiates the internal sensor params - page 256
    uart_printstr("connected to the AtMega328P Board ...\r\n"); // connected message       

    while (1) { 
        
        adc_getReading(adc_select_temp_sensor_interne);         // get reading raw
        buffer = adc_10_bit_rx();                               // transfer to buffer
        nbr_to_str((buffer - 353) * 100 / 122);                 // T = { [(ADCH << 8) | ADCL] - TOS} / k calculation - page 257
        ft_transfer(nbr_in_a_string, temp_reading);             // add result to reading variable

        _delay_ms(20);
        
        uart_printstr(temp_reading);                            // output reading
        uart_printstr("ºC");
        uart_printstr("\r\n");

    }

}
    
