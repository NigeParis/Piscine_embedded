/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/25 14:36:49 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "tools.h"
#include "adc_lib.h"
#include "led_lib.h"
#include "timers.h"

typedef unsigned char uint8_t;      // needed because not using stdlib
typedef unsigned int uint16_t;      // needed because not using stdlib

volatile char hex[3];               // global for function toHex()
volatile char nbr_in_a_string[7];   // global variable for function nbr_to_str()
volatile char pot_reading[5];   // global variable for function nbr_to_str()

/// NOTE: function calls adc reading function 
/// ARGS: function to be called
/// RETURNS: None
void adc_getReading(void(*function)()) {
    function();    
}


/// NOTE: function set pins D1 to D$ for output
/// ARGS: string label of the pin 'D?' 
/// RETURNS: None
void SetPinOutput(char *str) {

    if (str[0] == 'D') {

        if (str[1] == '1') 
            DDRB |= (1 << PB0);
        if (str[1] == '2') 
            DDRB |= (1 << PB1);
        if (str[1] == '3') 
            DDRB |= (1 << PB2);
        if (str[1] == '4') 
            DDRB |= (1 << PB4);
        if (str[1] == '5') { 
            DDRD |= (1 << PD3);  // BLUE
            DDRD |= (1 << PD5);  // RED
            DDRD |= (1 << PD6);  // GREEN
        }            
    }   
}

/// NOTE: function set leds D1 to D4 
/// NOTE: also changes the Direction D5 to turn off PB# and P
void ledScaler8(uint8_t nbr) {

    if (nbr == 0 || nbr == 255) {
        DDRD &= ~(1 << PD3);  // BLUE on INPUT   
        DDRD &= ~(1 << PD6);  // GREEN on INPUT  
    } else {
        DDRD |= (1 << PD3);   // BLUE on OUTPUT 
        DDRD |= (1 << PD6);   // GREEN on OUTPUT 
    }
    if (nbr == 0 && nbr  <  64) {
            PORTB &= ~(1 << PB0);
            PORTB &= ~(1 << PB1);
            PORTB &= ~(1 << PB2);
            PORTB &= ~(1 << PB4);    
        } else if (nbr >= 64 && nbr < 128) {
                PORTB |= (1 << PB0);
                PORTB &= ~(1 << PB1);
                PORTB &= ~(1 << PB2);
                PORTB &= ~(1 << PB4);
            } else { if (nbr >= 128 && nbr < 192) {
                    PORTB |= (1 << PB0);
                    PORTB |= (1 << PB1);
                    PORTB &= ~(1 << PB2);
                    PORTB &= ~(1 << PB4);
                } else if (nbr >= 192 && nbr < 254) {
                        PORTB |= (1 << PB0);
                        PORTB |= (1 << PB1);
                        PORTB |= (1 << PB2);
                        PORTB &= ~(1 << PB4);    
                    } else if (nbr == 255){
                            PORTB |= (1 << PB0);
                            PORTB |= (1 << PB1);
                            PORTB |= (1 << PB2);
                            PORTB |= (1 << PB4);  
                        }
    }    
}

int main(void) {

    uint16_t buffer;                                            // reception buffer for Raw output
    
    adc_init();
    adc_select_pot();
    init_rgb();
    SetPinOutput("D1");
    SetPinOutput("D2");
    SetPinOutput("D3");
    SetPinOutput("D4");
    SetPinOutput("D5");

    while (1) { 
        
        adc_getReading(adc_select_pot);         // get reading raw
        buffer = adc_rx();             // transfer to buffer
        ledScaler8(buffer);
        wheel(buffer);                 
    }
}
    
