/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrobinso <nrobinso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:41:53 by nrobinso          #+#    #+#             */
/*   Updated: 2026/04/21 19:28:49 by nrobinso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <util/delay.h>                         // delay for the debounce
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tools.h"
#include "uart_lib.h"
#include "config.h"
#include "led_lib.h"
#include "timers.h"

typedef unsigned char uint8_t;      // needed because not using stdlib

#define LED1 (1 << PB0)
#define SW1 ~(1 << PD2)

int led_flag = 0;

void timer_init_timer1() {

    TCCR1A &= ~((1 << COM1A1) | (1 << COM1B1));
    
    TCCR1C |= (1 << CS11) | (1 << CS10);  // prescaler 64
    

    EIMSK = (1 << INT0);  // page 81
    
    
    EICRA |= (1 << ISC01);  // trigger on falling edge of INT0
    
    // EICRA |= (1 << ISC01) | (1 << ISC00);  // trigger on rising edge
    
    SREG |= (1 << 7);  // ENABLE Global interupts page 20
    
    TCNT1 = 0;

    EICRA |= (1 << ISC01);  // trigger on falling edge of INT0
    
    // EICRA |= (1 << ISC01) | (1 << ISC00);  // trigger on rising edge
}

void __vector_1(void) __attribute__((signal)); // INTO_vect interupt 1 - page 66
void __vector_1(void)
{
    if(ISC01 && ISC01)
        PORTB &= ~(LED1);
    if(ISC01 == 1)
        PORTB |= LED1;
    
    led_flag = 0;

    
}



int main(void) {

    DDRB |= LED1;    // LED ! - D1 is opened and set on output   
    DDRD &= SW1;    // BUTTON ! - SW1 is opened and set on input   
    timer_init_timer1();
    
    while(1){

        if (led_flag == 0) {        
            EICRA &= ~(1 << ISC00);  // trigger on falling edge of INT0
            led_flag = 1;
        } else {
            EICRA |= (1 << ISC00);  // trigger on rising edge
        }
        
    }
}
