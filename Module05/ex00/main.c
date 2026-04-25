/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/25 14:43:21 by nrobinso         ###   ########.fr       */
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


/// NOTE: initiates adc adds division factor
/// ARGS: None
/// RETURNS: None

void adc_init(void) {
    
    ADCSRA |= (1 << ADEN) | (1 << ADPS2)| (1 << ADPS0);             // PAGE 258 - Enable ADC (ADEN) - Division Factor 32 
    ADMUX |= (1 << REFS0) | (1 << ADLAR);                           // PAGE 257 - LEFT ADJUST (ADLAF) and AVCC set with REFS0
}



int main(void) {

    unsigned char buffer = '\0';
    
    uart_Init();                     // initiate uarts connection
    adc_init(); 
    uart_printstr("connected to the AtMega328P Board ...\r\n\n\n");        

    while (1) { 
        
        adc_init_pot();
        _delay_ms(20);
        buffer = adc_rx();
        toHex(buffer);
        adc_tx(hex[0]);
        adc_tx(hex[1]);
        uart_tx('\r');
        uart_tx('\n');
    }

}
    
