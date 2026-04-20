/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/20 16:12:37 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/delay.h>                         // delay for the debounce
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer_lib.h"

#define LED_RED (1 << PD5)    // define RED led mask
#define LED_GREEN (1 << PD6)  // define GREEN led mask
#define LED_BLUE (1 << PD3)   // define BLUE led mask
      
void __vector_11(void) __attribute__((signal)); // TIMER1_COMPA interupt 11 - page 66
void __vector_11(void)
{
    PORTD = (0b00100000);       // LED RED on
    _delay_ms(1000);            // wait 1 sec 1000ms
    PORTD = (0b01000000);       // LED GREEN on
    _delay_ms(1000);            // Wait 1 sec
    PORTD = (0b00001000);       // LED BLUE on
    _delay_ms(1000);
    PORTD = (0b01100000);       // RED + GREEN = YELLOW
    _delay_ms(1000);
    PORTD = (0b01001000);       // GREEN + BLUE = LIGHT BLUE
    _delay_ms(1000);
    PORTD = (0b00101000);       // RED + BLUE = MAGENTAModule03/ex01
    _delay_ms(1000);
    PORTD = (0b01101000);       // RED + GREEN + BLUE = WHITE
    _delay_ms(1000);
}

int main(void) {
    
    DDRD |= LED_RED;    // open for output on PD5  
    DDRD |= LED_GREEN;  // open for output on PD6
    DDRD |= LED_BLUE;   // open for output on PD3
    
    timer_init();
    while (1) {}
}
