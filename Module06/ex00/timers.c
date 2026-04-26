/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:15:38 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/20 16:05:41 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timers.h"
#include <avr/io.h>


void timer_init_timer0(void) {

    TCCR0A= (1 << COM0B1)   // 8 bit Timer 0 after Schema works on PD6 and PD5 - page 113
            | (1 << COM0A1) 
            | (1 << WGM00)  // Mode 3 for timer 0 - WGM00 + WGM01 - page 115 datasheet
            | (1 << WGM01); // Mode 3 WGM00 + WGM01 --> SAME as WG10 + WGM11

    TCCR0B= (1 << CS00)     // prescaler 64 - also works with others prescaler setting
            | (1 << CS01);  // CS01 + C00  - page 117 datasheet
 }

/// NOTE: Timer 2 after Schema and page 162 in the datasheet

void timer_init_timer2(void) {

    TCCR2A=                 // 8 Bit Timer 2 - page 164 datasheet
            (1 << COM2B1)   // Only use COM2B1 this works with PD3 - see schema
            | (1 << WGM21)  // Mode 3 Fast PWM - TOP 0xFF Upd OCRx: BOTTOM Timer Over Flow Flag "TOV" MAX
            | (1 << WGM20); // PWM mode 3 - page 164 datasheet
            
    TCCR2B=                
            (1 << CS22);    // prescaler 64 - also works with others
}





void stop_timers(void) {

    TCCR2A &= ~((1 << COM2B1) | (1 << WGM21) | (1 << WGM20));
            
    TCCR2B &= ~(1 << CS22);


    TCCR0A &= ~((1 << COM0B1) | (1 << COM0A1) | (1 << WGM00) | (1 << WGM01));

    TCCR0B &= ~((1 << CS00) | (1 << CS01)); 
    
        
        
}