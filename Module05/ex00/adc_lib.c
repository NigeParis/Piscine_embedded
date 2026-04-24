/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc_lib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 12:53:23 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/24 17:07:08 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "adc_lib.h"
#include <avr/io.h>

/// NOTE: initiates the potentiometre pin PC0
/// ARGS: None
/// RETURNS: None

void adc_init_pot(void) {
        
        ADMUX |= (1 << REFS0) | (1 << ADLAR);                           // PAGE 257 - LEFT ADJUST (ADLAF) and AVCC set with REFS0
        ADMUX &= ~((1 << MUX3) |(1 << MUX2)|(1 << MUX1)|(1 << MUX0));   // PAGE 258 - ADC0 set bits all to 0 for PC0
        ADCSRA |= (1 << ADEN) | (1 << ADPS2)| (1 << ADPS0);             // PAGE 258 - Enable ADC (ADEN) - Division Factor 32 
}


/// NOTE: function sends (tx) to screen a char 
/// ARGS: unsigned char c
/// RETURNS: None
/// NEEDS: to be initiated with ADCSRA and ADMUX see page 258

void adc_tx(volatile unsigned char c)
{
    // Wait for empty transmit buffer
    while (!(ADCSRA & (1 << ADSC)) == 0) ;              // Wait while ADSC bit is not 0 message - page 258 
    UDR0 = c;                                           // Put data into buffer, sends the data
}

/// NOTE: function receives (Rx) a char form ADCH - page  259
/// ARGS: None
/// RETURNS: unsigned char from ADCH - 8 bit (ADLAR is set to 1)
/// NEEDS: to be initiated with ADCSRA and ADMUX see page 258

unsigned char adc_rx(void) {
    
    unsigned char c;
    ADCSRA |= (1 << ADSC);                              // page 258 / 24.9.2 start conversion
    while (!(ADCSRA & (1 << ADSC)) == 0) ;              // Recieve data Flag - page 200
    c = ADCH;                                           // receive data
    return (c);                                         // return c
}

