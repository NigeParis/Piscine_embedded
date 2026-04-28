/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc_lib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 12:53:23 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/28 09:43:36 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "adc_lib.h"
#include <avr/io.h>

typedef unsigned int uint16_t;      // needed because not using stdlib


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


/// NOTE: initiates adc in mode AVCC with divison factor 32 - Left Adjust
/// ARGS: None
/// RETURNS: None

void adc_init(void) {
    
    ADMUX |= (1 << REFS0) | (1 << ADLAR);                           // PAGE 257 - LEFT ADJUST (ADLAF) and AVCC set with REFS0
    ADCSRA |= (1 << ADEN) | (1 << ADPS0)| (1 << ADPS2);             // PAGE 258 - Enable ADC (ADEN) - Division Factor 32 
}


void adc_init_10_bit(void) {
    
    ADMUX |= (1 << REFS0);                           // PAGE 257 - NO ADJUST (ADLAF) and AVCC set with REFS0 10 bit
    ADMUX &= ~(1 << ADLAR);                           // PAGE 257 - NO LEFT ADJUST (ADLAF) and AVCC set with REFS0
    ADCSRA |= (1 << ADEN) | (1 << ADPS0)| (1 << ADPS2);             // PAGE 258 - Enable ADC (ADEN) - Division Factor 32 
}




/// NOTE: initiates the Temperature sensor, pin PC2
/// ARGS: None
/// RETURNS: None

void adc_select_ntc(void) {
    
    ADMUX &= ~((1 << MUX3) |(1 << MUX2)|(1 << MUX0));               // PAGE 258 - ADC0 set bits (0, 1, 3) to 0 for PC2
    ADMUX |= (1 << MUX1);                                          // and set bit 1 to 1 for PC1
}


/// NOTE: initiates the Light Dependent Resistor, pin PC1
/// ARGS: None
/// RETURNS: None

void adc_select_ldr(void) {
    
    ADMUX &= ~((1 << MUX3) |(1 << MUX2)|(1 << MUX1));               // PAGE 258 - ADC0 set bits (1 to 3) to 0 for PC1
    ADMUX |= (1 << MUX0);                                          // and set bit 0 to 1 for PC1
}


/// NOTE: initiates the potentiometre pin PC0
/// ARGS: None
/// RETURNS: None

void adc_select_pot(void) {
        
    ADMUX &= ~((1 << MUX3) |(1 << MUX2)|(1 << MUX1)|(1 << MUX0));   // PAGE 258 - ADC0 set bits all to 0 for PC0
}



/// NOTE: function receives (Rx) a char from ADC full 10 bits - page  259
/// ARGS: None
/// RETURNS: uint16_t from ADC - 10 bit (ADLAR is set to 0)
/// NEEDS: to be initiated with ADCSRA and ADMUX see page 258

uint16_t adc_10_bit_rx(void) {
    
    uint16_t c;
    ADCSRA |= (1 << ADSC);                              // page 258 / 24.9.2 start conversion
    while (!(ADCSRA & (1 << ADSC)) == 0) ;              // Recieve data Flag - page 200
    c = ADC;                                           // receive data
    return (c);                                         // return c
}






void adc_init_internal_temp_sensor(void) {
    
    ADMUX |= (1 << REFS0) | (1 << REFS1);               // PAGE 257 - NO ADJUST (ADLAF) 1.1 volt ref REFS0 + REFS1
    ADMUX &= ~(1 << ADLAR);                             // PAGE 257 - NO LEFT ADJUST (ADLAF) and AVCC set with REFS0
    ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // PAGE 258 - Enable ADC (ADEN) - Division Factor 32 

}

void adc_select_temp_sensor_interne(void) {
    
    ADMUX |= (1 << MUX3);                               // PAGE 258 - ADC8 set bits all to 0 for Int Temp Sensor
    ADMUX &= ~((1 << MUX2)|(1 << MUX1)|(1 << MUX0));    // PAGE 258 - ADC8 set bits all to 0 for Int Temp Sensor

}    




