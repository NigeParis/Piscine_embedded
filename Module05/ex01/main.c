/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/24 18:12:42 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "tools.h"
#include "uart_lib.h"
#include "adc_lib.h"

typedef unsigned char uint8_t;      // needed because not using stdlib
volatile char hex[3];               // global for function toHex()


/// NOTE: initiates the Light Dependent Resistor, pin PC1
/// ARGS: None
/// RETURNS: None


void adc_init(void) {
    
    ADMUX |= (1 << REFS0) | (1 << ADLAR);                           // PAGE 257 - LEFT ADJUST (ADLAF) and AVCC set with REFS0
    ADCSRA |= (1 << ADEN) | (1 << ADPS2)| (1 << ADPS0);             // PAGE 258 - Enable ADC (ADEN) - Division Factor 32 
    
}

void adc_init_ntc(void) {
        
    ADMUX &= ~((1 << MUX3) |(1 << MUX2)|(1 << MUX0));               // PAGE 258 - ADC0 set bits (0, 1, 3) to 0 for PC1
    ADMUX |= (1 << MUX1);                                          // and set bit 1 to 1 for PC1
}


// void adc_getReading(unsigned char buffer, void(*callback)()) {

//     callback();
//     buffer = adc_rx();
//     toHex(buffer);
//     adc_tx(hex[0]);
//     adc_tx(hex[1]);
    
// }


void adc_getReading(void(*callback)()) {

    callback();
    
}




int main(void) {

    unsigned char buffer = '\0';
    
    uart_Init();                     // initiate uarts connection
    adc_init(); 
    uart_printstr("connected to the AtMega328P Board ...\r\n");        

    
    while (1) { 
        
        adc_getReading(adc_init_pot);
        _delay_ms(1000);
        uart_tx(',');
        uart_tx(' ');
        adc_getReading(adc_init_ldr);
        _delay_ms(1000);
        uart_tx(',');
        uart_tx(' ');
        adc_getReading(adc_init_ntc);
        _delay_ms(1000);
        uart_tx('\r');
        uart_tx('\n');

    
    
    
    }

}
    
