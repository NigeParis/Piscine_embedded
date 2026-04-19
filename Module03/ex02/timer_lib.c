/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer_lib.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 17:06:08 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/19 17:14:10 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timer_lib.h"
#include <avr/io.h>

#define TARGET_HZ 1                                   // TARGET in HZ 0.5 = 2hz eg 1HZ = 1
#define PRESCALER 1024                                  // SYNIC with CS12 p142 ref: DataSheet
#define TIME_FREQUENCY (F_CPU / PRESCALER) / TARGET_HZ  // MAX Value to be stored in OCR1A


void timer_init(void) {

    // SET Fast PWM mode 14 - . page 142 
    TCCR1A |= ((1 << COM1A1) | (1 << WGM11)); 
    
    // SET Fast PWM mode 14 NB - TCCR1B timer counter regiister B setting - page 142
    TCCR1B |= ((1 << WGM12) | (1 << WGM13));  
    
    // Set Clk prescaler to F_CPU/1024 = 16MHz/1024 - page 139 & 143 
    TCCR1B |= ((1 << CS12) | (1 << CS10));
    
    // ICR1 TOP Value 100% on - 1 sec. - page 129 
    ICR1 = TIME_FREQUENCY - 1;

    TIMSK1 |= (1 << OCIE1A);  // ENABLE interupts on CTC  page - 144

    SREG |= (1 << 7);  // ENABLE Global interupts page 20
    
}
