/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   led_lib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 09:12:01 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/20 14:17:34 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include "led_lib.h"
#include "timers.h"


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

/// NOTE: function Sets the Wheel Color using  - Pd3 - PD5 and PD6 using
/// ARGS: Led Mask ex : (1 << PB1)
/// RETURNS: None
/// REFS: page 132 - Datasheet

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {

    OCR2B = b; // led BLUE
    OCR0A = g; // led GREEN
    OCR0B = r; // led RED
}

/// NOTE: function Sets up the timer 0 and 2  - Pd3 - PD5 and PD6 8 bit Timers
/// ARGS: None
/// RETURNS: None
/// REFS: Function fro EX02 - Module 03

void  init_rgb(void) {

    timer_init_timer0();    // timer 0 controls led PD5 and PD6 (OCR0A and OCR0B)
    timer_init_timer2();    // timer 2 controls led PD3 (OCR2B)
}