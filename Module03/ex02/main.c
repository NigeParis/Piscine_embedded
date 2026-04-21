/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/21 08:42:14 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/delay.h>                         // delay for the debounce
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart_lib.h"

#define LED_RED (1 << PD5)    // define RED led mask
#define LED_GREEN (1 << PD6)  // define GREEN led mask
#define LED_BLUE (1 << PD3)   // define BLUE led mask

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
    
void  init_rgb(void) {

    timer_init_timer0();    // timer 0 controls led PD5 and PD6 (OCR0A and OCR0B)
    timer_init_timer2();    // timer 2 controls led PD3 (OCR2B)
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {

    OCR2B = b; // led BLUE
    OCR0A = g; // led GREEN
    OCR0B = r; // led RED
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

int main(void) {
        
    DDRD |= LED_RED;    // open for output on PD5  
    DDRD |= LED_GREEN;  // open for output on PD6
    DDRD |= LED_BLUE;   // open for output on PD3

    init_rgb();
    


    
    while (1) {
  
        for(int pos = 0; pos < 255; pos++ ) {
        
            wheel(pos);
            _delay_ms(20);
        }
    }
}
