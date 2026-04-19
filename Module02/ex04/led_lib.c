/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   led_lib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 09:12:01 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/19 09:53:50 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include "led_lib.h"

#define PRESCALIER 256
#define TIME_FREQUENCY  (F_CPU / PRESCALIER)            // 16 Mhz / 256 = 62500

/// NOTE: function blinks LED D2 - PB1 using OCR1A and timer1 CTC
/// ARGS: Led Mask ex : (1 << PB1)
/// RETURNS: None
/// REFS: page 132 - Datasheet

void Blink(uint8_t led) {
    
    if (led == (1 << PB1)) {

        DDRB |= led;                   // PB1 set for OUTPUT - D2 led
        
        TCCR1A |= (1 << COM1A0);        // SET CTC mode - Toggle OC1A/OC1B on Compare Match. page 140 
        
        TCCR1B |= (1 << WGM12);         // Set CTC on TOP OCR1A NB - TCCR1B timer counter regiister B setting - page 142
        
        TCCR1B |= (1 << CS12);          // Set Clk prescaler to F_CPU/256 = 1MHz/256 = 0.256 (ms) per tick - page 139 & 143 
        
        OCR1A = (TIME_FREQUENCY / 2);   // set Value to 50%  - 0.5 sec. - page 129// Set value - 50%
    }    
}

/// NOTE: function Stops blinks of LED D2 - PB1 using OCR1A and timer1 CTC
/// ARGS: Led Mask ex : (1 << PB1)
/// RETURNS: None
/// REFS: page 132 - Datasheet

void StopBlink(uint8_t led) {

    if (led == (1 << PB1)) {
        TCCR1A &= ~(1 << COM1A0);       // Disable OC1A output by stopping CTC timer1
    }
}
