/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nr1obinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/15 10:52:02 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

typedef unsigned int uint16_t;
#define CALIBRATE_MS 8000
#define LED_MASK (1 << PB1)

static void loop_for_one_millisecond(volatile uint16_t loop_Max) {

    for (uint16_t i = 0; i < loop_Max; i++)
        ;
} 

static void timer_in_milliseconds(uint16_t time_in_ms) {

    while (time_in_ms > 0) {
        loop_for_one_millisecond(F_CPU / CALIBRATE_MS);       
        time_in_ms--;
    }
} 

int main(void) {
    DDRB |= LED_MASK;

    while (1) {
        timer_in_milliseconds(500);
        PORTB ^= LED_MASK;
    }

}

/// NOTE: system of two loops 
// one loop is a loop for a millisecond 
// the other loop in number of milliseconds needed
// PORTB ^= LED_MASK; toogles on and off the LED PB1
// Why loop in a loop is because the card memory max is 0 to 65,535.
// two loop reduces the size needed and speed of the loop to 1 ms 