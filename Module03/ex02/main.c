/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/19 19:59:18 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/delay.h>                         // delay for the debounce
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer_lib.h"

#define LED_RED (1 << PD5)    // define RED led mask
#define LED_GREEN (1 << PD6)  // define GREEN led mask
#define LED_BLUE (1 << PD3)   // define BLUE led mask

#define TARGET_HZ 1                                   // TARGET in HZ 0.5 = 2hz eg 1HZ = 1
#define PRESCALER 1024                                  // SYNIC with CS12 p142 ref: DataSheet
#define TIME_FREQUENCY (F_CPU / PRESCALER) / TARGET_HZ  // MAX Value to be stored in OCR1A

/*

 INIT
    ────
    Set Timer0 Fast PWM  → Red   (OC0A PD6)
                         → Green (OC0B PD5)

    Set Timer2 Fast PWM  → Blue  (OC2B PD3)
        TCCR2A = COM2B1 | WGM21 | WGM20    // non-inverting Fast PWM
        TCCR2B = CS22                        // prescaler 64

    Set DDRD pins
        PD6 = OUTPUT    // Red
        PD5 = OUTPUT    // Green
        PD3 = OUTPUT    // Blue

    Set position = 0        // 0 to 255
    Set speed    = 2        // step size


*/

void timer_init(void) {

   

     // CTC mode - clears TCNT2 when it matches OCR2A
    TCCR2A = (1 << WGM21);

    // prescaler 1024
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);

    // TOP value - timer resets when TCNT2 hits this
    OCR2A = 50;    // 16MHz / 1024 / 156 ≈ 100Hz

    // Second compare point - fires halfway through
    OCR2B = 25;     // roughly 50% of OCR2A

    // Start counter from 0
    TCNT2 = 6;

 
}



void timer_init_TCNT(void) {
    TCCR2A = (1 << COM2B1)                   // non-inverting OC2B
           | (1 << WGM21)  | (1 << WGM20);  // Fast PWM mode 3
    TCCR2B = (1 << CS22);                    // prescaler 64
    OCR2B  = 0;                              // Blue  duty cycle
}

    


void  init_rgb(void) {


    
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {

(void)r;
(void)g;
(void)b;

    

    
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



void __vector_11(void) __attribute__((signal)); // TIMER1_COMPA interupt 11 - page 66
void __vector_11(void)
{
    PORTD = (0b00100000);       // LED RED on
    _delay_ms(500);            // wait 1 sec 1000ms
    PORTD = (0b00000000);       // LED RED on
    _delay_ms(500);            // wait 1 sec 1000ms

}



int main(void) {
    
    DDRD |= LED_RED;    // open for output on PD5  
    DDRD |= LED_GREEN;  // open for output on PD6
    DDRD |= LED_BLUE;   // open for output on PD3

        
    // timer_init();
    timer_init_TCNT();


    
    while (1) {
   if (TIFR2 & (1 << OCF2A)) {
            PORTD ^= (1 << PD2);        // toggle RED LED
            TIFR2 |= (1 << OCF2A);      // clear flag by writing 1
        }

        // Poll OCF2B flag — fires when TCNT2 matches OCR2B (MID)
        if (TIFR2 & (1 << OCF2B)) {
            PORTD ^= (1 << PD3);        // toggle GREEN LED
            TIFR2 |= (1 << OCF2B);      // clear flag by writing 1
        }

    }
}
