/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/13 15:43:21 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main(void) {
    DDRB |= (1 << PB0);
    while(1)
        PORTB |= (1 << 0b00000000);
}

// DDRB "Data Direction Register B is set to OUTPUT 1 / INPUT 0"
// |= is a safe way to change the bit from 0 to 1 setting to OUTPUT
// PB0 = 0b00000000 = PIN0
// PB1 = 0b00000001 = PIN1
// DDRB - PB0 -to PC5
