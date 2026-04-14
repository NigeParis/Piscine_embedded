/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/14 18:33:18 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#define LOW 0


int main(void) {
    DDRB |= (1 << PB0);                   // set PIN PB0 to OUTPUT 1
    DDRD &= ~(1 << PD2);                  // set PIN PD@ (switch) to INPUT 0
    
    PORTD |= (1 << PD2);                  // pull up SWITCH HIGH
    while (1) {
        if ((PIND & (1 << PD2)) == LOW)   // if button pressed (goes LOW)
            PORTB |= (1 << PB0);          // turn LED on
        else
            PORTB &= ~(1 << PB0);         // turn LED off
    }
}

