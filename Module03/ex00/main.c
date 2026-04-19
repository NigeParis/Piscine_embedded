/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/19 16:04:52 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/delay.h>                         // delay for the debounce
#include <avr/io.h>

#define LED_RED (1 << PD5)    // define RED led mask
#define LED_GREEN (1 << PD6)  // define GREEN led mask
#define LED_BLUE (1 << PD3)   // define BLUE led mask
                        

int main(void) {
    

    DDRD |= LED_RED;    // open for output on PD5  
    DDRD |= LED_GREEN;  // open for output on PD6
    DDRD |= LED_BLUE;   // open for output on PD3
    
    while (1) {     
        for (;;) {
            
            PORTD &= ~(LED_BLUE);   // LED BLUE off
            PORTD |= LED_RED;       // LED RED on
            _delay_ms(1000);        // wait 1 sec 1000ms
            PORTD &= ~(LED_RED);    // LED RED off
            PORTD |= LED_GREEN;     // LED GREEN on
            _delay_ms(1000);        // Wait 1 sec
            PORTD &= ~(LED_GREEN);  // LED GREEN off
            PORTD |= LED_BLUE;      // LED BLUE on
            _delay_ms(1000);
        }     
    }
}
