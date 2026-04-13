/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/13 16:24:07 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main(void) {
    DDRB |= (1 << PB0);
    DDRD &= ~(1 << PD2);
    
    PORTD |= (1 << PD2);
    while (1) {
        if (!(PIND & (1 << PD2)))   // if button pressed (active LOW)
            PORTB |= (1 << PB0);    // turn LED on
        else
            PORTB &= ~(1 << PB0);       // turn LED off
    }
}

