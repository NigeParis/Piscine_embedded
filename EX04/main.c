/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/14 16:24:49 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define LED_MASK ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4))
#define BUTTON_MASK ((1 << PD2) | (1 << PD4))


void ledNumber(uint8_t count) {

    if (count)
        _delay_ms(10);
    
    switch(count) {

        case 1:
                PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB4));
                PORTB |= (1 << PB0);
                return;
        case 2:
                PORTB &= ~(1 << PB0);
                PORTB |= ((1 << PB0) << 1);  
                return;
        case 3:
                PORTB &= ~((1 << PB2) | (1 << PB4));
                PORTB |= (1 << PB0) | ((1 << PB0) << 1);  
                return;
        case 4:
                PORTB &= ~((1 << PB0) |(1 << PB1) | (1 << PB4));
                PORTB |= (1 << PB2);  
                return;
        case 5:
                PORTB &= ~((1 << PB1) | (1 << PB4));
                PORTB |= (1 << PB2) | (1 << PB0);  
                return;
        case 15:
                PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4) ;  
                return;
                
        default:
                PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4)) ;  

    }
}



int main(void) {
    volatile uint8_t count = 0;
    DDRB |= LED_MASK;
    PORTB |= LED_MASK;
    DDRD &= ~BUTTON_MASK;
    PORTD |= BUTTON_MASK;

    while (1) {

        if ((PIND & (1 << PD4)) == 0) {
            while ((PIND & (1 << PD4)) == 0) {; }
            if (count == 0)
            count = 16;            
            if (count > 0)
                count--;
            _delay_ms(16);
            ledNumber(count);

        }
        else if ((PIND & (1 << PD2)) == 0) {
            while ((PIND & (1 << PD2)) == 0) { ; }
            count++;
            _delay_ms(16);
            if (count > 15)
                count = 0;
            
            ledNumber(count);
          
        }
        _delay_ms(16);
    }

}

    