/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/14 08:57:08 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    
    DDRB |= (1 << PB0);
    DDRD |= (1 << PD2);
    PORTD |= (1 << PD2);
    PORTB |= (1 << PB0);

    while (1) {
        if ((PIND & (1 << PD2)) == 0) {
            _delay_ms(150);
            
            PORTB ^= (1 << PB0);
            
            while ((PIND & (1 << PD2)) == 0) {
                _delay_ms(150);    
            }                
        }
        _delay_ms(150);
    }     
}

/// NOTE: PORTB ^= (1 << PB0) toogle only bit 0 PB0 leaves the others alone
// PORTD is initialised HIGH - pulled up, so detection is easier to see
// PORTB is initialised HIGH also if button pressed (goes to LOW)
            
/// IMPORTANT: while in a while if PD@ is LOW enters if , 
// if i keep pressing enters second while until released

