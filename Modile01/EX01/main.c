/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/16 08:56:44 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

#define LED2 (1 << PB1)                         // PB1 mask select
#define PRESCALER 256                           // SYNIC with CS12 p142 ref: DataSheet
#define TIME_FREQUENCY ( F_CPU / PRESCALER)     // MAX Value to be stored in OCR1A
                                                // page 129 on datasheet
                                                // 16 bits = 65565 max
                                                // 16M/8/2 = 1000000 > 65565 too much
                                                // 16M/16/2 = 125000 > 65565 too much
                                                // 16M/256/2 = 31250 < 65565
int main(void) {

    // PB1 set for OUTPUT - D2 led
    DDRB |= LED2;

    // SET CTC mode - Toggle OC1A/OC1B on Compare Match. page 140 
    TCCR1A |= (1 << COM1A0); 

    // Set CTC on TOP OCR1A NB - TCCR1B timer counter regiister B setting - page 142
    TCCR1B |= (1 << WGM12);  

    // Set Clk prescaler to F_CPU/256 = 1MHz/256 = 0.256 (ms) per tick - page 139 & 143 
    TCCR1B |= (1 << CS12) ;

    // set Value to 50%  - 0.5 sec. - page 129// Set value - 50% 
    OCR1A = (TIME_FREQUENCY / 2); 

    while(1) {

        
    }
}
