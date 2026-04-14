/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/14 18:55:30 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define LED_MASK ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4))
#define BUTTON_MASK ((1 << PD2) | (1 << PD4))
#define LOW 0

void ledNumber(uint8_t count) {
    
    uint8_t bit3 = count &(0b00001000);             // isolate bit 3
    uint8_t bit4 = bit3 << 1;                       // move it to bit 4 (PB4)
    uint8_t first_three_bits = count &(0b00000111); // bits 0–2
    uint8_t val = first_three_bits | bit4;          // combine into LED pattern
    uint8_t cleared_led_bits = PORTB & ~LED_MASK;   // clear LED bits only
    PORTB = cleared_led_bits | val;                 // write new LED state
}


/// NOTE: bit Wise Operators
/// '|'  bit A | bit B ->  0 | 0 -> 0 ALL OTHERS 0 | 1 etc  -> 1
/// '&'  bit A & bit B ->  1 & 1 -> 1 ALL OTHERS 0 | 1 etc  -> 0
/// '^'  bit A ^ bit B ->  1 ^ 1 -> 0  0 ^ 0 -> 0 ALL OTHERS -> 1
/// '~'  bit AB 10 -> 01 and 01 - 10


int main(void) {
    volatile uint8_t count = 0;
    DDRB |= LED_MASK;
    PORTB &= ~LED_MASK;
    DDRD &= ~BUTTON_MASK;
    PORTD |= BUTTON_MASK;

    while (1) {

        if ((PIND & (1 << PD4)) == LOW) {
            while ((PIND & (1 << PD4)) == LOW) {; }
            if (count == 0)
                count = 16;            
            if (count > 0)
                count--;
            _delay_ms(16);
            ledNumber(count);

        }
        else if ((PIND & (1 << PD2)) == LOW) {
            while ((PIND & (1 << PD2)) == LOW) { ; }
            count++;
            _delay_ms(16);
            if (count > 15)
                count = 0;
            
            ledNumber(count);
          
        }
        _delay_ms(16);
    }

}

    