/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/25 11:11:19 by nrobinso         ###   ########.fr       */
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

volatile char hex[4];               // global for function toHex()
volatile char nbr_in_a_string[7];   // global variable for function nbr_to_str()



void adc_getReading(void(*function)()) {
    function();    
}



/// NOTE: function receives (Rx) a char form ADCH - page  259
/// ARGS: None
/// RETURNS: unsigned char from ADCH - 8 bit (ADLAR is set to 1)
/// NEEDS: to be initiated with ADCSRA and ADMUX see page 258

uint16_t adc_10_bit_rx(void) {
    
    uint16_t c;
    ADCSRA |= (1 << ADSC);                              // page 258 / 24.9.2 start conversion
    while (!(ADCSRA & (1 << ADSC)) == 0) ;              // Recieve data Flag - page 200
    c = ADC;                                           // receive data
    return (c);                                         // return c
}




int main(void) {

    uint16_t buffer;
    // volatile char result[19];
    
    uart_Init();                     // initiate uarts connection
    adc_init_10_bit(); 
    uart_printstr("connected to the AtMega328P Board ...\r\n");        

    // uint16_t i = 0;
    while (1) { 
        
        adc_getReading(adc_init_pot);
        buffer = adc_10_bit_rx();
        // toHex(buffer);
    
        // result[0] = hex[0];
        // result[1] = hex[1];
        // result[3] = hex[2];
        // result[4] = ',';
        // result[5] = ' ';
     
        // adc_getReading(adc_init_ldr);

        // buffer = adc_rx();
        // toHex(buffer);

        // result[4] = hex[0];
        // result[5] = hex[1];
        // result[6] = ',';
        // result[7] = ' ';
        
        // adc_getReading(adc_init_ntc);
        
        // buffer = adc_rx();
        // toHex(buffer);

        // result[8] = hex[0];
        // result[9] = hex[1];
        // result[10] = '\r';
        // result[11] = '\n';
        // result[12] = '\0';
        
        // uart_printstr(result);
        nbr_to_str(buffer);
        uart_printstr("\"");
        uart_printstr(nbr_in_a_string);
        uart_printstr("\"");
        uart_printstr("\r\n");
    }

}
    
