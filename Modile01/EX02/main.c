/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/15 20:50:48 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

#define DUTYC 1                                 // Duty cycle 10%
#define LED2 (1 << PB1)                         // PB1 mask select
#define PRESCALER 1024                          // SYNIC with CS12 p142 ref: DataSheet
#define TIME_FREQUENCY ( F_CPU / PRESCALER)     // MAX Value to be stored in OCR1A
                                                // page 129 on datasheet
                                                // 16 bits = 65565 max
                                                // 16M/8/2 = 1000000 > 65565 too much
                                                // 16M/16/2 = 125000 > 65565 too much
                                                // 16M/256/2 = 31250 < 65565
int main(void) {

    // PB1 set for OUTPUT - D2 led
    DDRB |= LED2;

    // SET Fast PWM mode 14 - . page 142 
    TCCR1A |= ((1 << COM1A1) | (1 << WGM11)); 
    
    // SET Fast PWM mode 14 NB - TCCR1B timer counter regiister B setting - page 142
    TCCR1B |= ((1 << WGM12) | (1 << WGM13));  
    
    // Set Clk prescaler to F_CPU/256 = 1MHz/1024 = 1.024 (ms) per tick - page 139 & 143 
    TCCR1B |= ((1 << CS12) | (1 << CS10));
    
    // ICR1 TOP Value 100% on - 1 sec. - page 129 
    ICR1 = TIME_FREQUENCY;
    
    // send to OCR1A - divide by 10 by %
    OCR1A = (ICR1 / 10);

    OCR1A = OCR1A * DUTYC;
    
    while(1) {
        
    }
}
